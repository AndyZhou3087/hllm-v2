#include "ByGift.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "ShopLayer.h"
#include "CommonLayer.h"
#include "HintBox.h"
int ByGift::AnimType = A_SMALLTONORMAL;
bool ByGift::isClickPop = false;
ByGift::ByGift()
{
}


ByGift::~ByGift()
{
	isClickPop = false;
}

bool ByGift::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	this->addChild(color);
	
	_layer = CSLoader::createNode("byGiftLayer.csb");
	addChild(_layer);

	cocos2d::ui::Widget* colseBtn = (cocos2d::ui::Widget*)_layer->getChildByName("closebtn");
	colseBtn->addTouchEventListener(CC_CALLBACK_2(ByGift::close, this));

#ifdef ZFTEXTCLEAR
	colseBtn->setVisible(false);
	cocos2d::ui::Widget* colseBtnclear = (cocos2d::ui::Widget*)_layer->getChildByName("closebtnclear");
	colseBtnclear->addTouchEventListener(CC_CALLBACK_2(ByGift::close, this));
	colseBtnclear->setVisible(true);
#endif

	getBtn = (cocos2d::ui::Widget*)_layer->getChildByName("giftbtn");
	getBtn->addTouchEventListener(CC_CALLBACK_2(ByGift::buy, this));
	getBtn->setTag(GJGIFT);
	getBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 0.95f), ScaleTo::create(0.5f, 1.0f), NULL)));

	cocos2d::ui::Widget* buytext0 = (cocos2d::ui::Widget*)getBtn->getChildByName("buytext0");
	cocos2d::ui::Widget* buytext1 = (cocos2d::ui::Widget*)getBtn->getChildByName("buytext1");
	if (GlobalData::getBuyTextType() == 0)
	{
		buytext0->setVisible(false);
		buytext1->setVisible(true);
	}
	else
	{
		buytext0->setVisible(true);
		buytext1->setVisible(false);
	}
	int price = checkprice();
	if (price == -1)
		price = 8;
	string titlestr = StringUtils::format("UI/bytitle%d.png", price);
	Sprite* title = Sprite::createWithSpriteFrameName(titlestr);
	title->setPosition(Vec2(270, 705));
	_layer->addChild(title);

	string titledesstr = StringUtils::format("UI/by%ddsc.png", price);
	Sprite* titledes = Sprite::createWithSpriteFrameName(titledesstr);
	titledes->setPosition(Vec2(270, 580));
	_layer->addChild(titledes);

	int gemcount = 6;
	if (price == 8)
	{
		gemcount = 6;
		getBtn->setTag(GJGIFT);
	}
	else if (price == 10)
	{
		gemcount = 8;
		getBtn->setTag(ZXGIFT);
	}
	else if (price == 15)
	{
		gemcount = 15;
		getBtn->setTag(TQGIFT);
	}
	else if (price == 20)
	{
		gemcount = 25;
		getBtn->setTag(ZZGIFT);
	}
	else if (price == 30)
	{
		gemcount = 30;
		getBtn->setTag(HHGIFT);
	}
	pindex = getBtn->getTag();
	string gemcountstr = StringUtils::format("x%d", gemcount);
	cocos2d::ui::TextBMFont* gemlbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName("gemlbl");
	gemlbl->setString(gemcountstr);
	
	int fontsize = 18;
	int opacity = 100;
#ifdef ZFTEXTCLEAR
	fontsize = 22;
	opacity = 200;
#endif
	string paystr = CommonFuncs::gbk2utf(StringUtils::format("您将通过短信购买此道具。信息费%d元/月。\n退订后次月不计费，奖励停止发放。\n如有问题请联系客服电话:4009908188", price).c_str());
	Label* paydsc = Label::createWithSystemFont(paystr, FONT_NAME, fontsize);
	paydsc->setPosition(Vec2(270, 170));
	paydsc->setOpacity(opacity);
	_layer->addChild(paydsc);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (AnimType == A_SMALLTONORMAL)
	{
		_layer->setAnchorPoint(Vec2(0.5f, 0.5f));
		_layer->setPosition(Vec2(270, 480));
		_layer->setScale(0.1f);
		_layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		_layer->setPosition(0, 960);
		_layer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}

#ifdef ANALYTICS
	if (isClickPop)
	{
		string enventstr = StringUtils::format("clickpop%s", packagename[pindex]);

		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", enventstr.c_str());
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
	}
	else
	{
		string enventstr = StringUtils::format("autopop%s", packagename[pindex]);
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", enventstr.c_str());
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
	}
#endif
	return true;
}

void ByGift::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#ifdef ANALYTICS
		if (isClickPop)
		{
			string enventstr = StringUtils::format("clickpopX%s", packagename[pindex]);

			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
		else
		{
			string enventstr = StringUtils::format("autopopX%s", packagename[pindex]);
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
#endif
		this->removeFromParentAndCleanup(true);
	}
}

int ByGift::checkprice()
{
	const char* str = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;

	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/OGThranPay", "checkBYPCode", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		str = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif

	if (strcmp(str, "hllm.lb.8") == 0)
		return 8;
	else if (strcmp(str, "hllm.lb.10.1") == 0)
		return 10;
	else if (strcmp(str, "hllm.lb.15") == 0)
		return 15;

	else if (strcmp(str, "hllm.lb.20") == 0)
		return 20;
	else if (strcmp(str, "hllm.lb.30") == 0)
		return 30;

	return -1;
}

void ByGift::buy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getBYTime() > 0)
		{
			HintBox* hint = HintBox::create("包月中", 0);
			addChild(hint, 1000);
			return;
		}
#ifdef ANALYTICS
		if (isClickPop)
		{
			string enventstr = StringUtils::format("clickpopOK%s", packagename[pindex]);

			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
		else
		{
			string enventstr = StringUtils::format("autopopOK%s", packagename[pindex]);
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", enventstr.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
		}
#endif
		int tag = getBtn->getTag() + sizeof(gemNum) / sizeof(gemNum[0]);
		ShopLayer::beginPayment(tag);
	}
}