#include "CMOverLayer.h"
#include "ThemeScene.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "Const.h"
#include "MyParticle.h"
#include "CommonLayer.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "SuccLayer.h"
#include "HpBuyLayer.h"
#include "MyTransitionScene.h"

// on "init" you need to initialize your instance

int CMOverLayer::lastbestscore = 0;
bool CMOverLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color,-2);

	SoundManager::sharedManager()->stopBackMusic();
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_SUCC);
	_root = CSLoader::createNode("CMOverLayer.csb");
	addChild(_root);

	ChallengeMode cmode = GlobalData::getChallengeMode();
	std::string textstr;
	if (cmode == CMFLASH)
	{
		textstr = StringUtils::format("UI/cmoverflashtitle.png");
		cocos2d::ui::ImageView* title = (cocos2d::ui::ImageView*)_root->getChildByName("title");
		title->loadTexture(textstr, cocos2d::ui::TextureResType::PLIST);
		title->setContentSize(Sprite::createWithSpriteFrameName(textstr)->getContentSize());
	}

	m_restartBtn = (cocos2d::ui::Widget*)_root->getChildByName("restartbtn");
	m_restartBtn->addTouchEventListener(CC_CALLBACK_2(CMOverLayer::restart, this));
	m_restartBtn->setVisible(false);

	usehpbmlbl = (cocos2d::ui::TextBMFont*)m_restartBtn->getChildByName("usehpbmlbl");

	if (GlobalData::getCModeCount(GlobalData::getChallengeMode()) > 0)
	{
		//m_restartBtn->getChildByName("freetext")->setVisible(true);
		usehpbmlbl->setVisible(false);
	}

	string usehpstr = StringUtils::format("x%d", GlobalData::getCurStageUseHp());
	usehpbmlbl->setString(usehpstr);

	cocos2d::ui::Widget* closebtn = (cocos2d::ui::Widget*)_root->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(CMOverLayer::close, this));

	m_shareBtn = (cocos2d::ui::Widget*)_root->getChildByName("sharebtn");
	m_shareBtn->addTouchEventListener(CC_CALLBACK_2(CMOverLayer::share, this));
	m_shareBtn->setVisible(false);

	std::string scoreStr = StringUtils::format("%d", g_gameScene->getScore());

	for (int i = (int)scoreStr.length() - 3; i > 0; i -= 3)
		scoreStr.insert(i, ",");

	scorebmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("scorelbl");
	scorebmlbl->setString(scoreStr);

	lastbestscore = GlobalData::getBestCMScore(GlobalData::getChallengeMode());

	std::string bestscoreStr = StringUtils::format("%d", lastbestscore);
	for (int i = (int)bestscoreStr.length() - 3; i > 0; i -= 3)
		bestscoreStr.insert(i, ",");
	cocos2d::ui::TextBMFont* bestscorebmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("bestscorelbl");
	bestscorebmlbl->setString(bestscoreStr);

	cocos2d::ui::TextBMFont* wingoldbmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("wingoldbmlbl");
	cocos2d::ui::TextBMFont* wingembmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("wingembmlbl");
	cocos2d::ui::TextBMFont* winhpbmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("winhpbmlbl");

	_root->getChildByName("gem")->setVisible(false);
	_root->getChildByName("hp")->setVisible(false);
	_root->getChildByName("sureropbox_1")->setVisible(false);
	_root->getChildByName("sureropbox_2")->setVisible(false);
	wingembmlbl->setVisible(false);
	winhpbmlbl->setVisible(false);

	bool ishisBest = g_gameScene->getScore() > lastbestscore;
	int playday = GlobalData::getCModeYearOfDay(GlobalData::getChallengeMode());

	int bonuseGem = 0;
	if (playday != GlobalData::getDayOfYear())//每天
	{
		if (ishisBest)
		{
			bonuseGem = 1;
			_root->getChildByName("gem")->setVisible(true);
			_root->getChildByName("hp")->setVisible(true);
			_root->getChildByName("sureropbox_1")->setVisible(true);
			_root->getChildByName("sureropbox_2")->setVisible(true);
			wingembmlbl->setVisible(true);
			wingembmlbl->setString("1");
			//winhpbmlbl->setVisible(true);
			//winhpbmlbl->setString("6");
			GlobalData::setGemCount(GlobalData::getGemCount() + 1);
			//GlobalData::setHpCount(GlobalData::getHpCount() + 6);
		}
		else
		{
			//_root->getChildByName("hp")->setVisible(true);
			//_root->getChildByName("hp")->setPosition(_root->getChildByName("gem")->getPosition());
			//winhpbmlbl->setPosition(wingembmlbl->getPosition());
			//winhpbmlbl->setVisible(true);
			//winhpbmlbl->setString("4");
			//GlobalData::setHpCount(GlobalData::getHpCount() + 4);
		}
	}
	else
	{
		if (ishisBest)
		{
			bonuseGem = 1;
			_root->getChildByName("gem")->setVisible(true);
			//_root->getChildByName("hp")->setVisible(true);
			_root->getChildByName("sureropbox_1")->setVisible(true);
			wingembmlbl->setVisible(true);
			wingembmlbl->setString("1");
			//winhpbmlbl->setVisible(true);
			//winhpbmlbl->setString("2");
			GlobalData::setGemCount(GlobalData::getGemCount() + 1);
			//GlobalData::setHpCount(GlobalData::getHpCount() + 2);
		}
	}
	if (ishisBest)
	{
		GlobalData::setBestCMScore(GlobalData::getChallengeMode(), g_gameScene->getScore());

#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "gem");
		BONUSETYPE type = BONUSE_DEFAULT;
		if (GlobalData::getChallengeMode() == CMCLASIC)
			type = BONUSE_CLACIS;
		else
			type = BONUSE_FLASH;
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, bonuseGem, type);
		}
#endif
	}

	int bonuseGold = getBonuseGold();
	string bonuseGoldStr = StringUtils::format("%d", bonuseGold);
	wingoldbmlbl->setString(bonuseGoldStr);
	GlobalData::setGoldCount(GlobalData::getGoldCount() + bonuseGold);


#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", "coin");
	BONUSETYPE type = BONUSE_DEFAULT;
	if (GlobalData::getChallengeMode() == CMCLASIC)
		type = BONUSE_CLACIS;
	else
		type = BONUSE_FLASH;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, bonuseGold, type);
	}
#endif

	if (g_gameScene != NULL)
	{
		int count = g_gameScene->getCleanCount();

		Achieve* data = GlobalData::getAchieveDataByID(1);
		if (data->finish != -1)
		{
			data->finish += count;
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(2);
		if (data->finish != -1)
		{
			data->finish += count;
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(3);
		if (data->finish != -1)
		{
			data->finish += count;
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(4);
		if (data->finish != -1)
		{
			data->finish += g_gameScene->getCleanAnimalCount(15);
			GlobalData::SaveAchieveData();
		}

		data = GlobalData::getAchieveDataByID(5);
		if (data->finish != -1)
		{
			data->finish += g_gameScene->getCleanAnimalCount(16);
			GlobalData::SaveAchieveData();
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	g_gameScene->clearData();

	_root->setAnchorPoint(Vec2(0.0f, 0.0f));
	_root->setPosition(0, 960);
	_root->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
	_visualscore = 0;
	this->schedule(schedule_selector(CMOverLayer::updatescore));
	return true;
}

int CMOverLayer::getBonuseGold()
{
	float base = 15.0f;
	float a = 0.05f * base * g_gameScene->getClasicStage();
	float b = 0.01f * base * g_gameScene->getclasiccleancount();
	return base + (int)(a + b);
}

void CMOverLayer::onEnterTransitionDidFinish()
{
	if (g_gameScene->getScore() > lastbestscore)
	{
		cocos2d::ui::TextBMFont* clasicbesticon = (cocos2d::ui::TextBMFont*)_root->getChildByName("clasicbesticon");
		clasicbesticon->setScale(4.0f);
		clasicbesticon->runAction(Sequence::create(DelayTime::create(0.8f), Show::create(), EaseSineIn::create(ScaleTo::create(0.4f, 1)), NULL));
		this->schedule(schedule_selector(CMOverLayer::showStarParticle), 2.0f);
#ifdef USENETWORK
		SuccLayer::updataUserData();
#endif
	}
	
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };

	if (GlobalData::getChallengeMode() == CMCLASIC)
		sprintf(p_str, "clasicstage%d", g_gameScene->getClasicStage());
	else
		sprintf(p_str, "flashstage%d", g_gameScene->getFlashAddTimes());
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "failLevel", "(Ljava/lang/String;)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
	}
#endif
	this->scheduleOnce(schedule_selector(CMOverLayer::delayPlayAnim), 0.5f);
}

void CMOverLayer::updatescore(float dt)
{
	int count = g_gameScene->getScore();
	if (count == 0)
	{
		_visualscore = 0;
	}
	else
	{
		float step = (count - _visualscore) * 0.04f;
		if (fabs(step) < 0.05)
		{
			step = 0.05 * ((step > 0) ? 1 : -1);
		}

		_visualscore += step;

		if (fabs(count - _visualscore) < 0.1f) {
			_visualscore = count;
		}
	}

	std::string str = StringUtils::format("%d", (int)_visualscore);
	for (int i = (int)str.length() - 3; i > 0; i -= 3)
		str.insert(i, ",");
	scorebmlbl->setString(str);
}

void CMOverLayer::showGiftLayer(int type)
{
	FourTypeGift::AnimType = A_UPTODOWN;
	FourTypeGift::_type = type;
	FourTypeGift* giftLayer = FourTypeGift::create();
	Director::getInstance()->getRunningScene()->addChild(giftLayer, 100, "giftlayer");
}

void CMOverLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(MyTransitionScene::scene());
	}
}

void CMOverLayer::restart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ChallengeMode cmode = GlobalData::getChallengeMode();
		int _useHpCount = CLASICUSEHP;
		if (GlobalData::getCModeCount(cmode) > 0)
			_useHpCount = 0;
		else
		{
			if (cmode == CMCLASIC)
				_useHpCount = CLASICUSEHP;
			else
				_useHpCount = 3;
		}

		GlobalData::setCurStageUseHp(_useHpCount);

		if (GlobalData::getHpCount() >= _useHpCount)
		{
			GlobalData::setHpCount(GlobalData::getHpCount() - GlobalData::getCurStageUseHp());

			if (g_gameScene != NULL)
			{
				this->removeFromParentAndCleanup(true);

				if (cmode == CMCLASIC)
					g_gameScene->restartClasic();
				else
					g_gameScene->restartFlash();

				if (GlobalData::getCurStageUseHp() > 0)
				{
					Achieve *data = GlobalData::getTaskDataByID(7);
					if (data->finish != -1)
					{
						data->finish += GlobalData::getCurStageUseHp();
						GlobalData::SaveTaskData();
					}
				}
			}
		}
		else
		{
			HpBuyLayer::AnimType = A_UPTODOWN;
			HpBuyLayer* layer = HpBuyLayer::create();
			this->addChild(layer, 100);
		}
	}
}

void CMOverLayer::share(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		utils::captureScreen(CC_CALLBACK_2(CMOverLayer::screenShot, this), "share.jpg");
	}
}

void CMOverLayer::screenShot(bool b, const std::string& name){

	if (b)
	{
		GlobalData::setShareImgPath(name);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo methodInfo;
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Share", "shareImg", "()V"))
		{
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		}
#endif
	}

}

void CMOverLayer::showStarParticle(float dt)
{
	static int randNum = 0;
	randNum += 60 * 60 *1000;
	int rand_seed = GlobalData::getSystemSecTime();
	rand_seed += randNum;
	srand(rand_seed);

	int x = (rand() % 28 + 12) * 10;
	int y = (rand() % 10 + 48) * 10;
	MyParticle::Show(this, SpeciallyName[7], x, y, true, 0.5f);
	MyParticle::Show(this, SpeciallyName[8], x, y, true, 0.3f);
	MyParticle::Show(this, SpeciallyName[9], x, y, true, 0.5f);
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_FIREWORKS);
}

void CMOverLayer::delayPlayAnim(float dt)
{
	showBtn();
	//Sprite* light = Sprite::createWithSpriteFrameName("UI/chgjs_0.png");
	//light->setPosition(Vec2(270, 700));
	//light->runAction(RepeatForever::create(RotateTo::create(4, 720)));
	//addChild(light, -2);
}

void CMOverLayer::showBtn()
{
	m_restartBtn->setVisible(true);
#ifdef THIRDLOGIN
	m_shareBtn->setVisible(true);
#else
	m_shareBtn->setVisible(false);
	m_restartBtn->setPositionX(270);
#endif
}