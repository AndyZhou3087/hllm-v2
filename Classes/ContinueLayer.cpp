#include "ContinueLayer.h"
#include "GlobalData.h"
#include "HintBox.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "CommonLayer.h"
int ContinueLayer::AnimType = A_SMALLTONORMAL;
ContinueLayer::ContinueLayer()
{
}


ContinueLayer::~ContinueLayer()
{
	AnimType = A_SMALLTONORMAL;
}
bool ContinueLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	_rootlayer = CSLoader::createNode("continueLayer.csb");
	addChild(_rootlayer);


	okBtn = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buypropbtn");
	okBtn->addTouchEventListener(CC_CALLBACK_2(ContinueLayer::ok, this));

	colseBtn = (cocos2d::ui::Widget*)_rootlayer->getChildByName("close");
	colseBtn->addTouchEventListener(CC_CALLBACK_2(ContinueLayer::close, this));

	cocos2d::ui::Widget* light = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buylight");
	light->runAction(RepeatForever::create(RotateTo::create(10,720)));

	string propStr = "buypropclock.png";
	Sprite * buygoods = Sprite::createWithSpriteFrameName(propStr);
	buygoods->setPosition(Vec2(light->getPositionX(), light->getPositionY()));
	_rootlayer->addChild(buygoods);

	int count = GlobalData::getAddtimeProp();
	usetext = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buypropbtn")->getChildByName("usetext");
	buyusetext = (cocos2d::ui::Widget*)_rootlayer->getChildByName("buypropbtn")->getChildByName("buyusetext");
	if (count > 0)
	{
		Sprite *redpoint = Sprite::createWithSpriteFrameName("UI/startredpoint.png");
		redpoint->setPosition(buygoods->getPositionX() + redpoint->getContentSize().width / 2 + 40, buygoods->getPositionY() + redpoint->getContentSize().height / 2 + 25);
		redpoint->setScale(1.5f);
		_rootlayer->addChild(redpoint);

		int addtimepropnum = GlobalData::getAddtimeProp();
		string propcount = StringUtils::format("%d", addtimepropnum);
		Label* countLabel = Label::createWithSystemFont(propcount, FONT_NAME, 20);
		countLabel->setPosition(Vec2(redpoint->getPositionX() - 3, redpoint->getPositionY() + 2));
		countLabel->setColor(Color3B(255, 255, 255));
		if (addtimepropnum >= 100)
			countLabel->setScale(0.8f);
		_rootlayer->addChild(countLabel);
		usetext->setVisible(true);
		_rootlayer->getChildByName("buypropbtn")->getChildByName("usepropbox")->setVisible(false);
		buyusetext->setVisible(false);
	}
	else
	{
		usetext->setVisible(false);
		buyusetext->setVisible(true);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (AnimType == A_SMALLTONORMAL)
	{
		_rootlayer->setAnchorPoint(Vec2(0.5f, 0.5f));
		_rootlayer->setPosition(Vec2(270, 480));
		_rootlayer->setScale(0.1f);
		_rootlayer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5, 1)), 2));
	}
	else
	{
		_rootlayer->setPosition(0, 960);
		_rootlayer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
	}

	this->scheduleOnce(schedule_selector(ContinueLayer::delayShowNewerGuide), 0.6f);
	return true;
}

void ContinueLayer::ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (GlobalData::checkGuide(19))
		{
			if (g_NewerLayer != NULL)
				g_NewerLayer->removeSelf();
			useProp(P_CLOCK);
			resumeGame();
			return;
		}
		if (GlobalData::getAddtimeProp() > 0)
		{
			GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() - 1);
			useProp(P_CLOCK);
			resumeGame();
			Achieve* data = GlobalData::getAchieveDataByID(7);
			if (data->finish != -1)
			{
				data->finish++;
				GlobalData::SaveAchieveData();
			}
		}
		else if (GlobalData::getGemCount() >= 20)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() - 20);
			useProp(P_CLOCK);
			resumeGame();
			Achieve* data = GlobalData::getAchieveDataByID(7);
			if (data->finish != -1)
			{
				data->finish++;
				GlobalData::SaveAchieveData();
			}
		}
		else
		{
			HintBox* hint = HintBox::create("钻石不足", 1);
			addChild(hint, 1000);
		}
	}
}

void ContinueLayer::useProp(PropsType type)
{
	g_gameScene->useClock();
}

void ContinueLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		resumeGame();
	}
}

void ContinueLayer::resumeGame()
{
	this->removeFromParentAndCleanup(true);
	g_gameScene->gameResume();
}


void ContinueLayer::delayShowNewerGuide(float dt)
{
	if (GlobalData::checkGuide(19) && GlobalData::getAddtimeProp() > 0)
	{
		usetext->setVisible(true);
		buyusetext->setVisible(false);
		vector<Node*> nodes;
		nodes.push_back(okBtn);
		g_NewerLayer = NewerGuideLayer::create(19);
		_rootlayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
}