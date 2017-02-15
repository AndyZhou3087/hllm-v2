#include "SuccLayer.h"
#include "ThemeScene.h"
#include "GameScene.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "Const.h"
#include "MyParticle.h"
#include "NewerGuideLayer.h"
#include "CommonLayer.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "HttpUtil.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "LotteryLayer.h"
#include "MyTransitionScene.h"

// on "init" you need to initialize your instance

int SuccLayer::lastbestscore = 0;
bool SuccLayer::init()
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
	_root = CSLoader::createNode("winLayer.csb");
	addChild(_root);

	m_winBackBtn = (cocos2d::ui::Widget*)_root->getChildByName("winBackBtn");
	m_winBackBtn->addTouchEventListener(CC_CALLBACK_2(SuccLayer::WinBack, this));
	m_winBackBtn->setVisible(false);

	cocos2d::ui::Widget* closebtn = (cocos2d::ui::Widget*)_root->getChildByName("winCloseBtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(SuccLayer::WinBack, this));

	m_winNextBtn = (cocos2d::ui::Widget*)_root->getChildByName("winNextBtn");
	m_winNextBtn->addTouchEventListener(CC_CALLBACK_2(SuccLayer::WinNext, this));
	m_winNextBtn->setVisible(false);

	m_stage = GlobalData::getPlayingStage() + 1;
	string stageStr = StringUtils::format("%d", m_stage);
	cocos2d::ui::TextBMFont* stagebmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("winstagebmlbl");
	stagebmlbl->setString(stageStr);

	std::string scoreStr = StringUtils::format("%d", g_gameScene->getScore());
	cocos2d::ui::TextBMFont* scorebmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("scorebmlbl");
	scorebmlbl->setString(scoreStr);

	lastbestscore = GlobalData::getBestCMScore(CMNORMAL);

	if (g_gameScene->getScore() > lastbestscore)
	{
		GlobalData::setBestCMScore(CMNORMAL, g_gameScene->getScore());
	}
	int stars = GlobalData::getStageStars();
	_wintitle = (cocos2d::ui::Widget*)_root->getChildByName("winTitle");
	_wintitle->setVisible(true);

	std::string isfirstpassstr = StringUtils::format("isfirstpass%d", GlobalData::getPlayingStage());
	isfirstpass = GlobalData::getIsFirstPass(isfirstpassstr);

	cocos2d::ui::TextBMFont* wingoldbmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("wingoldbmlbl");
	cocos2d::ui::TextBMFont* wingembmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("wingembmlbl");
	cocos2d::ui::TextBMFont* winhpbmlbl = (cocos2d::ui::TextBMFont*)_root->getChildByName("winhpbmlbl");
	int bonuseGold = g_gameScene->getBonuseGold();
	if (isfirstpass)
	{
		GlobalData::setIsFirstPass(isfirstpassstr.c_str(), false);
		if ((m_stage) % BOSSSTAGEPOS == 0)//boss关
		{
			bonuseGold += 35;
			wingembmlbl->setString("x1");
			GlobalData::setGemCount(GlobalData::getGemCount() + 1);
			winhpbmlbl->setString("x4");
			GlobalData::setHpCount(GlobalData::getHpCount() + 4);

#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "gem");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1, BONUSE_NORMAL);
			}

			sprintf(p_str, "%s", "hp");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 4, BONUSE_NORMAL);
			}
#endif
		}
		else
		{
			_root->getChildByName("gem")->setVisible(false);
			_root->getChildByName("sureropbox_1")->setVisible(false);
			_root->getChildByName("hp")->setVisible(false);
			_root->getChildByName("sureropbox_2")->setVisible(false);
			wingembmlbl->setVisible(false);
			winhpbmlbl->setVisible(false);

			if (stars >= 3)
			{
				bonuseGold += 35;
			}
			else if (stars >= 2)
			{
				bonuseGold += 20;
			}
			else
			{
				bonuseGold += 10;
			}
		}
	}
	else
	{
		_root->getChildByName("gem")->setVisible(false);
		_root->getChildByName("sureropbox_1")->setVisible(false);
		_root->getChildByName("hp")->setVisible(false);
		_root->getChildByName("sureropbox_2")->setVisible(false);
		wingembmlbl->setVisible(false);
		winhpbmlbl->setVisible(false);
		bonuseGold += 10;
	}
	string bonuseGoldStr = StringUtils::format("x%d", bonuseGold);
	wingoldbmlbl->setString(bonuseGoldStr);
	GlobalData::setGoldCount(GlobalData::getGoldCount() + bonuseGold);
	GlobalData::setCurStageContinueFailCount(0);
	GlobalData::setIsCurStagePass(true);
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", "coin");
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, bonuseGold, BONUSE_NORMAL);
	}
#endif

	saveData();

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


	Achieve* data = GlobalData::getAchieveDataByID(28);
	if (data->finish != -1)
	{
		data->finish = m_stage;
		GlobalData::SaveAchieveData();
	}

	data = GlobalData::getTaskDataByID(3);
	if (data->finish != -1)
	{
		data->finish++;
		GlobalData::SaveTaskData();
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

	return true;
}

void SuccLayer::onEnterTransitionDidFinish()
{
	int stage = GlobalData::getPlayingStage() + 1;
	int stagebestscore = GlobalData::getstagebestscore(stage);

	if (g_gameScene->getScore() > stagebestscore)
	{
		GlobalData::setstagebestscore(stage, g_gameScene->getScore());
#ifdef USENETWORK
		rapidjson::Document writedoc;
		writedoc.SetObject();
		rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();

		writedoc.AddMember("id", rapidjson::Value(GlobalData::getMyID().c_str(), allocator), allocator);
		writedoc.AddMember("stage", stage, allocator);
		writedoc.AddMember("bestscore", g_gameScene->getScore(), allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		writedoc.Accept(writer);
		string url;
		url.append(HTTPURL);
		url.append("llk_updatescore");
		url.append("&param=");
		url.append(buffer.GetString());
		HttpUtil::getInstance()->doData(url, SuccLayer::httpUpdateScoreCB);
#endif
	}
	int stars = GlobalData::getStageStars();
	if (stars == 0)
		stars = 1;

	_wintitle->runAction(Sequence::create(DelayTime::create(1.0f), CallFuncN::create(CC_CALLBACK_1(SuccLayer::delayPlayAnim, this)), NULL));
	
	for (int i = 0; i < stars; i++)
	{
		string starbgstr = StringUtils::format("winstar0_%d", i);
		cocos2d::ui::Widget* winstarbg = (cocos2d::ui::Widget*)_root->getChildByName(starbgstr);
		winstarbg->runAction(Sequence::create(DelayTime::create(1.0f + 0.3f * i), CallFuncN::create(CC_CALLBACK_1(SuccLayer::showStarParticle, this)), NULL));
	}

	if (stage == 3)
	{
		GlobalData::setGuide(8, false);
	}
	else if (stage == 4)
	{
		GlobalData::setGuide(9, false);
	}
	else if (stage == 5)
	{
		GlobalData::setGuide(10, false);
		GlobalData::setGuide(11, false);
	}
}
#ifdef USENETWORK
void SuccLayer::httpUpdateScoreCB(std::string retdata, int code, std::string tag)
{
	if (g_gameScene->getScore() > lastbestscore)
		updataUserData();
}

void SuccLayer::updataUserData()
{
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();

	writedoc.AddMember("id", rapidjson::Value(GlobalData::getMyID().c_str(), allocator), allocator);
	string namestr;
	const char* retstr = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "gbkToUTF8", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(GlobalData::getMyName().c_str());
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, para1);

		retstr = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	namestr = retstr;
#else
	namestr = StringUtils::format("%s", GlobalData::getMyName().c_str());
#endif
	writedoc.AddMember("nickname", rapidjson::Value(namestr.c_str(), allocator), allocator);
	writedoc.AddMember("level", GlobalData::getMyTotalSkillLevel(), allocator);

 	std::string myskillstr;
	std::string temp;
	int skillsize = LEVELTYPECOUNT;
	for (int i = 0; i < skillsize; i++)
	{
		if (i == skillsize - 1)
			temp = StringUtils::format("%d", GlobalData::getMySkillLevel(i));
		else
			temp = StringUtils::format("%d,", GlobalData::getMySkillLevel(i));

		myskillstr.append(temp);
	}

	writedoc.AddMember("skill_level", rapidjson::Value(myskillstr.c_str(), allocator), allocator);
	writedoc.AddMember("coin", GlobalData::getGoldCount(), allocator);
	writedoc.AddMember("diamond", GlobalData::getGemCount(), allocator);
	writedoc.AddMember("life", GlobalData::getHpCount(), allocator);
	writedoc.AddMember("lifeupperlimit", GlobalData::getMyMaxHp(), allocator);
	writedoc.AddMember("find", GlobalData::getPromptProp(), allocator);
	writedoc.AddMember("reset", GlobalData::getRefreshProp(), allocator);
	writedoc.AddMember("addtime", GlobalData::getAddtimeProp(), allocator);

	std::string stagestr;
	std::string hexstr;
	for (int i = 0; i < GlobalData::getMapCount(); i++)
	{
		std::string cstr = StringUtils::format("page%dstar", i);
		std::string starsStr = GlobalData::getChapterStageStar(cstr);
		long long cruPageStageStar = atoll(starsStr.c_str());

		hexstr = StringUtils::format("%05x", (int)cruPageStageStar);
		stagestr.append(hexstr);
	}
	writedoc.AddMember("finished_pass", rapidjson::Value(stagestr.c_str(), allocator), allocator);
	writedoc.AddMember("bestscore", GlobalData::getBestCMScore(CMNORMAL), allocator);
	writedoc.AddMember("bossstatus", GlobalData::getBestDefeatedBossCount(), allocator);
	writedoc.AddMember("bossbestscore", GlobalData::getBestCMScore(CMBOSS), allocator);
	writedoc.AddMember("login_days", GlobalData::getContinueLoginDay(), allocator);
	writedoc.AddMember("lasttakeawardtime", GlobalData::getLastLoginRewardSecond(), allocator);
	writedoc.AddMember("higheststage", GlobalData::getHighestStage(), allocator);
	writedoc.AddMember("classicscore", GlobalData::getBestCMScore(CMCLASIC), allocator);
	writedoc.AddMember("flashscore", GlobalData::getBestCMScore(CMFLASH), allocator);

	std::string achieveStr;
	GlobalData::sortAchieveDatabyId();
	for (unsigned int i = 0; i < GlobalData::achieveData.size(); i++)
	{
		int finish = GlobalData::achieveData[i].finish;
		if (finish == -1)
			achieveStr.append("fff");
		else
		{
			hexstr = StringUtils::format("%03x", finish);
			achieveStr.append(hexstr);
		}
	}
	writedoc.AddMember("achievement", rapidjson::Value(achieveStr.c_str(), allocator), allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	string url;
	url.append(HTTPURL);
	url.append("llk_updatedata");
	url.append("&param=");
	url.append(buffer.GetString());
	HttpUtil::getInstance()->doData(url);
}
#endif
void SuccLayer::showGiftLayer(int type)
{
	FourTypeGift::AnimType = A_UPTODOWN;
	FourTypeGift::_type = type;
	FourTypeGift* giftLayer = FourTypeGift::create();
	Director::getInstance()->getRunningScene()->addChild(giftLayer, 100, "giftlayer");
}

void SuccLayer::WinBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		GlobalData::setIsClickNext(false);
		Director::getInstance()->replaceScene(MyTransitionScene::scene());
	}
}

void SuccLayer::WinNext(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		/*
		if (GlobalData::checkGuide(11))
		{
			if (g_NewerLayer != NULL)
				g_NewerLayer->removeSelf();
		}
		*/
		GlobalData::setIsClickNext(true);
		Director::getInstance()->replaceScene(MyTransitionScene::scene());
	}
}

void SuccLayer::showStarParticle(Node* node)
{
	int x = node->getPositionX();
	int y = node->getPositionY() - 100;
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_SUCCSTAR);
	MyParticle::Show(this, SpeciallyName[7], x, y, true, 0.5);
	MyParticle::Show(this, SpeciallyName[8], x, y, true, 0.3f);
	MyParticle::Show(this, SpeciallyName[9], x, y, true, 0.5);
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_FIREWORKS);
}

void SuccLayer::delayPlayAnim(Ref* pSender)
{
	_wintitle->runAction(Sequence::create(DelayTime::create(0.5f), Show::create(), CallFunc::create(CC_CALLBACK_0(SuccLayer::showBtn, this)), NULL));
	Sprite* light = Sprite::createWithSpriteFrameName("UI/buylight.png");
	light->setPosition(Vec2(270, 700));
	light->runAction(RepeatForever::create(RotateTo::create(4, 720)));
	addChild(light, -2);

	auto action = CSLoader::createTimeline("winLayer.csb");
	_root->runAction(action);
	action->gotoFrameAndPlay(0, GlobalData::getStageStars() * 30, false);
}

void SuccLayer::showBtn()
{
	/*
	if (GlobalData::checkGuide(11))
	{
		m_winNextBtn->setEnabled(false);
		m_winBackBtn->setEnabled(false);
		this->scheduleOnce(schedule_selector(SuccLayer::ShowNewerGuide), 0.2f);
	}
	*/
	/*
	int isshowgift = false;
	if ((m_stage == 1 || (m_stage % 5 == 0)) && !GlobalData::getIsBuyGift(NEWERGIFT))
	{
		isshowgift = true;
		showGiftLayer(NEWERGIFT);
	}
	if (m_stage % 3 == 0 && !isshowgift)
	*/
	if (m_stage % 3 == 0)
	{
		//showGiftLayer(CGGIFT);
		CommonLayer::showGift(1);
	}

	if (m_stage % BOSSSTAGEPOS == 0)
	{
		LotteryLayer* layer = LotteryLayer::create(isfirstpass);
		addChild(layer, 10000);
	}
	m_winNextBtn->setVisible(true);
	m_winBackBtn->setVisible(true);
}

void SuccLayer::ShowNewerGuide(float dt)
{
	vector<Node*> nodes;
	nodes.push_back(m_winNextBtn);
	g_NewerLayer = NewerGuideLayer::create(11);
	_root->addChild(g_NewerLayer, NEWERLAYERZOER);
	g_NewerLayer->setData(nodes);
	m_winNextBtn->setEnabled(true);
	m_winBackBtn->setEnabled(true);

}
void SuccLayer::saveData()
{

	int curstage;
	int curchapter = GlobalData::covertChapterStage(GlobalData::getPlayingStage(), curstage);
	std::string chapter = StringUtils::format("page%dstar", curchapter);
	std::string starsStr = GlobalData::getChapterStageStar(chapter.c_str());

	long long cruPageStageStar = atoll(starsStr.c_str());

	int curstageStar = GlobalData::getStageStars();
	if (curstageStar == 0)
		curstageStar = 1;
	int beforeStageStar = cruPageStageStar >> (curstage * 2) & 3;

	if (curstageStar > beforeStageStar)
	{
		//long long pageStageStar = (cruPageStageStar & (0xFFFFFFFFFFFFFFFF << (2 * curstage) | 0xFFFFFFFFFFFFFFFF >> (64 - 2 * curstage))) | (curstageStar << (2 * curstage));
		long long pageStageStar = (cruPageStageStar & (0xFFFFFFFFFFFFFFFF << (2 * (curstage + 1)) | 0xFFFFFFFFFFFFFFFF >> (64 - 2 * curstage))) | (curstageStar << (2 * curstage));

		std::string stars = StringUtils::format("%lld", pageStageStar);
		GlobalData::setChapterStageStar(chapter.c_str(), stars.c_str());
	}

	//保存最大关卡
	int beforeMaxStage = GlobalData::getHighestStage();
	int maxStage = GlobalData::getPlayingStage();
	if (maxStage > beforeMaxStage)
		GlobalData::setHighestStage(maxStage);

	int totalstarnum = 0;
	for (int m = 0; m < GlobalData::getMapCount(); m++)
	{
		std::string pageStar = StringUtils::format("page%dstar", m);

		std::string starsStr = GlobalData::getChapterStageStar(pageStar.c_str());

		long long pageStageStar = atoll(starsStr.c_str());

		for (int i = 0; i < GlobalData::getMapCount(); i++)
		{
			int star = pageStageStar >> (i * 2) & 3;
			totalstarnum += star;
		}
	}


	Achieve* data = GlobalData::getAchieveDataByID(23);
	if (data->finish != -1)
	{
		data->finish = totalstarnum;
		GlobalData::SaveAchieveData();
	}

	data = GlobalData::getAchieveDataByID(24);
	if (data->finish != -1)
	{
		data->finish = totalstarnum;
		GlobalData::SaveAchieveData();
	}

	data = GlobalData::getAchieveDataByID(25);
	if (data->finish != -1)
	{
		data->finish = totalstarnum;
		GlobalData::SaveAchieveData();
	}
}