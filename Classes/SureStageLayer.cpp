#include "SureStageLayer.h"
#include "tinyxml2/tinyxml2.h"
#include "GlobalData.h"
#include "HintBox.h"
#include "MyParticle.h"
#include "SoundManager.h"
#include "HpBuyLayer.h"
#include "FourTypeGift.h"
#include "ShopLayer.h"
#include "SuccLayer.h"
#include "ThemeScene.h"
#include "CommonLayer.h"
#include "CommonFuncs.h"
#include "HttpUtil.h"
#include "NewerGuideLayer.h"

Label* SureStageLayer::desclbl = NULL;
cocos2d::ui::ScrollView* SureStageLayer::scrollview = NULL;
bool SureStageLayer::isShowing = false;

std::string propdesc[] = { "快速变身：开局随机将5对图案变为同一种图案", "超级炸弹：开局随机将3对图案变成炸弹并消除", "冻结时间：开局冻结时间持续15秒钟" };
SureStageLayer::SureStageLayer() :pNode(NULL)
{
	desclbl = NULL;
	scrollview = NULL;
	isShowing = true;
	for (int i = 0; i < 3; i++)
	{
		usenum[i] = useGoldGemNum[i];
		isPropGuide[i] = false;
	}
}


SureStageLayer::~SureStageLayer()
{
	desclbl = NULL;
	scrollview = NULL;
	isShowing = false;
}

void SureStageLayer::onExit()
{
	Layer::onExit();
}

bool SureStageLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	isClick = false;
	visibleSize = Director::getInstance()->getVisibleSize();

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	color->setPosition(Vec2(0,0));
	this->addChild(color);

	
	cmode = GlobalData::getChallengeMode();

	if (cmode == CMBOSS)
	{
		pNode = CSLoader::createNode("bosschallenge.csb");

		string name;
		cocos2d::ui::ScrollView* scrollview = (cocos2d::ui::ScrollView*)pNode->getChildByName("ScrollView_1");

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

		int passedcount = GlobalData::getBestDefeatedBossCount();
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

		cocos2d::ui::TextBMFont* bestpassedcountLbl = (cocos2d::ui::TextBMFont*)pNode->getChildByName("bestpassedcount");
		bestpassedcountLbl->setString(StringUtils::format("x%d", passedcount));
	}
	else if (cmode == CMCLASIC || cmode == CMFLASH)
	{
		pNode = CSLoader::createNode("CMStartLayer.csb");

		cocos2d::ui::Widget* hpIcon = (cocos2d::ui::Widget*)pNode->getChildByName("SureStartBtn")->getChildByName("hpicon");
		cocos2d::ui::TextBMFont* hpbmLbl = (cocos2d::ui::TextBMFont*)pNode->getChildByName("SureStartBtn")->getChildByName("usehpbmlbl");
		cocos2d::ui::Widget* startText = (cocos2d::ui::Widget*)pNode->getChildByName("SureStartBtn")->getChildByName("starttext");
		cocos2d::ui::ImageView* title = (cocos2d::ui::ImageView*)pNode->getChildByName("title");
		hpIcon->setVisible(true);
		hpbmLbl->setVisible(true);
		startText->setVisible(true);

		if (cmode == CMCLASIC)
		{
			if (GlobalData::getCModeCount(CMCLASIC) > 0)
			{
				hpbmLbl->setVisible(false);
				pNode->getChildByName("SureStartBtn")->getChildByName("freetext")->setVisible(true);
			}
		}
		else
		{
			std::string str = StringUtils::format("UI/flashtitle.png");
			title->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
			title->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
			if (GlobalData::getCModeCount(CMFLASH) > 0)
			{
				hpbmLbl->setVisible(false);
				pNode->getChildByName("SureStartBtn")->getChildByName("freetext")->setVisible(true);
			}
		}
	}
	else
	{
		pNode = CSLoader::createNode("SureStartLayer.csb");
		scrollview = (cocos2d::ui::ScrollView*)pNode->getChildByName("ranknode")->getChildByName("ScrollView");

		desclbl = Label::createWithSystemFont(CommonFuncs::gbk2utf("获取数据中！"), FONT_NAME, 25);
		desclbl->setColor(Color3B(255, 255, 255));
		desclbl->setPosition(Vec2(270, 180));
		pNode->addChild(desclbl);
	}

	pNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	pNode->setPosition(Vec2(270, 480));
	pNode->setScale(0.1f);
	pNode->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	addChild(pNode, 10);

	if (cmode == CMNORMAL)
	{
		pNode = pNode->getChildByName("contentnode");
#ifndef USENETWORK
		desclbl->setVisible(false);
		pNode->setPositionY(570);
		pNode->getParent()->getChildByName("ranknode")->setVisible(false);
#endif
	}

	SureStart(cmode);

	cocos2d::ui::Widget* m_sureCloseBtn = (cocos2d::ui::Widget*)pNode->getChildByName("close");
	m_sureCloseBtn->addTouchEventListener(CC_CALLBACK_2(SureStageLayer::close, this));


	int rand_seed = GlobalData::getSystemSecTime();
	srand(rand_seed);
	int tipszise = sizeof(tips) / sizeof(tips[0]);
	int r = rand() % tipszise;
	tipsbmlbl = (cocos2d::ui::TextBMFont*)pNode->getChildByName("tipsbmlbl");
	tipsbmlbl->setString(CommonFuncs::gbk2utf(tips[r].c_str()));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void SureStageLayer::isAnimFinished(float dt)
{
	isShowing = false;
}

void SureStageLayer::onEnterTransitionDidFinish()
{
	if (cmode == CMNORMAL)
	{
		this->scheduleOnce(schedule_selector(SureStageLayer::isAnimFinished), 0.5f);

		this->scheduleOnce(schedule_selector(SureStageLayer::delayShowNewerGuide), 0.5f);

		int stage = GlobalData::getPlayingStage() + 1;
		GlobalData::rankData.clear();
		string headimgname;
		for (int i = 1; i <= 3; i++)
		{
			headimgname = StringUtils::format("srhead%02d.png", i);
			std::string fullpath = FileUtils::getInstance()->getWritablePath() + headimgname;

			bool isHasfile = FileUtils::getInstance()->isFileExist(fullpath);
			if (isHasfile)
				FileUtils::getInstance()->removeFile(fullpath);
		}
#ifdef USENETWORK
		getStageRankData(stage);
#endif
	}
}
#ifdef USENETWORK
void SureStageLayer::getStageRankData(int stage)
{
	string url;
	url.append(HTTPURL);
	url.append("llk_stageranklist");
	url.append("&id=");
	url.append(GlobalData::getMyID());
	url.append("&stage=");
	std::string stagestr = StringUtils::format("%d", stage);
	url.append(stagestr);
	HttpUtil::getInstance()->doData(url, httpCompleteCB);
}

void SureStageLayer::httpCompleteCB(std::string retdata, int code, std::string tag)
{
	rapidjson::Document doc;

	if (code == 0)
	{
		doc.Parse<0>(retdata.c_str());
		if (doc.HasParseError())  //打印解析错误
		{
			//log("GetParseError %s\n", doc.GetParseError());
		}
		else
		{
			int myrank = 0;
			RankData myrankdata;
			if (doc.IsObject() && doc.HasMember("ret"))
			{
				rapidjson::Value& objectData = doc["ret"];
				myrank = objectData.GetInt();
				myrankdata.rank = myrank;
				strcpy(myrankdata.name, GlobalData::getMyName().c_str());
				strcpy(myrankdata.headimg, GlobalData::getHeadImg().c_str());
				GlobalData::rankData.push_back(myrankdata);
			}
			if (doc.IsObject() && doc.HasMember("data"))
			{
				rapidjson::Value& objectData = doc["data"];
				if (objectData.IsObject() && objectData.HasMember("list"))
				{
					rapidjson::Value& jsonArray = objectData["list"];
					if (jsonArray.IsArray())
					{
						int size = jsonArray.Size();
						for (int i = 0; i < size; i++)
						{
							rapidjson::Value& arraydoc = jsonArray[i];
			
							RankData rankdata;
							rankdata.rank = i + 1;
							rapidjson::Value& jsondata = arraydoc["nickname"];
							strcpy(rankdata.name, jsondata.GetString());
							jsondata = arraydoc["headportrait"];
							strcpy(rankdata.headimg, jsondata.GetString());

							jsondata = arraydoc["score"];
							rankdata.score = jsondata.GetInt();
							GlobalData::rankData.push_back(rankdata);

						}
					}
				}
			}
		}
	}
	if (GlobalData::rankData.size() > 1)
	{
		HttpUtil::getInstance()->doData(GlobalData::rankData[1].headimg, httpDownImgCB, "srhead01.png", HTTPTYPE::GET, "", "1");
	}
	else
	{
		if (desclbl != NULL)
			desclbl->setString(CommonFuncs::gbk2utf(""));
	}
}


void SureStageLayer::httpDownImgCB(std::string retdata, int code, std::string tag)
{
	int next = atoi(tag.c_str()) + 1;
	int size = GlobalData::rankData.size();
	if (size - 1 >= next)
	{
		string url = GlobalData::rankData[next].headimg;
		string headfilename = StringUtils::format("srhead%02d.png", next);
		string tag = StringUtils::format("%d", next);
		HttpUtil::getInstance()->doData(GlobalData::rankData[next].headimg, httpDownImgCB, headfilename, HTTPTYPE::GET, "", tag);
	}
	else
	{
		showRankData();
	}
}

void SureStageLayer::showRankData()
{
	if (scrollview == NULL)
		return;
	std::string myrankstr;
	cocos2d::ui::Text* myranktext = (cocos2d::ui::Text*)scrollview->getParent()->getChildByName("myranktext");

	int stage = GlobalData::getPlayingStage() + 1;
	if (GlobalData::rankData.size() > 0 && GlobalData::getstagebestscore(stage) > 0)
	{
		myrankstr = StringUtils::format("%d", GlobalData::rankData[0].rank);
		myranktext->setString(myrankstr);
	}
	
	int size = GlobalData::rankData.size();
	if (size > 0)
	{
		for (int i = 1; i < size; i++)
		{
			SRankItem* item = SRankItem::createWithData(GlobalData::rankData[i], i);
			item->setPosition(Vec2(72 + 165 * (i - 1), scrollview->getContentSize().height/2));
			std::string str = StringUtils::format("item%d", i);
			if (scrollview != NULL)
				scrollview->addChild(item, 0, str);
		}
		if (desclbl != NULL)
			desclbl->setString("");
	}
	else
	{
		if (desclbl != NULL)
			desclbl->setString(CommonFuncs::gbk2utf("获取数据失败，请检查网络"));
	}
}
#endif

void SureStageLayer::SureStart(ChallengeMode mode)
{

	m_sureStartBtn = (cocos2d::ui::Widget*)pNode->getChildByName("SureStartBtn");
	m_sureStartBtn->addTouchEventListener(CC_CALLBACK_2(SureStageLayer::start, this));

	if (mode == CMBOSS)
	{
		_useHpCount = 3;

		int size = GlobalData::mapBossSkillData.size();
		for (int i = 0; i < size; i++)
		{
			GlobalData::mapBossSkillData[i].tdata.clear();
			GlobalData::mapBossSkillData[i].edata.clear();
		}
		parseBossSkillConfig(GlobalData::getXMLDataPath("data/boss/bossskill.xml"));
	}
	else if (mode == CMCLASIC)
	{
		_useHpCount = CLASICUSEHP;
		if (GlobalData::getCModeCount(CMCLASIC) > 0)
			_useHpCount = 0;

		GlobalData::CPData.clear();
		if (GlobalData::CPData.size() <= 0)
		{
			parseCPConfig(GlobalData::getXMLDataPath("data/clasic/clasicprop.xml"));
		}
	}
	else if (mode == CMFLASH)
	{
		_useHpCount = 3;
		if (GlobalData::getCModeCount(CMFLASH) > 0)
			_useHpCount = 0;

		GlobalData::CPData.clear();
		if (GlobalData::CPData.size() <= 0)
		{
			parseCPConfig(GlobalData::getXMLDataPath("data/flash/flashprop.xml"));
		}

		if (GlobalData::FRuleData.size() <= 0)
			parseFlashRuleConfig(GlobalData::getXMLDataPath("data/flash/rule.xml"));

	}
	else
	{
		int stage;
		int chapter = GlobalData::covertChapterStage(GlobalData::getPlayingStage(), stage);

		string filename = StringUtils::format("data/%d-%d-%d.xml", chapter + 1, stage + 1, 1);
		ReadConfig(GlobalData::getXMLDataPath(filename));

		if ((GlobalData::getPlayingStage() + 1) % BOSSSTAGEPOS == 0)
		{
			int size = GlobalData::mapBossSkillData.size();
			for (int i = 0; i < size; i++)
			{
				GlobalData::mapBossSkillData[i].tdata.clear();
				GlobalData::mapBossSkillData[i].edata.clear();
			}
			parseBossSkillConfig(GlobalData::getXMLDataPath("data/bossskill.xml"));
		}
		cocos2d::ui::Widget* passBtn = (cocos2d::ui::Widget*)pNode->getChildByName("passbtn");
		passBtn->addTouchEventListener(CC_CALLBACK_2(SureStageLayer::passstage, this));
		passBtn->setVisible(GlobalData::getCurStageContinueFailCount() >= 3 ? true : false);

		string stageStr = StringUtils::format("%d", GlobalData::getPlayingStage() + 1);
		cocos2d::ui::TextBMFont* stagebmlbl = (cocos2d::ui::TextBMFont*)pNode->getChildByName("surestagebmlbl");
		stagebmlbl->setString(stageStr);

		std::string timeStr = StringUtils::format("%02d:%02d", _timeNum / 60, _timeNum % 60);
		cocos2d::ui::TextBMFont* timebmlbl = (cocos2d::ui::TextBMFont*)pNode->getChildByName("suretimebmlbl");
		timebmlbl->setString(timeStr);
		if (!passBtn->isVisible())
			m_sureStartBtn->setPositionX(0);
	}

	cocos2d::ui::TextBMFont* hpbmLbl = (cocos2d::ui::TextBMFont*)pNode->getChildByName("SureStartBtn")->getChildByName("usehpbmlbl");


	GlobalData::setCurStageUseHp(_useHpCount);
	string usehpstr = StringUtils::format("x%d", _useHpCount);
	hpbmLbl->setString(usehpstr);

	Node* selectpropNode = (cocos2d::ui::Widget*)pNode->getChildByName("selectpropnode");

	if (cmode == CMNORMAL)
	{
		selectpropNode = pNode;
	}
	/*普通关卡为了做新手引导 selectpropNode 弄成整个node的拿出来*/
	std::string str;
	for (int i = 0; i<3; i++)
	{
		str = StringUtils::format("surepropbtn%d",i);
		propBtn[i] = (cocos2d::ui::Widget*)selectpropNode->getChildByName(str);
		propBtn[i]->setTag(i);
		propBtn[i]->addTouchEventListener(CC_CALLBACK_2(SureStageLayer::selectProp, this));

		str = StringUtils::format("smallgold_%d", i + 1);
		goldIcon[i] = (cocos2d::ui::Widget*)selectpropNode->getChildByName(str);

		str = StringUtils::format("suregoldnumlbl_%d", i + 1);
		goldNum[i] = (cocos2d::ui::TextBMFont*)selectpropNode->getChildByName(str);

		std::string numstr = StringUtils::format("x%d", usenum[i]);
		goldNum[i]->setVisible(true);
		goldNum[i]->setString(numstr);

		str = StringUtils::format("startisbuy_%d", i);
		isbuyText[i] = (cocos2d::ui::Widget*)selectpropNode->getChildByName(str);
		isbuyText[i]->setVisible(false);

		//str = StringUtils::format("select%d", i);
		//select[i] = (cocos2d::ui::Widget*)selectpropNode->getChildByName(str);

		str = StringUtils::format("get_%d", i);
		getgou[i] = (cocos2d::ui::Widget*)selectpropNode->getChildByName(str);

		if (GlobalData::getIsFirst(mode, i))
		{
			goldIcon[i]->setVisible(false);
			goldNum[i]->setVisible(false);
			str = StringUtils::format("freetext_%d", i + 1);
			freeText[i] = (cocos2d::ui::Widget*)selectpropNode->getChildByName(str);
			freeText[i]->setVisible(true);
		}
	}
}

int SureStageLayer::ReadConfig(string fileName)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	unsigned char pBuffer[5000];
	memset(pBuffer, 0x00, sizeof(pBuffer));
	strcpy((char *)pBuffer, FileUtils::getInstance()->getStringFromFile(fileName).c_str());
	int err = pDoc->Parse((const char*)pBuffer);
	if (err != 0)
	{
		delete pDoc;
		return err;
	}

	tinyxml2::XMLElement *rootEle = pDoc->RootElement();
	tinyxml2::XMLElement *element = rootEle->FirstChildElement();

	if (element != NULL)
	{
		if (strcmp(element->Name(), "s") == 0)
		{
			_timeNum = element->IntAttribute("t");
			_useHpCount = element->IntAttribute("fb");
		}
	}
	delete pDoc;

	return 0;
}

void SureStageLayer::parseFlashRuleConfig(string filepath)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	unsigned char pBuffer[5000];
	memset(pBuffer, 0x00, sizeof(pBuffer));
	strcpy((char *)pBuffer, FileUtils::getInstance()->getStringFromFile(filepath).c_str());
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
		if (strcmp(element->Name(), "r") == 0)
		{
			vector<string> tmp;
			CommonFuncs::split(element->Attribute("ttime"), tmp, ",");
			vector<string> tmp2;
			CommonFuncs::split(element->Attribute("ecount"), tmp2, ",");
			vector<string> tmp3;
			CommonFuncs::split(element->Attribute("ele"), tmp3, ",");
			for (unsigned int i = 0; i < tmp.size(); i++)
			{
				FlashRuleData data;
				data.ttime = atof(tmp[i].c_str());
				data.ecount = atoi(tmp2[i].c_str());
				data.ele = atoi(tmp3[i].c_str());
				data.isdone = false;
				GlobalData::FRuleData.push_back(data);
			}
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

void SureStageLayer::parseCPConfig(string filepath)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	unsigned char pBuffer[5000];
	memset(pBuffer, 0x00, sizeof(pBuffer));
	strcpy((char *)pBuffer, FileUtils::getInstance()->getStringFromFile(filepath).c_str());
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
		if (strcmp(element->Name(), "p") == 0)
		{
			vector<string> tmp;
			CommonFuncs::split(element->Attribute("tcount"), tmp, ",");
			vector<string> tmp2;
			CommonFuncs::split(element->Attribute("type"), tmp2, ",");
			for (unsigned int i = 0; i < tmp.size(); i++)
			{
				ClasicPropData cpdata;
				cpdata.tcount = atoi(tmp[i].c_str());
				cpdata.etype = atoi(tmp2[i].c_str());
				GlobalData::CPData.push_back(cpdata);
			}
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

void SureStageLayer::parseBossSkillConfig(string filepath)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	unsigned char pBuffer[5000];
	memset(pBuffer, 0x00, sizeof(pBuffer));
	strcpy((char *)pBuffer, FileUtils::getInstance()->getStringFromFile(filepath).c_str());
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
			int bid = element->IntAttribute("bid");
			vector<string> tmp;
			CommonFuncs::split(element->Attribute("tcount"), tmp, ",");
			vector<string> tmp2;
			CommonFuncs::split(element->Attribute("ecount"), tmp2, ",");
			int type = element->IntAttribute("type");
			GlobalData::mapBossSkillData[bid - 1].id = bid;
			GlobalData::mapBossSkillData[bid - 1].type = type;
			for (unsigned int i = 0; i < tmp.size(); i++)
				GlobalData::mapBossSkillData[bid - 1].tdata.push_back(atoi(tmp[i].c_str()));
			for (unsigned int i = 0; i < tmp2.size(); i++)
				GlobalData::mapBossSkillData[bid - 1].edata.push_back(atoi(tmp2[i].c_str()));
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

void SureStageLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (isClick)
			return;
		for (int i = 0; i < 3; i++)
		{
			if (!propBtn[i]->isBright())
			{
				GlobalData::setGemCount(GlobalData::getGemCount() + usenum[i]);
			}
			getgou[i]->setVisible(false);
		}

		if (g_commomLayer != NULL)
			g_commomLayer->scheduleOnce(schedule_selector(CommonLayer::delayShowNewerGuide), 0.2f);

		this->removeFromParentAndCleanup(true);
	}
}

void SureStageLayer::start(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (isClick)
			return;

		isClick = true;

		if (g_NewerLayer != NULL)
		{
			g_NewerLayer->removeSelf();
		}

		int hpcount = GlobalData::getHpCount();
		bool useChange = false;
		bool useBomb = false;
		bool useIce = false;

		if (hpcount >= _useHpCount)
		{
			for (int i = 0; i < 3; i++)
			{
				if (!propBtn[i]->isBright())
				{
					if (i == 0)
					{
						useChange = true;
					}
					else if (i == 1)
					{
						useBomb = true;
					}
					else if (i == 2)
					{
						useIce = true;
					}
				}
			}


			if (useChange)
			{
				GlobalData::setIsFirst(cmode, 0, false);

				Achieve* data = GlobalData::getAchieveDataByID(12);
				if (data->finish != -1)
				{
					data->finish ++;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getAchieveDataByID(26);;
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}
			}
			if (useBomb)
			{
				GlobalData::setIsFirst(cmode, 1, false);
					
				Achieve* data = GlobalData::getAchieveDataByID(11);
				if (data->finish != -1)
				{
					data->finish ++;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getAchieveDataByID(26);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getTaskDataByID(1);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveTaskData();
				}

			}
			if (useIce)
			{
				GlobalData::setIsFirst(cmode, 2, false);
				Achieve* data = GlobalData::getAchieveDataByID(10);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getAchieveDataByID(26);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getTaskDataByID(5);
				if (data->finish != -1)
				{
					data->finish++;
					GlobalData::SaveTaskData();
				}
			}

#ifdef ANALYTICS
			if (useChange)
			{
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };

				sprintf(p_str, "%s", "change");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1, 0.0f);
				}

				sprintf(p_str, "%s", "change");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1);
				}

				sprintf(p_str, "%s", "coin");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 100);
				}
			}
			if (useBomb)
			{
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };

				sprintf(p_str, "%s", "bomb");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1, 0.0f);
				}

				sprintf(p_str, "%s", "bomb");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1);
				}
				sprintf(p_str, "%s", "coin");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 200);
				}
			}
			if (useIce)
			{
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };


				sprintf(p_str, "%s", "ice");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1, 20.0f);
				}

				sprintf(p_str, "%s", "ice");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1);
				}


			}

			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "hp");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, _useHpCount);
			}
#endif
			GlobalData::setIsUseChange(useChange);
			GlobalData::setIsUseBomb(useBomb);
			GlobalData::setIsUseIce(useIce);

			GlobalData::setHpCount(hpcount - _useHpCount);

			auto sprite = Sprite::createWithSpriteFrameName("UI/hp.png");
			sprite->setPosition(Vec2(35, 920));
			addChild(sprite, 1000, 90); 
			ccBezierConfig bezierCon;
			bezierCon.controlPoint_1 = Vec2(270, 700);//控制点1
			bezierCon.controlPoint_2 = Vec2(100, 320);//控制点2

			int x = m_sureStartBtn->getPositionX();
			int y = m_sureStartBtn->getPositionY();
			if (cmode == CMNORMAL)
			{
				x = pNode->getPositionX() + m_sureStartBtn->getPositionX();
				y = pNode->getPositionY() + m_sureStartBtn->getPositionY();
			}
			bezierCon.endPosition = Vec2(x,y);// 结束位置  
			ActionInterval * action = BezierTo::create(0.9f, bezierCon);

			sprite->runAction(Sequence::create(action, CallFunc::create(CC_CALLBACK_0(SureStageLayer::PlaySpecially, this)), NULL));
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_USEHP);
			if (_useHpCount > 0)
			{
				Achieve *data = GlobalData::getTaskDataByID(7);
				if (data->finish != -1)
				{
					data->finish += _useHpCount;
					GlobalData::SaveTaskData();
				}
			}
		}
		else
		{
			isClick = false;

			HpBuyLayer::AnimType = A_UPTODOWN;
			HpBuyLayer* layer = HpBuyLayer::create();
			this->addChild(layer, 100);
		}
	}
}

void SureStageLayer::passstage(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		
		passColorLayer = LayerColor::create(Color4B(11, 32, 22, 192));
		this->addChild(passColorLayer, 111);
		passlayernode = CSLoader::createNode("passStageLayer.csb");
		this->addChild(passlayernode, 111);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [=](Touch *touch, Event *event)
		{
			return true;
		};
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, passlayernode);

		cocos2d::ui::Widget* passclosebtn = (cocos2d::ui::Widget*)passlayernode->getChildByName("closebtn");
		passclosebtn->addTouchEventListener(CC_CALLBACK_2(SureStageLayer::passstage_close, this));

		cocos2d::ui::Widget* passokbtn = (cocos2d::ui::Widget*)passlayernode->getChildByName("okbtn");
		passokbtn->addTouchEventListener(CC_CALLBACK_2(SureStageLayer::passstage_ok, this));
	}

}

void SureStageLayer::passstage_close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		passColorLayer->removeFromParentAndCleanup(true);
		passlayernode->removeFromParentAndCleanup(true);

	}
}

void SureStageLayer::passstage_ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (GlobalData::getGemCount() >= 50)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() - 50);
			GlobalData::setStageStars(3);
			SuccLayer::saveData();
			if (g_ThemeScene != NULL)
				g_ThemeScene->showHereMove(GlobalData::getPlayingStage() + 1);

			for (int i = 0; i < 3; i++)
			{
				if (!propBtn[i]->isBright())
				{
					GlobalData::setGemCount(GlobalData::getGemCount() + usenum[i]);
				}
			}
			this->removeFromParentAndCleanup(true);
		}
		else
		{
			FourTypeGift::AnimType = A_UPTODOWN;
			FourTypeGift::_type = GEMGIFT;
			FourTypeGift* giftLayer = FourTypeGift::create();
			Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
		}
	}
}

void SureStageLayer::ReplaceScene(float t)
{
	GlobalData::setCModeYearOfDay(cmode, GlobalData::getDayOfYear());
	GlobalData::setCModeCount(cmode, GlobalData::getCModeCount(cmode) - 1);
	this->removeFromParentAndCleanup(true);
	Director::getInstance()->replaceScene(GameScene::scene());
}

void SureStageLayer::PlaySpecially()
{
	removeChildByTag(90);

	int x = m_sureStartBtn->getPositionX();
	int y = m_sureStartBtn->getPositionY();
	if (cmode == CMNORMAL)
	{
		x = pNode->getPositionX() + m_sureStartBtn->getPositionX();
		y = pNode->getPositionY() + m_sureStartBtn->getPositionY();
	}
	MyParticle::Show(this, SpeciallyName[0], x , y, false, 0.3f);
	MyParticle::Show(this, SpeciallyName[1], x, y, true, 0.3f);

	scheduleOnce(schedule_selector(SureStageLayer::ReplaceScene), 1);
}

void SureStageLayer::selectProp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	Node* sender = (Node*)pSender;
	int index = sender->getTag();

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		selectindex = index;
		propBtn[index]->setBright(!propBtn[index]->isBright());

		if (propBtn[index]->isBright())
		{
			GlobalData::setGemCount(GlobalData::getGemCount() + usenum[index]);

			//select[index]->stopAllActions();
			//select[index]->setRotation(0);
			//select[index]->setVisible(false);
			tipsbmlbl->setString(CommonFuncs::gbk2utf(tips[0].c_str()));
		}
		else
		{
			if (GlobalData::getIsFirst(cmode, index) || isPropGuide[index])
			{
				usenum[index] = 0;
			}
			else if (GlobalData::getGemCount() < usenum[index])
			{
				propBtn[index]->setBright(!propBtn[index]->isBright());
				HintBox* hint = HintBox::create("钻石不足", 1);
				addChild(hint, 1000);
				return;
			}

			GlobalData::setGemCount(GlobalData::getGemCount() - usenum[index]);
			this->scheduleOnce(schedule_selector(SureStageLayer::delayShowSelectAnim), 0.1f);
			tipsbmlbl->setString(CommonFuncs::gbk2utf(propdesc[index].c_str()));
			showStartGuide();

		}
		getgou[index]->setVisible(!propBtn[index]->isBright());
		if (GlobalData::getIsFirst(cmode, index))
		{
			goldIcon[index]->setVisible(false);
			goldNum[index]->setVisible(false);
			isbuyText[index]->setVisible(false);
			freeText[index]->setVisible(true);
		}
		else
		{
			goldIcon[index]->setVisible(propBtn[index]->isBright());
			goldNum[index]->setVisible(propBtn[index]->isBright());
			isbuyText[index]->setVisible(!goldNum[index]->isVisible());
		}
	}
	else if (type == ui::Widget::TouchEventType::BEGAN)
	{
		//select[index]->setVisible(false);
	}
}

void SureStageLayer::delayShowSelectAnim(float dt)
{
	//select[selectindex]->setVisible(true);
	//select[selectindex]->runAction(RepeatForever::create(RotateTo::create(4, 720)));
}

void SureStageLayer::showStartGuide()
{
	if (g_NewerLayer != NULL)
	{
		g_NewerLayer->removeSelf();

		if (GlobalData::checkGuide(11))
		{
			int stage = GlobalData::getPlayingStage();
			if (stage == 2 || stage == 3 || stage == 4)//第3,4,5关引导道具
			{
				std::vector<Node*> nodes;
				nodes.push_back(m_sureStartBtn);
				g_NewerLayer = NewerGuideLayer::create(11);
				g_NewerLayer->setPosition(Vec2(-pNode->getPositionX(), -pNode->getPositionY()));
				pNode->addChild(g_NewerLayer, NEWERLAYERZOER);
				g_NewerLayer->setData(nodes);
				return;
			}
		}
	}
}

void SureStageLayer::delayShowNewerGuide(float t)
{
	if (GlobalData::checkGuide(8))
	{
		if (GlobalData::getPlayingStage() == 2)//第3关引导道具
		{
			isPropGuide[0] = true;
			std::vector<Node*> nodes;
			nodes.push_back(propBtn[0]);
			nodes.push_back(getgou[0]);
			g_NewerLayer = NewerGuideLayer::create(8);
			g_NewerLayer->setPosition(Vec2(-pNode->getPositionX(), -pNode->getPositionY()));
			pNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(9))
	{
		if (GlobalData::getPlayingStage() == 3)//第4关引导道具
		{
			isPropGuide[1] = true;
			std::vector<Node*> nodes;
			nodes.push_back(propBtn[1]);
			nodes.push_back(getgou[1]);
			g_NewerLayer = NewerGuideLayer::create(9);
			g_NewerLayer->setPosition(Vec2(-pNode->getPositionX(), -pNode->getPositionY()));
			pNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(10))
	{
		if (GlobalData::getPlayingStage() == 4)//第5关引导道具
		{
			isPropGuide[2] = true;
			std::vector<Node*> nodes;
			nodes.push_back(propBtn[2]);
			nodes.push_back(getgou[2]);
			g_NewerLayer = NewerGuideLayer::create(10);
			g_NewerLayer->setPosition(Vec2(-pNode->getPositionX(), -pNode->getPositionY()));
			pNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
}

SRankItem::SRankItem()
{
	head = NULL;
}

SRankItem::~SRankItem()
{
	head = NULL;
}

SRankItem* SRankItem::createWithData(RankData data, int index)
{
	SRankItem* item = new SRankItem;
	item->init(data, index);
	item->autorelease();
	return item;
}

bool SRankItem::init(RankData data, int index)
{

	if (Sprite::initWithSpriteFrameName("UI/stagerankitem.png"))
	{
		Label * mname = Label::createWithSystemFont(data.name, FONT_NAME, 12);
		mname->setColor(Color3B(81, 44, 19));
		mname->setAnchorPoint(Vec2(0, 0.5));
		mname->setPosition(Vec2(65, 60));
		addChild(mname);

		Sprite* sepline= Sprite::createWithSpriteFrameName("UI/stageranksep.png");
		sepline->setAnchorPoint(Vec2(0, 0.5));
		sepline->setPosition(Vec2(65, mname->getPositionY() - mname->getContentSize().height));
		addChild(sepline);
		
		std::string str = StringUtils::format("%d", data.score);
		for (int i = (int)str.length() - 3; i > 0; i -= 3)
			str.insert(i, ",");
		Label * score = Label::createWithSystemFont(str, FONT_NAME, 18);
		score->setColor(Color3B(255, 255, 255));
		score->setAnchorPoint(Vec2(0, 0.5));
		score->setPosition(Vec2(65, 35));
		addChild(score);

		if (index > 0)
		{
			if (data.score == GlobalData::rankData[index - 1].score)
				data.rank = GlobalData::rankData[index - 1].rank;
		}
		if (index > 0)
		{
			std::string rankstr = StringUtils::format("UI/srankmedel%d.png", index);
			Sprite* rankMedel = Sprite::createWithSpriteFrameName(rankstr);
			rankMedel->setAnchorPoint(Vec2(0, 0.5));
			rankMedel->setPosition(-5, this->getContentSize().height - 8);
			addChild(rankMedel, 1);
			upataheadimg(index);
		}
	}

	return true;
}

void SRankItem::upataheadimg(int index)
{
	if (head == NULL)
	{
		head = Sprite::createWithSpriteFrameName("UI/settinghead.png");
		head->setScale(0.65f);
		head->setPosition(Vec2(32, this->getContentSize().height / 2 + 10));
		this->addChild(head);
	}

	std::string headimgname = StringUtils::format("srhead%02d.png", index);

	std::string path = FileUtils::getInstance()->getWritablePath() + headimgname;

	bool isHasfile = FileUtils::getInstance()->isFileExist(path);
	if (isHasfile)
	{
		Data filedata = FileUtils::getInstance()->getDataFromFile(path.c_str());
		if (!filedata.isNull())
		{
			Image* img = new Image;
			img->initWithImageData(filedata.getBytes(), filedata.getSize());
			Texture2D* texture = new cocos2d::Texture2D();
			bool isImg = texture->initWithImage(img);

			if (isImg)
			{
				head->setTexture(texture);
				int w = img->getWidth();
				int h = img->getHeight();
				head->setTextureRect(Rect(0, 0, w, h));
				head->setPositionY(head->getPositionY() - 10);
				head->setScaleX(55.0f / w);
				head->setScaleY(55.0f / h);
				texture->release();
			}
			img->release();
		}

	}
}