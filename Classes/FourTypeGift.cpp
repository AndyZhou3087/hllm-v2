#include "FourTypeGift.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "ShopLayer.h"
#include "GameScene.h"
#include "CommonLayer.h"
int FourTypeGift::AnimType = A_SMALLTONORMAL;
int FourTypeGift::_type = COINGIFT;
bool FourTypeGift::isshowinggift = false;
bool FourTypeGift::isClickPop = false;
static int giftPropCount[][5] = { { 500, 2000, 2, 2, 2 }, { 288, 2888, 0, 0, 0 }, { 100, 1000, 3, 3, 3 }, { 100, 0, 1, 0, 1 }, { 0, 1000, 1, 1, 0 }, { 0, 0, 3, 3, 3 }, { 300, 0, 0, 0, 0 }, { 0, 0, 2, 2, 2 }, { 88, 0, 1, 1, 1 },
								  { 500, 2000, 2, 2, 2 }, { 288, 2888, 0, 0, 0 }, { 100, 1000, 3, 3, 3 }, { 100, 0, 1, 0, 1 }, { 0, 1000, 1, 1, 0 }, { 0, 0, 3, 3, 3 },
								  { 500, 2000, 2, 2, 2 }, { 288, 2888, 0, 0, 0 }, { 100, 1000, 3, 3, 3 }, { 100, 0, 1, 0, 1 }, { 0, 1000, 1, 1, 0 }, { 0, 0, 3, 3, 3 },
								  { 500, 2000, 2, 2, 2 }, { 100, 0, 1, 0, 1 }, { 0, 1000, 1, 1, 0 }, { 0, 0, 3, 3, 3 }
};
FourTypeGift::FourTypeGift()
{
	isshowinggift = false;
}

FourTypeGift::~FourTypeGift()
{
	isshowinggift = false;
	isClickPop = false;
}

bool FourTypeGift::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	this->addChild(color);

	std::string giftName[] = {"giftonsaletitle", "giftrichtitle", "giftcgtitle", "giftgemtitle", "giftcointitle", "giftproptitle", "giftnewertitle", "giftrevivetitle", "giftfirsttitle", 
							  "giftonsaletitle", "giftrichtitle", "giftcgtitle", "giftgemtitle", "giftcointitle", "giftproptitle",
							  "giftonsaletitle", "giftrichtitle", "giftcgtitle", "giftgemtitle", "giftcointitle", "giftproptitle",
							  "giftonsaletitle", "giftgemtitle", "giftcointitle", "giftproptitle" };
	_layer = CSLoader::createNode("giftLayer.csb");
	addChild(_layer);

	cocos2d::ui::ImageView* titleimage = (cocos2d::ui::ImageView*)_layer->getChildByName("gifttitle");
	std::string titlestr = StringUtils::format("UI/%s.png", giftName[_type].c_str());
	titleimage->loadTexture(titlestr, cocos2d::ui::TextureResType::PLIST);
	titleimage->setContentSize(Sprite::createWithSpriteFrameName(titlestr)->getContentSize());

	cocos2d::ui::Widget* zkbox = (cocos2d::ui::Widget*)_layer->getChildByName("zkbox");

	std::string pricestr = StringUtils::format("%d.00", pgiftyuanprice[_type]);
	cocos2d::ui::TextBMFont* yuanpricelbl = (cocos2d::ui::TextBMFont*)zkbox->getChildByName("yuanprice");
	yuanpricelbl->setString(pricestr);

	pricestr = StringUtils::format("%d.00", pgiftprice[_type]);
	cocos2d::ui::TextBMFont* pricelbl = (cocos2d::ui::TextBMFont*)zkbox->getChildByName("price");
	pricelbl->setString(pricestr);

	cocos2d::ui::Button* colseBtn = (cocos2d::ui::Button*)_layer->getChildByName("closebtn");
	colseBtn->addTouchEventListener(CC_CALLBACK_2(FourTypeGift::close, this));
#ifdef ZFTEXTCLEAR
	colseBtn->setVisible(false);
	cocos2d::ui::Widget* colseBtnclear = (cocos2d::ui::Widget*)_layer->getChildByName("closebtnclear");
	colseBtnclear->addTouchEventListener(CC_CALLBACK_2(FourTypeGift::close, this));
	colseBtnclear->setVisible(true);
#endif

	getBtn = (cocos2d::ui::Widget*)_layer->getChildByName("giftbtn");
	getBtn->addTouchEventListener(CC_CALLBACK_2(FourTypeGift::get, this));
	getBtn->setTag(_type + sizeof(gemNum) / sizeof(gemNum[0]));

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
	int fontsize = 18;
	int opacity = 100;
#ifdef ZFTEXTCLEAR
	fontsize = 22;
	opacity = 200;
#endif

	string paystr = CommonFuncs::gbk2utf(StringUtils::format("您将通过短信购买此礼包，信息费%d元。\n如有问题请联系客服电话：4009908188。", pgiftprice[_type]).c_str());
	Label* paydsc = Label::createWithSystemFont(paystr, FONT_NAME, fontsize);
	paydsc->setPosition(Vec2(270, 80));
	paydsc->setOpacity(opacity);
	_layer->addChild(paydsc);

	cocos2d::ui::Widget* revivedesc = (cocos2d::ui::Widget*)_layer->getChildByName("revivedesc");
	revivedesc->setVisible(_type == REVIVEGIFT);

	cocos2d::ui::Widget* pbox;
	cocos2d::ui::TextBMFont* plbl;
	std::vector<int> vec_propindex;
	std::string pstr;
	for (int i = 0; i < 5; i++)
	{
		int gvalue = giftPropCount[_type][i];
		pstr = StringUtils::format("giftpp_%d", i);
		pbox = (cocos2d::ui::Widget*)_layer->getChildByName(pstr);
		plbl = (cocos2d::ui::TextBMFont*)pbox->getChildByName("numlbl");
		pstr = StringUtils::format("x%d", gvalue);
		plbl->setString(pstr);
		if (gvalue > 0)
		{
			vec_propindex.push_back(i);
		}
		else
		{
			pbox->setVisible(false);
		}
	}

	int count = vec_propindex.size();
	std::vector<Vec2> pos;
	if (count == 1)
	{
		pos.push_back(Vec2(300, 430));
	}
	else if (count == 2)
	{
		pos.push_back(Vec2(255, 420));
		pos.push_back(Vec2(385, 460));
	}
	else if (count == 3)
	{
		pos.push_back(Vec2(230, 450));
		pos.push_back(Vec2(390, 470));
		pos.push_back(Vec2(360, 365));
	}
	else if (count == 4)
	{
		pos.push_back(Vec2(220, 470));
		pos.push_back(Vec2(395, 470));
		pos.push_back(Vec2(245, 365));
		pos.push_back(Vec2(360, 365));
	}

	if (count < 5)
	{
		for (int i = 0; i < count; i++)
		{
			pstr = StringUtils::format("giftpp_%d", vec_propindex[i]);
			_layer->getChildByName(pstr)->setPosition(pos[i]);
		}
	}
	std::string animstr;
	if (count <= 3)
		animstr = "UI/giftanim0.png";
	else
		animstr = "UI/giftanim1.png";
	cocos2d::ui::ImageView* animimage = (cocos2d::ui::ImageView*)_layer->getChildByName("giftanim");
	
	animimage->loadTexture(animstr, cocos2d::ui::TextureResType::PLIST);
	animimage->setContentSize(Sprite::createWithSpriteFrameName(animstr)->getContentSize());

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

	isshowinggift = true;

#ifdef ANALYTICS
	if (isClickPop)
	{
		string enventstr = StringUtils::format("clickpop%s", packagename[_type]);
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
		string enventstr = StringUtils::format("autopop%s", packagename[_type]);
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
	this->schedule(schedule_selector(FourTypeGift::showAnim), 1.5f);
	return true;
}

void FourTypeGift::showAnim(float dt)
{
	MyParticle::Show(getBtn, SpeciallyName[35], getBtn->getContentSize().width / 2, getBtn->getContentSize().height / 2, true, 0.3f);
}

void FourTypeGift::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (_type != REVIVEGIFT)
		{
			if (g_gameScene != NULL && GlobalData::g_gamestatus != GAMEOVER)
				g_gameScene->gameResume();
		}
#ifdef ANALYTICS
		if (isClickPop)
		{
			string enventstr = StringUtils::format("clickpopX%s", packagename[_type]);

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
			string enventstr = StringUtils::format("autopopX%s", packagename[_type]);
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

void FourTypeGift::get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#ifdef ANALYTICS
		if (isClickPop)
		{
			string enventstr = StringUtils::format("clickpopOK%s", packagename[_type]);

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
			string enventstr = StringUtils::format("autopopOK%s", packagename[_type]);
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
		int tag = getBtn->getTag();
		ShopLayer::beginPayment(tag);
	}
}