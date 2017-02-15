#include "PauseLayer.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "ThemeScene.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "HpBuyLayer.h"
#include "MyTransitionScene.h"
PauseLayer::PauseLayer()
{
}


PauseLayer::~PauseLayer()
{
}

void PauseLayer::onExit()
{
	Layer::onExit();
	pauselayer = NULL;
}

bool PauseLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	Sprite* context = Sprite::create("Images/pausecontext.png");
	context->setPosition(270, 395);
	this->addChild(context);

	Node * layer = CSLoader::createNode("pauseLayer.csb");
	addChild(layer);

	_musicBtn = (cocos2d::ui::Button*)layer->getChildByName("settingmusicbtn");
	_musicBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::musicOption, this));
	_musicoff = (cocos2d::ui::Widget*)layer->getChildByName("settingmusicoff");

	bool musicon = UserDefault::getInstance()->getBoolForKey("ismusicon", true);
	_musicoff->setVisible(!musicon);
	_musicBtn->setBright(musicon);

	_soundBtn = (cocos2d::ui::Button*)layer->getChildByName("settingsoundbtn");
	_soundBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::soundOption, this));
	_soundoff = (cocos2d::ui::Widget*)layer->getChildByName("settingsoundoff");

	bool soundon = UserDefault::getInstance()->getBoolForKey("issoundon", true);
	_soundoff->setVisible(!soundon);
	_soundBtn->setBright(soundon);

	cocos2d::ui::Widget* gohomebtn = (cocos2d::ui::Widget*)layer->getChildByName("gohomebtn");
	gohomebtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::gohome, this));

	cocos2d::ui::Widget* restartBtn = (cocos2d::ui::Widget*)layer->getChildByName("restartbtn");
	restartBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::restartGame, this));

	cocos2d::ui::TextBMFont* hpbmLbl = (cocos2d::ui::TextBMFont*)restartBtn->getChildByName("usehpbmlbl");

	if (GlobalData::getCModeCount(GlobalData::getChallengeMode()) > 0)
	{
		//restartBtn->getChildByName("freetext")->setVisible(true);
		hpbmLbl->setVisible(false);
	}

	cocos2d::ui::Widget* continueBtn = (cocos2d::ui::Widget*)layer->getChildByName("continuebtn");
	continueBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::close, this));

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::close, this));


	int usehp = GlobalData::getCurStageUseHp();
	if (GlobalData::getChallengeMode() == CMCLASIC)
	{
		if (GlobalData::getCModeCount(CMCLASIC) <= 0)
			usehp = CLASICUSEHP;
	}

	if (GlobalData::getChallengeMode() == CMFLASH)
	{
		if (GlobalData::getCModeCount(CMFLASH) <= 0)
			usehp = 3;
	}

	string usehpstr = StringUtils::format("x%d", usehp);
	hpbmLbl->setString(usehpstr);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//g_gameScene->saveData();

	layer->setAnchorPoint(Vec2(0.5f, 0.5f));
	layer->setPosition(Vec2(270, 480));
	layer->setScale(0.1f);
	layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));

	return true;
}

void PauseLayer::musicOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (!_musicoff->isVisible())
		{
			SoundManager::sharedManager()->setIsMusicOn(false);
			_musicoff->setVisible(true);
			SoundManager::sharedManager()->pauseBackMusic();
			UserDefault::getInstance()->setBoolForKey("ismusicon", false);
			_musicBtn->setBright(false);
		}
		else
		{
			SoundManager::sharedManager()->setIsMusicOn(true);
			_musicoff->setVisible(false);

			if (SoundManager::sharedManager()->getMusicPlayed())
				SoundManager::sharedManager()->resumeBackMusic();
			else
				SoundManager::sharedManager()->playBackMusic("sound/bg.mp3");
			UserDefault::getInstance()->setBoolForKey("ismusicon", true);
			_musicBtn->setBright(true);
		}
	}
}

void PauseLayer::soundOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!_soundoff->isVisible())
		{
			SoundManager::sharedManager()->setIsSoundOn(false);
			_soundoff->setVisible(true);
			UserDefault::getInstance()->setBoolForKey("issoundon", false);
			_soundBtn->setBright(false);
		}
		else
		{
			SoundManager::sharedManager()->setIsSoundOn(true);
			_soundoff->setVisible(false);
			UserDefault::getInstance()->setBoolForKey("issoundon", true);

			_soundBtn->setBright(true);
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_PAUSESOUNDBUTTON);
		}
	}
}

void PauseLayer::gohome(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(MyTransitionScene::scene());
	}
}

void PauseLayer::restartGame(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GlobalData::setIsRestart(true);
		Director::getInstance()->replaceScene(MyTransitionScene::scene());
//
//		if (GlobalData::getHpCount() >= GlobalData::getCurStageUseHp())
//		{
//			this->removeFromParentAndCleanup(true);
//			GlobalData::setHpCount(GlobalData::getHpCount() - GlobalData::getCurStageUseHp());
//			if (GlobalData::getChallengeMode() == CMCLASIC)
//				g_gameScene->restartClasic();
//			else if (GlobalData::getChallengeMode() == CMFLASH)
//				g_gameScene->restartFlash();
//			else if (GlobalData::getChallengeMode() == CMBOSS)
//				g_gameScene->resetBossStage();
//			else
//				g_gameScene->restart();
//
//			GlobalData::setIsRestart(true);
//			Director::getInstance()->replaceScene(ThemeScene::Scene());
//
//			if (GlobalData::getCurStageUseHp() > 0)
//			{
//				Achieve *data = GlobalData::getTaskDataByID(7);
//				if (data->finish != -1)
//				{
//					data->finish += GlobalData::getCurStageUseHp();
//					GlobalData::SaveTaskData();
//				}
//			}
//#ifdef ANALYTICS
//			JniMethodInfo methodInfo;
//			char p_str[32] = { 0 };
//			sprintf(p_str, "%s", "hp");
//			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
//			{
//				jstring para1 = methodInfo.env->NewStringUTF(p_str);
//				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, GlobalData::getCurStageUseHp());
//			}
//#endif
//		}
//		else
//		{
//			HpBuyLayer::AnimType = A_UPTODOWN;
//			HpBuyLayer* layer = HpBuyLayer::create();
//			this->addChild(layer, 100);
//		}
	}
}

void PauseLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		g_gameScene->gameResume();

	}
}