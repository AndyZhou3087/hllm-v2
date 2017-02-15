#include "MyTransitionScene.h"
#include "ThemeScene.h"
#include "CommonFuncs.h"
#include "GlobalData.h"
#include "LoadingScene.h"

bool MyTransitionScene::isloadingUpgrade = false;
MyTransitionScene::MyTransitionScene()
{
	 
}

MyTransitionScene::~MyTransitionScene()
{
	isloadingUpgrade = false;
}

bool MyTransitionScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Sprite* bg = Sprite::createWithSpriteFrameName("transitionbg.jpg");
	bg->setPosition(Vec2(270, 480));
	addChild(bg);

	Label* text = Label::createWithSystemFont(CommonFuncs::gbk2utf("加载中"), "fonts/tips.fnt", 30);
	text->setColor(Color3B(21, 90, 149));
	text->setPosition(Vec2(240, 550));
	addChild(text);

	Sprite* progress = Sprite::createWithSpriteFrameName("transitionprogress.png");
	progress->setPosition(Vec2(300, 550));
	addChild(progress);
	progress->runAction(RepeatForever::create(RotateTo::create(2, 720)));

	int sec = GlobalData::getSystemSecTime();
	srand(sec);
	int rindex = rand() % 3;
	std::string str = StringUtils::format("transition%d.png", rindex);
	Sprite* cont = Sprite::createWithSpriteFrameName(str);
	cont->setPosition(Vec2(270, 250));
	addChild(cont);

	return true;
}

Scene* MyTransitionScene::scene()
{
	auto scene = Scene::create();

	Layer* layer = MyTransitionScene::create();

	scene->addChild(layer);

	return scene;
}

void MyTransitionScene::loadthemeScene(float dt)
{
	themScene = ThemeScene::Scene();
	Director::getInstance()->replaceScene(themScene);
}

void MyTransitionScene::onEnterTransitionDidFinish()
{
	if (isloadingUpgrade)
	{
		loadSDPng(GlobalData::getMapCount() + 1);
		int count = LoadingScene::folderCount(GlobalData::getDownloadMapPath());
		GlobalData::setServerChapterCount(count);
	}
	else
	{
		this->scheduleOnce(schedule_selector(MyTransitionScene::loadthemeScene), 0.2f);
	}
}

void MyTransitionScene::onExit()
{
	Layer::onExit();
}


void MyTransitionScene::loadSDPng(int index)
{
	m_serverChapter = index;
	string pngpath = StringUtils::format("%s/c%d/pic.png", GlobalData::getDownloadMapPath().c_str(), m_serverChapter);
	Director::getInstance()->getTextureCache()->addImageAsync(pngpath, CC_CALLBACK_1(MyTransitionScene::LoadpSDList, this));
}

void MyTransitionScene::LoadpSDList(cocos2d::Texture2D* texture)
{
	string plistpath = StringUtils::format("%s/c%d/pic.plist", GlobalData::getDownloadMapPath().c_str(), m_serverChapter);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistpath, texture);
	m_serverChapter++;

	if (m_serverChapter == GlobalData::getServerChapterCount() + LOCALTHEMECOUNT + 1)
	{
		loadBossPng(GlobalData::getMapCount() + 1);
	}
	else
	{
		loadSDPng(m_serverChapter);
	}
}

void MyTransitionScene::loadBossPng(int index)
{
	m_serverChapter = index;
	string pngpath = StringUtils::format("%s/c%d/ditu%02dpacker.png", GlobalData::getDownloadMapPath().c_str(), m_serverChapter, m_serverChapter);
	Director::getInstance()->getTextureCache()->addImageAsync(pngpath, CC_CALLBACK_1(MyTransitionScene::loadBossPlist, this));
}

void MyTransitionScene::loadBossPlist(cocos2d::Texture2D* texture)
{
	string plistpath = StringUtils::format("%s/c%d/ditu%02dpacker.plist", GlobalData::getDownloadMapPath().c_str(), m_serverChapter, m_serverChapter);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistpath, texture);
	m_serverChapter++;

	if (m_serverChapter == GlobalData::getServerChapterCount() + LOCALTHEMECOUNT + 1)
	{
		this->scheduleOnce(schedule_selector(MyTransitionScene::loadLayer), 0.2f);
	}
	else
	{
		loadBossPng(m_serverChapter);
	}
}

void MyTransitionScene::loadLayer(float dt)
{
	for (int i = GlobalData::getMapCount() + 1; i < GlobalData::getServerChapterCount() + LOCALTHEMECOUNT + 1; i++)
	{
		string str = StringUtils::format("theme%d.jpg", i);
		Sprite* theme = Sprite::createWithSpriteFrameName(str);

		theme->retain();
		mapbg.push_back(theme);

		str = StringUtils::format("%s/c%d/map%dLayer.csb", GlobalData::getDownloadMapPath().c_str(), i, i);
		Node* mapLayer = CSLoader::createNode(str);
		mapLayer->retain();
		mapStageLayer.push_back(mapLayer);

		str = StringUtils::format("%s/c%d/boss%02d.csb", GlobalData::getDownloadMapPath().c_str(), i, i);
		Node *AminLayer = CSLoader::createNode(str);
		AminLayer->setName(str);
		AminLayer->retain();
		bossNormalLayer.push_back(AminLayer);

		str = StringUtils::format("%s/c%d/boss%02dhited.csb", GlobalData::getDownloadMapPath().c_str(), i, i);
		Node * bossAminLayer = CSLoader::createNode(str);
		bossAminLayer->retain();
		bossAminLayer->setName(str);
		bossHitedLayer.push_back(bossAminLayer);
	}
	GlobalData::setMapCount(mapStageLayer.size());
	GlobalData::initUnlockData();
	this->scheduleOnce(schedule_selector(MyTransitionScene::loadthemeScene), 0.2f);
}