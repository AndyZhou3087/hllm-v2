#include "LoadingScene.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "ThemeScene.h"
#include "GameScene.h"
#include "SuccLayer.h"
#include "ShopLayer.h"
#include "CommonFuncs.h"
#include "tinyxml2/tinyxml2.h"
#include "HintBox.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "HttpUtil.h"
#include "SettingLayer.h"
#include "MyTransitionScene.h"
#include "MD5.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

std::string listpng[] = { "map", "map1","map2", "block", "ui", "ui1", "ui2", "ui3", "ui4", "ui5", "game" };
int bar[] = { 5, 15, 25, 35, 45, 55, 65, 75, 85, 90, 93, 95, 98 };
LoadingScene::LoadingScene():m_pngType(0)
{
	m_serverChapter = 0;
}


LoadingScene::~LoadingScene()
{

}

Scene* LoadingScene::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	LoadingScene *layer = LoadingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool LoadingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto bg = Sprite::create("loadingBg.jpg");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	addChild(bg);

	auto logo = Sprite::create("jzjm_logo.png");
	logo->setPosition(Vec2(200, 830));
	addChild(logo);
	
	auto load = CSLoader::createNode("LoadingLayer.csb");
	addChild(load, 10);

	auto action = CSLoader::createTimeline("LoadingLayer.csb");
	load->runAction(action);
	action->gotoFrameAndPlay(0, 54, true);

	int rand_seed = GlobalData::getSystemSecTime();
	srand(rand_seed);
	int tipszise = sizeof(tips) / sizeof(tips[0]);
	int r = rand() % tipszise;

	Label* tipslbl = Label::createWithBMFont("fonts/tips.fnt", CommonFuncs::gbk2utf(tips[r].c_str()));
	tipslbl->setPosition(Vec2(270, 50));
	tipslbl->setScale(0.8f);
	addChild(tipslbl);

	loadingBar = ProgressTimer::create(Sprite::create("progressBar.png"));
	loadingBar->setType(ProgressTimer::Type::BAR);
	loadingBar->setBarChangeRate(Vec2(1, 0));
	loadingBar->setMidpoint(Vec2(0, 0));
	loadingBar->setAnchorPoint(Vec2(0, 0.5f));
	loadingBar->setPosition(Vec2(32, 105));
	loadingBar->setPercentage(5);
	load->addChild(loadingBar);

	loadprogress = Sprite::create("progressBarSunshine.png");
	loadprogress->setPosition(Vec2(32, 113));
	loadprogress->setVisible(false);
	this->addChild(loadprogress ,11);

	MyParticle::Show(loadprogress, SpeciallyName[30], loadprogress->getContentSize().width *0.8f, loadprogress->getContentSize().height / 2, true, -1);

	return true;

}
void LoadingScene::delayShowAnim(float dt)
{
	log("loading delayShowAnim start");
	bool isSoundOn = UserDefault::getInstance()->getBoolForKey("issoundon", true);
	SoundManager::sharedManager()->setIsSoundOn(isSoundOn);

	bool isMusicOn = UserDefault::getInstance()->getBoolForKey("ismusicon", true);
	SoundManager::sharedManager()->setIsMusicOn(isMusicOn);

	this->scheduleOnce(schedule_selector(LoadingScene::loadPng), 1.1f);
	log("loading delayShowAnim end");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	getUmpData();
#endif
}

void LoadingScene::onEnterTransitionDidFinish()
{
	log("loading onEnterTransitionDidFinish start");
	scheduleUpdate();

	GlobalData::setPlayingStage(-1);
	int count = folderCount(GlobalData::getDownloadMapPath());
	GlobalData::setServerChapterCount(count);
	this->scheduleOnce(schedule_selector(LoadingScene::delayShowAnim), 0.1f);
	log("loading onEnterTransitionDidFinish end");
	if (UserDefault::getInstance()->getBoolForKey("firstloading", true))
	{
		UserDefault::getInstance()->setBoolForKey("firstloading", false);

#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "enterloading");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
	}
}

void LoadingScene::update(float dt)
{
	int movex = loadingBar->getPositionX() + loadingBar->getPercentage()*loadingBar->getContentSize().width / 100 - 50;
	loadprogress->setVisible(true);
	loadprogress->setPositionX(movex);
}

void LoadingScene::updateBarLight()
{
	ProgressFromTo * fromto = ProgressFromTo::create(0.5f, loadingBar->getPercentage(), bar[m_pngType]);
	loadingBar->runAction(fromto);
}


void LoadingScene::loadPng(float dt)
{
	log("loading loadPng start m_pngType = %d", m_pngType);
	loadnextListPng(m_pngType);
	log("loading loadPng end m_pngType = %d", m_pngType);
}

void LoadingScene::loadnextListPng(int index)
{
	log("loading loadnextListPng start");
	Director::getInstance()->getTextureCache()->addImageAsync(listpng[index] + ".png", CC_CALLBACK_1(LoadingScene::LoadpList, this));
	log("loading loadnextListPng end");
}

void LoadingScene::LoadpList(cocos2d::Texture2D* texture)
{
	log("loading LoadpList start");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(listpng[m_pngType] + ".plist", texture);
	log("loading LoadpList start1111");
	updateBarLight();
	log("loading LoadpList start222");
	m_pngType++;
	log("loading LoadpList start m_pngType = %d", m_pngType);
	if (m_pngType == sizeof(listpng) / sizeof(listpng[0]))
	{
		if (GlobalData::getServerChapterCount() <= 0)
			this->scheduleOnce(schedule_selector(LoadingScene::delayLoadData), 0.5f);
		else
		{
			loadSDPng(LOCALTHEMECOUNT + 1);
		}
	}
	else
	{
		this->scheduleOnce(schedule_selector(LoadingScene::loadPng), 0.05f);
		//loadnextListPng(m_pngType);
	}
	log("loading LoadpList end listpngsize = %d", sizeof(listpng) / sizeof(listpng[0]));
}

void LoadingScene::loadSDPng(int index)
{
	log("loading loadSDPng start index = %d", index);
	m_serverChapter = index;
	string pngpath = StringUtils::format("%s/c%d/pic.png", GlobalData::getDownloadMapPath().c_str(), m_serverChapter);
	Director::getInstance()->getTextureCache()->addImageAsync(pngpath, CC_CALLBACK_1(LoadingScene::LoadpSDList, this));
	log("loading loadSDPng end index = %d", index);
}

void LoadingScene::LoadpSDList(cocos2d::Texture2D* texture)
{
	string plistpath = StringUtils::format("%s/c%d/pic.plist", GlobalData::getDownloadMapPath().c_str(), m_serverChapter);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistpath, texture);
	m_serverChapter++;

	log("loading LoadpSDList start m_serverChapter = %d", m_serverChapter);
	if (m_serverChapter == GlobalData::getServerChapterCount() + LOCALTHEMECOUNT + 1)
	{
		loadBossPng(LOCALTHEMECOUNT + 1);
	}
	else
	{
		loadSDPng(m_serverChapter);
	}
	log("loading LoadpSDList end GlobalData::getServerChapterCount() = %d", GlobalData::getServerChapterCount());
}

void LoadingScene::loadBossPng(int index)
{
	log("loading loadBossPng start index = %d", index);
	m_serverChapter = index;
	string pngpath = StringUtils::format("%s/c%d/ditu%02dpacker.png", GlobalData::getDownloadMapPath().c_str(), m_serverChapter, m_serverChapter);
	Director::getInstance()->getTextureCache()->addImageAsync(pngpath, CC_CALLBACK_1(LoadingScene::loadBossPlist, this));
	log("loading loadBossPng end index = %d", index);
}

void LoadingScene::loadBossPlist(cocos2d::Texture2D* texture)
{
	string plistpath = StringUtils::format("%s/c%d/ditu%02dpacker.plist", GlobalData::getDownloadMapPath().c_str(), m_serverChapter, m_serverChapter);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistpath, texture);
	m_serverChapter++;
	log("loading loadBossPlist start index = %d", m_serverChapter);
	if (m_serverChapter == GlobalData::getServerChapterCount() + LOCALTHEMECOUNT + 1)
	{
		this->scheduleOnce(schedule_selector(LoadingScene::delayLoadData), 0.5f);
	}
	else
	{
		loadBossPng(m_serverChapter);
	}
	log("loading loadBossPlist end GlobalData::getServerChapterCount() = %d", GlobalData::getServerChapterCount());
}

void LoadingScene::delayLoadData(float dt)
{
	log("loading delayLoadData start");
	parseSkillConfig();

	parseAchieveConfig(GlobalData::getXMLDataPath("data/achieve.xml"));
	SoundManager::sharedManager()->loadSounds();
	updateBarLight();
	this->scheduleOnce(schedule_selector(LoadingScene::delayLoadHeroPng), 0.5f);
	getPayData();
	log("loading delayLoadData end");
}

void LoadingScene::delayLoadHeroPng(float dt)
{
	Director::getInstance()->getTextureCache()->addImageAsync("heropacker.png", CC_CALLBACK_1(LoadingScene::loadHeroPlist, this));
}

void LoadingScene::loadHeroPlist(cocos2d::Texture2D* texture)
{
	this->scheduleOnce(schedule_selector(LoadingScene::preLoadLayer), 0.5f);
}

void LoadingScene::getPayData()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/GMAction", "getPayData", "()V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	}
#endif
}
void LoadingScene::preLoadLayer(float dt)
{
	//g_LoginRewardLayer = CSLoader::createNode("loginRewardLayer.csb");
	//g_LoginRewardLayer->retain();

	//g_shopCsbLayer = CSLoader::createNode("ShopLayer.csb");
	//g_shopCsbLayer->retain();
	log("loading preLoadLayer start");
	for (int i = 1; i <= LOCALTHEMECOUNT; i++)
	{
		string str = StringUtils::format("theme%d.jpg", i);
		Sprite* theme = Sprite::createWithSpriteFrameName(str);

		theme->retain();
		mapbg.push_back(theme);

		str = StringUtils::format("map%dLayer.csb", i);
		Node* mapLayer = CSLoader::createNode(str);
		mapLayer->setName(str);
		mapLayer->retain();
		mapStageLayer.push_back(mapLayer);

		str = StringUtils::format("boss%02d.csb", i);
		Node *AminLayer = CSLoader::createNode(str);
		AminLayer->retain();
		AminLayer->setName(str);
		bossNormalLayer.push_back(AminLayer);

		str = StringUtils::format("boss%02dhited.csb", i);
		Node * bossAminLayer = CSLoader::createNode(str);
		bossAminLayer->retain();
		bossAminLayer->setName(str);
		bossHitedLayer.push_back(bossAminLayer);
	}

	for (int i = 0; i < GlobalData::getServerChapterCount(); i++)
	{
		string str = StringUtils::format("theme%d.jpg", i + LOCALTHEMECOUNT + 1);
		Sprite* theme = Sprite::createWithSpriteFrameName(str);

		theme->retain();
		mapbg.push_back(theme);

		str = StringUtils::format("%s/c%d/map%dLayer.csb", GlobalData::getDownloadMapPath().c_str(), i + LOCALTHEMECOUNT + 1, i + LOCALTHEMECOUNT + 1);
		Node* mapLayer = CSLoader::createNode(str);
		mapLayer->retain();
		mapStageLayer.push_back(mapLayer);

		str = StringUtils::format("%s/c%d/boss%02d.csb", GlobalData::getDownloadMapPath().c_str(), i + LOCALTHEMECOUNT + 1, i + LOCALTHEMECOUNT + 1);
		Node *AminLayer = CSLoader::createNode(str);
		AminLayer->setName(str);
		AminLayer->retain();
		bossNormalLayer.push_back(AminLayer);

		str = StringUtils::format("%s/c%d/boss%02dhited.csb", GlobalData::getDownloadMapPath().c_str(), i + LOCALTHEMECOUNT + 1, i + LOCALTHEMECOUNT + 1);
		Node * bossAminLayer = CSLoader::createNode(str);
		bossAminLayer->retain();
		bossAminLayer->setName(str);
		bossHitedLayer.push_back(bossAminLayer);
	}
	//load hereanim

	heroAminNode = CSLoader::createNode("hero.csb");
	heroAminNode->setName("hero.csb");
	heroAminNode->retain();

	m_pngType++;
	updateBarLight();
	this->scheduleOnce(schedule_selector(LoadingScene::delayEnterStartLayer), 0.3f);
	log("loading preLoadLayer end m_pngType = %d", m_pngType);
}

void LoadingScene::delayEnterStartLayer(float dt)
{
	log("loading delayEnterStartLayer start");
	GlobalData::setMapCount(mapStageLayer.size());

	if (UserDefault::getInstance()->getBoolForKey("firstloadingend", true))
	{
		UserDefault::getInstance()->setBoolForKey("firstloadingend", false);

#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "enterloadingend");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
	}
	login(GlobalData::getLoginType());
	//Director::getInstance()->replaceScene(StartScene::scene());
}

void LoadingScene::parseSkillConfig()
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	string filename = GlobalData::getXMLDataPath("data/skill.xml");
	string filedata = FileUtils::getInstance()->getStringFromFile(filename);

	int err = pDoc->Parse(filedata.c_str());
	if (err != 0)
	{
		delete pDoc;
		return;
	}
	tinyxml2::XMLElement *rootEle = pDoc->RootElement();
	//int total = rootEle->IntAttribute("total");
	//获取第一个节点属性
	//const XMLAttribute *attribute = rootEle->FirstAttribute();
	//打印节点属性名和值
	//log("attribute<em>name = %s,attribute</em>value = %s", attribute->Name(), attribute->Value());</p>
	tinyxml2::XMLElement *element = rootEle->FirstChildElement();
	while (element != NULL)
	{
		if (strcmp(element->Name(), "s") == 0)
		{
			SkillData skillData;
			skillData.id = element->IntAttribute("id");
			skillData.name = element->Attribute("name");
			skillData.desc = element->Attribute("des");
			skillData.maxleve = element->IntAttribute("max");
			vector<string> tmp;
			CommonFuncs::split(element->Attribute("ef"), tmp);
			for (unsigned int i = 0; i < tmp.size(); i++)
			{
				skillData.data.push_back(atof(tmp[i].c_str()));
			}
			GlobalData::mapSkillData[skillData.id - 1] = skillData;
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

void LoadingScene::parseAchieveConfig(string filepath)
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
			Achieve achieveData;
			achieveData.id = element->IntAttribute("id");
			strcpy(achieveData.name, element->Attribute("name"));
			strcpy(achieveData.desc, element->Attribute("des"));
			achieveData.count = element->IntAttribute("count");
			achieveData.achType = element->IntAttribute("ex");
			achieveData.rwdtype = element->IntAttribute("rtype");
			achieveData.rcount = element->IntAttribute("rcount");
			achieveData.finish = 0;
			achieveData.datetime = 0;
			GlobalData::achieveData.push_back(achieveData);
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

int LoadingScene::folderCount(string folderPath)
{
	int filepathcount = 0;
#ifdef WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		log("zhou --> can not match the folder path");
	}
	else
	{
		do{
			if (FileInfo.attrib & _A_SUBDIR)
			{
				if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
				{
					string dirname = FileInfo.name;
					int mapnum = atoi(dirname.substr(1).c_str());
					if (mapnum >= LOCALTHEMECOUNT + 1)
						filepathcount++;
				}
			}
		} while (_findnext(Handle, &FileInfo) == 0);
		_findclose(Handle);
	}

#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if ((dp = opendir(folderPath.c_str())) == NULL) 
	{
		log("zhou --> cannot open directory: %s\n", folderPath.c_str());
	}
	else
	{
		chdir(folderPath.c_str());
		while ((entry = readdir(dp)) != NULL) 
		{
			lstat(entry->d_name, &statbuf);
			if (S_ISDIR(statbuf.st_mode)) {

				if (strcmp(".", entry->d_name) == 0 ||
					strcmp("..", entry->d_name) == 0)
					continue;
				string dirname = entry->d_name;
				int mapnum = atoi(dirname.substr(1).c_str());
				if (mapnum >= LOCALTHEMECOUNT + 1)
					filepathcount++;
			}
		}
		chdir("..");
		closedir(dp);
	}
#endif
	return filepathcount;
}

void LoadingScene::login(int logintype)
{
	if (UserDefault::getInstance()->getBoolForKey("firststart", true))
	{
		UserDefault::getInstance()->setBoolForKey("firststart", false);

#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "enterstart");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
	}

	GlobalData::setLoginType(logintype);
#ifdef USENETWORK
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/hllm", "login", "(I)V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, logintype);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	string headurl = "";
	string locid = "local003";
	if (logintype > 0)
	{
		headurl = "http://q.qlogo.cn/qqapp/1105233824/EF95C27003021EBA5F65B86F205948EE/40";
		if (logintype == LOGIN_QQ)
			locid = "EF95C27003021EBA5F65B86F205948EE";
		else
			locid = "w1234567";
	}
	getUserData(locid, "", headurl, "win32", "com.kuxx.hllm");
#endif
#else
	g_StartScene->scheduleOnce(schedule_selector(StartScene::loadUserData), 0.05f);
#endif
}

void LoadingScene::setLoginData()
{
	int syssec = GlobalData::getSystemSecTime();
	int lastloginzerotime = GlobalData::getLastLoginRewardDaySecond();
	if (syssec - lastloginzerotime >= 24 * 60 * 60)
	{
		GlobalData::setIsGetLoginReward(false);
	}
}


std::string LoadingScene::getUpgradeTime()
{
	string rfilename = StringUtils::format("%s/%s", GlobalData::getBasePath().c_str(), UPGRADETIMEFILE);
	FILE* rfile = fopen(rfilename.c_str(), "r");
	char buf[32];
	int len = 0;
	string result;
	if (rfile)
	{
		fseek(rfile, 0, SEEK_END);
		len = ftell(rfile);
		rewind(rfile);
		int rLen = fread(buf, sizeof(char), len, rfile);
		buf[rLen] = '\0';
		fclose(rfile);
		result = buf;
	}
	return result;
}

void LoadingScene::getUserData(string localid, string nickname, string headurl, string channelid, string pkgname)
{
	std::string nameutf8str;
	const char* retstr = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "gbkToUTF8", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(nickname.c_str());
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, para1);

		retstr = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	nameutf8str = retstr;
#else
	nameutf8str = CommonFuncs::gbk2utf(nickname.c_str());
#endif

	string url;
	url.append(HTTPURL);
	url.append("llk_login");
	url.append("&package_name=");
	url.append(pkgname);
	url.append("&playerid=");
	url.append(localid);
	url.append("&cid=");
	url.append(channelid);
	url.append("&nickname=");
	url.append(nameutf8str);
	url.append("&headportrait=");
	url.append(headurl);
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d", GlobalData::getVersionCode());
	url.append(vercodestr);
	url.append("&upgradetime=");
	url.append(getUpgradeTime());
	/*取自己数据*/
	HttpUtil::getInstance()->doData(url, httpCompleteCB, "", HTTPTYPE::GET, "", headurl);
}

void LoadingScene::httpCompleteCB(std::string retdata, int code, std::string tag)
{
	//log("retdata = %s", retdata.c_str());
	rapidjson::Document doc;

	bool isdataok = false;

	if (code == 0)
	{
		doc.Parse<0>(retdata.c_str());
		if (!doc.HasParseError())  //打印解析错误
		{
			if (doc.IsObject() && doc.HasMember("data"))
			{
				rapidjson::Value& objectData = doc["data"];

				int mytotallv = 0;
				std::string skill = "";
				int coin = 0;
				int gem = 0;
				int hpcount = 25;
				int hpmax = 25;
				int prompt = 0;
				int refresh = 0;
				int addtime = 0;
				std::string stage = "";
				int bestscore = 0;
				int bossbestscore = 0;
				int bossbestcount = 0;
				int cotinuelogindays = 0;
				int maxstage = -1;
				int lasttakeawardtime = 0;
				std::string achivestr = "";

				if (objectData.HasMember("id"))
				{
					rapidjson::Value& data = objectData["id"];
					string idstr = StringUtils::format("%d", data.GetInt());
					GlobalData::setMyID(idstr);
					GlobalData::init();
					//std::string mynamestr = objectData["nickname"].GetString();
					//if (GlobalData::getMyName().length() <= 0)
					//	GlobalData::setMyName(mynamestr);
					GlobalData::setHeadImg(tag);
				}

				if (objectData.HasMember("level"))
				{
					rapidjson::Value& data = objectData["level"];
					mytotallv = data.GetInt();
					if (GlobalData::getMyTotalSkillLevel() <= 0)
						GlobalData::setMyTotalSkillLvel(mytotallv);
				}

				if (objectData.HasMember("skill_level"))
				{
					rapidjson::Value& data = objectData["skill_level"];
					skill = data.GetString();
				}
				if (skill.length() <= 0)
				{
					skill = "0,0,0,0,0,0";
				}
				vector<string> tmp1;
				CommonFuncs::split(skill, tmp1, ",");
				for (unsigned int i = 0; i < tmp1.size(); i++)
				{
					if (GlobalData::getMySkillLevel(i) <= 0)
						GlobalData::setMySkilLevel(i, atoi(tmp1[i].c_str()));
				}
				if (objectData.HasMember("coin"))
				{
					rapidjson::Value& data = objectData["coin"];
					coin = data.GetInt();
					if (GlobalData::getGoldCount() <= 0)
						GlobalData::setGoldCount(coin);
				}
				if (objectData.HasMember("diamond"))
				{
					rapidjson::Value& data = objectData["diamond"];
					gem = data.GetInt();
					if (GlobalData::getGemCount() <= 0)
						GlobalData::setGemCount(gem);
				}
				if (objectData.HasMember("life"))
				{
					rapidjson::Value& data = objectData["life"];
					hpcount = data.GetInt();
				}

				if (objectData.HasMember("lifeupperlimit"))
				{
					rapidjson::Value& data = objectData["lifeupperlimit"];
					hpmax = data.GetInt();
				}
				if (hpmax == 0)
				{
					hpmax = 25;
					hpcount = 25;
					GlobalData::setHpCount(hpcount);
					GlobalData::setMyMaxHp(hpmax);
				}
				if (hpmax > GlobalData::getMyMaxHp())
				{
					GlobalData::setMyMaxHp(hpmax);
				}

				if (objectData.HasMember("find"))
				{
					rapidjson::Value& data = objectData["find"];
					prompt = data.GetInt();
					if (GlobalData::getPromptProp() <= 0)
						GlobalData::setPromptProp(prompt);
				}
				if (objectData.HasMember("reset"))
				{
					rapidjson::Value& data = objectData["reset"];
					refresh = data.GetInt();
					if (GlobalData::getRefreshProp() <= 0)
						GlobalData::setRefreshProp(refresh);
				}
				if (objectData.HasMember("addtime"))
				{
					rapidjson::Value& data = objectData["addtime"];
					addtime = data.GetInt();
					if (GlobalData::getAddtimeProp() <= 0)
						GlobalData::setAddtimeProp(addtime);
				}
				if (objectData.HasMember("finished_pass"))
				{
					rapidjson::Value& data = objectData["finished_pass"];
					stage = data.GetString();
				}

				vector<string> stagestrvec;

				int mapcount = GlobalData::getMapCount();
				int stagestrlen = stage.length();
				for (int i = 0; i < mapcount; i++)
				{
					if (stagestrlen < mapcount * 5)
						stagestrvec.push_back("000000");
					else
						stagestrvec.push_back(stage.substr(i * 5, 5));
					std::string chapter = StringUtils::format("page%dstar", i);
					long pageStageStar = strtol(stagestrvec[i].c_str(), NULL, 16);

					std::string stars = StringUtils::format("%ld", pageStageStar);

					std::string starsStr = GlobalData::getChapterStageStar(chapter.c_str());
					long long curPageStageStar = atoll(starsStr.c_str());
					if (curPageStageStar <= 0)
						GlobalData::setChapterStageStar(chapter.c_str(), stars.c_str());
				}
				if (objectData.HasMember("bestscore"))
				{
					rapidjson::Value& data = objectData["bestscore"];
					bestscore = data.GetInt();
					if (GlobalData::getBestCMScore(CMNORMAL) <= 0)
						GlobalData::setBestCMScore(CMNORMAL, bestscore);
				}
				if (objectData.HasMember("bossstatus"))
				{
					rapidjson::Value& data = objectData["bossstatus"];
					bossbestcount = data.GetInt();
					if (GlobalData::getBestDefeatedBossCount() <= 0)
						GlobalData::setBestDefeatedBossCount(bossbestcount);
				}
				if (objectData.HasMember("bossbestscore"))
				{
					rapidjson::Value& data = objectData["bossbestscore"];
					bossbestscore = data.GetInt();
					if (GlobalData::getBestCMScore(CMBOSS))
						GlobalData::setBestCMScore(CMBOSS, bossbestscore);
				}

				if (objectData.HasMember("higheststage"))
				{
					rapidjson::Value& data = objectData["higheststage"];
					maxstage = data.GetInt();
					if (GlobalData::getHighestStage() <= -1)
						GlobalData::setHighestStage(maxstage);
				}
				if (objectData.HasMember("achievement"))
				{
					rapidjson::Value& data = objectData["achievement"];
					achivestr = data.GetString();
				}

				if (objectData.HasMember("buy"))
				{
					rapidjson::Value& data = objectData["buy"];
				}
				if (objectData.HasMember("url"))
				{
					rapidjson::Value& data = objectData["url"];
					GlobalData::setShareUrl(data.GetString());
				}
				if (objectData.HasMember("classicscore"))
				{
					rapidjson::Value& data = objectData["classicscore"];
					if (GlobalData::getBestCMScore(CMCLASIC) <= 0)
						GlobalData::setBestCMScore(CMCLASIC, data.GetInt());
				}
				if (objectData.HasMember("flashscore"))
				{
					rapidjson::Value& data = objectData["flashscore"];
					if (GlobalData::getBestCMScore(CMFLASH) <= 0)
						GlobalData::setBestCMScore(CMFLASH, data.GetInt());
				}
				if (objectData.HasMember("upgradeurl"))
				{
					rapidjson::Value& data = objectData["upgradeurl"];
					GlobalData::setUpgradeUrl(data.GetString());
				}
				/*
				if (objectData.HasMember("login_days"))
				{
				rapidjson::Value& data = objectData["login_days"];
				cotinuelogindays = data.GetInt();
				}
				if (objectData.HasMember("lasttakeawardtime"))
				{
				rapidjson::Value& data = objectData["lasttakeawardtime"];
				lasttakeawardtime = data.GetInt();
				}
				GlobalData::setContinueLoginDay(cotinuelogindays);
				GlobalData::setLastLoginRewardSecond(lasttakeawardtime);
				*/
				string hexstr;
				int achieveSize = GlobalData::achieveData.size();
				int strsize = achivestr.size();
				if (strsize < achieveSize * 3)
				{
					achivestr.clear();

					for (int i = 0; i < achieveSize; i++)
					{
						achivestr.append("000");
					}
				}

				for (int i = 0; i < achieveSize; i++)
				{
					hexstr = achivestr.substr(i * 3, 3);

					if (strcmp(hexstr.c_str(), "fff") == 0)
						GlobalData::achieveData[i].finish = -1;
					else
						GlobalData::achieveData[i].finish = (int)strtol(hexstr.c_str(), NULL, 16);
				}
				isdataok = true;
				GlobalData::setPlayingStage(-1);
			}
		}
	}
	else
	{
		if (GlobalData::getMyID().length() <= 0)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			JniMethodInfo methodInfo;
			const char* str = "";
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "UUID", "()Ljava/lang/String;"))
			{
				jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
				str = methodInfo.env->GetStringUTFChars(jstr, 0);
			}
			GlobalData::setMyID(str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

			if (GlobalData::getLoginType() == LOGIN_NONE)
			{
				GlobalData::setMyID("loc123");
			}
			else if (GlobalData::getLoginType() == LOGIN_QQ)
			{
				GlobalData::setMyID("q123");
			}
			else if (GlobalData::getLoginType() == LOGIN_WX)
			{
				GlobalData::setMyID("w123");
			}
#endif
		}
	}

	if (!isdataok)
	{
		GlobalData::setLoginType(LOGIN_NONE);

		HintBox* hint = HintBox::create("获取数据失败，转为游客登录！", 0);
		Director::getInstance()->getRunningScene()->addChild(hint, 1000);
		Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(LoadingScene::loadUserData), 0.05f);
	}
	else
	{
		/*取自己头像*/
		int logintype = GlobalData::getLoginType();
		if (logintype > LOGIN_QQ)
		{
			string headimgname = "qhead00.png";
			if (logintype == LOGIN_WX)
				headimgname = "wxhead00.png";

			HttpUtil::getInstance()->doData(GlobalData::getHeadImg(), NULL, headimgname);
		}
		//g_StartScene->scheduleOnce(schedule_selector(StartScene::loadUserData), 0.05f);
		Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(LoadingScene::getMessageData), 0.05f);
	}
}

void LoadingScene::loadUserData(float dt)
{
	getAchieveData();
	getTaskData();
	GlobalData::init();
	setLoginData();
	GlobalData::setTotalLoginDay(GlobalData::getTotalLoginDay() + 1);
	GlobalData::setIsHomeAutoPopGift(true);

	//兼容前几个版本，最大关卡：10001 start
	int maxstage = GlobalData::getHighestStage();
	int chapter = maxstage / 10000;
	int stage = maxstage % 10000;
	//兼容前几个版本，最大关卡：10001 end
	GlobalData::setHighestStage(chapter * 10 + stage);

	int level = GlobalData::getMyTotalSkillLevel();
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "setPlayerLevel", "(I)V"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, level);
	}
#endif


	string myname = GlobalData::getMyName();
	if (myname.compare("") == 0)
	{
		myname = StringUtils::format("游客%d", GlobalData::getSystemSecTime());
		GlobalData::setMyName(CommonFuncs::gbk2utf(myname.c_str()));
	}
#ifdef USENETWORK
	sendUserAnalyticsData();
#endif
	if (SettingLayer::setChangelogin > LOGIN_NONE)
		Director::getInstance()->replaceScene(MyTransitionScene::scene());

	else
		Director::getInstance()->replaceScene(ThemeScene::Scene());

}

void LoadingScene::getAchieveData()
{
	std::string achieveStr = GlobalData::getAchieveData();
	if (achieveStr.length() > 0)//读保存的成就数据格式：1,2,3,;4,5,6;7,8,9;10,11,12;
	{
		vector<string> tmp;
		CommonFuncs::split(achieveStr, tmp, ";");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			vector<string> data;
			CommonFuncs::split(tmp[i], data, ",");

			int id = atoi(data[0].c_str());

			for (unsigned int m = 0; m < GlobalData::achieveData.size(); m++)
			{
				if (id == GlobalData::achieveData[m].id)
				{
					GlobalData::achieveData[m].finish = atoi(data[7].c_str());
					GlobalData::achieveData[m].datetime = atoi(data[8].c_str());
					break;
				}
			}
		}
	}
}

void LoadingScene::getTaskData()
{
	parseTaskConfig(GlobalData::getXMLDataPath("data/task.xml"));

	int dayofyear = GlobalData::getDayOfYear();
	if (GlobalData::getLastLoginDayOfYear() != dayofyear)
	{
		GlobalData::clearTaskData();
		GlobalData::setLastLoginDayOfYear(dayofyear);
	}

	std::string taskStr = GlobalData::getTaskData();
	if (taskStr.length() > 0)//读保存的成就数据格式：1,2,3,;4,5,6;7,8,9;10,11,12;
	{
		vector<string> tmp;
		CommonFuncs::split(taskStr, tmp, ";");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			vector<string> data;
			CommonFuncs::split(tmp[i], data, ",");

			int id = atoi(data[0].c_str());

			for (unsigned int m = 0; m < GlobalData::taskData.size(); m++)
			{
				if (id == GlobalData::taskData[m].id)
				{
					GlobalData::taskData[m].finish = atoi(data[7].c_str());
					GlobalData::taskData[m].datetime = atoi(data[8].c_str());
					break;
				}
			}
		}
	}
}

void LoadingScene::getMessageData(float dt)
{
	string url;
	url.append(HTTPURL);
	url.append("llk_MessageList");
	url.append("&id=");
	url.append(GlobalData::getMyID());
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d", GlobalData::getVersionCode());
	url.append(vercodestr);
	url.append("&cid=");
	url.append(GlobalData::getCid());
	HttpUtil::getInstance()->doData(url, httpMessageDataCompleteCB);
}

void LoadingScene::httpMessageDataCompleteCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		parseMessageJson(retdata);
	}
	Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(LoadingScene::getEventData), 0.02f);
}

void LoadingScene::getEventData(float dt)
{
	std::string url;
	url.append(HTTPURL);
	url.append("llk_EventList");
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d", GlobalData::getVersionCode());
	url.append(vercodestr);
	url.append("&cid=");
	url.append(GlobalData::getCid());
	HttpUtil::getInstance()->doData(url, httpEventDataCompleteCB);
}

void LoadingScene::httpEventDataCompleteCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		parseEventJson(retdata);
	}

	Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(LoadingScene::loadUserData), 0.05f);
}

void LoadingScene::parseEventJson(std::string retdata)
{
	rapidjson::Document doc;
	doc.Parse<0>(retdata.c_str());
	if (!doc.HasParseError())  //打印解析错误
	{
		if (doc.IsObject() && doc.HasMember("data"))
		{
			rapidjson::Value& objectData = doc["data"];
			if (objectData.HasMember("events"))
			{
				rapidjson::Value& Data = objectData["events"];
				if (Data.IsArray())
				{
					for (unsigned int i = 0; i < GlobalData::EventsData.size(); i++)
					{
						GlobalData::EventsData.clear();
					}

					MEvent mdata;
					for (unsigned int i = 0; i < Data.Size(); i++)
					{
						rapidjson::Value& mevent = Data[i];
						rapidjson::Value& valuedata = mevent["type"];
						int type = valuedata.GetInt();
						mdata.type = type;

						valuedata = mevent["id"];
						mdata.id = valuedata.GetInt();

						valuedata = mevent["title"];
						strcpy(mdata.title, valuedata.GetString());

						valuedata = mevent["rule"];
						strcpy(mdata.rulestr, valuedata.GetString());

						valuedata = mevent["perioddesc"];
						strcpy(mdata.timestr, valuedata.GetString());

						if (mevent.HasMember("data1"))
						{
							valuedata = mevent["data1"];
							strcpy(mdata.data, valuedata.GetString());
						}
						if (mevent.HasMember("starttime"))
						{
							valuedata = mevent["starttime"];
							mdata.startime = valuedata.GetInt();
						}

						if (mevent.HasMember("endtime"))
						{
							valuedata = mevent["endtime"];
							mdata.endtime = valuedata.GetInt();
						}

						valuedata = mevent["awardrule"];
						string propstr = valuedata.GetString();
						vector<string> tmp1;
						CommonFuncs::split(propstr, tmp1, ";");

						int count = tmp1.size() > 3 ? 3 : tmp1.size();

						for (int m = 0; m < count; m++)
						{
							vector<string> tmp2;
							CommonFuncs::split(tmp1[m], tmp2, "-");
							if (tmp2.size() >= 2)
							{
								mdata.propid[m] = atoi(tmp2[0].c_str());
								mdata.propnum[m] = atoi(tmp2[1].c_str());
							}
						}

						if (type <= MAXEVENTTYPE)
							GlobalData::EventsData.push_back(mdata);
					}
				}
			}
		}
	}
}

void LoadingScene::parseMessageJson(std::string retdata)
{
	rapidjson::Document doc;
	doc.Parse<0>(retdata.c_str());
	if (!doc.HasParseError())  //打印解析错误
	{
		if (doc.IsObject() && doc.HasMember("data"))
		{
			rapidjson::Value& objectData = doc["data"];
			if (objectData.HasMember("msgs"))
			{
				rapidjson::Value& Data = objectData["msgs"];
				if (Data.IsArray())
				{
					for (unsigned int i = 0; i < GlobalData::mapMessageData.size(); i++)
					{
						GlobalData::mapMessageData[i].clear();
					}

					Message msgdata;
					for (unsigned int i = 0; i < Data.Size(); i++)
					{
						rapidjson::Value& message = Data[i];
						rapidjson::Value& valuedata = message["type"];
						int type = valuedata.GetInt();
						msgdata.type = type;

						valuedata = message["mid"];
						msgdata.id = valuedata.GetInt();

						valuedata = message["title"];
						strcpy(msgdata.title, valuedata.GetString());

						valuedata = message["content"];
						strcpy(msgdata.content, valuedata.GetString());

						valuedata = message["prize"];
						strcpy(msgdata.prize, valuedata.GetString());

						valuedata = message["status"];
						msgdata.status = valuedata.GetInt();

						valuedata = message["time"];
						msgdata.datetime = valuedata.GetInt();

						if (type == 0)
							GlobalData::mapMessageData[type].push_back(msgdata);
						else
							GlobalData::mapMessageData[type - 1].push_back(msgdata);
					}
				}
			}
		}
	}
}

void LoadingScene::parseTaskConfig(std::string filepath)
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
			Achieve taskData;
			taskData.id = element->IntAttribute("id");
			strcpy(taskData.name, element->Attribute("name"));
			strcpy(taskData.desc, element->Attribute("des"));
			taskData.count = element->IntAttribute("count");
			taskData.achType = element->IntAttribute("ex");
			taskData.rwdtype = element->IntAttribute("rtype");
			taskData.rcount = element->IntAttribute("rcount");
			taskData.finish = 0;
			taskData.datetime = 0;
			GlobalData::taskData.push_back(taskData);
		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}

void LoadingScene::sendUserAnalyticsData()
{
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	writedoc.AddMember("uid", rapidjson::Value(GlobalData::getMyID().c_str(), allocator), allocator);

	string macstr = "123456";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "MACID", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		macstr = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif
	writedoc.AddMember("mac", rapidjson::Value(macstr.c_str(), allocator), allocator);
	writedoc.AddMember("logintype", GlobalData::getLoginType(), allocator);
	
	int logindays = GlobalData::getContinueLoginDay();

	int count = logindays;

	int lastLoginRewardSecond = GlobalData::getLastLoginRewardSecond();

	int syssec = GlobalData::getSystemSecTime();

	time_t t = time(0);
	tm* tm = localtime(&t);

	int nowzero = syssec - (tm->tm_hour * 60 * 60 + tm->tm_min * 60 + tm->tm_sec);

	if (nowzero - lastLoginRewardSecond > 0)
	{
		if (nowzero - lastLoginRewardSecond < 24 * 60 * 60)
		{
			logindays++;
			count = logindays;
			if (count > 7)
				count = count - 7;
		}
		else
		{
			logindays = 1;
			count = logindays;
			GlobalData::setContinueLoginDay(1);
		}
	}
	else
	{
		if (count > 7)
			count = count - 7;
	}

	writedoc.AddMember("logindays", count, allocator);

	writedoc.AddMember("hp", GlobalData::getHpCount(), allocator);

	writedoc.AddMember("coin", GlobalData::getGoldCount(), allocator);

	writedoc.AddMember("gem", GlobalData::getGemCount(), allocator);

	writedoc.AddMember("bomb", 0, allocator);

	writedoc.AddMember("change", 0, allocator);

	writedoc.AddMember("ice", 0, allocator);

	writedoc.AddMember("find", GlobalData::getPromptProp(), allocator);

	writedoc.AddMember("refresh", GlobalData::getRefreshProp(), allocator);

	writedoc.AddMember("addtime", GlobalData::getAddtimeProp(), allocator);

	writedoc.AddMember("histage", GlobalData::getHighestStage() + 1, allocator);


	int totalstars = 0;
	for (int i = 0; i < GlobalData::getMapCount(); i++)
	{
		std::string chapter = StringUtils::format("page%dstar", i);
		std::string starsStr = GlobalData::getChapterStageStar(chapter.c_str());

		long long cruPageStageStar = atoll(starsStr.c_str());

		for (int m = 0; m < 10; m++)
		{
			totalstars += cruPageStageStar >> (m * 2) & 3;
		}
	}

	writedoc.AddMember("totalstars", totalstars, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	string url;
	url.append(ANALYTICS_HTTPURL);
	url.append("llk_insertplayerdata");
	url.append("&param=");
	url.append(buffer.GetString());
	HttpUtil::getInstance()->doData(url);
}

void LoadingScene::getUmpData()
{
	const char* imsi = "123456";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "GetIMSI", "()Ljava/lang/String;"))
	{
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);

		imsi = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif

	std::string AppKey = "baoyue";
	string url;
	url.append("http://211.154.162.11/getUmpStatusByImsi.action?");
	url.append("imsi=");
	url.append(imsi);
	url.append("&sign=");
	url.append(md5(AppKey + imsi));
	/*取自己数据*/
	HttpUtil::getInstance()->doData(url, httpUmpDataCompleteCB);

}

void LoadingScene::httpUmpDataCompleteCB(std::string retdata, int code, std::string tag)
{
	rapidjson::Document doc;

	GlobalData::setIsUmp(false);
	if (code == 0)
	{
		doc.Parse<0>(retdata.c_str());
		if (!doc.HasParseError())  //打印解析错误
		{
			if (doc.IsObject() && doc.HasMember("result"))
			{
				rapidjson::Value& objectData = doc["result"];
				GlobalData::setIsUmp(objectData.GetInt() == 1 ? true : false);
			}
		}
	}
}

void LoadingScene::cancelThirdLogin()
{
	if (SettingLayer::setChangelogin == LOGIN_NONE)
		login(LOGIN_NONE);
	else
	{
		Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	}
}
