#include "CommonLayer.h"
#include "GlobalData.h"
#include "HpBuyLayer.h"
#include "MyParticle.h"
#include "SoundManager.h"
#include "NewerGuideLayer.h"
#include "SettingLayer.h"
#include "FourTypeGift.h"
#include "LoginRewardLayer.h"
#include "HintBox.h"
#include "FourTypeGift.h"
#include "ByGift.h"
#include "CommonFuncs.h"
#include "MoreLayer.h"
#include "LotteryLayer.h"
#include "TopComNode.h"
#include "ShopLayer.h"
#include "EventLayer.h"
#include "RankLayer.h"
#include "SureStageLayer.h"

using namespace std;

CommonLayer* g_commomLayer = NULL;

CommonLayer::CommonLayer()
{
	isnewerguiding = false;
}

CommonLayer::~CommonLayer()
{
	isnewerguiding = false;
	g_commomLayer = NULL;
}

bool CommonLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	_bgNode = CSLoader::createNode("CommonLayer.csb");
	addChild(_bgNode);

	auto action = CSLoader::createTimeline("CommonLayer.csb");
	_bgNode->runAction(action);
	action->gotoFrameAndPlay(0, 35, true);

	TopComNode* topnode = TopComNode::create();
	topnode->setPosition(Vec2(270, 910));
	_bgNode->addChild(topnode);


	string lightstr = StringUtils::format("giftbthlight_%d", 1);
	light = (cocos2d::ui::Widget*)_bgNode->getChildByName(lightstr);
	light->runAction(RepeatForever::create(RotateTo::create(4, 720)));

	m_onsaleGiftBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("onsaleGiftBtn");
	m_onsaleGiftBtn->setTag(HOME_ONSALE);
	m_onsaleGiftBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::onGift, this));

	m_shopBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("shopBtn");
	m_shopBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::Shop, this));

	m_vipBtnNode = (cocos2d::ui::Widget*)_bgNode->getChildByName("vipbtnnode");
	m_vipBtn = (cocos2d::ui::Widget*)m_vipBtnNode->getChildByName("vipbtn");
	m_vipBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::vip, this));
	m_vipBtn->setVisible(false);

	bool showvip = ByGift::checkprice() == -1 ? false : true;
	if (showvip)
	{
		m_vipBtn->setVisible(true);
		schedule(schedule_selector(CommonLayer::vipAnim), 0.8f);
	}

	resetBtnPos();
	m_moreBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("morebtn");
	m_moreBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::more, this));

	m_shareBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("sharebtn");
	m_shareBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::share, this));

#ifndef THIRDLOGIN
	m_shareBtn->setVisible(false);
#endif
	
	m_eventBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("enventbtn");
	m_eventBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::onevent, this));

#ifndef USENETWORK
	m_eventBtn->setVisible(false);
#endif
	onsaleRedPoint = (cocos2d::ui::Widget*)m_onsaleGiftBtn->getChildByName("startredpoint");
	moreRedPoint = (cocos2d::ui::Widget*)m_moreBtn->getChildByName("startredpoint");
	vipRedPoint = (cocos2d::ui::Widget*)m_vipBtn->getChildByName("startredpoint");
	eventRedPoint = (cocos2d::ui::Widget*)m_eventBtn->getChildByName("startredpoint");

	m_BossChallengeBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("bossBtn");
	m_BossChallengeBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::BossChallenge, this));
	m_BossChallengeBtn->setVisible(GlobalData::getHighestStage() + 1 >= BOSSOPENSTAGE);

	m_ClasicBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("clasicbtn");
	m_ClasicBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::ClasicChallenge, this));
	m_ClasicBtn->setVisible(GlobalData::getHighestStage() + 1 >= CLASICOPENSTAGE);

	m_flashBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("flashbtn");
	m_flashBtn->addTouchEventListener(CC_CALLBACK_2(CommonLayer::FlashChallenge, this));
	m_flashBtn->setVisible(GlobalData::getHighestStage() + 1 >= FLASHOPENSTAGE);

	clasicRedPoint = (cocos2d::ui::Widget*)m_ClasicBtn->getChildByName("startredpoint");
	clasicRedPoint->setVisible(false);

	flashRedPoint = (cocos2d::ui::Widget*)m_flashBtn->getChildByName("startredpoint");
	flashRedPoint->setVisible(false);

	int curyearofday = GlobalData::getDayOfYear();
	if (GlobalData::getCModeYearOfDay(CMCLASIC) != curyearofday)
	{
		GlobalData::setCModeCount(CMCLASIC, 1);
	}
	if (GlobalData::getCModeYearOfDay(CMFLASH) != curyearofday)
	{
		GlobalData::setCModeCount(CMFLASH, 1);
	}

	updateUI();

	if (GlobalData::getHighestStage() >= 0)
	{
		checkShowNewerGuide();
		if (isnewerguiding)
		{
			scheduleOnce(schedule_selector(CommonLayer::delayShowNewerGuide), 0.5f);
		}
		else
		{
			if (GlobalData::getHighestStage() >= 4 && !GlobalData::getIsGetLoginReward())
				scheduleOnce(schedule_selector(CommonLayer::delayShowLoginReward), 0.5f);
		}
	}
	else
	{
		scheduleOnce(schedule_selector(CommonLayer::delayShowNewerGuide), 0.5f);
	}

	if (!isnewerguiding && GlobalData::getIsGetLoginReward())
	{
		scheduleOnce(schedule_selector(CommonLayer::delayShowGift), 0.75f);
	}

	return true;
}

void CommonLayer::checkShowNewerGuide()
{
	if (GlobalData::checkGuide(21) && GlobalData::getHighestStage() == 8)
	{
		isnewerguiding = true;
	}
#ifdef USENETWORK
	else if (GlobalData::checkGuide(17) && GlobalData::getHighestStage() == 6)
	{
		isnewerguiding = true;
	}
#endif
	else if (GlobalData::checkGuide(47) && GlobalData::getHighestStage() + 1 == CLASICOPENSTAGE)
	{
		isnewerguiding = true;
	}
	else if (GlobalData::checkGuide(48) && GlobalData::getHighestStage() + 1 == FLASHOPENSTAGE)
	{
		isnewerguiding = true;
	}
	else if (GlobalData::checkGuide(23) && GlobalData::getHighestStage() + 1 == BOSSOPENSTAGE)
	{
		isnewerguiding = true;
	}
		
}

void CommonLayer::delayShowNewerGuide(float t)
{
	std::vector<Node*> nodes;

	if (GlobalData::checkGuide(0))
	{
		if (GlobalData::getHighestStage() < 0)
		{
			g_NewerLayer = NewerGuideLayer::create(0);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
#ifdef USENETWORK
	if (GlobalData::checkGuide(17))
	{
		if (GlobalData::getHighestStage() == 6)
		{
			AutoclickMoreBtn();
			return;
		}
	}
#endif
	if (GlobalData::checkGuide(21))
	{
		if (GlobalData::getHighestStage() == 8)
		{
			AutoclickMoreBtn();
			return;
		}
	}

	if (GlobalData::checkGuide(47))
	{
		if (GlobalData::getHighestStage() + 1 >= CLASICOPENSTAGE)
		{
			nodes.push_back(m_ClasicBtn);
			g_NewerLayer = NewerGuideLayer::create(47);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}

	if (GlobalData::checkGuide(48))
	{
		if (GlobalData::getHighestStage() + 1 >= FLASHOPENSTAGE)
		{
			nodes.push_back(m_flashBtn);
			g_NewerLayer = NewerGuideLayer::create(48);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}

	if (GlobalData::checkGuide(23))
	{
		if (GlobalData::getHighestStage() + 1 >= BOSSOPENSTAGE)
		{
			nodes.push_back(m_BossChallengeBtn);
			g_NewerLayer = NewerGuideLayer::create(23);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
}

void CommonLayer::vipAnim(float dt)
{
	MyParticle::Show(m_vipBtn, SpeciallyName[31], m_vipBtn->getContentSize().width/2 - 5, 60, true, 0.1f);

	MyParticle::Show(m_vipBtn, SpeciallyName[32], m_vipBtn->getContentSize().width / 2 - 5, 60, true, 0.1f);
	MyParticle::Show(m_vipBtn, SpeciallyName[33], m_vipBtn->getContentSize().width / 2 - 5, 60, true, 0.1f);
	MyParticle::Show(m_vipBtn, SpeciallyName[34], m_vipBtn->getContentSize().width / 2 - 5, 60, true, 0.1f);
}

void CommonLayer::updateRedPoint(float dt)
{
	resetBtnPos();
	moreRedPoint->setVisible(!GlobalData::getIsGetLoginReward() || MoreLayer::isHasRedPoint());

	eventRedPoint->setVisible(hasNewEvent());

	int cmcount = GlobalData::getCModeCount(CMCLASIC);

	if (cmcount > 0)
	{
		std::string countstr = StringUtils::format("%d", cmcount);
		cocos2d::ui::Text* countlbl = (cocos2d::ui::Text*)clasicRedPoint->getChildByName("countlbl");
		countlbl->setString(countstr);
		clasicRedPoint->setVisible(true);
	}
	else
	{
		clasicRedPoint->setVisible(false);
	}

	cmcount = GlobalData::getCModeCount(CMFLASH);
	if (cmcount > 0)
	{
		std::string countstr = StringUtils::format("%d", cmcount);
		cocos2d::ui::Text* countlbl = (cocos2d::ui::Text*)flashRedPoint->getChildByName("countlbl");
		countlbl->setString(countstr);
		flashRedPoint->setVisible(true);
	}
	else
	{
		flashRedPoint->setVisible(false);
	}
}

void CommonLayer::updateUI()
{
	schedule(schedule_selector(CommonLayer::updateRedPoint), 0.5f);
}

void CommonLayer::AutoclickMoreBtn()
{
	more(m_moreBtn, ui::Widget::TouchEventType::ENDED);
}

void CommonLayer::setMorebtnVisible(bool val)
{
	m_moreBtn->setVisible(val);
}

void CommonLayer::vip(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ByGift::isClickPop = true;
		Layer* layer = ByGift::create();
		this->addChild(layer);
	}

}

void CommonLayer::more(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		setMorebtnVisible(false);
		MoreLayer* layer = MoreLayer::create();
		this->addChild(layer);
	}
}

void CommonLayer::share(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo methodInfo;
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Share", "showShare", "()V"))
		{
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		}
#endif
	}
}

void CommonLayer::onevent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GlobalData::saveEventsIds();
		EventLayer* layer = EventLayer::create();
		g_EventLayer = layer;
		this->addChild(layer);
	}
}

void CommonLayer::Shop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer *pShop = ShopLayer::create();
		addChild(pShop);
	}
}

void CommonLayer::BossChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getHighestStage() + 1 >= BOSSOPENSTAGE)
		{
			if (GlobalData::checkGuide(23))
			{
				g_commomLayer->isnewerguiding = false;
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
			}
			showCMStartFrame(CMBOSS);
		}
		else
		{
			string tipstr = StringUtils::format("通关第%d关解锁", BOSSOPENSTAGE);
			HintBox* hint = HintBox::create(tipstr, 0);
			addChild(hint, 1000);
		}
	}
}

void CommonLayer::ClasicChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (GlobalData::getHighestStage() + 1 >= CLASICOPENSTAGE)
		{
			if (GlobalData::checkGuide(47))
			{
				g_commomLayer->isnewerguiding = false;
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
			}

			showCMStartFrame(CMCLASIC);
		}
		else
		{
			string tipstr = StringUtils::format("通关第%d关解锁", CLASICOPENSTAGE);
			HintBox* hint = HintBox::create(tipstr, 0);
			addChild(hint, 1000);
		}
	}
}

void CommonLayer::FlashChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getHighestStage() + 1 >= FLASHOPENSTAGE)
		{
			if (GlobalData::checkGuide(48))
			{
				g_commomLayer->isnewerguiding = false;
				if (g_NewerLayer != NULL)
					g_NewerLayer->removeSelf();
			}
			showCMStartFrame(CMFLASH);
		}
		else
		{
			string tipstr = StringUtils::format("通关第%d关解锁", FLASHOPENSTAGE);
			HintBox* hint = HintBox::create(tipstr, 0);
			addChild(hint, 1000);
		}
	}
}
void CommonLayer::showCMStartFrame(int mode)
{
#ifdef USENETWORK
	RankLayer::rankMode = mode;
	Layer* layer = RankLayer::create();
	this->addChild(layer);
#else
	GlobalData::setChallengeMode((ChallengeMode)mode);
	Layer* layer = SureStageLayer::create();
	this->addChild(layer);
#endif
}

void CommonLayer::Action(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION);
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION1);
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		((cocos2d::ui::Widget*)pSender)->runAction(Button_ACTION1);
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
	}
}

bool CommonLayer::hasNewEvent()
{
	std::string locids = GlobalData::getEventsIds();
	std::string ids;
	for (unsigned int i = 0; i < GlobalData::EventsData.size(); i++)
	{
		std::string tmp = StringUtils::format("%d", GlobalData::EventsData[i].id);
		ids.append(tmp);
		ids.append("-");
	}
	if (ids.length() <= 0 || locids.compare(ids) == 0)
		return false;
	return true;

}

void CommonLayer::onGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btn = (Node*)pSender;
		FourTypeGift::_type = btn->getTag();
		FourTypeGift::isClickPop = true;
		Layer* giftLayer = FourTypeGift::create();
		Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
	}
}

void CommonLayer::delayShowLoginReward(float dt)
{
	LoginRewardLayer::AnimType = A_UPTODOWN;
	LoginRewardLayer* layer = LoginRewardLayer::create();
	Director::getInstance()->getRunningScene()->addChild(layer, 100);
}

void CommonLayer::delayShowGift(float dt)
{
	if (GlobalData::getIsHomeAutoPopGift())
	{
		showGift(0);
	}
	GlobalData::setIsHomeAutoPopGift(false);
}

void CommonLayer::showGift(int pos)
{
#ifdef AUTOPOPGIFT
	const char* str = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/OGThranPay", "checkPCode", "(I)Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, pos);
		str = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif
	int index = -1;
	int pcodecount = sizeof(PayCode) / sizeof(PayCode[0]);
	for (int i = 0; i < pcodecount; i++)
	{
		if (PayCode[i].compare(str) == 0)
		{
			index = i;
			break;
		}
	}
	int gindex = index - sizeof(gemNum) / sizeof(gemNum[0]);
	if (gindex >= 0)
	{
		if (gindex < GJGIFT)
		{
			FourTypeGift::AnimType = A_UPTODOWN;
			FourTypeGift::_type = gindex;
			Layer* giftLayer = FourTypeGift::create();
			Director::getInstance()->getRunningScene()->addChild(giftLayer, 1000, "giftlayer");

		}
		else if (gindex >= GJGIFT && gindex < ENDGIFT)
		{
			Layer* giftLayer = ByGift::create();
			Director::getInstance()->getRunningScene()->addChild(giftLayer, 1000, "giftlayer");
		}
	}
#endif
}

void CommonLayer::resetBtnPos()
{
	int gemcount = sizeof(gemNum) / sizeof(gemNum[0]);
	bool ret = GlobalData::checkPayCode(gemcount + HOME_ONSALE, 1);
	m_onsaleGiftBtn->setVisible(ret);
	light->setVisible(ret);
	if (!ret)
		m_shopBtn->setPositionY(m_onsaleGiftBtn->getPositionY());
}