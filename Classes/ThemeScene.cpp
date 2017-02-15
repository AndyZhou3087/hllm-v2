#include "ThemeScene.h"
#include "GlobalData.h"
#include "CommonLayer.h"
#include "SoundManager.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "SureStageLayer.h"
#include "Stage.h"
#include "UnlockLayer.h"
#include "MyTransitionScene.h"
#include "HintBox.h"
#include "tinyxml2/tinyxml2.h"
#include "NewerGuideLayer.h"
vector<Sprite*> mapbg;
vector<Node*> mapStageLayer;
vector<Node*> bossNormalLayer;
vector<Node*> bossHitedLayer;
vector<Stage*> vec_stages;
Node* heroAminNode;
bool ThemeScene::isShowingUnlock = false;

ThemeScene* g_ThemeScene = NULL;

ThemeScene::ThemeScene()
{
	isShowingUnlock = false;
	vec_stages.clear();
	themecount = 0;
	maxs = 0;
}

ThemeScene::~ThemeScene()
{
	for (unsigned int i = 0; i < vec_stages.size(); i++)
	{
		Stage* s = vec_stages[i];
		delete s;
		s = NULL;
	}
	vec_stages.clear();
	g_ThemeScene = NULL;
}

Scene* ThemeScene::Scene()
{
	auto scene = Scene::create();

	g_ThemeScene = ThemeScene::create();

	scene->addChild(g_ThemeScene);

	return scene;
}

bool ThemeScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();

	themecount = GlobalData::getMapCount();

	parseChapterNum(GlobalData::getXMLDataPath("data/chapternum.xml"));

	maxs = GlobalData::getTotalStage();

	parseUnlockstarsXml(GlobalData::getXMLDataPath("data/unlockstars.xml"));

	addScrollView();
	//addBackBtn();
	
	g_commomLayer = CommonLayer::create();
	commonLayer = g_commomLayer;
	addChild(commonLayer,1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan=CC_CALLBACK_2(ThemeScene::onTouchBegan,this);

	listener->onTouchMoved = CC_CALLBACK_2(ThemeScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ThemeScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	SoundManager::sharedManager()->playBackMusic("sound/bg.mp3");

	if (UserDefault::getInstance()->getBoolForKey("firstinstall", true))
	{
		UserDefault::getInstance()->setBoolForKey("firstinstall", false);

#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "enterhome");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
	}

	return true;
}

void ThemeScene::onEnterTransitionDidFinish()
{
	doBYData();

	if (GlobalData::getIsGetLoginReward())
		this->schedule(schedule_selector(ThemeScene::checkShowUnlockAnim), 1.4f);

	if (GlobalData::getIsClickNext())
	{
		GlobalData::setPlayingStage(GlobalData::getPlayingStage() + 1);
		this->scheduleOnce(schedule_selector(ThemeScene::delayShowSureStageLayer), 2.0f);
	}

	if (GlobalData::getIsRestart())
	{
		GlobalData::setIsRestart(false);
		this->scheduleOnce(schedule_selector(ThemeScene::restartShowSureStageLayer), 0.4f);
	}

	this->scheduleOnce(schedule_selector(ThemeScene::delayShowNewerGuide), 0.5f);
}

void ThemeScene::parseUnlockstarsXml(std::string filename)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	unsigned char pBuffer[5000];
	memset(pBuffer, 0x00, sizeof(pBuffer));
	strcpy((char *)pBuffer, FileUtils::getInstance()->getStringFromFile(filename).c_str());
	int err = pDoc->Parse((const char*)pBuffer);
	if (err != 0)
	{
		delete pDoc;
		return;
	}
	tinyxml2::XMLElement *rootEle = pDoc->RootElement();

	tinyxml2::XMLElement *element = rootEle->FirstChildElement();
	while (element != NULL)
	{
		if (strcmp(element->Name(), "s") == 0)
		{
			std::string str = element->Attribute("svalue");
			std::vector<std::string> vec_temp;
			CommonFuncs::split(str, vec_temp, ",");
			GlobalData::vec_unlockstars.clear();
			for (unsigned int i = 0; i < vec_temp.size(); i++)
			{
				GlobalData::vec_unlockstars.push_back(atoi(vec_temp[i].c_str()));
			}
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}


void ThemeScene::parseChapterNum(std::string filename)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	unsigned char pBuffer[5000];
	memset(pBuffer, 0x00, sizeof(pBuffer));
	strcpy((char *)pBuffer, FileUtils::getInstance()->getStringFromFile(filename).c_str());
	int err = pDoc->Parse((const char*)pBuffer);
	if (err != 0)
	{
		delete pDoc;
		return;
	}
	tinyxml2::XMLElement *rootEle = pDoc->RootElement();

	tinyxml2::XMLElement *element = rootEle->FirstChildElement();
	while (element != NULL)
	{
		if (strcmp(element->Name(), "num") == 0)
		{
			std::string str = element->Attribute("nvalue");
			std::vector<std::string> vec_temp;
			CommonFuncs::split(str, vec_temp, ",");
			GlobalData::vec_chapternum.clear();
			for (unsigned int i = 0; i < vec_temp.size(); i++)
			{
				GlobalData::vec_chapternum.push_back(atoi(vec_temp[i].c_str()));
			}

			str = element->Attribute("totalstage");
			GlobalData::setTotalStage(atoi(str.c_str()));
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

void ThemeScene::addScrollView()
{
	bool isScrollAnim = false;
	Point center = Point(visibleSize.width / 2, visibleSize.height / 2);

	int maxStage = GlobalData::getHighestStage();

	int herestage = GlobalData::getPlayingStage();

	if (herestage < 0)
	{
		herestage = maxStage + 1;
	}

	int lastherestage = herestage;
	if (herestage >= maxStage + 1)
	{
		herestage = maxStage + 1;
	}
	else
	{
		if (GlobalData::getIsCurStagePass())
		{
			herestage += 1;
		}
	}
	if (herestage >= maxs)
	{
		herestage = maxs - 1;
		lastherestage = maxs - 1;
	}

	GlobalData::setIsCurStagePass(false);

	_scrolllayer = Layer::create();
	_spritesContainer = Layer::create();

	int c = 0; 
	for (c = 0; c < themecount; c++)
	{
		mapbg.at(c)->setPosition(Vec2(center) + Vec2(0, visibleSize.height * c));
		mapStageLayer.at(c)->setPosition(Vec2(0,0) + Vec2(0, visibleSize.height * c));

		auto action = CSLoader::createTimeline(mapStageLayer.at(c)->getName());
		mapStageLayer.at(c)->runAction(action);
		action->gotoFrameAndPlay(0);

		_spritesContainer->addChild(mapbg.at(c), -1);
		_spritesContainer->addChild(mapStageLayer.at(c), 2);
	}
	//add coming.jpg

	int maxstagepos;
	int maxchapter = GlobalData::covertChapterStage(maxs - 1, maxstagepos);

	std::string maxstagestr = StringUtils::format("stagebtn%d", maxstagepos);
	Node* maxStageItem = mapStageLayer[maxchapter]->getChildByName(maxstagestr);

	Sprite* coming = Sprite::createWithSpriteFrameName("themecover.png");
	coming->setAnchorPoint(Vec2(0.5, 0));
	coming->setPosition(Vec2(visibleSize.width / 2, maxStageItem->getPositionY() + 960 * maxchapter));
	_spritesContainer->addChild(coming, 3);

	Sprite* comingtext = Sprite::createWithSpriteFrameName("themecomingtext.png");
	comingtext->setPosition(Vec2(coming->getContentSize().width / 2, coming->getPositionY() + (960 - maxStageItem->getPositionY()) / 2));
	_spritesContainer->addChild(comingtext, 3);

	//GlobalData::setUpgradeUrl("http://121.199.57.69:7777/applist/1.zip");
	GlobalData::setUpgradeUrl("");
	if (GlobalData::getUpgradeUrl().length() > 0)
	{
		comingtext->setVisible(false);
		Sprite* item0 = Sprite::createWithSpriteFrameName("updatebtn.png");
		Sprite* item1 = Sprite::createWithSpriteFrameName("updatebtn.png");
		upgradebtn = MenuItemSprite::create(
			item0,
			item1,
			item0,
			CC_CALLBACK_1(ThemeScene::getUpdateCallback, this));
		upgradebtn->setPosition(comingtext->getPosition());
		item1->setScale(0.95f);
		float scale = 1.0f - item1->getScale();
		item1->setPosition(item1->getContentSize().width*scale / 2.0f, item1->getContentSize().height*scale / 2.0f);
		Menu* menu = Menu::create();
		menu->addChild(upgradebtn);
		menu->setPosition(0, 0);
		_spritesContainer->addChild(menu, 3);
	}


	int mapsize = themecount;
	for (int i = 0; i < mapsize; i++)
	{
		std::string pageStar = StringUtils::format("page%dstar", i);
		std::string starsStr = GlobalData::getChapterStageStar(pageStar);
		long long pageStageStar = atoll(starsStr.c_str());

		int lastchapterStagenums = 0;
		for (int n = 0; n < i; n++)
		{
			lastchapterStagenums += GlobalData::vec_chapternum.at(n);
		}

		int chaptersize = GlobalData::vec_chapternum.at(i);
		for (int m = 0; m < chaptersize; m++)
		{
			bool isboss = (lastchapterStagenums + m + 1) % BOSSSTAGEPOS == 0 ? true : false;
			int star = pageStageStar >> (m * 2) & 3;

			std::string btnstr = StringUtils::format("stagebtn%d", m);
			cocos2d::ui::Widget* stagebtn = (cocos2d::ui::Widget*)mapStageLayer[i]->getChildByName(btnstr);

			stagebtn->addTouchEventListener(CC_CALLBACK_2(ThemeScene::clickStageCallback, this));
			Stage* stage = new Stage(stagebtn->getChildByName("stagenode"));
			stage->setStar(star, isboss);
			stage->setStageNum(lastchapterStagenums + m + 1);
			stagebtn->setTag(lastchapterStagenums + m);
			stagebtn->setEnabled(lastchapterStagenums + m <= maxStage + 1);
			//stagebtn->setEnabled(true);
			if (lastchapterStagenums + m + 1 > GlobalData::getTotalStage())
			{
				stagebtn->setVisible(false);
			}
			vec_stages.push_back(stage);
			
			if (lastchapterStagenums + m == maxStage + 1)
			{
				stage->nomal();
			}
			if (star >= 3)
			{
				stage->hilight();
			}

			//解锁条件

			if (i > 0 && m == 0)
			{
				int lastpagestarcount = 0;
				for (int c = 0; c <= i - 1; c++)
				{
					int laststagesize = GlobalData::vec_chapternum[c];
					for (int n = 0; n < laststagesize; n++)
					{
						std::string lastpagestr = StringUtils::format("page%dstar", c);
						std::string lastpagestarsStr = GlobalData::getChapterStageStar(lastpagestr);
						long long lastpageStars = atoll(lastpagestarsStr.c_str());
						int star = lastpageStars >> (n * 2) & 3;
						lastpagestarcount += star;
					}
				}
				int stagecount = 0;
				for (int n = 0; n < i; n++)
				{
					stagecount += GlobalData::vec_chapternum[n];
				}

				if (lastpagestarcount < GlobalData::vec_unlockstars[i - 1])
				{
					if (!GlobalData::getChapterUnlock(i - 1))
					{
						stage->showLock(GlobalData::vec_unlockstars[i - 1]);

						if (maxStage >= stagecount)
						{
							stage->removeLock();
							GlobalData::setChapterUnlock(i - 1, true);
							GlobalData::setShowChapterUnlock(i - 1, true);
						}
					}
				}
				else
				{
					if (!GlobalData::getChapterUnlock(i - 1))
					{
						stage->showLock(GlobalData::vec_unlockstars[i - 1]);

						if (maxStage >= stagecount)
						{
							stage->removeLock();
							GlobalData::setChapterUnlock(i - 1, true);
							GlobalData::setShowChapterUnlock(i - 1, true);
						}

						if (maxStage >= stagecount - 1)
						{
							GlobalData::setChapterUnlock(i - 1, true);
						}

					}
				}
			}
		}
	}

	int herestagepos;
	int herechapter = GlobalData::covertChapterStage(herestage, herestagepos);
	std::string tmpstr = StringUtils::format("stagebtn%d", herestagepos);

	offset = mapStageLayer[herechapter]->getChildByName(tmpstr)->getPositionY() + herechapter * 960 - 480;
	if (offset <= 0)
		offset = 0;

	_spritesContainer->setPosition(0, 0);
	scrollView = ScrollView::create();
	scrollView->setContainer(_spritesContainer);
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setPosition(0, 0);
	scrollView->setViewSize(visibleSize);
	scrollView->setContentSize(Size(visibleSize.width, visibleSize.height * (themecount)));

	scrollView->setContentOffset(Point(0, -offset), isScrollAnim);

	scrollView->setDelegate(this);

	scrollView->setBounceable(true);
	_scrolllayer->addChild(scrollView);
	addChild(_scrolllayer, 0);

	int laststagepos;
	int lastchapter = GlobalData::covertChapterStage(lastherestage, laststagepos);
	string lastherestagestr = StringUtils::format("stagebtn%d", laststagepos);
	Node* hereStageItem = mapStageLayer[lastchapter]->getChildByName(lastherestagestr);

	here = Sprite::createWithSpriteFrameName("hereblank.png");

	here->setPosition(Vec2(hereStageItem->getPositionX(), herechapter * 960 + hereStageItem->getPositionY() + 60));
	_spritesContainer->addChild(here, 4);

	Sprite* myhead = Sprite::createWithSpriteFrameName("here.png");
	myhead->setPosition(Vec2(here->getContentSize().width / 2, here->getContentSize().height / 2));
	here->addChild(myhead, 0, "myhead");

	hereqqNode = CSLoader::createNode("stagegx.csb");
	hereqqNode->setVisible(false);
	_spritesContainer->addChild(hereqqNode, 1);

	if (lastherestage < herestage)
	{
		string str = StringUtils::format("stagebtn%d", herestagepos);
		Node* moveToItem = mapStageLayer[herechapter]->getChildByName(str);

		if (herechapter > 0 && herestagepos == 0)
		{
			here->setPosition(Vec2(hereStageItem->getPositionX(), (herechapter - 1) * 960 + hereStageItem->getPositionY() + 60));
		}

		showStageboom(Vec2(moveToItem->getPositionX(), herechapter * 960 + moveToItem->getPositionY()));

		here->runAction(Sequence::create(DelayTime::create(0.5f), MoveTo::create(1.0f, Vec2(moveToItem->getPositionX(), herechapter * 960 + moveToItem->getPositionY() + 60)), NULL));
		this->scheduleOnce(schedule_selector(ThemeScene::delayShowHeadUpAndDown), 1.7f);
	}
	else
	{
		showMyHeadAnim(Vec2(hereStageItem->getPositionX(), herechapter * 960 + hereStageItem->getPositionY()));
		here->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1, Vec2(0, 5)), MoveBy::create(1, Vec2(0, -5)), NULL)));
	}

	int logintype = GlobalData::getLoginType();
	if (logintype > LOGIN_NONE)
	{
		checkMyHead(0);
	}
}

void ThemeScene::showHereMove(int stage)
{
	if (stage < maxs)
	{
		int stagepos;
		int chapter = GlobalData::covertChapterStage(stage, stagepos);
		string str = StringUtils::format("stagebtn%d", stagepos);
		Node* moveToItem = mapStageLayer[chapter]->getChildByName(str);
		Stage* ostage = vec_stages[stage];
		ostage->nomal();
		showStageboom(Vec2(moveToItem->getPositionX(), chapter * 960 + moveToItem->getPositionY()));
		here->runAction(Sequence::create(DelayTime::create(0.5f), MoveTo::create(1.0f, Vec2(moveToItem->getPositionX(), chapter * 960 + moveToItem->getPositionY() + 60)), NULL));
		this->scheduleOnce(schedule_selector(ThemeScene::delayShowHeadUpAndDown), 1.7f);
	}

	if (stage - 1 >= 0)
	{
		int stagepos;
		int chapter = GlobalData::covertChapterStage(stage, stagepos);
		string str = StringUtils::format("stagebtn%d", stagepos);
		Node* curItem = mapStageLayer[chapter]->getChildByName(str);
		Stage* ostage = vec_stages[stage - 1];
		ostage->setStar(3);
		ostage->hilight();
	}
}

void ThemeScene::delayShowHeadUpAndDown(float dt)
{
	showMyHeadAnim(Vec2(here->getPositionX(), here->getPositionY() - 50));
	here->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1, Vec2(0, 5)), MoveBy::create(1, Vec2(0, -5)), NULL)));
}

void ThemeScene::showStageboom(Vec2 pos)
{
	stageboomnode = CSLoader::createNode("stageboom.csb");
	stageboomnode->setPosition(pos);
	stageboomnode->setVisible(false);
	_spritesContainer->addChild(stageboomnode, 10);
	this->scheduleOnce(schedule_selector(ThemeScene::delayShowStageboom), 0.3f);
}

void ThemeScene::delayShowStageboom(float dt)
{
	ActionTimeline* action = CSLoader::createTimeline("stageboom.csb");
	stageboomnode->setVisible(true);
	stageboomnode->runAction(action);
	action->gotoFrameAndPlay(0, false);
	this->scheduleOnce(schedule_selector(ThemeScene::HideStageBoom), 1.5f);
}

void ThemeScene::HideStageBoom(float dt)
{
	stageboomnode->setVisible(false);
}

void ThemeScene::clickStageCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		log("ThemeScene clickStageCallback GlobalData::getIsClickNext() = %d, SureStageLayer::isShowing = %d", GlobalData::getIsClickNext(), SureStageLayer::isShowing);

		if (g_NewerLayer != NULL)
		{
			_scrolllayer->setLocalZOrder(0);
			g_NewerLayer->removeSelf();
		}

		if (GlobalData::getIsClickNext() || SureStageLayer::isShowing)
			return;

		Node* item = (Node*)pSender;
		int tag = item->getTag();
		GlobalData::setPlayingStage(tag);

		int stagepos;
		int chapter = GlobalData::covertChapterStage(tag, stagepos);
		here->setPosition(Vec2(item->getPositionX(), chapter * 960 + item->getPositionY() + 60));

		showMyHeadAnim(Vec2(item->getPositionX(), chapter * 960 + item->getPositionY()));
		delayShowSureStageLayer(0);
	}
}

void ThemeScene::showMyHeadAnim(Vec2 pos)
{
	hereqqNode->setVisible(true);
	hereqqNode->setPosition(pos);
	hereqqNode->stopAllActions();
	hereqqAction = CSLoader::createTimeline("stagegx.csb");
	hereqqAction->gotoFrameAndPlay(0, true);
	hereqqNode->runAction(hereqqAction);
}


void ThemeScene::delayShowSureStageLayer(float dt)
{
	GlobalData::setIsClickNext(false);

	if (g_commomLayer != NULL)
	{
		log("ThemeScene::delayShowSureStageLayer g_commomLayer->isnewerguiding = %d", g_commomLayer->isnewerguiding);
		if (g_commomLayer->isnewerguiding)
			return;
	}

	log("ThemeScene::delayShowSureStageLayer g_commomLayer->isShowingUnlock = %d", ThemeScene::isShowingUnlock);
	if (ThemeScene::isShowingUnlock)
		return;

	log("ThemeScene::delayShowSureStageLayer GlobalData::getPlayingStage() = %d, maxs = %d", GlobalData::getPlayingStage(), maxs);
	if (GlobalData::getPlayingStage() >= maxs)
		return;

	int stagepos;
	int unlockchapter = GlobalData::covertChapterStage(GlobalData::getPlayingStage(), stagepos);
	if (unlockchapter > 0 && !GlobalData::getChapterUnlock(unlockchapter - 1))
	{
		int lastpagestarcount = 0;

		for (int c = 0; c <= unlockchapter - 1; c++)
		{
			int laststagesize = GlobalData::vec_chapternum.at(c);
			for (int n = 0; n < laststagesize; n++)
			{
				std::string lastpagestr = StringUtils::format("page%dstar", c);
				std::string lastpagestarsStr = GlobalData::getChapterStageStar(lastpagestr);
				long long lastpageStars = atoll(lastpagestarsStr.c_str());
				int star = lastpageStars >> (n * 2) & 3;
				lastpagestarcount += star;
			}
		}
		UnlockLayer* layer = UnlockLayer::create(unlockchapter - 1, GlobalData::vec_unlockstars[unlockchapter - 1] - lastpagestarcount);
		Director::getInstance()->getRunningScene()->addChild(layer);
		return;
	}
	openSureStageLayer(0);
}

void ThemeScene::openSureStageLayer(float dt)
{
	GlobalData::setChallengeMode(CMNORMAL);
	SureStageLayer *pLayer = SureStageLayer::create();
	pLayer->setPosition(Vec2(0, 0));
	Director::getInstance()->getRunningScene()->addChild(pLayer);
}

void ThemeScene::restartShowSureStageLayer(float dt)
{
	GlobalData::setChallengeMode((ChallengeMode)GlobalData::getChallengeMode());

	SureStageLayer *pLayer = SureStageLayer::create();
	pLayer->setPosition(Vec2(0, 0));
	Director::getInstance()->getRunningScene()->addChild(pLayer);
}

void ThemeScene::checkMyHead(float dt)
{
	bool isneedupdate = true;
	string headimgname;
	if (GlobalData::getLoginType() == LOGIN_QQ)
		headimgname = "qhead00.png";
	else
		headimgname = "wxhead00.png";
	std::string fullpath = FileUtils::getInstance()->getWritablePath() + headimgname;

	bool isHasfile = FileUtils::getInstance()->isFileExist(fullpath);
	if (isHasfile)
	{
		Data filedata = FileUtils::getInstance()->getDataFromFile(fullpath.c_str());
		if (!filedata.isNull())
		{
			Image* img = new Image;
			img->initWithImageData(filedata.getBytes(), filedata.getSize());
			Texture2D* texture = new cocos2d::Texture2D();
			bool isImg = texture->initWithImage(img);
			if (isImg)
			{
				isneedupdate = false;
				unschedule(schedule_selector(ThemeScene::checkMyHead));
				here->removeChildByName("myhead");
				Sprite* head = Sprite::createWithTexture(texture);
				head->setPosition(Vec2(here->getContentSize().width / 2, here->getContentSize().height / 2 + 5));
				int w = img->getWidth();
				int h = img->getHeight();
				head->setTextureRect(Rect(0, 0, w, h));
				head->setScaleX(54.0f / w);
				head->setScaleY(50.0f / h);
				here->addChild(head);
				texture->release();
			}
			img->release();
		}
		Sprite* headbox = Sprite::createWithSpriteFrameName("herebox.png");
		headbox->setPosition(Vec2(here->getContentSize().width / 2, here->getContentSize().height / 2));
		here->addChild(headbox);
	}

	if (isneedupdate)
	{
		if (!this->getScheduler()->isScheduled(schedule_selector(ThemeScene::checkMyHead), this))
			this->schedule(schedule_selector(ThemeScene::checkMyHead), 0.15f);
	}
}

void ThemeScene::checkShowUnlockAnim(float dt)
{
	int size = themecount;
	int maxstage = GlobalData::getHighestStage();
	for (int i = 0; i < size - 1; i++)
	{
		int paststage = 0;
		if (GlobalData::getChapterUnlock(i) && !GlobalData::getShowChapterUnlock(i))
		{
			this->unschedule(schedule_selector(ThemeScene::checkShowUnlockAnim));
			GlobalData::setShowChapterUnlock(i, true);

			for (int n = 0; n <= i; n++)
				paststage += GlobalData::vec_chapternum.at(n);
			Stage* ostage = vec_stages[paststage];
			ostage->removeLock();
			ShowUnlockAnim();
			break;
		}
	}
}

void ThemeScene::removeUnlockAnim(float dt)
{
	this->removeChildByName("unlocklayer");
	isShowingUnlock = false;
}

void ThemeScene::ShowUnlockAnim()
{
	isShowingUnlock = true;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	color->setPosition(Vec2(0, 0));
	this->addChild(color, 10, "unlocklayer");

	Sprite* light = Sprite::createWithSpriteFrameName("UI/lotterylight1.png");
	light->setPosition(color->getContentSize().width / 2, color->getContentSize().height / 2 + 150);
	color->addChild(light);
	light->runAction(RepeatForever::create(RotateTo::create(4, 720)));

	Sprite* unlocktext = Sprite::createWithSpriteFrameName("unlocktext.png");
	unlocktext->setPosition(light->getPosition());
	color->addChild(unlocktext);

	this->schedule(schedule_selector(ThemeScene::delayShowParticle), 0.8f, 1, 0.3f);

	this->scheduleOnce(schedule_selector(ThemeScene::removeUnlockAnim), 2.5f);
}

void ThemeScene::delayShowParticle(float dt)
{
	MyParticle::Show(getChildByName("unlocklayer"), SpeciallyName[7], 270, 700, true, 0.5);
	MyParticle::Show(getChildByName("unlocklayer"), SpeciallyName[8], 270, 700, true, 0.3f);
	MyParticle::Show(getChildByName("unlocklayer"), SpeciallyName[9], 270, 700, true, 0.5);
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_FIREWORKS);
}

bool ThemeScene::onTouchBegan(Touch * touch, Event *unused_event)
{
	return true;

}

void ThemeScene::onTouchMoved(Touch*touch, Event*unused_event)
{

}

void ThemeScene::onTouchEnded(Touch*touch, Event *unused_event)
{

}


void ThemeScene::scrollViewDidScroll(ScrollView* view)
{

}

void ThemeScene::getUpdateCallback(Ref* pSender)
{
	if (GlobalData::getBasePath().length() <= 0)
	{

	}
	else
	{
		upgradeLayer = UpgradeLayer::create();
		Director::getInstance()->getRunningScene()->addChild(upgradeLayer, 100);
		upgradeManager = new UpgradeManager(GlobalData::getUpgradeUrl().c_str(), GlobalData::getBasePath().c_str());
		upgradeManager->setDelegate(this);
		upgradeManager->update();
	}
}

void ThemeScene::delayShowNewerGuide(float t)
{
	std::vector<Node*> nodes;

	if (!GlobalData::checkGuide(0) && GlobalData::checkGuide(7))
	{
		if (GlobalData::getHighestStage() < 0)
		{
			_scrolllayer->setLocalZOrder(2);
			cocos2d::ui::Widget* stagebtn = (cocos2d::ui::Widget*)mapStageLayer[0]->getChildByName("stagebtn0");
			nodes.push_back(stagebtn);
			g_NewerLayer = NewerGuideLayer::create(7);
			mapStageLayer[0]->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}

	if (!GlobalData::checkGuide(21) && GlobalData::getHighestStage() == 8 && GlobalData::checkGuide(13))
	{
		_scrolllayer->setLocalZOrder(2);
		mapStageLayer[0]->setLocalZOrder(3);
		cocos2d::ui::Widget* stagebtn = (cocos2d::ui::Widget*)mapStageLayer[0]->getChildByName("stagebtn9");
		nodes.push_back(stagebtn);
		g_NewerLayer = NewerGuideLayer::create(13);
		g_NewerLayer->setPositionY(offset);
		mapStageLayer[0]->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
		return;
	}
}

void ThemeScene::onError(UpgradeManager::ErrorCode errorCode)
{
	upgradeLayer->removeFromParentAndCleanup(true);
	upgradeLayer = NULL;
}

/**
* @brief 下载进度（0 - 100）
* @param percent 进度百分比
*/
void ThemeScene::onProgress(double downedbytesize, double totalbytesize)
{
	upgradeLayer->updataUI(downedbytesize, totalbytesize);
}

/**
* @brief 下载下载完成并且解压完成
*/
void ThemeScene::onSuccess() 
{
	string wfile = StringUtils::format("%s/%s", GlobalData::getBasePath().c_str(), UPGRADETIMEFILE);
	FILE* file = fopen(wfile.c_str(), "wb");
	string systime = StringUtils::format("%d", GlobalData::getSystemSecTime());
	if (file)
	{
		fwrite(systime.c_str(), sizeof(unsigned char), systime.length(), file);
		fclose(file);
	}
	upgradebtn->setVisible(false);
	upgradeLayer->removeFromParentAndCleanup(true);
	upgradeLayer = NULL;

	MyTransitionScene::isloadingUpgrade = true;
	Director::getInstance()->replaceScene(MyTransitionScene::scene());
}

void ThemeScene::doBYData()
{

	int mmount = GlobalData::getBYMonth();
	int mday = GlobalData::getBYDay();

	char para[30] = { 0 };

	int pastday = 0;

	int buybytime = GlobalData::getBYTime();

	int systime = GlobalData::getSystemSecTime();

	pastday = (systime - buybytime) / (3600 * 24);

	bool ret = GlobalData::getIsUmp();
	if (buybytime == 0 && ret)
	{
		pastday = 1;
	}

	int  month = GlobalData::getMonthOfYear();
	int  day = GlobalData::getDayOfMonth();

	int bprice = GlobalData::getBYPrice();

	bool isGetBYGem = false;
	if (pastday > 0 && mday != day)
	{
		if (month != mmount)
		{
			if (ret)
			{
				isGetBYGem = true;
				GlobalData::setBYTime(GlobalData::getSystemSecTime());
				GlobalData::setBYMonth(month);
				GlobalData::setBYDay(day);
			}
			else
			{
				if (pastday <= 31)
				{
					isGetBYGem = true;
					GlobalData::setBYDay(day);

				}
				else
				{
					GlobalData::setBYTime(0);
				}
			}
		}
		else
		{
			isGetBYGem = true;
			GlobalData::setBYDay(day);
		}

	}
	if (isGetBYGem && bprice > 0)
	{
		string giftname[] = {"高级", "尊享", "特权", "至尊", "豪华" };
		int gemnum[] = {6, 8, 15, 25, 35};
		int i = 0;
		for (i = 0; i < sizeof(gemnum) / sizeof(gemnum[0]); i++)
		{
			if (bprice == gemnum[i])
				break;
		}
		GlobalData::setGemCount(GlobalData::getGemCount() + gemnum[i]);
		string desc = StringUtils::format("已成功领取今日%s礼包中的%d个钻石", giftname[i].c_str(), gemnum[i]);
		HintBox* hint = HintBox::create(desc, 0);

		addChild(hint, 1000);
	}
}
