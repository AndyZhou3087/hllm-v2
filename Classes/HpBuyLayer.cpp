#include "HpBuyLayer.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "HintBox.h"
#include "CommonLayer.h"

int HpBuyLayer::AnimType = A_SMALLTONORMAL;
int buyHpGem = 100;
HpBuyLayer::HpBuyLayer()
{
}


HpBuyLayer::~HpBuyLayer()
{
}
bool HpBuyLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	Node * layer = CSLoader::createNode("buyHpLayer.csb");
	addChild(layer);

	_closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("close");
	_closeBtn->addTouchEventListener(CC_CALLBACK_2(HpBuyLayer::close, this));

	initTime();
	_recoveryTime = (cocos2d::ui::TextBMFont*)layer->getChildByName("hprecoverytime");
	_hpRecoveryText = (cocos2d::ui::Widget*)layer->getChildByName("hprecoverytext");
	_hpMaxTextlbl = (cocos2d::ui::Widget*)layer->getChildByName("hpmaxtextlbl");
	_hpRecoveryTimeBox = (cocos2d::ui::Widget*)layer->getChildByName("hprecoverytimebox");

	cocos2d::ui::Widget* hpUpBtn = (cocos2d::ui::Widget*)layer->getChildByName("hpupbtn");
	hpUpBtn->addTouchEventListener(CC_CALLBACK_2(HpBuyLayer::hpUp, this));

	cocos2d::ui::Widget* hpMaxBtn = (cocos2d::ui::Widget*)layer->getChildByName("hpmaxbtn");
	hpMaxBtn->addTouchEventListener(CC_CALLBACK_2(HpBuyLayer::hpMax, this));

	cocos2d::ui::Widget* onsalelight = (cocos2d::ui::Widget*)layer->getChildByName("buylight");
	onsalelight->runAction(RepeatForever::create(RotateTo::create(6, 720)));
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (AnimType == A_SMALLTONORMAL)
	{
		_isAutoPop = false;
		layer->setAnchorPoint(Vec2(0.5f, 0.5f));
		layer->setPosition(Vec2(270, 480));
		layer->setScale(0.1f);
		layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		_isAutoPop = true;
		layer->setPosition(0, 960);
		layer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}
	schedule(schedule_selector(HpBuyLayer::updateTime), 0.1f);
	return true;
}

void HpBuyLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void HpBuyLayer::hpUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int buyHpCount = GlobalData::getBuyHpTimes();
		if (buyHpCount >= 1 || GlobalData::getMyMaxHp() >= 35)
		{
			HintBox* hint = HintBox::create("已达到体力上限", 0);
			addChild(hint, 1000);
		}
		else
		{
			int needGem = buyHpGem;
			if (GlobalData::getGemCount() >= needGem)
			{
				GlobalData::setBuyHpTimes(GlobalData::getBuyHpTimes() + 1);
				GlobalData::setGemCount(GlobalData::getGemCount() - needGem);
				GlobalData::setMyMaxHp(GlobalData::getMyMaxHp() + 10);
				GlobalData::setHpCount(GlobalData::getHpCount() + 10);
				HintBox* hint = HintBox::create("购买成功", 0);
				addChild(hint, 1000);
				showBuyOKAnim("UI/hp.png", Vec2(270, 540), Vec2(30, 925));
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDHP);
#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };
				sprintf(p_str, "%s", "hp");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					float perprice =  ((float)buyHpGem)/GlobalData::getMyMaxHp();
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, GlobalData::getMyMaxHp(), perprice);
				}
#endif
			}
			else
			{
				HintBox* hint = HintBox::create("钻石不足", 1);
				addChild(hint, 1000);
			}
		}
	}
}

void HpBuyLayer::hpMax(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getHpCount() >= GlobalData::getMyMaxHp())
		{
			HintBox* hint = HintBox::create("体力已满", 0);
			addChild(hint, 1000);
		}
		else
		{
			if (GlobalData::getGemCount() >= 50)
			{
				GlobalData::setHpCount(GlobalData::getMyMaxHp());
				GlobalData::setGemCount(GlobalData::getGemCount() - 50);
				HintBox* hint = HintBox::create("购买成功", 0);
				addChild(hint, 1000);
				//struct timeval now;
				//gettimeofday(&now, NULL);
				int syssec = GlobalData::getSystemSecTime();
				GlobalData::setHpTime(syssec);

				showBuyOKAnim("UI/hp.png", Vec2(270, 540), Vec2(30, 925));
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDHP);
				if (_isAutoPop)
				{
					this->scheduleOnce(schedule_selector(HpBuyLayer::removeSelf), 1.0f);
				}
#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };
				sprintf(p_str, "%s", "hp");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					float perprice =  (50.0f/GlobalData::getMyMaxHp());
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, GlobalData::getMyMaxHp(), perprice);
				}
#endif
			}
			else
			{
				HintBox* hint = HintBox::create("钻石不足", 1);
				addChild(hint, 1000);
			}
		}
	}
}

void HpBuyLayer::updateTime(float t)
{
	maxHpCount = GlobalData::getMyMaxHp();
	_powertime = GlobalData::getHpTime();
	if (_powercount < maxHpCount)
	{
		//struct timeval now;
		//gettimeofday(&now, NULL);
		//int nowtimestamp = now.tv_sec;

		int nowtimestamp = GlobalData::getSystemSecTime();
		if (nowtimestamp > 0)
		{
			char timeStr[6] = { 0 };
			int tmp = nowtimestamp - _powertime;
			if (tmp >= 600)
			{
				_powertime += 10 * 60;
				_powercount++;
				tmp = nowtimestamp - _powertime;
				SaveTime();
			}
			tmp = 600 - tmp;
			if (_powercount < maxHpCount)
			{
				sprintf(timeStr, "%02d:%02d", tmp / 60, tmp % 60);
				_recoveryTime->setString(timeStr);
			}
		}
	}
	if (GlobalData::getHpCount() >= GlobalData::getMyMaxHp())
	{
		_recoveryTime->setVisible(false);
		_hpRecoveryText->setVisible(false);
		_hpRecoveryTimeBox->setVisible(false);
		_hpMaxTextlbl->setVisible(true);
	}
	else
	{
		_recoveryTime->setVisible(true);
		_hpRecoveryText->setVisible(true);
		_hpRecoveryTimeBox->setVisible(true);
		_hpMaxTextlbl->setVisible(false);
	}
}


void HpBuyLayer::initTime()
{
	//struct timeval now;
	//gettimeofday(&now, NULL);
	//int nowtimestamp = now.tv_sec;

	int nowtimestamp = GlobalData::getSystemSecTime();
	int maxSec = 600;
	_powerSec = 0;

	_powertime = GlobalData::getHpTime();
	_powercount = GlobalData::getHpCount();
	maxHpCount = GlobalData::getMyMaxHp();

	if (nowtimestamp < _powertime)
		_powertime = nowtimestamp;

	if (_powercount < maxHpCount)
	{
		int c = floor((nowtimestamp - _powertime) / (10 * 60));
		if (c > 0)
		{
			_powercount += c;
			if (_powercount >= maxHpCount)
			{
				_powercount = maxHpCount;
				_powertime = nowtimestamp;
			}
			else
			{
				_powertime += c * 10 * 60;
				_powerSec = maxSec - (nowtimestamp - _powertime);
			}

		}
		else
		{
			_powerSec = maxSec - (nowtimestamp - _powertime);
		}
	}
	else
	{
		_powertime = nowtimestamp;
	}

	SaveTime();
}

void HpBuyLayer::SaveTime()
{
	GlobalData::setHpCount(_powercount);
	GlobalData::setHpTime(_powertime);
}

void HpBuyLayer::showBuyOKAnim(const char* pic, Vec2 spoint, Vec2 dpoint)
{
	for (int i = 0; i<6; i++)
	{
		Sprite* s = Sprite::createWithSpriteFrameName(pic);
		s->setOpacity(0);
		s->setPosition(spoint);
		this->addChild(s, 100);
		s->runAction(Sequence::create(DelayTime::create(i*0.06f), Spawn::create(FadeIn::create(0.2f), EaseSineIn::create(MoveTo::create(0.3f, dpoint)), NULL), FadeOut::create(0.05f), NULL));
	}
}

void HpBuyLayer::removeSelf(float dt)
{
	this->removeFromParentAndCleanup(true);
}