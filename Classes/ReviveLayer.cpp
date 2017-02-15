#include "ReviveLayer.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "Const.h"
#include "ThemeScene.h"
#include "HintBox.h"
#include "MyParticle.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "SureStageLayer.h"
#include "BossSettlement.h"
#include "CommonLayer.h"
#include "HpBuyLayer.h"
#include "MyTransitionScene.h"
int ReviveLayer::AnimType = A_SMALLTONORMAL;
ReviveLayer::ReviveLayer()
{

}
ReviveLayer::~ReviveLayer()
{

}

void ReviveLayer::onExit()
{
	Layer::onExit();
}

bool ReviveLayer::init()
{
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();


	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	_failNode = CSLoader::createNode("FailLayer.csb");
	addChild(_failNode);

	if (GlobalData::getChallengeMode() == CMBOSS)
	{
		_failNode->getChildByName("failtitle")->setVisible(false); 
		_failNode->getChildByName("failbosstitle")->setVisible(true);
	}
	m_restart = (cocos2d::ui::Widget*)_failNode->getChildByName("restartbtn");
	m_restart->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::restart, this));
	m_restart->setPositionX(270);

	string usehpstr = StringUtils::format("x%d", GlobalData::getCurStageUseHp());
	cocos2d::ui::TextBMFont* hpbmLbl = (cocos2d::ui::TextBMFont*)m_restart->getChildByName("usehpbmlbl");
	hpbmLbl->setString(usehpstr);


	cocos2d::ui::Widget* onsalebtn = (cocos2d::ui::Widget*)_failNode->getChildByName("onsalebtn");
	onsalebtn->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::onSale, this));
	onsalebtn->setVisible(false);

	m_close = (cocos2d::ui::Widget*)_failNode->getChildByName("failCloseBtn");
	m_close->addTouchEventListener(CC_CALLBACK_2(ReviveLayer::FailExit, this));

	int tipsindex[] = { 1, 2, 3, 5, 7, 9,15,16,17,18,19 };
	int rand_seed = GlobalData::getSystemSecTime();
	srand(rand_seed);
	int tipszise = sizeof(tipsindex) / sizeof(tipsindex[0]);
	int r = rand() % tipszise;
	cocos2d::ui::TextBMFont* tipsbmlbl = (cocos2d::ui::TextBMFont*)_failNode->getChildByName("tipsbmlbl");
	tipsbmlbl->setString(CommonFuncs::gbk2utf(tips[tipsindex[r]].c_str()));

	SoundManager::sharedManager()->stopBackMusic();
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_FAIL);

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

	MyParticle::Show(_failNode, SpeciallyName[48], 270, 900, true, -1.f);
	MyParticle::Show(_failNode, SpeciallyName[6], 270, 1000, false, 100);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	g_gameScene->clearData();

	//if (AnimType == A_SMALLTONORMAL)
	//{
	//	_failNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	//	_failNode->setPosition(Vec2(270, 480));
	//	_failNode->setScale(0.1f);
	//	_failNode->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	//}
	//else
	//{
	//	_failNode->setAnchorPoint(Vec2(0.0f, 0.0f));
	//	_failNode->setPosition(0, 960);
	//	_failNode->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
	//	AnimType = A_SMALLTONORMAL;
	//}
	_failNode->setOpacity(0);
	_failNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	_failNode->setPosition(Vec2(270, 480));
	_failNode->runAction(FadeIn::create(0.5f));
	int continuefailCount = GlobalData::getCurStageContinueFailCount();

	GlobalData::setCurStageContinueFailCount(continuefailCount + 1);

	return true;
}

void ReviveLayer::onEnterTransitionDidFinish()
{
	this->scheduleOnce(schedule_selector(ReviveLayer::delayShowReviveGift), 0.8f);
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };

	if (GlobalData::getChallengeMode() == CMBOSS)
		sprintf(p_str, "bossstage%d", GlobalData::getDefeatedBossCount() + 1);
	else if (GlobalData::getChallengeMode() == CMNORMAL)
		sprintf(p_str, "stage%d", GlobalData::getPlayingStage() + 1);

	if (GlobalData::getChallengeMode() == CMBOSS || GlobalData::getChallengeMode() == CMNORMAL)
	{
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "failLevel", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
	}
#endif
	
}

void ReviveLayer::delayShowReviveGift(float dt)
{
	if (GlobalData::checkPayCode(sizeof(gemNum) / sizeof(gemNum[0]), 1))
	{
		FourTypeGift::AnimType = A_UPTODOWN;
		FourTypeGift::_type = REVIVEGIFT;
		FourTypeGift* giftLayer = FourTypeGift::create();
		Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
	}
}

void ReviveLayer::onSale(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		FourTypeGift::_type = ONSALEGIFT;
		FourTypeGift::isClickPop = true;
		Layer* giftLayer = FourTypeGift::create();
		Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
	}
}

void ReviveLayer::FailExit(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getChallengeMode() == CMBOSS)
		{
			this->removeFromParentAndCleanup(true);
			delayShowBossSettlement();
		}
		else
		{
			if (g_gameScene != NULL)
			{
				Director::getInstance()->replaceScene(MyTransitionScene::scene());
			}
			else
				this->removeFromParentAndCleanup(true);
		}
	}
}

void ReviveLayer::restart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		GlobalData::setIsRestart(true);
		Director::getInstance()->replaceScene(MyTransitionScene::scene());

//		if (GlobalData::getHpCount() >= GlobalData::getCurStageUseHp())
//		{
//			this->removeFromParentAndCleanup(true);
//
//			GlobalData::setHpCount(GlobalData::getHpCount() - GlobalData::getCurStageUseHp());
//			if (GlobalData::getChallengeMode() == CMNORMAL)
//				g_gameScene->restart();
//			else if (GlobalData::getChallengeMode() == CMBOSS)
//				g_gameScene->resetBossStage();
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
//			Director::getInstance()->getRunningScene()->addChild(layer, 100);
//		}
	}
}

void ReviveLayer::delayShowBossSettlement()
{
	if (g_gameScene != NULL)
		g_gameScene->scheduleOnce(schedule_selector(ReviveLayer::enterBossSettlementLayer), 0.8f);

}

void ReviveLayer::enterBossSettlementLayer(float dt)
{
	BossSettlement* pLayer = BossSettlement::create();
	Director::getInstance()->getRunningScene()->addChild(pLayer, 1000);
}

void ReviveLayer::delayPlayWinSound(float dt)
{
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_WIN);
}