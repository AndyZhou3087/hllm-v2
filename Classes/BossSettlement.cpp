#include "BossSettlement.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "ShopLayer.h"
#include "GameScene.h"
#include "ThemeScene.h"
#include "CommonLayer.h"
#include "SuccLayer.h"
#include "tinyxml2/tinyxml2.h"
#include "MyTransitionScene.h"
BossSettlement::BossSettlement()
{
}


BossSettlement::~BossSettlement()
{
}

bool BossSettlement::init()
{
	if (!Layer::init())
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	_layer = CSLoader::createNode("bosssettlement.csb");
	addChild(_layer);

	parseBossRewardConfig();

	cocos2d::ui::Widget* colseBtn = (cocos2d::ui::Widget*)_layer->getChildByName("winCloseBtn");
	colseBtn->addTouchEventListener(CC_CALLBACK_2(BossSettlement::close, this));

	startBtn = (cocos2d::ui::Widget*)_layer->getChildByName("okbtn");
	startBtn->addTouchEventListener(CC_CALLBACK_2(BossSettlement::close, this));

	cocos2d::ui::ImageView* r1 = (cocos2d::ui::ImageView*)_layer->getChildByName("r1");
	cocos2d::ui::ImageView* r2 = (cocos2d::ui::ImageView*)_layer->getChildByName("r2");
	cocos2d::ui::ImageView* r3 = (cocos2d::ui::ImageView*)_layer->getChildByName("r3");

	cocos2d::ui::TextBMFont* propnumlbl_1 = (cocos2d::ui::TextBMFont*)_layer->getChildByName("propnumlbl_1");
	cocos2d::ui::TextBMFont* propnumlbl_2 = (cocos2d::ui::TextBMFont*)_layer->getChildByName("propnumlbl_2");
	cocos2d::ui::TextBMFont* propnumlbl_3 = (cocos2d::ui::TextBMFont*)_layer->getChildByName("propnumlbl_3");

	r1->setVisible(false);
	r2->setVisible(false);
	r3->setVisible(false);
	propnumlbl_1->setVisible(false);
	propnumlbl_2->setVisible(false);
	propnumlbl_3->setVisible(false);

	if (GlobalData::getDefeatedBossCount() > GlobalData::getBestDefeatedBossCount())
		GlobalData::setBestDefeatedBossCount(GlobalData::getDefeatedBossCount());

	int count = 0;
	const char* rewardpath[] = { "UI/gold.png", "UI/diamond.png", "UI/hp.png", "UI/rprop3.png", "UI/rprop4.png", "UI/rprop5.png" };
	for (unsigned int i = 0; i < rewards.size(); i++)
	{
		if (rewards[i] > 0)
		{
			count++;
			if (count == 1)
			{
				std::string str = StringUtils::format("%s", rewardpath[i]);
				r1->setVisible(true);
				r1->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
				propnumlbl_1->setVisible(true);
				std::string propstr = StringUtils::format("%d", rewards[i]);
				propnumlbl_1->setString(propstr);
			}
			else if (count == 2)
			{
				std::string str = StringUtils::format("%s", rewardpath[i]);
				r2->setVisible(true);
				r2->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
				propnumlbl_2->setVisible(true);
				std::string propstr = StringUtils::format("%d", rewards[i]);
				propnumlbl_2->setString(propstr);
			}
			else if (count == 3)
			{
				std::string str = StringUtils::format("%s", rewardpath[i]);
				r3->setVisible(true);
				r3->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

				propnumlbl_3->setVisible(true);
				std::string propstr = StringUtils::format("%d", rewards[i]);
				propnumlbl_3->setString(propstr);
			}
		}
	}

	string name;
	cocos2d::ui::ScrollView* scrollview = (cocos2d::ui::ScrollView*)_layer->getChildByName("ScrollView_1");

	int itemwidth = 130;
	int totalBossCount = GlobalData::getMapCount();

	scrollview->setInnerContainerSize(Size(totalBossCount*itemwidth, scrollview->getContentSize().height));

	for (int i = 0; i < totalBossCount; i++)
	{
		string str = StringUtils::format("vsboss%d.png", i + 1);
		Sprite* vsboss = Sprite::createWithSpriteFrameName(str);
		vsboss->setScale(0.7f);
		vsboss->setPosition(Vec2(55 + i*itemwidth, 120));
		str = StringUtils::format("vsboss%d", i + 1);
		scrollview->addChild(vsboss, 1, str);

		str = StringUtils::format("boss%dhei.png", i + 1);
		Sprite* bosshei = Sprite::createWithSpriteFrameName(str);
		bosshei->setPosition(Vec2(55 + i*itemwidth, 120));
		bosshei->setScale(0.7f);
		str = StringUtils::format("boss%dhei", i + 1);
		scrollview->addChild(bosshei, 1, str);

		Sprite* passbox = Sprite::createWithSpriteFrameName("UI/bossgetbox.png");
		passbox->setPosition(Vec2(40 + i*itemwidth, 34));
		str = StringUtils::format("passbox%d", i + 1);
		scrollview->addChild(passbox, 1, str);

		Sprite* passget = Sprite::createWithSpriteFrameName("UI/bossget.png");
		passget->setPosition(Vec2(40 + i*itemwidth, 35));
		str = StringUtils::format("passget%d", i + 1);
		scrollview->addChild(passget, 1, str);

	}

	int passedcount = GlobalData::getDefeatedBossCount();
	float totalW = scrollview->getInnerContainerSize().width;
	if (passedcount > 3)
	{
		int offset = (passedcount)* itemwidth;
		float percent = offset*100.0f / totalW;
		scrollview->scrollToPercentHorizontal(percent, 3.0f, true);
	}

	if (passedcount < totalBossCount)
	{
		for (int i = passedcount; i < totalBossCount; i++)
		{
			name = StringUtils::format("vsboss%d", i + 1);
			scrollview->getChildByName(name)->setVisible(false);

			name = StringUtils::format("passbox%d", i + 1);
			scrollview->getChildByName(name)->setVisible(false);

			name = StringUtils::format("passget%d", i + 1);
			scrollview->getChildByName(name)->setVisible(false);


		}
	}
	for (int i = 0; i < passedcount; i++)
	{
		name = StringUtils::format("boss%dhei", i + 1);

		scrollview->getChildByName(name)->setVisible(false);
	}

	int myscore = g_gameScene->getScore();
	cocos2d::ui::TextBMFont* scoreLbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName("scoreLbl");
	std::string str;
	str = StringUtils::format("%d", myscore);
	scoreLbl->setString(str);

	bossbestscore = GlobalData::getBestCMScore(CMBOSS);

	if (myscore > bossbestscore)
	{
		bossbestscore = g_gameScene->getScore();
		GlobalData::setBestCMScore(CMBOSS, bossbestscore);
	}
	cocos2d::ui::TextBMFont* bestScoreLbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName("bestScoreLbl");
	str = StringUtils::format("%d", bossbestscore);
	bestScoreLbl->setString(str);

	cocos2d::ui::TextBMFont* defeatedBossCountLbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName("defeatedBossCountLbl");

	str = StringUtils::format("%d", GlobalData::getDefeatedBossCount());
	defeatedBossCountLbl->setString(str);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (rewards.size() > 0)
	{
		GlobalData::setGoldCount(GlobalData::getGoldCount() + rewards[0]);
		GlobalData::setGemCount(GlobalData::getGemCount() + rewards[1]);
		GlobalData::setHpCount(GlobalData::getHpCount() + rewards[2]);
		GlobalData::setPromptProp(GlobalData::getPromptProp() + rewards[3]);
		GlobalData::setRefreshProp(GlobalData::getRefreshProp() + rewards[4]);
		GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + rewards[5]);

#ifdef ANALYTICS
		const char* popstr[] = { "coin", "gem", "hp", "prompt", "refresh", "addtime" };
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		for (int i=0;i<rewards.size();i++)
		{
			if (rewards[i] > 0)
			{
				sprintf(p_str, "%s", popstr[i]);
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, rewards[i], BONUSE_BOSSS);
				}
			}
		}
#endif
	}
	return true;
}

void BossSettlement::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(MyTransitionScene::scene());
	}
}

void BossSettlement::onEnterTransitionDidFinish()
{
#ifdef USENETWORK
	if (g_gameScene->getScore() > bossbestscore)
		SuccLayer::updataUserData();
#endif
}

void BossSettlement::parseBossRewardConfig()
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	
	string xmlfile = GlobalData::getXMLDataPath("data/boss/bossreward.xml");
	string filedata = FileUtils::getInstance()->getStringFromFile(xmlfile);
	int err = pDoc->Parse(filedata.c_str());
	if (err != 0)
	{
		delete pDoc;
		return;
	}
	tinyxml2::XMLElement *rootEle = pDoc->RootElement();

	tinyxml2::XMLElement *element = rootEle->FirstChildElement();
	while (element != NULL)
	{
		if (strcmp(element->Name(), "b") == 0)
		{
			int id = element->IntAttribute("id");
			if (id == GlobalData::getDefeatedBossCount())
			{
				rewards.push_back(element->IntAttribute("r1"));
				rewards.push_back(element->IntAttribute("r2"));
				rewards.push_back(element->IntAttribute("r3"));
				rewards.push_back(element->IntAttribute("r4"));
				rewards.push_back(element->IntAttribute("r5"));
				rewards.push_back(element->IntAttribute("r6"));
			}
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}
