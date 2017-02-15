#include "ShopLayer.h"
#include "Const.h"
#include "GlobalData.h"
#include "HintBox.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "ComfirmBuyLayer.h"
#include "ReviveLayer.h"
#include "CommonLayer.h"
#include "EventLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif
int ShopLayer::whichClicked = 2;
int ShopLayer::_payIndex = 0;
cocos2d::ui::TextBMFont* ShopLayer::myPrompLbl = NULL;
cocos2d::ui::TextBMFont* ShopLayer::myRefreshLbl = NULL;
cocos2d::ui::TextBMFont* ShopLayer::myAddTimeLbl = NULL;
Node* g_shopCsbLayer = NULL;

static int price = 0;
int ShopLayer::AnimType = A_SMALLTONORMAL;
bool ShopLayer::isPaying = false;

ShopLayer::ShopLayer() 
{
	m_isScrolling = false;
	isPaying = false;
}

ShopLayer::~ShopLayer()
{
	whichClicked = 2;
	myPrompLbl = NULL;
	myRefreshLbl = NULL;
	myAddTimeLbl = NULL;
	g_shopCsbLayer = NULL;
}

bool ShopLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	isFirstEnter = true;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	g_shopCsbLayer = CSLoader::createNode("ShopLayer.csb");
	addChild(g_shopCsbLayer); 

	checkGemPCode();
	checkGiftPCode();

	LoadingShop();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void ShopLayer::LoadingShop()
{
	myPrompLbl = (cocos2d::ui::TextBMFont*)g_shopCsbLayer->getChildByName("ScrollView_2")->getChildByName("myprompttext")->getChildByName("mypromptnum");
	myRefreshLbl = (cocos2d::ui::TextBMFont*)g_shopCsbLayer->getChildByName("ScrollView_2")->getChildByName("myrefreshtext")->getChildByName("myrefreshnum");
	myAddTimeLbl = (cocos2d::ui::TextBMFont*)g_shopCsbLayer->getChildByName("ScrollView_2")->getChildByName("myaddtimetext")->getChildByName("myaddtimenum");
	reFreshPropLable();
	cocos2d::ui::Widget* m_shopCloseBtn = (cocos2d::ui::Widget*)g_shopCsbLayer->getChildByName("shopCloseBtn");
	m_shopCloseBtn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::ShopClose, this));

	char pBuff[20] = { 0 };
	memset(pBuff, 0x00, sizeof(pBuff));
	for (int i = 0; i < 3; i++)
	{
		sprintf(pBuff, "shopExcheng%d", i + 1);
		m_shopExchangBtn[i] = (cocos2d::ui::Widget*)g_shopCsbLayer->getChildByName(pBuff);
		m_shopExchangBtn[i]->addTouchEventListener(CC_CALLBACK_2(ShopLayer::Exchang, this));

		sprintf(pBuff, "ScrollView_%d", i + 1);
		m_ScrollView[i] = (cocos2d::ui::ScrollView*)g_shopCsbLayer->getChildByName(pBuff);

		sprintf(pBuff, "x_%d_0", i);
		m_shop[i] = (cocos2d::ui::Widget*)g_shopCsbLayer->getChildByName(pBuff);


		if (i == 0)
		{
			
			int gemcount = sizeof(gemNum) / sizeof(gemNum[0]);
			int glodcount = sizeof(goldNum) / sizeof(goldNum[0]);
			int gemAndGoldCount = vec_gemindex.size();

			int itemheight = 120;
			m_ScrollView[i]->setInnerContainerSize(Size(m_ScrollView[i]->getInnerContainerSize().width, gemAndGoldCount * (itemheight)));
			for (int k = 0; k < gemcount + glodcount; k++)
			{
				sprintf(pBuff, "shopPropBg_%d", k + 1);
				m_buyDiamondItem[k] = (cocos2d::ui::Widget*)m_ScrollView[i]->getChildByName(pBuff);
				m_buyDiamondItem[k]->setVisible(false);
				sprintf(pBuff, "buy_%d", k + 1);
				m_buyDiamondBtn[k] = (cocos2d::ui::Widget*)m_ScrollView[i]->getChildByName(pBuff);
				m_buyDiamondBtn[k]->setVisible(false);
			}

			for (int m = 0; m < gemAndGoldCount;m++)
			{
				int gemindex = vec_gemindex[m];
				m_buyDiamondItem[gemindex]->setVisible(true);
				m_buyDiamondItem[gemindex]->setPositionY(m_ScrollView[i]->getInnerContainerSize().height - (itemheight + 10)/ 2 - m * (itemheight));
				cocos2d::ui::TextBMFont* numlbl = (cocos2d::ui::TextBMFont*)m_buyDiamondItem[gemindex]->getChildByName("gemnumlbl");
				if (gemindex < gemcount)
					sprintf(pBuff, "%d", gemNum[gemindex]);
				else
					sprintf(pBuff, "%d", goldNum[gemindex - gemcount]);
				numlbl->setString(pBuff);
				m_buyDiamondBtn[gemindex]->setVisible(true);
				m_buyDiamondBtn[gemindex]->setPositionY(m_buyDiamondItem[gemindex]->getPositionY());
				m_buyDiamondBtn[gemindex]->setTag(gemindex);
				m_buyDiamondBtn[gemindex]->addTouchEventListener(CC_CALLBACK_2(ShopLayer::BuyGameCurrency, this));
				if (gemindex < gemcount)
					sprintf(pBuff, "¥%.2f", gemprice[gemindex]);
				else
					sprintf(pBuff, "x%d", goldprice[gemindex - gemcount]);
				numlbl = (cocos2d::ui::TextBMFont*)m_buyDiamondBtn[gemindex]->getChildByName("pricelbl");
				numlbl->setString(CommonFuncs::gbk2utf(pBuff));
			}
			//m_ScrollView[i]->setPosition(Vec2(270, 420));
			m_ScrollView[i]->setPosition(Vec2(270, 320));
		}

		else if (i == 1)
		{
			for (int n = 0; n < 3;n++)
			{
				sprintf(pBuff, "shopPropBg_%d", n + 1);
				cocos2d::ui::TextBMFont* numlbl = (cocos2d::ui::TextBMFont*)m_ScrollView[i]->getChildByName(pBuff)->getChildByName("propnumlbl");
				sprintf(pBuff, "x%d", propNum[n]);
				numlbl->setString(pBuff);
				sprintf(pBuff, "buy_%d", n + 1);
				m_buyPropBtn[n] = (cocos2d::ui::Widget*)m_ScrollView[i]->getChildByName(pBuff);
				m_buyPropBtn[n]->setTag(n + 1);
				m_buyPropBtn[n]->addTouchEventListener(CC_CALLBACK_2(ShopLayer::BuyProp, this));
				sprintf(pBuff, "x%d", buyPropGemOrGoldNum[n]);
				numlbl = (cocos2d::ui::TextBMFont*)m_buyPropBtn[n]->getChildByName("pricelbl");
				numlbl->setString(pBuff);
			}
			//m_ScrollView[i]->setPosition(Vec2(270, 410));
			m_ScrollView[i]->setPosition(Vec2(270, 320));
		}
		else
		{
			cocos2d::ui::Widget* giftbg[sizeof(giftNum) / sizeof(giftNum[0])];

			int giftcount = vec_giftindex.size();
			int totalgiftcount = sizeof(giftNum) / sizeof(giftNum[0]);

			int itemheight = 185;
			m_ScrollView[i]->setInnerContainerSize(Size(m_ScrollView[i]->getInnerContainerSize().width, giftcount * (itemheight)));

			for (int k = 0; k < totalgiftcount; k++)
			{
				sprintf(pBuff, "shopGiftbg_%d", k + 1);
				giftbg[k] = (cocos2d::ui::Widget*)m_ScrollView[i]->getChildByName(pBuff);
				giftbg[k]->setVisible(false);

				sprintf(pBuff, "buy_%d", k + 1);
				m_buyGiftBtn[k] = (cocos2d::ui::Widget*)m_ScrollView[i]->getChildByName(pBuff);
				m_buyGiftBtn[k]->setVisible(false);
			}

			for (int j = 0; j < giftcount;j++)
			{
				int giftindex = vec_giftindex[j];
				giftbg[giftindex]->setVisible(true);

				giftbg[giftindex]->setPositionY(m_ScrollView[i]->getInnerContainerSize().height - (itemheight + 8) / 2 - j * (itemheight));

				cocos2d::ui::TextBMFont* numlbl = (cocos2d::ui::TextBMFont*)giftbg[giftindex]->getChildByName("propnumlbl1");
				if (giftNum[giftindex][0] > 0)
				{
					sprintf(pBuff, "x%d", giftNum[giftindex][0]);
					numlbl->setString(pBuff);
				}
				if (giftNum[giftindex][1] > 0)
				{
					numlbl = (cocos2d::ui::TextBMFont*)giftbg[giftindex]->getChildByName("propnumlbl2");
					sprintf(pBuff, "x%d", giftNum[giftindex][1]);
					numlbl->setString(pBuff);
				}
				if (giftNum[giftindex][2] > 0)
				{
					numlbl = (cocos2d::ui::TextBMFont*)giftbg[giftindex]->getChildByName("propnumlbl3");
					sprintf(pBuff, "x%d", giftNum[giftindex][2]);
					numlbl->setString(pBuff);
				}
				if (giftNum[giftindex][3] > 0)
				{
					numlbl = (cocos2d::ui::TextBMFont*)giftbg[giftindex]->getChildByName("propnumlbl4");
					sprintf(pBuff, "x%d", giftNum[giftindex][3]);
					numlbl->setString(pBuff);
				}
				if (giftNum[giftindex][4] > 0)
				{
					numlbl = (cocos2d::ui::TextBMFont*)giftbg[giftindex]->getChildByName("propnumlbl5");
					sprintf(pBuff, "x%d", giftNum[giftindex][4]);
					numlbl->setString(pBuff);
				}
				m_buyGiftBtn[giftindex]->setVisible(true);
				m_buyGiftBtn[giftindex]->setPositionY(giftbg[giftindex]->getPositionY() - 40);
				m_buyGiftBtn[giftindex]->setTag(giftindex + sizeof(gemNum) / sizeof(gemNum[0]));
				m_buyGiftBtn[giftindex]->addTouchEventListener(CC_CALLBACK_2(ShopLayer::BuyPackage, this));
				sprintf(pBuff, "%d", pgiftprice[giftindex]);
				numlbl = (cocos2d::ui::TextBMFont*)giftbg[giftindex]->getChildByName("pricelbl");
				numlbl->setString(CommonFuncs::gbk2utf(pBuff));

				sprintf(pBuff, "%d.00", pgiftyuanprice[giftindex]);
				cocos2d::ui::TextBMFont* yuanpricelbl = (cocos2d::ui::TextBMFont*)giftbg[giftindex]->getChildByName("yuanprice");
				yuanpricelbl->setString(CommonFuncs::gbk2utf(pBuff));
			}

			//m_ScrollView[i]->setPosition(Vec2(800, 380));
			m_ScrollView[i]->setPosition(Vec2(270, 320));
		}
	}

	m_pTag = m_ScrollView[whichClicked];
	Exchang(m_shopExchangBtn[whichClicked], ui::Widget::TouchEventType::ENDED);

	if (AnimType == A_SMALLTONORMAL)
	{
		g_shopCsbLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
		g_shopCsbLayer->setPosition(Vec2(270, 480));
		g_shopCsbLayer->setScale(0.1f);
		g_shopCsbLayer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		g_shopCsbLayer->setAnchorPoint(Vec2(0.0f, 0.0f));
		g_shopCsbLayer->setPosition(0, 960);
		g_shopCsbLayer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}
}

void ShopLayer::BuyGameCurrency(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btn = (Node*)pSender;
		int tag = btn->getTag();
		int gemcount = sizeof(gemNum) / sizeof(gemNum[0]);
		if (tag < gemcount)
			beginPayment(tag);
		else
		{
			if (GlobalData::getGemCount() >= goldprice[tag - gemcount])
			{
				GlobalData::setGoldCount(GlobalData::getGoldCount() + goldNum[tag - gemcount]);
				GlobalData::setGemCount(GlobalData::getGemCount() - goldprice[tag - gemcount]);
				showBuyOKAnim("UI/gold.png", Vec2(btn->getParent()->getPositionX() + btn->getPositionX() + btn->getContentSize().width / 2 - 30, btn->getParent()->getPositionY() + btn->getPositionY() + 200), Vec2(388, 925));
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDCOIN);
				reFreshPropLable();

				HintBox* hint = HintBox::create("购买成功", 0);
				addChild(hint);

#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };
				sprintf(p_str, "%s", "coin");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					float perprice = ((float)goldprice[tag - gemcount])/goldNum[tag - gemcount];
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1,  goldNum[tag - gemcount], perprice);
				}
#endif
			}
			else
			{
				HintBox* hint = HintBox::create("钻石不足", 0);
				addChild(hint);
				Exchang(m_shopExchangBtn[0], ui::Widget::TouchEventType::ENDED);
			}
		}
	}
}

void ShopLayer::BuyProp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btn = (Node*)pSender;
		int tag = btn->getTag();
		reFreshPropLable();
		if (tag == 3)
		{
			if (GlobalData::getGemCount() >= buyPropGemOrGoldNum[tag - 1])
			{
				GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + propNum[tag - 1]);
				GlobalData::setGemCount(GlobalData::getGemCount() - buyPropGemOrGoldNum[tag - 1]);
				reFreshPropLable();
				HintBox* hint = HintBox::create("购买成功", 0);
				addChild(hint);

#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };
				sprintf(p_str, "%s", "addtime");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					float perprice = ((float)buyPropGemOrGoldNum[tag - 1])/propNum[tag - 1];
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, propNum[tag - 1], perprice);
				}
#endif
				return;
			}
		}
		else if (tag == 2)
		{
			if (GlobalData::getGemCount() >= buyPropGemOrGoldNum[tag - 1])
			{
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() + propNum[tag - 1]);
				GlobalData::setGemCount(GlobalData::getGemCount() - buyPropGemOrGoldNum[tag - 1]);
				reFreshPropLable();

				HintBox* hint = HintBox::create("购买成功", 0);
				addChild(hint);
#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };
				sprintf(p_str, "%s", "refresh");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					float perprice = ((float)buyPropGemOrGoldNum[tag - 1]) / propNum[tag - 1];
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, propNum[tag - 1], perprice);
				}
#endif
				return;
			}
		}
		else if (tag == 1)
		{
			if (GlobalData::getGoldCount() >= buyPropGemOrGoldNum[tag - 1])
			{
				GlobalData::setPromptProp(GlobalData::getPromptProp() + propNum[tag - 1]);
				GlobalData::setGoldCount(GlobalData::getGoldCount() - buyPropGemOrGoldNum[tag - 1]);
				reFreshPropLable();

				HintBox* hint = HintBox::create("购买成功", 0);
				addChild(hint);

#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };
				sprintf(p_str, "%s", "prompt");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					float perprice = ((float)buyPropGemOrGoldNum[tag - 1]) / propNum[tag - 1];
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, propNum[tag - 1], perprice);
				}
#endif
				return;
			}
		}

		string hitstr = "钻石不足";
		if (tag == 1)
			hitstr = "金币不足";
		HintBox* hint = HintBox::create(hitstr, 0);
		addChild(hint);
		Exchang(m_shopExchangBtn[0], ui::Widget::TouchEventType::ENDED);
	}
}

void ShopLayer::BuyPackage(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btn = (Node*)pSender;
		int tag = btn->getTag();
		beginPayment(tag);
	}
}
void ShopLayer::Exchang(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (m_isScrolling)
			return;
		if (!isFirstEnter)
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);

		isFirstEnter = false;
		for (int i = 0; i < 3; i++)
		{
			if (pSender == m_shopExchangBtn[i])
			{	
				m_shopExchangBtn[i]->setBright(false);
				m_isScrolling = true;
				/*
				Point size = m_pTag->getPosition();
				m_pTag->runAction(MoveTo::create(0.1f, m_ScrollView[i]->getPosition()));

				m_ScrollView[i]->runAction(Sequence::create(DelayTime::create(0.1f), MoveTo::create(0.1f, size), CallFunc::create(CC_CALLBACK_0(ShopLayer::scrollFinish, this)), NULL));
				*/
				m_ScrollView[i]->setVisible(true);
				scrollFinish();
				m_pTag = m_ScrollView[i];
				char pBuff[20] = { 0 };
				m_shop[i]->setVisible(false);

				sprintf(pBuff, "x_%d_1", i);
				g_shopCsbLayer->getChildByName(pBuff)->setVisible(true);
			}
			else
			{
				m_ScrollView[i]->setVisible(false);
				m_shopExchangBtn[i]->setBright(true);
				char pBuff[20] = { 0 };
				m_shop[i]->setVisible(true);

				sprintf(pBuff, "x_%d_1", i);
				g_shopCsbLayer->getChildByName(pBuff)->setVisible(false);
			}
		}
	}
}

void ShopLayer::scrollFinish()
{
	m_isScrolling = false;
}

void ShopLayer::reFreshPropLable()
{
	std::string numStr = StringUtils::format("%d", GlobalData::getPromptProp());
	if (myPrompLbl != NULL)
		myPrompLbl->setString(numStr);

	numStr = StringUtils::format("%d", GlobalData::getRefreshProp());
	if (myRefreshLbl != NULL)
		myRefreshLbl->setString(numStr);

	numStr = StringUtils::format("%d", GlobalData::getAddtimeProp());
	if (myAddTimeLbl != NULL)
		myAddTimeLbl->setString(numStr);
}

void ShopLayer::ShopClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		if (g_gameScene != NULL && g_gameScene->gamePlayingShowShop)
		{
			g_gameScene->gameResume();
		}
	}
}

void ShopLayer::beginPayment(int index)
{
	if (isPaying)
		return;
	isPaying = true;
	_payIndex = index;


#ifdef ANALYTICS

	if (_payIndex >= 0)
	{
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		int price = 0;
		int amount = 0;
		int gemPayCount = sizeof(gemNum) / sizeof(gemNum[0]);
		if (_payIndex < gemPayCount)
		{
			sprintf(p_str, "%s%d", "gem", gemNum[_payIndex]);
			price = (int)gemprice[_payIndex];
			amount =  gemNum[_payIndex];
		}
		else
		{
			sprintf(p_str, "%s", packagename[_payIndex - gemPayCount]);
			price = pgiftprice[_payIndex - gemPayCount];
			amount = gemAmount[_payIndex - gemPayCount];
		}
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "pay", "(Ljava/lang/String;II)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, price, amount);
		}
	}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", PayCode[index].c_str());
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/GMAction", "goby", "(Ljava/lang/String;)V"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, str1);
	}
#else
	setMessage(0);
#endif


}

void ShopLayer::setMessage(int ret)
{
	int gemPayCount = sizeof(gemNum) / sizeof(gemNum[0]);
	int gindex = 0;
	int buygemcount = 0;
	int coincount = 0;
	int promptcount = 0;
	int refreshcount = 0;
	int addtimecount = 0;
	int eventgiftindex = -1;
	int buyprice = 0;
	isPaying = false;
	if (ret == 0)
	{	
		if (_payIndex < gemPayCount)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() + gemNum[_payIndex]);
			//showBuyOKAnim("UI/diamond.png", Vec2(130 + _payIndex % 3 * 150, 500 -_payIndex / 3 * 220), Vec2(457, 920));
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDGEM);
			buygemcount = gemNum[_payIndex];
			buyprice = (int)gemprice[_payIndex];

#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s%d", "gem", buygemcount);
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
#endif
		}
		else
		{
			Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(ShopLayer::delayRemoveGiftLayer), 0.1f);
			gindex = _payIndex - gemPayCount;

			if (gindex == COINGIFT || gindex == ENTER_COINGIFT || gindex == P3_COINGIFT || gindex == LESS_COINGIFT)
			{
				int coinindex = COINGIFT;
				GlobalData::setGoldCount(GlobalData::getGoldCount() + giftNum[coinindex][0]);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDCOIN);


				GlobalData::setPromptProp(GlobalData::getPromptProp() + giftNum[coinindex][1]);
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() + giftNum[coinindex][2]);

				buygemcount = 0;
				eventgiftindex = COINGIFT;
				promptcount = giftNum[coinindex][1];
				refreshcount = giftNum[coinindex][2];
			}
			else if (gindex == PROPGIFT || gindex == ENTER_PROPGIFT || gindex == P3_PROPGIFT || gindex == GAME_PROPGIFT)
			{
				int propindex = PROPGIFT;
				GlobalData::setPromptProp(GlobalData::getPromptProp() + giftNum[propindex][0]);
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() + giftNum[propindex][1]);
				GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + giftNum[propindex][2]);

				if (g_gameScene != NULL && GlobalData::g_gamestatus != GAMEOVER)
				{
					g_gameScene->gameResume();
				}
				buygemcount = 0;
				eventgiftindex = PROPGIFT;
				promptcount = giftNum[propindex][0];
				refreshcount = giftNum[propindex][1];
				addtimecount = giftNum[propindex][2];

				GlobalData::setBuyPropGiftDay(GlobalData::getDayOfYear());
			}
			else if (gindex == GEMGIFT || gindex == ENTER_GEMGIFT || gindex == P3_GEMGIFT || gindex == LESS_GEMGIFT)
			{
				int gemindex = GEMGIFT;
				GlobalData::setGemCount(GlobalData::getGemCount() + giftNum[gemindex][0]);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDGEM);

				GlobalData::setPromptProp(GlobalData::getPromptProp() + giftNum[gemindex][1]);
				GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + giftNum[gemindex][2]);

				buygemcount = giftNum[gemindex][0];
				eventgiftindex = GEMGIFT;
				promptcount = giftNum[gemindex][1];
				addtimecount = giftNum[gemindex][2];
			}
			else if (gindex == RICHGIFT || gindex == ENTER_RICHGIFT || gindex == P3_RICHGIFT)
			{
				int richindex = RICHGIFT;
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDGEM);
				GlobalData::setGemCount(GlobalData::getGemCount() + giftNum[richindex][0]);
				GlobalData::setGoldCount(GlobalData::getGoldCount() + giftNum[richindex][1]);

				buygemcount = giftNum[richindex][0];
				coincount = giftNum[richindex][1];
				eventgiftindex = RICHGIFT;
			}
			else if (gindex == FIRSTGIFT)
			{
				GlobalData::setIsBuyGift(gindex, true);
				GlobalData::setGemCount(GlobalData::getGemCount() + 88);
				GlobalData::setPromptProp(GlobalData::getPromptProp() + 1);
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() + 1);

				buygemcount = 88;
				promptcount = 1;
				refreshcount = 1;

				eventgiftindex = FIRSTGIFT;
			}
			else if (gindex == ONSALEGIFT || gindex == ENTER_ONSALEGIFT || gindex == P3_ONSALEGIFT || gindex == HOME_ONSALE)
			{
				int onsaleindex = ONSALEGIFT;
				GlobalData::setGemCount(GlobalData::getGemCount() + giftNum[onsaleindex][0]);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDGEM);

				GlobalData::setGoldCount(GlobalData::getGoldCount() + giftNum[onsaleindex][1]);
				GlobalData::setPromptProp(GlobalData::getPromptProp() + giftNum[onsaleindex][2]);
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() + giftNum[onsaleindex][3]);
				GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + giftNum[onsaleindex][4]);
				GlobalData::setBuyOnsaleGiftDay(GlobalData::getDayOfYear());

				buygemcount = giftNum[onsaleindex][0];
				eventgiftindex = ONSALEGIFT;
				coincount = giftNum[onsaleindex][1];
				promptcount = giftNum[onsaleindex][2];
				refreshcount = giftNum[onsaleindex][3];
				addtimecount = giftNum[onsaleindex][4];
			}
			else if (gindex == NEWERGIFT)//新手
			{
				GlobalData::setIsBuyGift(gindex, true);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDGEM);
				GlobalData::setGemCount(GlobalData::getGemCount() + 300);
				buygemcount = 300;
				eventgiftindex = NEWERGIFT;
			}
			else if (gindex == REVIVEGIFT)//复活
			{
				GlobalData::setPromptProp(GlobalData::getPromptProp() + 2);
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() + 2);
				GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + 2);
				Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(ShopLayer::delayReviveGame), 0.1f);

				buygemcount = 0;
				promptcount = 2;
				refreshcount = 2;
				addtimecount = 2;
				eventgiftindex = REVIVEGIFT;
			}
			else if (gindex == CGGIFT || gindex == ENTER_CGGIFT || gindex == P3_CGGIFT)
			{
				GlobalData::setGemCount(GlobalData::getGemCount() + 100);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ADDGEM);
				GlobalData::setGoldCount(GlobalData::getGoldCount() + 1000);
				GlobalData::setPromptProp(GlobalData::getPromptProp() + 3);
				GlobalData::setRefreshProp(GlobalData::getRefreshProp() + 3);
				GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + 3);

				buygemcount = 100;
				eventgiftindex = CGGIFT;
				coincount = 1000;
				promptcount = 3;
				refreshcount = 3;
				addtimecount = 3;
			}
			else if (gindex >= GJGIFT && gindex < ENDGIFT)
			{
				GlobalData::setBYTime(GlobalData::getSystemSecTime());
				GlobalData::setBYMonth(GlobalData::getMonthOfYear());
				GlobalData::setBYDay(GlobalData::getDayOfMonth());
				int byprice[] = { 8, 10, 15, 20, 30 };
				int bygemcount[] = {6,8,15,25,35};
				GlobalData::setGemCount(GlobalData::getGemCount() + bygemcount[gindex - GJGIFT]);
				GlobalData::setBYPrice(byprice[gindex - GJGIFT]);
				buygemcount = bygemcount[gindex - GJGIFT];
				eventgiftindex = gindex;
			}
#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", packagename[gindex]);
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
#endif
			buyprice = pgiftprice[gindex];
		}
		reFreshPropLable();
#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "payok", "()V"))
		{
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		}

		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF("coin");
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, coincount, 0.0f);
		}
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF("prompt");
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, promptcount, 0.0f);
		}
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF("refresh");
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, refreshcount, 0.0f);
		}
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF("addtime");
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, addtimecount, 0.0f);
		}
#endif

	}

	Achieve* data = GlobalData::getAchieveDataByID(6);
	if (data->finish != -1)
	{
		data->finish += buygemcount;
		GlobalData::SaveAchieveData();
	}
#ifdef USENETWORK
	MEvent* mevent = GlobalData::getEventByType(2);
	bool ishasNotice = false;

	if (mevent != NULL && GlobalData::getEventstatus(2) != 1)
	{
		bool isfinished = false;
		int dataeindex = atoi(mevent->data);
		if (dataeindex >= 100)
		{
			if (dataeindex - 100 == eventgiftindex)
			{
				isfinished = true;
			}
		}
		else
		{
			if (dataeindex == _payIndex)
			{
				isfinished = true;
			}
		}
		if (isfinished)
		{
			int loctime = GlobalData::getSystemSecTime();
			if (loctime >= mevent->startime && loctime <= mevent->endtime)
			{
				EventLayer::doEventNotice(2);
			}
		}
	}

	mevent = GlobalData::getEventByType(3);
	if (mevent != NULL)
	{
		int lastcount = GlobalData::getEventstatus(3);
		if (lastcount < atoi(mevent->data))
		{
			int finishcount = lastcount + buyprice;
			if (finishcount >= atoi(mevent->data))
			{
				//notice
				int loctime = GlobalData::getSystemSecTime();
				if (loctime >= mevent->startime && loctime <= mevent->endtime)
				{
					EventLayer::doEventNotice(3);
				}
			}
			else
			{
				GlobalData::setEventstatus(3, finishcount);
			}
		}
	}
#endif
}

void ShopLayer::delayRemoveGiftLayer(float dt)
{
	Director::getInstance()->getRunningScene()->removeChildByName("giftlayer");

}
void ShopLayer::delayReviveGame(float dt)
{
	if (g_gameScene != NULL)
	{
		g_gameScene->getChildByName("rootcsb")->removeChildByName("revivelayer");
		g_gameScene->gameRevive();
	}
}

void ShopLayer::showBuyOKAnim(const char* pic, Vec2 spoint, Vec2 dpoint)
{
	//if (g_gameScene != NULL)
	//	return;

	for (int i = 0; i<6; i++)
	{
		Sprite* s = Sprite::createWithSpriteFrameName(pic);
		s->setOpacity(0);
		s->setPosition(spoint);
		//g_shopCsbLayer->addChild(s, 100);
		Director::getInstance()->getRunningScene()->addChild(s, 100);
		s->runAction(Sequence::create(DelayTime::create(i*0.06f), Spawn::create(FadeIn::create(0.2f), EaseSineIn::create(MoveTo::create(0.3f, dpoint)), NULL), FadeOut::create(0.05f), Hide::create(), NULL));
	}
}

void ShopLayer::checkGemPCode()
{
	char p_str[64] = { 0 };
	bool ret = true;
	int gemcount = sizeof(gemNum) / sizeof(gemNum[0]);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	for (int i = 0; i < gemcount; i++)
	{
		ret = GlobalData::checkPayCode(i, 1);
		if (ret)
			vec_gemindex.push_back(i);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	bool testret[sizeof(gemNum) / sizeof(gemNum[0])] = { true, true, true, true, true };
	for (int i = 0; i < gemcount; i++)
	{
		if (testret[i])
			vec_gemindex.push_back(i);
	}
#endif

	int glodcount = sizeof(goldNum) / sizeof(goldNum[0]);

	for (int i = 0; i < glodcount; i++)
	{
		vec_gemindex.push_back(gemcount + i);
	}
}

void ShopLayer::checkGiftPCode()
{
	char p_str[64] = { 0 };
	bool ret = true;
	int gemcount = sizeof(gemNum) / sizeof(gemNum[0]);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	for (int i = 0; i <= PROPGIFT; i++)
	{
		ret = GlobalData::checkPayCode(gemcount + i, 1);
		if (ret)
			vec_giftindex.push_back(i);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	bool testret[PROPGIFT + 1] = { false, true, true, false, false, false };
	for (int i = 0; i < PROPGIFT + 1; i++)
	{
		if (testret[i])
			vec_giftindex.push_back(i);
	}
#endif
}