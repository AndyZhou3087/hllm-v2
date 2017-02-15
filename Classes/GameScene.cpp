#include "GameScene.h"
#include "GlobalData.h"
#include "ReviveLayer.h"
#include "SuccLayer.h"
#include "ContinueLayer.h"
#include "tinyxml2/tinyxml2.h"
#include <algorithm>
#include "Const.h"
#include "SoundManager.h"
#include "PauseLayer.h"
#include "MyParticle.h"
#include "VSLayer.h"
#include "NewerGuideLayer.h"
#include "MyMenu.h"
#include "HintBox.h"
#include "ShopLayer.h"
#include "FourTypeGift.h"
#include "BossSettlement.h"
#include "ThemeScene.h"
#include "CommonFuncs.h"
#include "FreeGift.h"
#include "CMOverLayer.h"
#include "CommonLayer.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "HttpUtil.h"
using namespace tinyxml2;

GameScene* g_gameScene = NULL;
int GameScene::g_whattobuy = 0;
float GameScene::_pasttime = 0;
int GameScene::_stageSecond = 0;
int GameScene::leftTime = 0;
Layer* pauselayer = NULL;


GameScene::GameScene()
{
	_SNUp = NULL;
	_SNDown = NULL;
	_SNLeft = NULL;
	_SNRight = NULL;

	_wallleft = NULL;
	_wallright = NULL;

	boomlight1 = NULL;
	boomlight2 = NULL;
}

GameScene::~GameScene()
{

}

Scene* GameScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
	addLayer(scene);

    // return the scene
    return scene;
}

void GameScene::addLayer(Scene* scene)
{
	// 'layer' is an autorelease object
	GameScene *layer = GameScene::create();

	g_gameScene = layer;
	scene->addChild(layer);
}
void GameScene::onExit()
{
#ifdef USENETWORK
	sendStageData();
#endif
	Layer::onExit();
	g_gameScene = NULL;
	pauselayer = NULL;
	boomlight1 = NULL;
	boomlight2 = NULL;

	GlobalData::setChallengeMode(CMNORMAL);
	GlobalData::setDefeatedBossCount(0);
	GlobalData::setIsUseChange(false);
	GlobalData::setIsUseBomb(false);
	GlobalData::setIsUseIce(false);

	
}
// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	for (int i = 0; i < ELECOUNT; i++)
	{
		animals[i] = 0;
	}
	rettype = 2;
	isClickBomb = false;
	gamePlayingShowShop = false;
	isNewerGuide = false;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	rootCsbLayer = CSLoader::createNode("GameLayer.csb");
	addChild(rootCsbLayer, 0, "rootcsb");

	int chapter = GlobalData::covertChapterStage(GlobalData::getPlayingStage(), _curStage);
	_curChapter = chapter + 1;
	_curStage += 1;
	isbossmode = (GlobalData::getChallengeMode() == CMNORMAL && (GlobalData::getPlayingStage()+1)% BOSSSTAGEPOS == 0) || GlobalData::getChallengeMode() == CMBOSS;

	string str;
	if (isbossmode)
		str = StringUtils::format("Images/topboss.jpg");
	else
		str = StringUtils::format("Images/top%d.jpg", (_curChapter - 1) % 8);
	cocos2d::ui::ImageView* topimage = (cocos2d::ui::ImageView*)rootCsbLayer->getChildByName("topimage");
	topimage->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);

	cocos2d::ui::ImageView* stagetext = (cocos2d::ui::ImageView*)rootCsbLayer->getChildByName("stagetext");

	_chapterLbl = (cocos2d::ui::TextBMFont*)rootCsbLayer->getChildByName("stagelbl");

	timebarbg = (cocos2d::ui::Widget*)rootCsbLayer->getChildByName("gametimeprogressbg");

	progressTimer = (cocos2d::ui::LoadingBar*)rootCsbLayer->getChildByName("timebar");
	progressTimer->setPercent(100);

	for (int i = 0; i < 3; i++)
	{
		str = StringUtils::format("starbg%d", i);
		_starbg[i] = (cocos2d::ui::Widget*)rootCsbLayer->getChildByName(str);
		str = StringUtils::format("star%d", i);
		_starlv[i] = (cocos2d::ui::Widget*)rootCsbLayer->getChildByName(str);
	}

	pauseBtn = (cocos2d::ui::Button*)rootCsbLayer->getChildByName("pausebtn");
	pauseBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::onPauseClick, this));

	icefrozen = (cocos2d::ui::Widget*)rootCsbLayer->getChildByName("gameice");
	icefrozen->setVisible(false);

	_promptBtn = (cocos2d::ui::Button*)rootCsbLayer->getChildByName("promptbtn");
	_promptBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::onPromptClick, this));
	_promptBtn->setLocalZOrder(10);

	promptPrice = (cocos2d::ui::Widget*)_promptBtn->getChildByName("icon");
	promptPrice->setLocalZOrder(10);
	promptPrice->setVisible(false);

	_promptcountLabel = (cocos2d::ui::TextBMFont*)_promptBtn->getChildByName("count");
	_promptcountLabel->setLocalZOrder(10);

	_refreshBtn = (cocos2d::ui::Button*)rootCsbLayer->getChildByName("refreshbtn");
	_refreshBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::onRefreshClick, this));
	_refreshBtn->setLocalZOrder(10);

	refreshPrice = (cocos2d::ui::Widget*)_refreshBtn->getChildByName("icon");
	refreshPrice->setVisible(false);
	refreshPrice->setLocalZOrder(10);

	_refreshcountLabel = (cocos2d::ui::TextBMFont*)_refreshBtn->getChildByName("count");
	_refreshcountLabel->setLocalZOrder(10);

	_clockBtn = (cocos2d::ui::Button*)rootCsbLayer->getChildByName("clockbtn");
	_clockBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::onClockClick, this));
	_clockBtn->setLocalZOrder(10);

	clockPrice = (cocos2d::ui::Widget*)_clockBtn->getChildByName("icon");
	clockPrice->setVisible(false);
	clockPrice->setLocalZOrder(10);

	_addtimecountLabel = (cocos2d::ui::TextBMFont*)_clockBtn->getChildByName("count");
	_addtimecountLabel->setLocalZOrder(10);

	string filename;
	string xmlfile;
	if (GlobalData::getChallengeMode() == CMBOSS)
	{
		_curChapter = 1;
		int syssec = GlobalData::getSystemSecTime();
		srand(syssec);
		int r = rand() % 5;
		xmlfile = StringUtils::format("data/boss/b%d-%d.xml", GlobalData::getDefeatedBossCount() + 1, r + 1);
		//string xmlfile = StringUtils::format("data/boss/b%d-%d.xml", GlobalData::getDefeatedBossCount() + 1, GlobalData::bossstasge);

		str = "gamebosstext.png";
		stagetext->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		stagetext->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		std::string capterstr = StringUtils::format("%d", _curChapter);
		_chapterLbl->setString(capterstr);
	}
	else if (GlobalData::getChallengeMode() == CMCLASIC)
	{
		str = "gameclasictext.png";
		stagetext->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		stagetext->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
		stagetext->setPositionY(928);
		_curChapter = 1;
		int syssec = GlobalData::getSystemSecTime();
		srand(syssec);
		int r = rand() % 5;
		xmlfile = StringUtils::format("data/clasic/c1-%d.xml", r + 1);

		_chapterLbl->setVisible(false);
	}
	else if (GlobalData::getChallengeMode() == CMFLASH)
	{
		str = "gameflashtext.png";
		stagetext->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		stagetext->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
		stagetext->setPositionY(928);

		_chapterLbl->setVisible(false);
		xmlfile = "data/flash/stage.xml";

		_clockBtn->setEnabled(false);
		clockPrice->setVisible(false);
		_addtimecountLabel->setVisible(false);
		_clockBtn->getChildByName("disable")->setVisible(true);
	}
	else
	{
		if (isfirstStageGuide())
		{
			isNewerGuide = true;
			xmlfile = "data/0-0-0.xml";
			_chapterLbl->setString("0");
		}
		else
		{
			xmlfile = StringUtils::format("data/%d-%d-%d.xml", _curChapter, _curStage, 1);
			std::string capterstr = StringUtils::format("%d", GlobalData::getPlayingStage() + 1);
			_chapterLbl->setString(capterstr);
		}
	}
	filename = GlobalData::getXMLDataPath(xmlfile);
	log("filename = %s", filename.c_str());

	parseXML(filename);

	_lastBlock = NULL;
	_curBlock = NULL;
	_promptBlock1 = NULL;
	_promptBlock2 = NULL;

	bossnormal = NULL;
	bosshited = NULL;

	icesec = 0.0f;
	_pasttime = 0.0f;

	_autopromptCount = 3;

	_stageSecond = _stage.time;

	int myAddTimeLv = GlobalData::getMySkillLevel(S_ADDTIME);
	if (myAddTimeLv > 0)
		//_stageSecond += _stageSecond * GlobalData::mapSkillData[S_ADDTIME].data[myAddTimeLv - 1] / 100;
		_stageSecond += GlobalData::mapSkillData[S_ADDTIME].data[myAddTimeLv - 1];

	skillAdd1SecondCount = 0;
	_noCleanTime = 0.0f;

	cmpasttime = 0;

	//makebackground(_stage.rows, _stage.cols);
	batch = SpriteBatchNode::create("block.png");
	rootCsbLayer->addChild(batch);

	_go = Sprite::createWithSpriteFrameName("go.png");
	_go->setPosition(Point(SCREEN_WIDTH / 2, 480));
	_go->setVisible(false);
	this->addChild(_go, 100);

	_refreshword = Sprite::createWithSpriteFrameName("refreshword.png");
	_refreshword->setPosition(Point(270, 480));
	this->addChild(_refreshword, 100);
	_refreshword->setVisible(false);

	float percent[] = { (float)(_stage.time1) / _stageSecond, (float)(_stage.time2) / _stageSecond, (float)(_stage.time3) / _stageSecond };
	for (int i = 0; i < 3; i++)
	{
		float x = (timebarbg->getPositionX() - timebarbg->getContentSize().width / 2) + percent[i] * progressTimer->getContentSize().width;
		//if (i == 0)
		//	x += starBg->getContentSize().width;
		_starbg[i]->setPositionX(x);
		_starbg[i]->setVisible(GlobalData::getChallengeMode() == CMNORMAL);
		_starlv[i]->setPosition(_starbg[i]->getPosition());
		_starlv[i]->setVisible(GlobalData::getChallengeMode() == CMNORMAL);
	}
	updateStarlv();

	_timeupShowAddTime = true;


	this->schedule(schedule_selector(GameScene::showPropPrice));
	string gametime = StringUtils::format("%d", _stage.time - (int)_pasttime);

	_leftCountLabel = (cocos2d::ui::TextBMFont*)rootCsbLayer->getChildByName("timelbl");
	_leftCountLabel->setString(gametime);

	_isPlayTimeUp = false;
	_fireBlink = false;
	_fire = Sprite::createWithSpriteFrameName("fire.png");
	_fire->setVisible(false);
	_fire->setPosition(Point(60, 480));
	this->addChild(_fire);
	_fire1 = Sprite::createWithSpriteFrameName("fire.png");
	_fire1->setVisible(false);
	_fire1->setPosition(Point(480, 480));
	_fire1->setRotationSkewY(180);
	this->addChild(_fire1);

	_gameStateTextBox = Sprite::createWithSpriteFrameName("showtextbox.png");
	_gameStateTextBox->setPosition(Vec2(270, 480));
	_gameStateTextBox->setVisible(false);
	this->addChild(_gameStateTextBox,200);

	_gameStateText = Sprite::createWithSpriteFrameName("timeuptext.png");
	_gameStateText->setPosition(Vec2(_gameStateTextBox->getContentSize().width / 2, _gameStateTextBox->getContentSize().height/2));
	_gameStateTextBox->addChild(_gameStateText);
	
	makeMap();
	GlobalData::g_gamestatus = GAMEREADY;

	//GlobalData::setScore(0);
	_lastlinkpasttime = 0;
	_keepingcount = 1;

	clasicstage = 0;
	clasiccleancount = 0;
	int myComboLv = GlobalData::getMySkillLevel(S_ADDCOMBOTIME);
	if (myComboLv > 0)
	{
		_keeptime = GlobalData::mapSkillData[S_ADDCOMBOTIME].data[myComboLv - 1];
	}
	else
	{
		_keeptime = 1.5f;
	}

	_overblockcount = 0;
	_cleanBlockTimes = 0;
	_cleanGlodBlockTimes = 0;

	_canCleanBarriers = 0;
	_clickCleanBlockCount = 0;

	_nolinktime = 0;
	_lastRandom = -1;
	_isMoving = false;
	_isMovingClean = false;
	useAddTime = 0;
	isctogoldskill = false;

	_combosprite = NULL;
	_heroComboWord = NULL;
	_comboscore = 0;
	extrasocre = 0;

	_isAddingBlocks = false;
	if (GlobalData::getChallengeMode() == CMFLASH)
	{
		initFlashModeData();
	}

	schedule(schedule_selector(GameScene::update));
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	heroAminNode->setScale(0.45f);
	heroAminNode->setRotationSkewY(180);
	rootCsbLayer->addChild(heroAminNode, 800);
	heroAminNode->setVisible(false);

	if (isbossmode)
	{
		SoundManager::sharedManager()->playBackMusic("sound/bgboss.mp3");
		heroAminNode->setPosition(Vec2(70, 730));
		scheduleOnce(schedule_selector(GameScene::delayShowHeroAnim), 2.15f);
		scheduleOnce(schedule_selector(GameScene::showVsLayer), 0.15f);
	}
	else
	{
		SoundManager::sharedManager()->playBackMusic("sound/bg2.mp3");
		heroAminNode->setPosition(Vec2(270, 730));
		this->scheduleOnce(schedule_selector(GameScene::delayShowHeroAnim), 0.5f);
		scheduleOnce(schedule_selector(GameScene::checkUseProp), 0.15f);
	}

	if (isfirstStageGuide())
	{
		changeGuideBlocks();
	}
	return true;
}

void GameScene::delayShowFreeGift(float dt)
{
	if (!UserDefault::getInstance()->getBoolForKey("isgetfreegift", false) && GlobalData::getHighestStage() < 0)
	{
		Director::getInstance()->getRunningScene()->addChild(FreeGift::create(), 0, "giftlayer");
	}
}

void GameScene::delayShowHeroAnim(float dt)
{
	heroAminNode->setVisible(true);
	heroAction = CSLoader::createTimeline(heroAminNode->getName()); //加载动画：
	heroAminNode->runAction(heroAction);
	heroAction->play("stand", true);
}

void GameScene::delayShowBossAnim(float dt)
{
	bosshited->setVisible(false);
	bossnormal->setVisible(true);
}

void GameScene::checkUseProp(float dt)
{
	float delay = 1.0f;
	bool isShowPaticle = false;
	if (GlobalData::getIsUseChange())
	{
		for (unsigned int i= 0; i < _vecBlocks.size(); i++)
		{
			_vecBlocks[i]->runAction(Sequence::create(DelayTime::create(1.0f), ScaleTo::create(0.25f, 0), NULL));
		}
		this->scheduleOnce(schedule_selector(GameScene::showUsePropPaticle), 1.0f);
		isShowPaticle = true;
		this->scheduleOnce(schedule_selector(GameScene::showUseChange), 1.25f);
		delay = 2.0f;
	}
	if (GlobalData::getIsUseBomb())
	{
		bool isfind = false;

		for (int i = 0; i < 3; i++)
		{
			Block* A = NULL;
			Block* B = NULL;

			find2RandomSameBlocks(&A, &B);
			if (A != NULL && B != NULL)
			{
				A->stype(B_BOMB);
				B->stype(B_BOMB);
				A->runAction(Sequence::create(DelayTime::create(delay), CallFuncN::create(CC_CALLBACK_1(GameScene::setBombBlockZoder, this)), ScaleTo::create(0.5f, 1.3f), CallFuncN::create(CC_CALLBACK_1(GameScene::showBombPropAnim, this)), NULL));
				B->runAction(Sequence::create(DelayTime::create(delay), CallFuncN::create(CC_CALLBACK_1(GameScene::setBombBlockZoder, this)), ScaleTo::create(0.5f, 1.3f), CallFuncN::create(CC_CALLBACK_1(GameScene::showBombPropAnim, this)), NULL));
				isfind = true;
			}
		}
		if (isfind)
		{
			if (!isShowPaticle)
				this->scheduleOnce(schedule_selector(GameScene::showUsePropPaticle), delay);
			delay += 0.5f;
		}
	}
	if (GlobalData::getIsUseIce())
	{
		icesec = ICETIME;
	}
	scheduleOnce(schedule_selector(GameScene::delayShowSkillAddTime), delay);
}

void GameScene::showUsePropPaticle(float dt)
{
	MyParticle::Show(this, SpeciallyName[42], 270, 480, true, 0.08f);
	MyParticle::Show(this, SpeciallyName[43], 270, 480, true, 0.1f);

	MyParticle::Show(this, SpeciallyName[44], 270, 480, true, 0.1f);
	MyParticle::Show(this, SpeciallyName[45], 270, 480, true, 0.08f);
	MyParticle::Show(this, SpeciallyName[46], 270, 480, true, 0.1f);
}

void GameScene::setBombBlockZoder(Ref* pSender)
{
	Node* block = (Node*)pSender;
	block->setLocalZOrder(81 + block->getLocalZOrder());
}

void GameScene::showUseChange(float dt)
{
	int CHANGECOUNT = 5;
	Block* A = NULL;
	Block* B = NULL;
	bool isfind = false;
	find2RandomSameBlocks(&A, &B);
	if (A != NULL && B != NULL)
	{
		std::vector<Block*> startblocks;
		std::vector<Block*> changeblocks;
		int findcount = 0;
		unsigned int i = 0;
		for (i = 0; i < _vecBlocks.size(); i++)
		{
			Block* temp = _vecBlocks[i];
			if ((temp->stype() == B_NORMAL || temp->stype() == B_COVER || temp->stype() == B_TURN) && temp->state() != DESTROID && findcount < CHANGECOUNT)
			{
				isfind = true;
				++findcount;
				startblocks.push_back(temp);
				changeblocks.push_back(temp);
				temp->setChangeMask(true);
			}
			if (findcount >= CHANGECOUNT)
				break;
		}
		findcount = 0;
		for (unsigned int m = 0; m < startblocks.size(); m++)
		{
			Block* a = startblocks[m];

			for (unsigned int n = i + 1; n < _vecBlocks.size(); n++)
			{
				Block* b = _vecBlocks[n];
				if (a->type() == b->type() && !b->getChangeMask() && findcount < CHANGECOUNT)
				{
					b->setChangeMask(true);
					changeblocks.push_back(b);
					findcount++;
					break;
				}
				if (findcount >= CHANGECOUNT)
					break;
			}
			if (findcount >= CHANGECOUNT)
				break;
		}

		if (isfind)
		{
			for (unsigned int m = 0; m < changeblocks.size(); m++)
			{
				Block* a = changeblocks[m];
				//a->setLocalZOrder(81 + a->getLocalZOrder());
				//a->setTag(m);
				//a->runAction(Sequence::create(ScaleTo::create(0.5f, 1.3f), CallFuncN::create(CC_CALLBACK_1(GameScene::showChangePropAnim, this, A->type())), NULL));
				a->changeType(A->type());
				a->setFocus(true);
			}
		}
	}
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		_vecBlocks[i]->setChangeMask(false);
		_vecBlocks[i]->runAction(ScaleTo::create(0.25f, _stage.cols >= TOSMALLCOLS ? 0.9f : 1.0f));
	}
}

void GameScene::delayShowReadyGo(float dt)
{
	if (isfirstStageGuide())
	{
		vector<Node*> nodes;
		nodes.push_back(_vecBlocks[1]);
		nodes.push_back(_vecBlocks[4]);
		g_NewerLayer = NewerGuideLayer::create(1);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
		GlobalData::g_gamestatus = GAMESTART;
	}
	else
		showReadyAnim();
}

void GameScene::delayShowSkillAddTime(float dt)
{
	float delay = 0.0f;

	int myAddTimeLv = GlobalData::getMySkillLevel(S_ADDTIME);
	if (myAddTimeLv > 0)
	{
		_saddTimeText = Sprite::createWithSpriteFrameName("saddtimetext.png");
		_saddTimeText->setPosition(Vec2(270, 710));

		std::string str = StringUtils::format("%d", _stageSecond - _stage.time);
		Label* timenum = Label::createWithBMFont("fonts/unlocknum.fnt", str);
		timenum->setPosition(Vec2(100, 15));
		_saddTimeText->setScale(0.8f);
		_saddTimeText->addChild(timenum);
		addChild(_saddTimeText, 10);
		Vec2 movePos = Vec2(_leftCountLabel->getPositionX() + _saddTimeText->getContentSize().width/2 - 15, _leftCountLabel->getPositionY());
		_saddTimeText->runAction(Sequence::create(Spawn::create(MoveTo::create(1.5f, movePos), ScaleTo::create(1.0f, 1.0f),DelayTime::create(0.5f), NULL), CallFuncN::create(CC_CALLBACK_1(GameScene::removeAddTimeText, this)), NULL));

		delay = 0.3f;
	}
	scheduleOnce(schedule_selector(GameScene::delayShowReadyGo), delay);
}

void GameScene::removeAddTimeText(Ref* pSender)
{
	std::string str = StringUtils::format("%d", _stageSecond);
	_leftCountLabel->setString(str);
	Node* node = (Node*)pSender;
	node->stopAllActions();
	node->removeFromParentAndCleanup(true);
	node = NULL;
}

void GameScene::remove1SText(Ref* pSender)
{
	Node* node = (Node*)pSender;
	node->stopAllActions();
	node->removeFromParentAndCleanup(true);
	node = NULL;
}
void GameScene::showAdd1SAnim()
{
	if (_curBlock != NULL)
	{
		Sprite*  sadd1sText = Sprite::createWithSpriteFrameName("sadd1stext.png");
		sadd1sText->setPosition(_curBlock->getPosition());
		addChild(sadd1sText, 10);
		Vec2 movePos = Vec2(_leftCountLabel->getPositionX() + sadd1sText->getContentSize().width/2, _leftCountLabel->getPositionY());
		sadd1sText->runAction(Sequence::create(Spawn::create(MoveTo::create(0.8f, movePos), ScaleTo::create(0.8f, 0.8f), NULL), CallFuncN::create(CC_CALLBACK_1(GameScene::remove1SText, this)), NULL));
	}
}

void GameScene::showVsLayer(float dt)
{
	VSLayer * vslayer = VSLayer::create();
	vslayer->setBoss(_curChapter);
	addChild(vslayer, 1000);

	if (bossnormal != NULL)
	{
		bossnormal->removeFromParentAndCleanup(true);
		bossnormal = NULL;
	}

	bossnormal = bossNormalLayer[_curChapter - 1];

	bossnormal->setPosition(Vec2(475, 810));
	bossnormal->setVisible(false);

	bossNoramlAnim = CSLoader::createTimeline(bossNormalLayer[_curChapter - 1]->getName());
	bossnormal->runAction(bossNoramlAnim);
	bossNoramlAnim->gotoFrameAndPlay(0, true);

	if (bosshited != NULL)
	{
		bosshited->removeFromParentAndCleanup(true);
		bosshited = NULL;
	}

	bosshited = bossHitedLayer[_curChapter - 1];

	bosshited->setPosition(Vec2(475, 810));
	bosshited->setVisible(false);

	rootCsbLayer->addChild(bossnormal, 800);
	rootCsbLayer->addChild(bosshited, 800);

	scheduleOnce(schedule_selector(GameScene::delayShowBossAnim), 2.0f);
	scheduleOnce(schedule_selector(GameScene::checkUseProp), 1.5f);
}

int GameScene::getBonuesTime()
{
	return 0;
}

void GameScene::showChangePropAnim(Ref* pSender, int type)
{
	Block* block = (Block*)pSender;
	block->changeType(type);
	if (block->getTag() == 0)
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BLOCKCHANGE);
	block->runAction(Sequence::create(ScaleTo::create(0.3f, _stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f), CallFunc::create(CC_CALLBACK_0(GameScene::resetBlockZorder, this)), NULL));
}

void GameScene::showBombPropAnim(Ref* pSender)
{
	Block* block = (Block*)pSender;

	block->changeSpecialType(B_BOMB);
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BLOCKCHANGE);
	block->runAction(Sequence::create(ScaleTo::create(0.3f, _stage.cols >= TOSMALLCOLS ? 0.9f : 1.0f), NULL));
}
void GameScene::resetBlockZorder()
{
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		_vecBlocks[i]->setLocalZOrder(_vecBlocks.size() - i);
	}
}

void GameScene::changeBombMove(Ref* pSender)
{
	Block* block = (Block*)pSender;
	_lastBlock = block;
	_curBlock = block;
	beginmove();
}

int GameScene::getBonuseGold()
{
	return _cleanGlodBlockTimes * 2;
}

int GameScene::getScore()
{
	return extrasocre + _comboscore + getLeftSecondTime() * 10;
}
void GameScene::revive()
{
	SoundManager::sharedManager()->stopAllEffectSound();

	GlobalData::g_gamestatus = GAMESTART;
	updateTimeUi(_stage.time);
	SoundManager::sharedManager()->resumeBackMusic();
}

void GameScene::restart()
{
	extrasocre = 0;
	_comboscore = 0;
	reset();
	updateTimeUi(_stage.time);

	scheduleOnce(schedule_selector(GameScene::checkUseProp), 0.2f);
	SoundManager::sharedManager()->stopAllEffectSound();
	if (isbossmode)
	{
		SoundManager::sharedManager()->playBackMusic("sound/bgboss.mp3");
		heroAminNode->setPosition(Vec2(70, 730));
		
	}
	else
	{
		SoundManager::sharedManager()->playBackMusic("sound/bg2.mp3");
		heroAminNode->setPosition(Vec2(270, 730));
	}
	heroAminNode->setScale(0.45f);
	heroAminNode->setRotationSkewY(180);
	delayShowHeroAnim(0);
}

void GameScene::restartClasic()
{
	int freecount = GlobalData::getCModeCount(CMCLASIC);
	if (freecount > 0)
	{
		GlobalData::setCModeCount(CMCLASIC, --freecount);
	}
	if (freecount <= 0)
	{
		GlobalData::setCurStageUseHp(CLASICUSEHP);
	}
	_curChapter = 1;
	clasiccleancount = 0;
	restart();
}

void GameScene::restartFlash()
{
	int freecount = GlobalData::getCModeCount(CMFLASH);
	if (freecount > 0)
	{
		GlobalData::setCModeCount(CMFLASH, --freecount);
	}
	if (freecount <= 0)
	{
		GlobalData::setCurStageUseHp(3);
	}
	restart();
	initFlashModeData();
}

void GameScene::reset()
{
	_lastBlock = NULL;
	_curBlock = NULL;
	_promptBlock1 = NULL;
	_promptBlock2 = NULL;

	_overblockcount = 0;
	_canCleanBarriers = 0;
	_clickCleanBlockCount = 0;
	_fireBlink = false;
	_nolinktime = 0;
	_lastRandom = -1;
	_isMoving = false;
	_isMovingClean = false;

	_noCleanTime = 0.0f;

	removeCSB();
	stopFireAction();
	_vecCloudBlocks.clear();
	_vecNumBlocks.clear();

	skillAdd1SecondCount = 0;
	reMakeMap();

	_isPlayTimeUp = false;
	GlobalData::g_gamestatus = GAMEREADY;

	for (int i= 0; i < 3; i++)
	{
		_starlv[i]->setVisible(GlobalData::getChallengeMode() == CMNORMAL);
	}
	useAddTime = 0;
	_timeupShowAddTime = true;
	SoundManager::sharedManager()->resumeBackMusic();
}

void GameScene::updateTimeUi(int time)
{
	_lastlinkpasttime = 0;
	_keepingcount = 1;

	_pasttime = 0;
	string gametime = StringUtils::format("%d", time);

	_leftCountLabel->setString(gametime);
	progressTimer->setPercent(100);

	_stageSecond = time;
	int myAddTimeLv = GlobalData::getMySkillLevel(S_ADDTIME);
	if (myAddTimeLv > 0)
		_stageSecond += GlobalData::mapSkillData[S_ADDTIME].data[myAddTimeLv - 1];
}

void GameScene::resetNextSmallStageData()
{
	_lastBlock = NULL;
	_curBlock = NULL;
	_promptBlock1 = NULL;
	_promptBlock2 = NULL;

	_lastlinkpasttime = 0;
	_keepingcount = 1;
	_overblockcount = 0;
	_canCleanBarriers = 0;
	_clickCleanBlockCount = 0;
	_fireBlink = false;
	_nolinktime = 0;
	_lastRandom = -1;
	_isMoving = false;
	_isMovingClean = false;
	clearData();
}

void GameScene::delayNextBossStage(float dt)
{
	GlobalData::setIsUseChange(false);
	GlobalData::setIsUseBomb(false);
	GlobalData::setIsUseIce(false);
	showBossStage();
}

void GameScene::showBossStage()
{
	_curChapter = GlobalData::getDefeatedBossCount() + 1;

	std::string capterstr = StringUtils::format("%d", _curChapter);
	_chapterLbl->setString(capterstr);

	reset();
	updateTimeUi(_stage.time);
	showVsLayer(0);

#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };

	sprintf(p_str, "bossstage%d", _curChapter);
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "startLevel", "(Ljava/lang/String;)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
	}
#endif
}
void GameScene::resetBossStage()
{
	extrasocre = 0;
	_comboscore = 0;
	GlobalData::setDefeatedBossCount(0);
	showBossStage();
}

void GameScene::resetNextClasicStage(float dt)
{
	GlobalData::setIsUseChange(false);
	GlobalData::setIsUseBomb(false);
	GlobalData::setIsUseIce(false);
	_curChapter = _curChapter + 1;
	clasicstage++;
	reset();
	_pasttime = _stageSecond - getLeftSecondTime();
	GameStart();

#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };

	sprintf(p_str, "clasicstage%d", _curChapter);
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "startLevel", "(Ljava/lang/String;)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
	}
#endif
}

int GameScene::getClasicStage()
{
	return clasicstage;
}

int GameScene::getFlashAddTimes()
{
	return flashAddtimes;
}

void GameScene::reMakeMap()
{
	string filename;
	string xmlfile;
	if (GlobalData::getChallengeMode() == CMBOSS)
	{
		int syssec = GlobalData::getSystemSecTime();
		srand(syssec);
		int r = rand() % 5;
		xmlfile = StringUtils::format("data/boss/b%d-%d.xml", GlobalData::getDefeatedBossCount() + 1, r + 1);
		//string xmlfile = StringUtils::format("data/boss/b%d-%d.xml", GlobalData::getDefeatedBossCount() + 1, GlobalData::bossstasge);
	}
	else if (GlobalData::getChallengeMode() == CMCLASIC)
	{
		int syssec = GlobalData::getSystemSecTime();
		srand(syssec);
		int r = rand() % 5;
		xmlfile = StringUtils::format("data/clasic/c%d-%d.xml", _curChapter, r + 1);
	}
	else if (GlobalData::getChallengeMode() == CMFLASH)
	{
		xmlfile = "data/flash/stage.xml";
	}
	else
	{
		xmlfile = StringUtils::format("data/%d-%d-%d.xml", _curChapter, _curStage, 1);
	}
	filename = GlobalData::getXMLDataPath(xmlfile);
	parseXML(filename);
	log("filename = %s", filename.c_str());
	for (unsigned int n = 0; n < _vecBlocks.size(); n++)
	{
		Sprite* block = _vecBlocks[n];
		block->removeFromParentAndCleanup(true);
		block = NULL;
	}
	_vecBlocks.clear();

	for (unsigned int n = 0; n < _vecLines.size(); n++)
	{
		Sprite* line = _vecLines[n];
		line->removeFromParentAndCleanup(true);
		line = NULL;
	}
	_vecLines.clear();

	clearSNBlocks();

	makeMap();
}

void GameScene::cleanSpecialBlock(Block* a, Block* b)
{
	SpecialType type = b->stype();
	bool isPlaySound = true;
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* block = _vecBlocks[i];
		if (type == B_RHOMBIC && block->stype() == B_RHOMBICBARRIER)
		{
			block->state(DESTROID);
			boom(block);
			_overblockcount ++;
			_canCleanBarriers++;
			clasiccleancount++;
			if (isPlaySound)
			{
				isPlaySound = false;
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BARRIERBOMB);
			}
		}
		else if (type == B_HEXAGON && block->stype() == B_HEXAGONBARRIER)
		{
			block->state(DESTROID);
			boom(block);
			_overblockcount ++;
			_canCleanBarriers++;
			clasiccleancount++;
			if (isPlaySound)
			{
				isPlaySound = false;
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BARRIERBOMB);
			}
		}
		else if (type == B_GOLDKEY)
		{
			if (block->sstate() == SS_GOLDLOCK)
			{
				boom(block);
				block->sstate(SS_NORMAL);
			}
			else if (block->sstate() == SS_GOLDLOCKFROZEN)
			{
				boom(block);
				block->sstate(SS_FROZEN);
			}
			if (isPlaySound)
			{
				isPlaySound = false;
				SoundManager::sharedManager()->playSound(SoundManager::SOUDN_ID_LOCKBOMB);
			}
		}
		else if (type == B_SILVERKEY)
		{
			if (block->sstate() == SS_SILVERLOCK)
			{
				boom(block);
				block->sstate(SS_NORMAL);
			}
			else if (block->sstate() == SS_SILVERLOCKFROZEN)
			{
				boom(block);
				block->sstate(SS_FROZEN);
			}
			if (isPlaySound)
			{
				isPlaySound = false;
				SoundManager::sharedManager()->playSound(SoundManager::SOUDN_ID_LOCKBOMB);
			}
			
		}

		if (block->sstate() == SS_FROZEN)
		{
			int isUnfrozen = false;
			if (a->row() == block->row() && (a->col() + 1 == block->col() || a->col() - 1 == block->col()))
			{
				isUnfrozen = true;
			}
			else if (a->col() == block->col() && (a->row() + 1 == block->row() || a->row() - 1 == block->row()))
			{
				isUnfrozen = true;
			}
			if (b->row() == block->row() && (b->col() + 1 == block->col() || b->col() - 1 == block->col()))
			{
				isUnfrozen = true;
			}
			else if (b->col() == block->col() && (b->row() + 1 == block->row() || b->row() - 1 == block->row()))
			{
				isUnfrozen = true;
			}
			if (isUnfrozen)
			{
				MyParticle::Show(this, SpeciallyName[19], block->getPositionX(), block->getPositionY(), true, 0.3f);
				MyParticle::Show(this, SpeciallyName[20], block->getPositionX(), block->getPositionY(), true, 0.3f);
				MyParticle::Show(this, SpeciallyName[21], block->getPositionX(), block->getPositionY(), true, 0.3f);
				if (isPlaySound)
				{
					isPlaySound = false;
					SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ICEBOMB);
				}
				block->sstate(SS_NORMAL);
			}
		}
	}
}
void GameScene::changeNumBlocks()
{
	if (_vecNumBlocks.empty())
		return;

	_vecNumBlocks.erase(_vecNumBlocks.begin());
	_vecNumBlocks.erase(_vecNumBlocks.begin());
	if (_vecNumBlocks.size() >= 2)
	{
		_vecNumBlocks[0]->state(NORMALBLOCK);
		_vecNumBlocks[1]->state(NORMALBLOCK);
	}
}

void GameScene::clearSNBlocks()
{

	if (_SNUp != NULL)
	{
		_SNUp->removeFromParentAndCleanup(true);
		_SNUp = NULL;
	}

	if (_SNDown != NULL)
	{
		_SNDown->removeFromParentAndCleanup(true);
		_SNDown = NULL;
	}

	if (_SNLeft != NULL)
	{
		_SNLeft->removeFromParentAndCleanup(true);
		_SNLeft = NULL;
	}
	
	if (_SNRight != NULL)
	{
		_SNRight->removeFromParentAndCleanup(true);
		_SNRight = NULL;
	}
	if (_wallleft != NULL)
	{
		_wallleft->removeFromParentAndCleanup(true);
		_wallleft = NULL;
	}

	if (_wallright != NULL)
	{
		_wallright->removeFromParentAndCleanup(true);
		_wallright = NULL;
	}
	/*
	for (n = 0; n < _vecSNLeft.size(); n++)
	{
		_vecSNLeft[n]->removeFromParentAndCleanup(true);
		_vecSNLeft[n] = NULL;
	}
	_vecSNLeft.clear();

	for (n = 0; n < _vecSNRight.size(); n++)
	{
		_vecSNRight[n]->removeFromParentAndCleanup(true);
		_vecSNRight[n] = NULL;
	}
	_vecSNRight.clear();
	*/
}



void GameScene::updateStarlv()
{
	int timeleft = (_stageSecond - (floor(_pasttime) + 1));
	if (_starlv[2]->isVisible() && timeleft < _stage.time3)
	{
		_starlv[2]->setVisible(false);
	}
	else if (!_starlv[2]->isVisible() && timeleft >= _stage.time3 && GlobalData::getChallengeMode() == CMNORMAL)
	{
		_starlv[2]->setVisible(true);
	}

	if (_starlv[1]->isVisible() && timeleft < _stage.time2)
	{
		_starlv[1]->setVisible(false);
	}
	else if (!_starlv[1]->isVisible() && timeleft >= _stage.time2 && GlobalData::getChallengeMode() == CMNORMAL)
	{
		_starlv[1]->setVisible(true);
	}

	if (_starlv[0]->isVisible() && timeleft < _stage.time1)
	{
		_starlv[0]->setVisible(false);
	}
	else if (!_starlv[0]->isVisible() && timeleft >= _stage.time1 && GlobalData::getChallengeMode() == CMNORMAL)
	{
		_starlv[0]->setVisible(true);
	}
}

void GameScene::showCountDownTime(float dt)
{
	if (isfirstStageGuide())
		return;
	if (GlobalData::g_gamestatus == GAMEPAUSE || GlobalData::g_gamestatus == GAMEREADY || GlobalData::g_gamestatus == GAMEOVER)
		return;
	float before = _pasttime;
	icesec -= dt;
	if (icesec <= 0)
	{
		icesec = 0.0f;
		_pasttime += dt;
		icefrozen->setVisible(false);
	}
	else
	{
		icefrozen->setVisible(true);
	}
		
	_nolinktime += dt;

	updateStarlv();

	_noCleanTime += dt;

	if (_noCleanTime >= 10.0f && GlobalData::checkGuide(14) && GlobalData::getPromptProp() > 0 && GlobalData::getHighestStage() >= 1)
	{
		GlobalData::g_gamestatus = GAMEPAUSE;
		NewerGuideClickCB(14);
	}
	else if (_noCleanTime >= 10.0f && GlobalData::checkGuide(15) && GlobalData::getRefreshProp() > 0 && GlobalData::getHighestStage() >= 1)
	{
		GlobalData::g_gamestatus = GAMEPAUSE;
		NewerGuideClickCB(15);
	}

	int myAutoPrompLv = GlobalData::getMySkillLevel(S_AUTOPROMT);
	if (myAutoPrompLv > 0 && _autopromptCount > 0 && _noCleanTime >= GlobalData::mapSkillData[S_AUTOPROMT].data[myAutoPrompLv - 1] && !_isMoving)
	{
		_noCleanTime = -1000.0f;
		usePrompt();
		_autopromptCount--;
	}

	if (GlobalData::getChallengeMode() == CMFLASH)
	{
		cmpasttime += dt;
		int index = -1;
		for (unsigned int i = 0; i < GlobalData::FRuleData.size(); i++)
		{
			if (cmpasttime >= GlobalData::FRuleData[i].ttime && !GlobalData::FRuleData[i].isdone)
			{
				index = i;
				GlobalData::FRuleData[i].isdone = true;
				break;
			}
		}
		if (index >= 0)
		{
			flashAddtimes++;
			doFlashModeRule(index);
		}

	}

	if (_pasttime >= _stageSecond)
	{
		GlobalData::g_gamestatus = GAMEOVER;
		progressTimer->setPercent(0);
		_leftCountLabel->setString("0");

		stopFireAction();

		if (!isCleanAll() || GlobalData::getChallengeMode() == CMFLASH)
		{

			if (!_isPlayTimeUp)
			{
				_isPlayTimeUp = true;
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_TIMEUP);
				//_gameStateTextBox->setVisible(true);
			}
			if (_timeupShowAddTime && GlobalData::getHighestStage() >= 0)
			{
				_timeupShowAddTime = false;
				//gamePause();
				GlobalData::g_gamestatus = GAMEPAUSE;
				this->scheduleOnce(schedule_selector(GameScene::enterAddTimeLayer), 1.5f);
			}
			else
			{
				/*
				_gameStateTextBox->setPositionY(960 + _gameStateTextBox->getContentSize().height / 2);
				_gameStateTextBox->setVisible(true);
				_gameStateTextBox->runAction(EaseSineIn::create(MoveTo::create(0.3f, Vec2(270, 480))));
				_gameStateText->setSpriteFrame("gamefailtext.png");
				this->scheduleOnce(schedule_selector(GameScene::enterReviveLayer), 1.8f);
				*/

				if (GlobalData::getChallengeMode() == CMCLASIC || GlobalData::getChallengeMode() == CMFLASH)
				{
					GlobalData::g_gamestatus = GAMEPAUSE;
					this->scheduleOnce(schedule_selector(GameScene::enterCMOverLayer), 0.5f);
				}
				else
				{
					if (GlobalData::getHighestStage() < 0)
					{
						_pasttime = 0;
						string tstr = StringUtils::format("%d", _stageSecond);
						_leftCountLabel->setString(tstr);
						progressTimer->setPercent(100);
						GlobalData::setGuide(5, true);
						NewerGuideClickCB(5);
					}
					else
					{
						this->scheduleOnce(schedule_selector(GameScene::delayPlayLossSound), 0.6f);
						this->scheduleOnce(schedule_selector(GameScene::playFailAnim), 0.5f);
					}
				}

			}
		}
		return;
	}
	else
	{
		if (floor(_pasttime) - floor(before) >= 1 && _stageSecond - _pasttime < 10)
		{
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_TIMEWARNING);
		}
		if (_stageSecond - _pasttime < 10 && !_fireBlink)
		{
			_fire->setVisible(true);
			_fire1->setVisible(true);
			_fire->runAction(RepeatForever::create(Sequence::create(Blink::create(1, 4), NULL)));
			_fire1->runAction(RepeatForever::create(Sequence::create(Blink::create(1, 4), NULL)));
			_fireBlink = true;
		}
		if (_nolinktime > 10)
		{
			_nolinktime = 0;

		}
	}
	leftTime = (int)(_stageSecond - (floor)(_pasttime));
	string timeStr = StringUtils::format("%d", leftTime);
	_leftCountLabel->setString(timeStr);
	progressTimer->setPercent((_stageSecond - _pasttime) * 100 / _stageSecond);
	if (_stageSecond - _pasttime >= 10 && _fireBlink)
	{
		stopFireAction();
	}
}

void GameScene::enterAddTimeLayer(float dt)
{
	_gameStateTextBox->setVisible(false);
	ContinueLayer::AnimType = A_UPTODOWN;
	g_whattobuy = P_CLOCK;
	ContinueLayer* layer = ContinueLayer::create();
	Director::getInstance()->getRunningScene()->addChild(layer, 1000);
}

void GameScene::playFailAnim(float dt)
{
	heroAminNode->stopAllActions();
	heroAminNode->setRotationSkewY(0);
	heroAminNode->setPosition(Vec2(270, 370));
	heroAminNode->setScale(0.8f);
	heroAction = CSLoader::createTimeline(heroAminNode->getName()); //加载动画：
	heroAminNode->runAction(heroAction);
	heroAction->play("fail", true);
	this->scheduleOnce(schedule_selector(GameScene::enterReviveLayer), 1.5f);
}

void GameScene::enterReviveLayer(float dt)
{
	rettype = 1;
	_gameStateTextBox->setVisible(false);
	ReviveLayer::AnimType = A_UPTODOWN;
	Layer* layer = ReviveLayer::create();
	rootCsbLayer->addChild(layer, 700, "revivelayer");
}

void GameScene::enterCMOverLayer(float dt)
{
	rettype = 0;
	_gameStateTextBox->setVisible(false);
	ReviveLayer::AnimType = A_UPTODOWN;
	Layer* layer = CMOverLayer::create();
	Director::getInstance()->getRunningScene()->addChild(layer, 0, "clasicoverlayer");
}

float GameScene::getUseSecondTime()
{
	return _pasttime + useAddTime * 15.0f + skillAdd1SecondCount * 1.0f;
}

int GameScene::getLeftSecondTime()
{
	return leftTime;
}

void GameScene::stopFireAction()
{
	_fire->setVisible(false);
	_fire->stopAllActions();
	_fire1->setVisible(false);
	_fire1->stopAllActions();
	_fireBlink = false;
}

void GameScene::onPromptClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (isCleanAll() || GlobalData::g_gamestatus == GAMEOVER)
			return;

		g_whattobuy = P_PROMPT;

		_noCleanTime = 0.0f;
		if (GlobalData::getPromptProp() <= 0)
		{
			if (GlobalData::getGoldCount() >= 200)
			{
				GlobalData::setGoldCount(GlobalData::getGoldCount() - 200);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_USEPROP);
				usePrompt();

				Achieve* data = GlobalData::getAchieveDataByID(19);
				if (data->finish != -1)
				{
					data->finish += 200;
					GlobalData::SaveAchieveData();
				}

				data = GlobalData::getTaskDataByID(4);
				if (data->finish != -1)
				{
					data->finish += 200;
					GlobalData::SaveTaskData();
				}
#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };


				sprintf(p_str, "%s", "prompt");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1, 0.0f);
				}

				sprintf(p_str, "%s", "coin");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 200);
				}
#endif
			}
			else
			{
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
				gamePause();
				HintBox* hint = HintBox::create("金币不足", 0);
				Director::getInstance()->getRunningScene()->addChild(hint, 1000);

				showBuyFrame(1);
			}

			return;
		}


		GlobalData::setPromptProp(GlobalData::getPromptProp() - 1);
		string propNum = StringUtils::format("x%d", GlobalData::getPromptProp());
		_promptcountLabel->setString(propNum);

		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_USEPROP);

		usePrompt();

		if (g_NewerLayer != NULL)
		{
			g_NewerLayer->removeSelf();
			if (GlobalData::g_gamestatus == GAMEPAUSE)
				GlobalData::g_gamestatus = GAMESTART;
		}
	}
}

void GameScene::usePrompt()
{
	if (GlobalData::g_gamestatus == GAMEOVER)
		return;

	Achieve* data = GlobalData::getAchieveDataByID(26);
	if (data->finish != -1)
	{
		data->finish ++;
		GlobalData::SaveAchieveData();
	}

	data = GlobalData::getAchieveDataByID(9);
	if (data->finish != -1)
	{
		data->finish++;
		GlobalData::SaveAchieveData();
	}
	data = GlobalData::getTaskDataByID(2);
	if (data->finish != -1)
	{
		data->finish++;
		GlobalData::SaveTaskData();
	}

#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };

	sprintf(p_str, "%s", "prompt");
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1);
	}
#endif

	if (_lastBlock != NULL)
	{
		_lastBlock->setFocus(false);
		_lastBlock = NULL;
	}
	for (int i = 0; i < _stage.rows * _stage.cols - 1; i++)
	{
		for (int j = i + 1; j < _stage.rows * _stage.cols; j++)
		{
			if (_vecBlocks[i]->stype() == B_BOMB && _vecBlocks[i]->state() == NORMALBLOCK)
			{
				_promptBlock1 = _vecBlocks[i];
				_promptBlock2 = _promptBlock1;
				removeCSB();
				playPromptAnim(_promptBlock1);

				return;
			}
			else if (verify2points(_vecBlocks[i], _vecBlocks[j]))
			{
				
				_promptBlock1 = _vecBlocks[i];
				_promptBlock2 = _vecBlocks[j];
				removeCSB();
				playPromptAnim(_promptBlock1);
				playPromptAnim(_promptBlock2);

				return;
			}
		}
	}
}

void GameScene::playPromptAnim(Node* node)
{

	playUsePromptParticle(_promptBtn, node->getPosition());
	node->setVisible(true);
	Node * animNode = CSLoader::createNode("PromptNode.csb");
	animNode->setPosition(Vec2(node->getPositionX(), node->getPositionY()));
	rootCsbLayer->addChild(animNode, 200, 79);
	animNode->setScale(0.8f);
	auto action = CSLoader::createTimeline("PromptNode.csb");
	animNode->runAction(action);
	action->gotoFrameAndPlay(0, 115, true);
	Block* b = (Block*)node;
	b->runUpAndDown();
}

void GameScene::removeCSB()
{
	for (int i = 0; i < 2; i++)
		rootCsbLayer->removeChildByTag(79);

	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		_vecBlocks[i]->reset();
	}
}

void GameScene::onRefreshClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (isCleanAll() || GlobalData::g_gamestatus == GAMEOVER)
			return;

		g_whattobuy = P_REFRESH;
		_noCleanTime = 0.0f;
		if (GlobalData::getRefreshProp() <= 0)
		{
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
			if (GlobalData::getGemCount() >= 20)
			{
				GlobalData::setGemCount(GlobalData::getGemCount() - 20);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_USEPROP);


#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };


				sprintf(p_str, "%s", "refresh");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1, 20.0f);
				}
#endif

				useRefresh();
			}
			else
			{
				gamePause();
				HintBox* hint = HintBox::create("钻石不足", 0/*1*/);
				addChild(hint, 1000);

				showBuyFrame(1);
			}
			return;
		}

		GlobalData::setRefreshProp(GlobalData::getRefreshProp() - 1);
		string propNum = StringUtils::format("x%d", GlobalData::getRefreshProp());
		_refreshcountLabel->setString(propNum);

		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_USEPROP);
		useRefresh();

		if (g_NewerLayer != NULL)
		{
			g_NewerLayer->removeSelf();

			if (GlobalData::g_gamestatus == GAMEPAUSE)
				GlobalData::g_gamestatus = GAMESTART;
		}
	}

}

void GameScene::useRefresh()
{
	if (GlobalData::g_gamestatus == GAMEOVER)
		return;

	Achieve* data = GlobalData::getAchieveDataByID(26);
	if (data->finish != -1)
	{
		data->finish ++;
		GlobalData::SaveAchieveData();
	}

	data = GlobalData::getAchieveDataByID(8);
	if (data->finish != -1)
	{
		data->finish++;
		GlobalData::SaveAchieveData();
	}

	data = GlobalData::getTaskDataByID(6);
	if (data->finish != -1)
	{
		data->finish++;
		GlobalData::SaveTaskData();
	}
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };

	sprintf(p_str, "%s", "refresh");
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1);
	}
#endif

	if (_lastBlock != NULL)
	{
		_lastBlock->setFocus(false);
		_lastBlock = NULL;
	}
	if (_promptBlock1 != NULL)
	{
		removeCSB();
		_promptBlock1->setFocus(false);
		_promptBlock1 = NULL;
		_promptBlock2->setFocus(false);
		_promptBlock2 = NULL;
	}

	playUsePromptParticle(_refreshBtn, Vec2(270, 400));
	this->scheduleOnce(schedule_selector(GameScene::useRefreshShuffle), 0.4f);

}

void GameScene::playUsePromptParticle(Node* node, Vec2 despos)
{
	MyParticle* p = MyParticle::Show(rootCsbLayer, SpeciallyName[47], node->getPositionX(), node->getPositionY(), true, 0.4f);
	p->setScale(0.25f);
	p->runAction(MoveTo::create(0.35f, despos));
}

void GameScene::useRefreshShuffle(float dt)
{
	shuffle();
}

void GameScene::onClockClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (isCleanAll() || GlobalData::g_gamestatus == GAMEOVER)
			return;

		g_whattobuy = P_CLOCK;
		_noCleanTime = 0.0f;
		if (GlobalData::getAddtimeProp() <= 0)
		{
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);

			if (GlobalData::getGemCount() >= 20)
			{
				GlobalData::setGemCount(GlobalData::getGemCount() - 20);
				SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_USEPROP);

#ifdef ANALYTICS
				JniMethodInfo methodInfo;
				char p_str[32] = { 0 };


				sprintf(p_str, "%s", "addtime");
				if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "buy", "(Ljava/lang/String;IF)V"))
				{
					jstring para1 = methodInfo.env->NewStringUTF(p_str);
					methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1, 20.0f);
				}
#endif
				useClock();
			}
			else
			{
				gamePause();
				HintBox* hint = HintBox::create("钻石不足", 0/*1*/);
				addChild(hint, 1000);

				showBuyFrame(1);
			}

			return;
		}

		GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() - 1);
		string propNum = StringUtils::format("x%d", GlobalData::getAddtimeProp());
		_addtimecountLabel->setString(propNum);
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_USEPROP);
		useClock();
	}
}

void GameScene::useClock()
{
	if (GlobalData::g_gamestatus == GAMEOVER)
		return;

	Achieve* data = GlobalData::getAchieveDataByID(26);
	if (data->finish != -1)
	{
		data->finish++;
		GlobalData::SaveAchieveData();
	}

	data = GlobalData::getAchieveDataByID(7);
	if (data->finish != -1)
	{
		data->finish++;
		GlobalData::SaveAchieveData();
	}


#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", "addtime");
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 1);
	}
#endif
	playUsePromptParticle(_clockBtn, _leftCountLabel->getPosition());

	if (_pasttime - 15 < 0)
		_pasttime = 0;
	else
		_pasttime -= 15;

	useAddTime++;
}

void GameScene::onPauseClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::g_gamestatus == GAMEREADY)
			return;

		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_PAUSEBUTTON);
		gamePause();

		pauselayer = PauseLayer::create();
		if (g_gameScene != NULL)
			g_gameScene->addChild(pauselayer, 2000);
	}

}

void GameScene::makeMap()
{

	int normalelescount = 0;	//正常元素的个数
	vector<string> es;
	CommonFuncs::split(_stage.es, es, ",");

	int syssec = GlobalData::getSystemSecTime();
	int static randNum = 0;
	randNum += 60 * 60;
	syssec += randNum;
	srand(syssec);

	if (GlobalData::getChallengeMode() == CMFLASH)
	{
		std::random_shuffle(es.begin(), es.end());
	}
	vector<int> vecTypes;


	randNum += 60 * 60;
	syssec += randNum;
	srand(syssec);

	for (int i = 0;i < PREELECOUNT; i++)
	{
		vecTypes.push_back(i);
	}
	if (_stage.elements_num > PREELECOUNT)
	{
		vector<int> tmpVec;
		for (int j = PREELECOUNT; j < ELECOUNT; j++)
		{
			tmpVec.push_back(j);
		}
		std::random_shuffle(tmpVec.begin(), tmpVec.end());
		for (int m = 0; m < _stage.elements_num - PREELECOUNT; m++)
		{
			vecTypes.push_back(tmpVec[m]);
		}
	}

	std::random_shuffle(vecTypes.begin(), vecTypes.end());

	for (unsigned int n = 0; n < es.size(); n++)
	{
		if (es[n].substr(0, 1).compare("1") == 0 || es[n].substr(0, 1).compare("7") == 0 || es[n].substr(0, 1).compare("j") == 0)
			normalelescount++;
		int pos = (_stage.rows - 1 - n / _stage.cols) * _stage.cols + n % _stage.cols;
		_mr[pos] = (MoveRule)convert16toint(es[n].substr(2, 1).at(0));
	}

	vector<int> vecEles;

	int m = 0;
	for (int j = 0; j < normalelescount / 2; j++)
	{
		if (m >= _stage.elements_num)
			m = 0;
		vecEles.push_back(vecTypes[m]);
		vecEles.push_back(vecTypes[m]);
		m++;
	}
	std::random_shuffle(vecEles.begin(), vecEles.end());


	int row;
	int col;
	float x;
	float y;

	if (_stage.cols >= TOSMALLCOLS)
	{
		BOXWIDTH = 50;
		BOXHEIGHT = 58;
	}
	else
	{
		BOXWIDTH = 58;
		BOXHEIGHT = 67;
	}

	_leftoffset = (SCREEN_WIDTH - _stage.cols * BOXWIDTH) / 2;
	_topoffset = (SCREEN_HEIGHT - _stage.rows * BOXHEIGHT) / 2 - TOPOFFSET;

	for (int k = 0; k < (_stage.rows + 2) * (_stage.cols + 2); k++)
	{
		row = k / (_stage.cols + 2) - 1;
		col = k % (_stage.cols + 2) - 1;
		for (int l = 0; l < 4; l++)
		{
			x = _leftoffset + col * BOXWIDTH + BOXWIDTH / 2;
			y = _topoffset + row * BOXHEIGHT + BOXHEIGHT / 2 - TOPOFFSET;
			if ((LineType)l == LINE_LEFT)
				x -= BOXWIDTH / 4;
			else if ((LineType)l == LINE_RIGHT)
				x += BOXWIDTH / 4;
			else if ((LineType)l == LINE_TOP)
				y += BOXHEIGHT / 4;
			else if ((LineType)l == LINE_BOTTOM)
				y -= BOXHEIGHT / 4;
			Line * line = Line::createWithType((LineType)l, x, y, row, col);
			_vecLines.push_back(line);
			line->setPosition(Point(x, y));
			line->setVisible(false);
			rootCsbLayer->addChild(line);
		}
	}

	int nn = 0;
	_blankblockcount = 0;
	_unavailableBlockCount = 0;
	for (int n = _stage.rows * _stage.cols - 1; n >= 0; n--)
	{
		row = n / _stage.cols;
		col = n % _stage.cols;
		x = _leftoffset + col * BOXWIDTH + BOXWIDTH / 2;
		y = _topoffset + row * BOXHEIGHT + BOXHEIGHT / 2 - TOPOFFSET;
		//SpecialType stype, SpecialStatus sstatus, MoveRule mr
		int pos = (_stage.rows - 1 - n / _stage.cols) * _stage.cols + n % _stage.cols;
		Block * block;
		SpecialType stype = (SpecialType)converToDecimal(es[pos].substr(0, 1).at(0));
		SpecialStatus sstatus = (SpecialStatus)convert16toint(es[pos].substr(1, 1).at(0));
		MoveRule mr = (MoveRule)convert16toint(es[pos].substr(2, 1).at(0));

		if (es[pos].substr(0, 1).compare("1") == 0 || es[pos].substr(0, 1).compare("7") == 0 || es[pos].substr(0, 1).compare("j") == 0)
		{
			int type = vecEles[nn];
			block = Block::createWithType(type, x, y, row, col, stype, sstatus, mr, _stage.cols >= TOSMALLCOLS ? true : false);
			nn++;
		}
		else
			block = Block::createWithType(-1, x, y, row, col, stype, sstatus, mr, _stage.cols >= TOSMALLCOLS ? true : false);
		block->setAnchorPoint(Point(0.5, 0.5));
		block->setPosition(Point(x, y));
		if (es[pos].substr(0, 1).compare("0") == 0)
		{
			block->state(DESTROID);
			_blankblockcount++;
		}
		else if (es[pos].substr(0, 1).compare("2") == 0)
			_unavailableBlockCount++;

		if (stype >= B_ONE && stype <= B_NINE)
		{
			block->state(DISABLE);
			_vecNumBlocks.push_back(block);
		}
		else if (stype == B_COVER)
		{
			_vecCloudBlocks.push_back(block);
		}
		_vecBlocks.push_back(block);
		block->setOpacity(0);
		rootCsbLayer->addChild(block);
	}
	sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);
	sort(_vecNumBlocks.begin(), _vecNumBlocks.end(), sortNumBlocks);
	resetBlockZorder();

	if (_vecNumBlocks.size() >= 2)
	{
		_vecNumBlocks[0]->state(NORMALBLOCK);
		_vecNumBlocks[1]->state(NORMALBLOCK);
	}

	if (_stage.blockup)
	{

		int SNHEIGHT = 38;
		/*
		int sltw = _stage.cols* BOXWIDTH + 2 * SNHEIGHT;
		int slCount = sltw % SNWIDTH == 0 ? sltw / SNWIDTH : (sltw / SNWIDTH + 1);
		float startX = (SCREEN_WIDTH - slCount * SNWIDTH) / 2 + SNWIDTH / 2;
		y = _topoffset - TOPOFFSET + _stage.rows * BOXHEIGHT + 10 + SNHEIGHT / 2;
		for (int i = 0; i < slCount; i++)
		{
			Sprite * s = Sprite::createWithSpriteFrameName("wh.png");
			s->setPosition(Point(startX + SNWIDTH * i, 270));
			_vecSNUp.push_back(s);
			addChild(s);
		}
		*/
		_SNUp = Sprite::create("wh.png");
		float y = _stage.rows * BOXHEIGHT + _SNUp->getContentSize().height / 2 - 2 + _topoffset - TOPOFFSET;
		_SNUp->setPosition(Vec2(269, y));

		_SNUp->setTextureRect(Rect(0, 0, _stage.cols * BOXWIDTH + SNHEIGHT , _SNUp->getContentSize().height));
		rootCsbLayer->addChild(_SNUp, 1);
	}

	if (_stage.blockleft)
	{
		int SNWIDTH = 34;
		float y = (_stage.rows * BOXHEIGHT) / 2 + _topoffset - TOPOFFSET ;
		x =_leftoffset - SNWIDTH / 2 - 3;

		_SNLeft = Sprite::create("wv.png");
		_SNLeft->setPosition(x, y);
		//_vecSNLeft.push_back(s);
		_SNLeft->setTextureRect(Rect(0, 0, SNWIDTH, _stage.rows * BOXHEIGHT + 8));
		rootCsbLayer->addChild(_SNLeft, 1);
	}

	if (_stage.blockright)
	{

		int SNWIDTH = 34;
		float y = (_stage.rows * BOXHEIGHT) / 2 + _topoffset - TOPOFFSET;
		x = _leftoffset + _stage.cols * BOXWIDTH + SNWIDTH / 2 - 3;

		_SNRight = Sprite::create("wv.png");
		_SNRight->setPosition(x, y);
		//_vecSNRight.push_back(s);
		_SNRight->setTextureRect(Rect(0, 0, SNWIDTH, _stage.rows * BOXHEIGHT + 8));
		rootCsbLayer->addChild(_SNRight, 1);
	}

	if (_stage.blockdown)
	{
		int SNHEIGHT = 38;
		/*
		int sltw = _stage.cols* BOXWIDTH + 2 * SNHEIGHT;
		int slCount = sltw % SNWIDTH == 0 ? sltw / SNWIDTH : (sltw / SNWIDTH + 1);
		float startX = (SCREEN_WIDTH - slCount * SNWIDTH) / 2 + SNWIDTH / 2;
		y = _topoffset - TOPOFFSET - SNHEIGHT / 2 - 10;
		for (int i = 0; i < slCount; i++)
		{
			Sprite * s = Sprite::createWithSpriteFrameName("wh.png");
			s->setPosition(Point(startX + SNWIDTH * i, y));
			_vecSNDown.push_back(s);
			addChild(s);
		}
		*/
		_SNDown = Sprite::create("wh.png");
		float y = _topoffset - TOPOFFSET - _SNDown->getContentSize().height / 2;
		_SNDown->setPosition(Vec2(269, y));
		_SNDown->setTextureRect(Rect(0, 0, _stage.cols * BOXWIDTH + SNHEIGHT, _SNDown->getContentSize().height));
		rootCsbLayer->addChild(_SNDown, 1);
	}

	if (_SNLeft != NULL)
	{
		int SNWIDTH = 16;
		int hoffset = 0;
		if (_SNUp != NULL)
			hoffset += 25;
		if (_SNDown != NULL)
			hoffset += 25;

		_wallleft = Sprite::create("wall.png");
		_wallleft->setPosition(Vec2(_SNLeft->getPositionX() - SNWIDTH / 2 - _wallleft->getContentSize().width / 2, _SNLeft->getPositionY() + 5));
		_wallleft->setTextureRect(Rect(0, 0, _wallleft->getContentSize().width, _stage.rows * BOXHEIGHT + hoffset));
		rootCsbLayer->addChild(_wallleft);
	}
	if (_SNRight != NULL)
	{
		int SNWIDTH = 18;
		int hoffset = 0;
		if (_SNUp != NULL)
			hoffset += 25;
		if (_SNDown != NULL)
			hoffset += 25;
		_wallright = Sprite::create("wall.png");
		_wallright->setPosition(Vec2(_SNRight->getPositionX() + SNWIDTH / 2 + _wallright->getContentSize().width / 2 + 5, _SNRight->getPositionY() + 5));
		_wallright->setTextureRect(Rect(0, 0, _wallright->getContentSize().width, _stage.rows * BOXHEIGHT + hoffset));
		rootCsbLayer->addChild(_wallright);
	}

	if (!checkiflive())
		shuffleOver();


	if (GlobalData::getChallengeMode() == CMCLASIC)
	{
		showBlockBack();
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			_vecBlocks[i]->setScale(0);
			_vecBlocks[i]->setOpacity(255);
			_vecBlocks[i]->runAction(Sequence::create(DelayTime::create(0.15f), ScaleTo::create(0.05f, _stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f), NULL));
		}
		this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create(CC_CALLBACK_0(GameScene::hideBlockBack, this)), NULL));
	}
	else
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			_vecBlocks[i]->setScale(0);
			_vecBlocks[i]->setOpacity(255);
			_vecBlocks[i]->runAction(ScaleTo::create(0.8f, _stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f));
		}
	}

	if (_vecCloudBlocks.size() > 0)
		schedule(schedule_selector(GameScene::showCloudBlocks), 2.5f);

}

void GameScene::showCloudBlocks(float dt)
{
	if (_vecCloudBlocks.empty())
	{
		unschedule(schedule_selector(GameScene::showCloudBlocks));
		return;
	}
	static int randNum = 0;
	randNum += 60 * 60 * 1000;
	int rand_seed = GlobalData::getSystemSecTime();
	srand(rand_seed);

	int size = _vecCloudBlocks.size();
	int r = 0;
	if (size > 1)
	{
		for (;;)
		{	
			r = rand() % _vecCloudBlocks.size();
			if (r != _lastRandom)
			{
				_lastRandom = r;
				break;
			}
		}
	}
	_vecCloudBlocks[r]->unCoverCloud();
}

bool GameScene::isObstruction(Block* block)
{
	return false;
}

void GameScene::shuffleBlocks(vector<Block*> &ret)
{
	if (ret.empty())
		return;
	vector<int> tmp;
	
	for (unsigned int i = 0; i < ret.size(); i++)
	{
		Block* block = ret[i];
		if (isCanShuffle(block))
		{
			tmp.push_back(block->type());
		}
	}

	int static randNum = 0;
	randNum += 60 * 60;
	int syssec = GlobalData::getSystemSecTime();
	syssec += randNum;
	srand(syssec);

	std::random_shuffle(tmp.begin(), tmp.end());

	int n = 0;
	for (unsigned int i = 0; i < ret.size(); i++)
	{
		Block* block = ret[i];
		if (isCanShuffle(block))
		{
			//block->changeSpecialType(block->stype(), tmp[n]);
			block->changeType(tmp[n]);
			n++;
		}
	}
	tmp.clear();
}

void GameScene::shuffleBlocksOther(vector<Block*> &ret)
{

	/*
	int static randNum = 0;
	randNum += 60 * 60 * 3;
	int syssec = GlobalData::getSystemSecTime();
	syssec += randNum;
	srand(syssec);
	std::random_shuffle(_vecBlocks.begin(), _vecBlocks.end());

	int row;
	int col;
	float x;
	float y;

	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		row = i / _stage.cols;
		col = i % _stage.cols;
		_vecBlocks[i]->row(row);
		_vecBlocks[i]->col(col);
		x = _leftoffset + col * BOXWIDTH + BOXWIDTH / 2;
		y = _topoffset + row * BOXHEIGHT + BOXHEIGHT / 2 - TOPOFFSET;
		_vecBlocks[i]->setAnchorPoint(Point(0.5f, 0.5f));
		_vecBlocks[i]->x(x);
		_vecBlocks[i]->y(y);
		_vecBlocks[i]->setPosition(Point(x, y));

	}
	*/

	if (ret.empty())
		return;
	vector<Block*> tmp;

	for (unsigned int i = 0; i < ret.size(); i++)
	{
		Block* block = ret[i];
		if (isCanShuffle(block))
		{
			tmp.push_back(block);
		}
	}
	//verify2points(_vecBlocks[i], _vecBlocks[j]);
	bool isverify = false;
	Block* a = NULL;
	Block* b = NULL;
	int findtype = -1;
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		a = tmp[i];
		for (unsigned int j = i + 1; j < tmp.size(); j++)
		{
			b = tmp[j];
			int type = b->type();
			b->changeType(a->type());
			isverify = verify2points(a, b);
			if (isverify)
			{
				findtype = type;
				break;
			}
			else
			{
				b->changeType(type);
			}
		}
		if (isverify)
		{
			break;
		}
	}

	if (isverify)
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* c = _vecBlocks[i];
			if (c != a && c != b && c->type() == findtype)
			{
				c->changeType(findtype);
			}
		}
	}
}

/*
void GameScene::shuffleBlocks(vector<Block*> &ret)
{
	if (ret.empty())
		return;
	vector<Block*> tmp;
	
	for (unsigned int i = 0; i < ret.size(); i++)
	{
		Block* block = ret[i];
		if (isCanShuffle(block))
		{
			tmp.push_back(block);
		}
	}

	std::random_shuffle(tmp.begin(), tmp.end());

	int n = 0;
	for (unsigned int i = 0; i < ret.size(); i++)
	{
		Block* block = ret[i];
		if (isCanShuffle(block))
		{
			ret[i] = tmp[n];
			n++;
		}
	}
	tmp.clear();
}
*/


void GameScene::shuffleOver()
{
	//struct timeval now;
	//gettimeofday(&now, NULL);

	//std::random_shuffle(_vecBlocks.begin(), _vecBlocks.end());
	static int autoRefreshCount = 0;//自动重排次数，5次后，换一种重排
	autoRefreshCount++;
	if (autoRefreshCount <= 5)
		shuffleBlocks(_vecBlocks);
	else
	{
		if (_lastBlock != NULL)
		{
			_lastBlock->setFocus(false);
			_lastBlock = NULL;
		}
		if (_promptBlock1 != NULL)
		{
			removeCSB();
			_promptBlock2->setFocus(false);
			_promptBlock1->setFocus(false);
			_promptBlock1 = NULL;
			_promptBlock2 = NULL;
		}
		shuffleBlocksOther(_vecBlocks);
	}

	if (autoRefreshCount > 5)//如果只有2个冰块
	{
		int frozenCount = 0;
		int frozenIndex = 0;
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* block = _vecBlocks[i];
			if (block->state() != DESTROID && block->sstate() == SS_FROZEN)
			{ 
				frozenCount++;
				frozenIndex = i;
			}
		}
		if (frozenCount > 0)
		{
			_vecBlocks[frozenIndex]->sstate(SS_NORMAL);
		}
	}
	/*
	int row;
	int col;
	float x;
	float y;

	sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);
	for (int n = _vecBlocks.size() - 1; n >= 0; n--)
	{
		_vecBlocks[n]->setScale(0.001f);
		row = n / _stage.cols;
		col = n % _stage.cols;
		_vecBlocks[n]->row(row);
		_vecBlocks[n]->col(col);
	}

	if (!checkiflive())
	{
		shuffleOver();
	}

	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		row = i / _stage.cols;
		col = i % _stage.cols;
		x = _leftoffset + col * BOXWIDTH + BOXWIDTH / 2;
		y = _topoffset + row * BOXHEIGHT + BOXHEIGHT / 2 - TOPOFFSET;
		_vecBlocks[i]->setAnchorPoint(Point(0.5f, 0.5f));
		_vecBlocks[i]->setPosition(Point(x, y));
		if (isCanShuffle(_vecBlocks[i]))
			_vecBlocks[i]->runAction(ScaleTo::create(0.5f, _stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f));
		else
			_vecBlocks[i]->setScale(_stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f);

		_vecBlocks[i]->setLocalZOrder(_vecBlocks.size() - i);
	}
	*/
	sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);

	if (!checkiflive())
	{
		shuffleOver();
		return;
	}

	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		if (isCanShuffle(_vecBlocks[i]))
		{
			_vecBlocks[i]->setScale(0.0f);
			_vecBlocks[i]->runAction(ScaleTo::create(0.5f, _stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f));
		}
		else
			_vecBlocks[i]->setScale(_stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f);
		_vecBlocks[i]->setLocalZOrder(_vecBlocks.size() - i);
		_vecBlocks[i]->setFocus(false);
	}
	autoRefreshCount = 0;
}
bool GameScene::isCanShuffle(Block* block)
{
	return (block->stype() == B_NORMAL || block->stype() == B_COVER || block->stype() == B_TURN) && block->state() != DESTROID;
}
void GameScene::shuffle()
{
	GlobalData::g_gamestatus = GAMEPAUSE;

	float scale = 1.0f;

	//refreshword->runAction(Sequence::create(DelayTime::create(0.6f), Hide::create(), CallFunc::create(CC_CALLBACK_0(GameScene::shuffleOver, this)), NULL));
	_refreshword->runAction(Sequence::create(Show::create(), DelayTime::create(0.6f), Hide::create(), NULL));
	this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create(CC_CALLBACK_0(GameScene::shuffleOver, this)), DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(GameScene::shuffeOverGameStart, this)), NULL));
}

void GameScene::shuffeOverGameStart()
{
	if (pauselayer != NULL)
		return;

	if (GlobalData::g_gamestatus == GAMEPAUSE)
		GlobalData::g_gamestatus = GAMESTART;
}

void GameScene::update(float dt)
{
	int count = GlobalData::getAddtimeProp();
	string propNum = StringUtils::format("x%d", count);

	_addtimecountLabel->setString(propNum);

	count = GlobalData::getPromptProp();
	propNum = StringUtils::format("x%d", count);
	_promptcountLabel->setString(propNum);

	count = GlobalData::getRefreshProp();
	propNum = StringUtils::format("x%d", count);

	_refreshcountLabel->setString(propNum);

	if (GlobalData::g_gamestatus == GAMERESTART)
	{
		Achieve* data = GlobalData::getAchieveDataByID(26);
		if (data->finish != -1)
		{
			data->finish++;
			GlobalData::SaveAchieveData();
		}

		revive();
		return;
	}

	if (GlobalData::g_gamestatus != GAMESTART)
		return;

	if (GlobalData::g_gamestatus == GAMESTART)
	{
		_lastlinkpasttime += dt;

		if (_lastlinkpasttime > _keeptime)
		{
			_lastlinkpasttime = 0;
			_keepingcount = 1;
		}
	}

	showCountDownTime(dt);
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	/*
	if (GlobalData::g_gamestatus == GAMEREADY)
	{
		_go->stopAllActions();
		_go->setVisible(false);
		GlobalData::g_gamestatus = GAMESTART;
	}
	*/

	if (GlobalData::g_gamestatus != GAMESTART && !GlobalData::getIsShowNewerGuidePromp())
		return true;

	if (_isMoving)
		return true;
	auto target = static_cast<Sprite*>(event->getCurrentTarget());

	Point location = target->convertToNodeSpace(touch->getLocation());

	if (location.x < _leftoffset || location.x > _leftoffset + _stage.cols * BOXWIDTH
		|| location.y < _topoffset - TOPOFFSET || location.y > _topoffset - TOPOFFSET + BOXHEIGHT * _stage.rows)
	{
		if (_lastBlock != NULL)
		{
			_lastBlock->setFocus(false);
			_lastBlock = NULL;
		}
		if (_promptBlock1 != NULL)
		{
			removeCSB();
			_promptBlock2->setFocus(false);
			//	else
			_promptBlock1->setFocus(false);
			_promptBlock1 = NULL;
			_promptBlock2 = NULL;
		}
		return true;
	}

	int col = (location.x - _leftoffset) / BOXWIDTH;
	int row = (location.y - _topoffset + TOPOFFSET) / BOXHEIGHT;
	if (row >= _stage.rows || col >= _stage.cols)
	{
		return true;
	}
	Block* tmpBlock = _vecBlocks[row * _stage.cols + col];

	tmpBlock->hideBack();

	if (tmpBlock->state() == DESTROID)
	{
		if (_lastBlock != NULL)
		{
			_lastBlock->setFocus(false);
			_lastBlock = NULL;
		}
		return true;

	}

	if (tmpBlock->state() == DESTROID
		|| tmpBlock->stype() == B_NOTHING
		|| tmpBlock->stype() == B_UNAVAILABLE
		|| tmpBlock->stype() == B_RHOMBICBARRIER
		|| tmpBlock->stype() == B_HEXAGONBARRIER
		|| tmpBlock->state() == DISABLE
		)
		return true;

	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_CLICKBLOCK);

	if (_lastBlock == NULL)
	{
		_lastBlock = _vecBlocks[row * _stage.cols + col];
		//if (_promptBlock1 != NULL && ((_lastBlock->row() == _promptBlock1->row() && _lastBlock->col() == _promptBlock1->col())
		//	|| (_lastBlock->row() == _promptBlock2->row() && _lastBlock->col() == _promptBlock2->col())))
		//{
		//	if (_lastBlock->row() == _promptBlock1->row() && _lastBlock->col() == _promptBlock1->col())
		if (_promptBlock1 != NULL)
		{
			removeCSB();
			_promptBlock2->setFocus(false);
			//	else
			_promptBlock1->setFocus(false);
			_promptBlock1 = NULL;
			_promptBlock2 = NULL;
		}
		//	return;
		//}
		_lastBlock->setFocus(true);

		if (_lastBlock->stype() == B_BOMB)
		{
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_CLEAR);

			if (checkMoveable())
				_isMoving = true;
			_lastlinkpasttime = 0;
			_overblockcount++;
			_clickCleanBlockCount++;
			clasiccleancount++;
			_curBlock = _lastBlock;
			_lastBlock->state(DESTROID);
			boom(_lastBlock);
			extrasocre += 10;

			_lastBlock->setFocus(false);
			beginmove();
			if (_vecBlocks.size() - _blankblockcount - _unavailableBlockCount - _overblockcount > 1)
			{
				clickBombProp();
				this->runAction(Sequence::create(DelayTime::create(0.12f), CallFunc::create(CC_CALLBACK_0(GameScene::beginmove, this)), NULL));
			}
			else
			{
				this->scheduleOnce(schedule_selector(GameScene::delayShuffle), 0.15f);
			}

		}
		return true;
	}
	if (_lastBlock->row() == row && _lastBlock->col() == col)
	{
		return true;
	}
	_curBlock = _vecBlocks[row * _stage.cols + col];
	//_curBlock->setFocus(true);

	bool ret;
	if (_lastBlock->row() * _stage.cols + _lastBlock->col() > _curBlock->row() * _stage.cols + _curBlock->col())
		ret = verify2points(_curBlock, _lastBlock);
	else
		ret = verify2points(_lastBlock, _curBlock);
	if (ret)
	{
		_lastBlock->setFocus(false);
		_curBlock->setFocus(false);
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_CLEAR);
		_lastlinkpasttime = 0;
		_overblockcount += 2;
		_clickCleanBlockCount += 2;
		clasiccleancount += 2;
		if (_lastBlock->type() >= 0 && _lastBlock->type() < ELECOUNT)
			animals[_lastBlock->type()] += 2;

		if (checkMoveable())
			_isMoving = true;
		//if (_keepingcount > 0)
		showComboHitAnim();
		_keepingcount++;

		showLineAnimation();
		_curBlock->state(DESTROID);
		_lastBlock->state(DESTROID);

		boom(_curBlock);
		boom(_lastBlock);
		playHitBossAnim();
		beginmove();
		//this->runAction(Sequence::create(DelayTime::create(0.12f), CallFunc::create(CC_CALLBACK_0(GameScene::beginmove, this)), NULL));
	}
	else
	{
		_keepingcount = 1;
		_lastBlock->setFocus(false);
		_curBlock->setFocus(true);
		_lastBlock = _curBlock;
		_curBlock = NULL;
	}
	return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{

	if (GlobalData::g_gamestatus != GAMESTART && !GlobalData::getIsShowNewerGuidePromp())
		return;

	if (_isMoving || _isMovingClean)
		return;


	auto target = static_cast<Sprite*>(event->getCurrentTarget());

	Point location = target->convertToNodeSpace(touch->getLocation());

	if (location.x < _leftoffset || location.x > _leftoffset + _stage.cols * BOXWIDTH
		|| location.y < _topoffset - TOPOFFSET || location.y > _topoffset - TOPOFFSET + BOXHEIGHT * _stage.rows)
		return;

	int col = (location.x - _leftoffset) / BOXWIDTH;
	int row = (location.y - _topoffset + TOPOFFSET) / BOXHEIGHT;
	if (row >= _stage.rows || col >= _stage.cols)
		return;


	Block* tmpBlock = _vecBlocks[row * _stage.cols + col];
	if (tmpBlock->state() == DESTROID
		|| tmpBlock->stype() == B_NOTHING
		|| tmpBlock->stype() == B_UNAVAILABLE
		|| tmpBlock->stype() == B_RHOMBICBARRIER
		|| tmpBlock->stype() == B_HEXAGONBARRIER
		|| tmpBlock->state() == DISABLE
		)
		return;

	if (_lastBlock == NULL)
	{
		tmpBlock->setFocus(true);
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_CLICKBLOCK);
		_lastBlock = tmpBlock;
		if (_promptBlock1 != NULL)
		{
			removeCSB();
			_promptBlock2->setFocus(false);
			//	else
			_promptBlock1->setFocus(false);
			_promptBlock1 = NULL;
			_promptBlock2 = NULL;
		}
		return;
	}

	if (_lastBlock->row() == row && _lastBlock->col() == col)
	{
		return;
	}

	if (_curBlock != NULL && (_curBlock->stype() == B_NORMAL || _curBlock->stype() == B_COIN))
	{
		_curBlock->setScale(_stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f);
	}
	_curBlock = _vecBlocks[row * _stage.cols + col];

	if (_curBlock->stype() == B_NORMAL || _curBlock->stype() == B_COIN)
	{
		_curBlock->setScale(_stage.cols >= TOSMALLCOLS ? 0.9f : 1.1f);
	}

	bool ret;
	if (_lastBlock->row() * _stage.cols + _lastBlock->col() > _curBlock->row() * _stage.cols + _curBlock->col())
		ret = verify2points(_curBlock, _lastBlock);
	else
		ret = verify2points(_lastBlock, _curBlock);
	if (ret)
	{
		_lastBlock->setFocus(false);
		_curBlock->setFocus(false);
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_CLEAR);
		_lastlinkpasttime = 0;
		_overblockcount += 2;
		_clickCleanBlockCount += 2;
		clasiccleancount += 2;
		if (_lastBlock->type() >= 0 && _lastBlock->type() < ELECOUNT)
			animals[_lastBlock->type()] += 2;
		_isMovingClean = true;
		if (checkMoveable())
			_isMoving = true;
		//if (_keepingcount > 0)
		showComboHitAnim();
		_keepingcount++;

		showLineAnimation();
		_curBlock->state(DESTROID);
		_lastBlock->state(DESTROID);

		boom(_curBlock);
		boom(_lastBlock);
		playHitBossAnim();
		beginmove();
		//this->runAction(Sequence::create(DelayTime::create(0.12f), CallFunc::create(CC_CALLBACK_0(GameScene::beginmove, this)), NULL));
	}
	return;
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{

	if (GlobalData::g_gamestatus != GAMESTART && !GlobalData::getIsShowNewerGuidePromp())
		return;

	_isMovingClean = false;
	if (_curBlock != NULL && (_curBlock->stype() == B_NORMAL || _curBlock->stype() == B_COIN))
	{
		_curBlock->setScale(_stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f);
	}
}


void GameScene::beginmove()
{
	if (_lastBlock != _curBlock)
	{
		cleanSpecialBlock(_lastBlock, _curBlock);
		eraseCloudBlock(_lastBlock, _curBlock);
		if (_curBlock->stype() >= B_ONE && _curBlock->stype() <= B_NINE)
			changeNumBlocks();

		_nolinktime = 0;

		_cleanBlockTimes++;
		if (_curBlock->stype() == B_COIN)
			_cleanGlodBlockTimes++;

		//useCleanAfterSkills(_lastBlock, _curBlock);
	}

	bool needMove = checkMoveable();
	if (needMove)
	{
		if (!checkMoveOrder(_lastBlock->row(), _lastBlock->col()))
		{
			allMove(_curBlock->row(), _curBlock->col());
			allMove(_lastBlock->row(), _lastBlock->col());
		}
		else
		{
			allMove(_lastBlock->row(), _lastBlock->col());
			allMove(_curBlock->row(), _curBlock->col());
		}
	}
	sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);
	
	float dt = 0.0f;
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		if ((_vecBlocks[i]->x() != _vecBlocks[i]->getPositionX() || _vecBlocks[i]->y() != _vecBlocks[i]->getPositionY()) && needMove)
		{
			dt = 0.1f;
			_vecBlocks[i]->runAction(Sequence::create(DelayTime::create(dt), EaseSineIn::create(MoveTo::create(dt, Point(_vecBlocks[i]->x(), _vecBlocks[i]->y()))), NULL));			
		}
		_vecBlocks[i]->setLocalZOrder(_vecBlocks.size() - i);
	}

	if (_lastBlock != _curBlock)
	{
		exeBlockProp(_lastBlock, _curBlock);
		useCleanAfterSkills(_lastBlock, _curBlock);
	}

	_curBlock = NULL;
	_lastBlock = NULL;

	float cdt = 0.0f;
	if (isctogoldskill)
		cdt = dt + 0.05f;

	int delay = 0;
	if (isbossmode)
	{
		delay = 0.2f;
		this->scheduleOnce(schedule_selector(GameScene::bossSkill), dt*2.0 + cdt);
	}

	if (GlobalData::getChallengeMode() == CMCLASIC || GlobalData::getChallengeMode() == CMFLASH)
		this->scheduleOnce(schedule_selector(GameScene::addBlockProp), dt*2.0 + delay + 0.05f);

	this->scheduleOnce(schedule_selector(GameScene::delayShuffle), dt*2.0 + cdt + delay + 0.05f);

	_noCleanTime = 0.0f;

	if (GlobalData::checkGuide(1) && GlobalData::getHighestStage() <= 0)
	{
		if (g_NewerLayer != NULL)
		{
			g_NewerLayer->removeSelf();
			NewerGuideClickCB(2);
			return;
		}
	}

	if (GlobalData::checkGuide(2) && GlobalData::getHighestStage() <= 0)
	{
		if (g_NewerLayer != NULL)
		{
			g_NewerLayer->removeSelf();
			NewerGuideClickCB(3);
			return;
		}
	}
}

bool GameScene::checkMoveable()
{
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		if (_mr[i] != M_STOP)
			return true;
	}
	return false;
}

void GameScene::bossSkill(float dt)
{
	int bossIndex = _curChapter - 1;

	int index = -1;
	for (unsigned int i = 0; i < GlobalData::mapBossSkillData[bossIndex].tdata.size(); i++)
	{
		if (GlobalData::mapBossSkillData[bossIndex].tdata.at(i) == _clickCleanBlockCount)
		{
			index = i;
			break;
		}
	}
	if (index >= 0)
	{
		if (isCleanAll())
			return;

		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BOSSSKILL);

		if (GlobalData::mapBossSkillData[bossIndex].type == BS_TOBACK || GlobalData::mapBossSkillData[bossIndex].type == BS_TOTRUN || GlobalData::mapBossSkillData[bossIndex].type == BS_FROZEN)
		{
			vector<Block*> vec;
			findRandNormalBlocks(vec, GlobalData::mapBossSkillData[bossIndex].edata.at(index));
			bossskillAmin(vec);
			this->runAction(Sequence::create(DelayTime::create(0.15f), CallFuncN::create(CC_CALLBACK_1(GameScene::exeBossSkill, this, vec)), NULL));

		}
		else if (GlobalData::mapBossSkillData[bossIndex].type == BS_COVER)
		{
			int syssec = GlobalData::getSystemSecTime();
			int static randNum = 0;
			randNum += 60 * 60 * 1000;
			syssec += randNum;
			srand(syssec);

			Vec2 movepos = Vec2(rand() % 250 + 150, rand() % 280 + 260);
			Sprite* cloud = Sprite::create("Images/bcloud.png");
			cloud->setPosition(bosshited->getPosition());
			cloud->setScale(0.1f);
			cloud->setOpacity(240);
			addChild(cloud, 900);
			cloud->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 1), MoveTo::create(1, movepos), NULL), DelayTime::create(3.0f), FadeOut::create(1.0f), CallFuncN::create(CC_CALLBACK_1(GameScene::removeSelf, this)), NULL));
		}
		else if (GlobalData::mapBossSkillData[bossIndex].type == BS_TOGRAY)
		{
			vector<Block*> vec;
			for (unsigned int i = 0; i < _vecBlocks.size(); i++)
			{
				Block* b = _vecBlocks[i];
				if (b->state() != DESTROID)
				{
					vec.push_back(b);
				}
			}
			vector<Block*> blankVec;
			bossskillAmin(blankVec);
			this->runAction(Sequence::create(DelayTime::create(0.15f), CallFuncN::create(CC_CALLBACK_1(GameScene::exeBossSkill, this, vec)), NULL));
			this->scheduleOnce(schedule_selector(GameScene::removeGray), 5.0f);
		}

		else if (GlobalData::mapBossSkillData[bossIndex].type == BS_ADDELE)
		{
			int addcount = GlobalData::mapBossSkillData[bossIndex].edata.at(index);
			vector<Block*> addblocks;

			findDestroyBlocks(addblocks, addcount);
			bossskillAmin(addblocks);

			this->runAction(Sequence::create(DelayTime::create(0.15f), CallFuncN::create(CC_CALLBACK_1(GameScene::exeBossSkill, this, addblocks)), NULL));

			_overblockcount -= addcount;
		}
	}
}

void GameScene::bossskillAmin(vector<Block *> vec)
{
	if (vec.size() > 0)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			createRunLight(vec[i]);
		}
	}
	else
	{
		createRunLight(NULL);
	}
}

void GameScene::createRunLight(Node* A)
{
	int posx = 0;
	int posy = 0;
	if (A == NULL)
	{
		posx = 270;
		posy = 480;
	}
	else
	{
		posx = A->getPosition().x;
		posy = A->getPosition().y;
	}
	MyParticle* paticle = MyParticle::Show(this, SpeciallyName[29], bosshited->getPosition().x, bosshited->getPosition().y, true, 2.0f);
	paticle->runAction(Sequence::create(MoveTo::create(0.6f, Vec2(posx, posy)), DelayTime::create(0.2f), CallFuncN::create(CC_CALLBACK_1(GameScene::removeSelf, this)), NULL));
}

void GameScene::exeBossSkill(Ref* pSender, vector<Block *> vec)
{
	int bossIndex = _curChapter - 1;
	if (GlobalData::mapBossSkillData[bossIndex].type == BS_TOBACK)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			vec[i]->showBack();
		}
	}
	else if (GlobalData::mapBossSkillData[bossIndex].type == BS_TOTRUN)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			vec[i]->changeSpecialType(B_TURN, vec[i]->type());
		}
	}

	else if (GlobalData::mapBossSkillData[bossIndex].type == BS_FROZEN)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			vec[i]->sstate(SS_FROZEN);
		}
	}
	else if (GlobalData::mapBossSkillData[bossIndex].type == BS_TOGRAY)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			vec[i]->addGray();
		}
	}
	else if (GlobalData::mapBossSkillData[bossIndex].type == BS_ADDELE)
	{
		addEleBlocks(vec, ELECOUNT);
		_isAddingBlocks = false;
	}

	if (!checkiflive())
		shuffle();
}

void GameScene::addEleBlocks(vector<Block *> vec, int Eletype)
{
	_isAddingBlocks = true;
	int syssec = GlobalData::getSystemSecTime();
	int static randNum = 0;
	randNum += 60 * 60;
	syssec += randNum;
	srand(syssec);

	vector<int> vecTypes;

	for (int i = 0; i < PREELECOUNT; i++)
	{
		vecTypes.push_back(i);
	}
	if (Eletype > PREELECOUNT)
	{
		vector<int> tmpVec;
		for (int j = PREELECOUNT; j < ELECOUNT; j++)
		{
			tmpVec.push_back(j);
		}
		std::random_shuffle(tmpVec.begin(), tmpVec.end());
		for (int m = 0; m < Eletype - PREELECOUNT; m++)
		{
			vecTypes.push_back(tmpVec[m]);
		}
	}

	randNum += 60 * 60;
	syssec += randNum;
	srand(syssec);
	std::random_shuffle(vecTypes.begin(), vecTypes.end());

	vector<int> seleleTypes;

	for (int i = 0; i < Eletype; i++)
	{
		seleleTypes.push_back(vecTypes[i]);
	}

	randNum += 60 * 60 * 1000;
	syssec += randNum;
	srand(syssec);

	for (unsigned int i = 0; i < vec.size() / 2; i++)
	{
		Block* a = vec[i * 2];
		Block* b = vec[i * 2 + 1];
		int r = rand() % seleleTypes.size();

		a->state(NORMALBLOCK);
		a->changeSpecialType(B_NORMAL, seleleTypes[r]);
		if (a->hasProp())
			a->removeProp();
		
		a->setVisible(true);
		a->setScale(_stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f);

		b->state(NORMALBLOCK);
		b->changeSpecialType(B_NORMAL, seleleTypes[r]);
		if (b->hasProp())
			b->removeProp();
		b->setVisible(true);
		b->setScale(_stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f);
	}
}

void GameScene::removeSelf(Ref* pSender)
{
	Node* node = (Node*)pSender;
	node->removeFromParentAndCleanup(true);
}

void GameScene::removeGray(float dt)
{
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* b = _vecBlocks[i];
		if (b->state() != DESTROID)
		{
			b->removeGray();
		}
	}
}

void GameScene::useCleanAfterSkills(Block *a, Block *b)
{
	//struct timeval now;
	//gettimeofday(&now, NULL);
	//unsigned int rand_seed = (unsigned int)(now.tv_sec * 1000 + now.tv_usec / 1000);

	int syssec = GlobalData::getSystemSecTime();
	int static randNum = 0;
	randNum += 60 * 60 * 1000;
	syssec += randNum;
	srand(syssec);

	int myCleanAddTimelv = GlobalData::getMySkillLevel(S_ADDONESECOND);

	if (myCleanAddTimelv > 0)
	{
		int r = rand() % 100;
		if (r < GlobalData::mapSkillData[S_ADDONESECOND].data[myCleanAddTimelv - 1])
		{
			_pasttime -= 1.0f;
			skillAdd1SecondCount++;
			showAdd1SAnim();
		}
	}

	int myChangeGoldlv = GlobalData::getMySkillLevel(S_CTOBGLOD);
	if (myChangeGoldlv > 0 && (a->stype() != B_COIN || b->stype() != B_COIN))
	{
		int syssec = GlobalData::getSystemSecTime();
		randNum += 60 * 60 * 1000;
		syssec += randNum;
		srand(syssec);
		int r = rand() % 100;
		if (r < GlobalData::mapSkillData[S_CTOBGLOD].data[myChangeGoldlv - 1])
		{
			isctogoldskill = true;
			Block* A = NULL;
			Block* B = NULL;
			find2RandomSameBlocks(&A, &B);
			if (A != NULL && B != NULL)
			{
				A->stype(B_COIN);
				A->setTag(0);
				A->setLocalZOrder(81 + A->getLocalZOrder());
				B->stype(B_COIN);
				B->setTag(1);
				B->setLocalZOrder(81 + B->getLocalZOrder());
				A->runAction(Sequence::create(ScaleTo::create(0.3f, 1.3f), DelayTime::create(0.3f), CallFuncN::create(CC_CALLBACK_1(GameScene::showChangeGoldBlock, this)), NULL));
				B->runAction(Sequence::create(ScaleTo::create(0.3f, 1.3f), DelayTime::create(0.3f), CallFuncN::create(CC_CALLBACK_1(GameScene::showChangeGoldBlock, this)), NULL));
			}
		}
	}

	int myAutocleanlv = GlobalData::getMySkillLevel(S_AUTOCLEAN);
	if (myAutocleanlv > 0)
	{
		if (_lastBlock != NULL)
		{
			_lastBlock->setFocus(false);
			_lastBlock = NULL;
		}

		int syssec = GlobalData::getSystemSecTime();
		randNum += 60 * 60 * 1000;
		syssec += randNum;
		srand(syssec);
		int r = rand() % 100;
		static int isAutoClean;
		isAutoClean = false;
		if (r < GlobalData::mapSkillData[S_AUTOCLEAN].data[myAutocleanlv - 1] && !isAutoClean)
		{
			isAutoClean = true;
			this->scheduleOnce(schedule_selector(GameScene::delayShowAutoCleanSkill), 0.15f);
		}
	}

}

void GameScene::exeBlockProp(Block *a, Block *b)
{
	if (a == NULL || b == NULL)
		return;

	if (a->prop() == BP_BOMB || b->prop() == BP_BOMB)
	{
		this->scheduleOnce(schedule_selector(GameScene::delayShowAutoCleanSkill), 0.15f);
	}
	else if (a->prop() == BP_X2 || b->prop() == BP_X2)
	{
		extrasocre += 20;
	}
	else if (a->prop() == BP_ADDTIME || b->prop() == BP_ADDTIME)
	{
		_pasttime -= 3.0f;
	}
}

void GameScene::addBlockProp(float dt)
{
	int index = -1;
	for (unsigned int i = 0; i < GlobalData::CPData.size(); i++)
	{
		if (GlobalData::CPData.at(i).tcount == clasiccleancount)
		{
			index = i;
			break;
		}
	}
	if (index >= 0)
	{
		if ((BlockProp)GlobalData::CPData.at(index).etype == BP_CTOGOLD)
		{
			Block* A = NULL;
			Block* B = NULL;
			find2RandomSameBlocks(&A, &B);
			if (A != NULL && B != NULL)
			{
				A->stype(B_COIN);
				A->setTag(0);
				A->setLocalZOrder(81 + A->getLocalZOrder());
				B->stype(B_COIN);
				B->setTag(1);
				B->setLocalZOrder(81 + B->getLocalZOrder());
				A->runAction(Sequence::create(ScaleTo::create(0.3f, 1.3f), DelayTime::create(0.3f), CallFuncN::create(CC_CALLBACK_1(GameScene::showChangeGoldBlock, this)), NULL));
				B->runAction(Sequence::create(ScaleTo::create(0.3f, 1.3f), DelayTime::create(0.3f), CallFuncN::create(CC_CALLBACK_1(GameScene::showChangeGoldBlock, this)), NULL));
			}
		}
		else
		{
			Block* A = NULL;
			Block* B = NULL;
			find2RandomSameBlocks(&A, &B);
			if (A != NULL && B != NULL)
			{
				int syssec = GlobalData::getSystemSecTime();
				int static randNum = 0;
				randNum += 60 * 60 * 1000;
				syssec += randNum;
				srand(syssec);
				if (rand() % 2 == 0)
				{
					A->addProp((BlockProp)GlobalData::CPData.at(index).etype);
				}
				else
				{
					B->addProp((BlockProp)GlobalData::CPData.at(index).etype);
				}
			}
		}
	}
}

void GameScene::delayShowAutoCleanSkill(float dt)
{
	for (int i = 0; i < _stage.rows * _stage.cols - 1; i++)
	{
		for (int j = i + 1; j < _stage.rows * _stage.cols; j++)
		{
			if (verify2points(_vecBlocks[i], _vecBlocks[j]))
			{
				_overblockcount += 2;
				_clickCleanBlockCount += 2;
				clasiccleancount += 2;
				_lastBlock = _vecBlocks[i];
				_curBlock = _vecBlocks[j];
				if (_lastBlock->type() >= 0 && _lastBlock->type() < ELECOUNT)
					animals[_lastBlock->type()] += 2;
				_isMovingClean = true;
				if (checkMoveable())
					_isMoving = true;

				_vecBlocks[i]->state(DESTROID);
				_vecBlocks[j]->state(DESTROID);

				boom(_vecBlocks[i]);
				boom(_vecBlocks[j]);
				extrasocre += 20;
				beginmove();
				return;

			}
		}
	}
}

void GameScene::find2RandomSameBlocks(Block** a, Block** b)
{
	vector<Block*> tempblock;
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* block = _vecBlocks[i];
		if (block->stype() == B_NORMAL && block->state() == NORMALBLOCK && block->sstate() == SS_NORMAL && !block->hasProp())
		{
			int count = 0;
			for (unsigned int j = 0; j<_vecBlocks.size(); j++)
			{
				if (_vecBlocks[j]->stype() == B_NORMAL && _vecBlocks[j]->state() == NORMALBLOCK && block->sstate() == SS_NORMAL && _vecBlocks[j]->type() == block->type() && !block->hasProp())
					count++;
			}
			if (count >= 2)
			{
				if (tempblock.size() <= 0)
					tempblock.push_back(block);
				else
				{
					int c = 0;
					for (unsigned int m = 0; m < tempblock.size(); m++)
					{
						if (block->type() != tempblock[m]->type())
							c++;
					}
					if (c == tempblock.size())
						tempblock.push_back(block);
				}
			}
		}
	}
	int static randNum = 0;
	if (tempblock.size() > 0)
	{
		//struct timeval now;
		//gettimeofday(&now, NULL);
		//unsigned int rand_seed = (unsigned int)(now.tv_sec * 1000 + now.tv_usec / 1000);

		int syssec = GlobalData::getSystemSecTime();
		randNum += 60 * 60 * 1000;
		syssec += randNum;
		srand(syssec);

		int r = rand() % tempblock.size();

		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* block = _vecBlocks[i];
			if (block->stype() == B_NORMAL && block->state() == NORMALBLOCK && block->sstate() == SS_NORMAL && block->type() == tempblock[r]->type())
			{
				if (*a == NULL)
				{
					*a = block;
				}
				*b = block;
			}
		}	
	}

}

void GameScene::findDestroyBlocks(std::vector<Block *> &vec, int &count)
{
	vector<Block*> tempblock;
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* block = _vecBlocks[i];
		if (block->state() == DESTROID && !block->isVisible())
		{
			tempblock.push_back(block);
		}
	}
	int tempsize = tempblock.size();
	
	int dsize = tempsize / 2 > count / 2 ? count / 2 : tempsize / 2;
	count = dsize*2;
	int syssec = GlobalData::getSystemSecTime();
	int static randNum = 0;
	randNum += 60 * 60 * 1000;
	syssec += randNum;
	srand(syssec);
	for (int i = 0; i < count; i++)
	{
		int r = rand() % tempblock.size();
		vec.push_back(tempblock[r]);
		tempblock.erase(tempblock.begin() + r);
	}
}

void GameScene::findRandNormalBlocks(std::vector<Block *> &vec, int count)
{
	vector<Block*> tempblock;
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* block = _vecBlocks[i];
		if (block->stype() == B_NORMAL && block->state() == NORMALBLOCK && block->sstate() == SS_NORMAL && !block->isBack())
		{
			tempblock.push_back(block);
		}
	}
	int tmpsize = tempblock.size();
	count = tmpsize>count ? count : tempblock.size();
	int syssec = GlobalData::getSystemSecTime();
	int static randNum = 0;
	randNum += 60 * 60 * 1000;
	syssec += randNum;
	srand(syssec);
	for (int i = 0; i < count; i++)
	{
		if (tempblock.size() > 0)
		{
			int r = rand() % tempblock.size();
			vec.push_back(tempblock[r]);
			tempblock.erase(tempblock.begin() + r);
		}
	}
}

void GameScene::showChangeGoldBlock(Ref* pSender)
{
	Block* block = (Block*)pSender;
	if (block->isBack())
		block->hideBack();
	block->changeSpecialType(B_COIN);
	if (block->getTag() == 1)
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BLOCKCHANGE);
	block->runAction(Sequence::create(ScaleTo::create(0.2f, _stage.cols >= TOSMALLCOLS ? 0.8f : 1.0f), CallFunc::create(CC_CALLBACK_0(GameScene::resetBlockZorder, this)), NULL));
}
void GameScene::delayShuffle(float dt)
{
	shuffleAction();
	_isMoving = false;
}

void GameScene::eraseCloudBlock(Block *a, Block *b)
{
	if (a->stype() == B_COVER)
	{
		for (unsigned int i = 0; i < _vecCloudBlocks.size(); i++)
		{
			if (_vecCloudBlocks[i]->state() == DESTROID)
				_vecCloudBlocks.erase(_vecCloudBlocks.begin() + i);
		}
	}
	if (b->stype() == B_COVER)
	{
		for (unsigned int i = 0; i < _vecCloudBlocks.size(); i++)
		{
			if (_vecCloudBlocks[i]->state() == DESTROID)
				_vecCloudBlocks.erase(_vecCloudBlocks.begin() + i);
		}
	}
}

bool GameScene::checkMoveOrder(int row, int col)
{
	Block * t = getBlock(row + 1, col);
	bool iscur = (row + 1 == _curBlock->row() && col == _curBlock->col()) ? true : false;
	//if (t != NULL && (t->stype() == B_NORMAL || t->stype() == B_COIN || t->stype() == B_BOMB) && !(t->state() == DESTROID && !iscur))
	if (t != NULL && (t->stype() != B_NOTHING) && !(t->state() == DESTROID && !iscur))
	{
		if (_mr[(row + 1) * _stage.cols + col] == M_DOWN)
		{
			if (t->row() == _curBlock->row() && t->col() == _curBlock->col())
				return false;
			return checkMoveOrder(row + 1, col);
		}
	}
	t = getBlock(row - 1, col);
	iscur = (row - 1 == _curBlock->row() && col == _curBlock->col()) ? true : false;
	if (t != NULL && (t->stype() != B_NOTHING) && !(t->state() == DESTROID && !iscur))
	{
		if (_mr[(row - 1) * _stage.cols + col] == M_UP)
		{
			if (t->row() == _curBlock->row() && t->col() == _curBlock->col())
				return false;
			return checkMoveOrder(row - 1, col);
		}
	}
	t = getBlock(row, col - 1);
	iscur = (row == _curBlock->row() && col - 1 == _curBlock->col()) ? true : false;
	if (t != NULL && (t->stype() != B_NOTHING) && !(t->state() == DESTROID && !iscur))
	{
		if (_mr[row * _stage.cols + col - 1] == M_RIGHT)
		{
			if (t->row() == _curBlock->row() && t->col() == _curBlock->col())
				return false;
			return checkMoveOrder(row, col - 1);
		}
	}
	t = getBlock(row, col + 1);
	iscur = (row == _curBlock->row() && col + 1 == _curBlock->col()) ? true : false;
	if (t != NULL && (t->stype() != B_NOTHING) && !(t->state() == DESTROID && !iscur))
	{
		if (_mr[row * _stage.cols + col + 1] == M_LEFT)
		{
			if (t->row() == _curBlock->row() && t->col() == _curBlock->col())
				return false;
			return checkMoveOrder(row, col + 1);
		}
	}

	return true;
}

bool GameScene::allMove(int row, int col)
{
	Block * b = getBlock(row, col);
	//上下左右
	Block * t = getBlock(row + 1, col);
	//if (t != NULL && (t->stype() == B_NORMAL || t->stype() == B_COIN || t->stype() == B_BOMB) && t->state() == NORMALBLOCK)
	if (t != NULL && (t->state() != DESTROID))
	{
		if (_mr[(row + 1) * _stage.cols + col] == M_DOWN)
		{
			t->row(row);
			float y = t->y();
			//t->runAction(CCMoveTo::create(0.3, Point(b->x(), b->y())));
			//t->setPosition(Point(b->x(), b->y()));
			t->y(b->y());
			b->row(row + 1);
			//b->setPositionY(y);
			b->y(y);
			sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);
			allMove(row + 1, col);
			return true;
		}
	}

	t = getBlock(row - 1, col);
	//if (t != NULL && (t->stype() == B_NORMAL || t->stype() == B_COIN || t->stype() == B_BOMB) && t->state() == NORMALBLOCK)
	if (t != NULL && (t->state() != DESTROID))
	{
		if (_mr[(row - 1) * _stage.cols + col] == M_UP)
		{
			t->row(row);
			float y = t->y();
			//t->runAction(CCMoveTo::create(0.3, Point(b->x(), b->y())));
			//t->setPosition(Point(b->x(), b->y()));
			t->y(b->y());
			b->row(row - 1);
			//b->setPositionY(y);
			b->y(y);
			sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);
			allMove(row - 1, col);
			return true;
		}
	}

	t = getBlock(row, col - 1);
	//if (t != NULL && (t->stype() == B_NORMAL || t->stype() == B_COIN || t->stype() == B_BOMB) && t->state() == NORMALBLOCK)
	if (t != NULL && (t->state() != DESTROID))
	{
		if (_mr[row * _stage.cols + col - 1] == M_RIGHT)
		{
			t->col(col);
			float x = t->x();
			//t->runAction(CCMoveTo::create(0.3, Point(b->x(), b->y())));
			//t->setPosition(Point(b->x(), b->y()));
			t->x(b->x());
			b->col(col - 1);
			//b->setPositionX(x);
			b->x(x);
			sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);
			allMove(row, col - 1);
			return true;
		}
	}

	t = getBlock(row, col + 1);
	//if (t != NULL && (t->stype() == B_NORMAL || t->stype() == B_COIN || t->stype() == B_BOMB) && t->state() == NORMALBLOCK)
	if (t != NULL && (t->state() != DESTROID))
	{
		if (_mr[row * _stage.cols + col + 1] == M_LEFT)
		{
			t->col(col);
			float x = t->x();
			//t->runAction(CCMoveTo::create(0.3, Point(b->x(), b->y())));
			//t->setPosition(Point(b->x(), b->y()));
			t->x(b->x());
			b->col(col + 1);
			//b->setPositionX(x);
			b->x(x);
			sort(_vecBlocks.begin(), _vecBlocks.end(), lesser_callback);
			allMove(row, col + 1);
			return true;
		}
	}

	return false;
}

bool GameScene::lesser_callback(Block *a, Block *b)
{
	if (a->row() * 10 + a->col() < b->row() * 10 + b->col())
		return true;
	else
		return false;
}

bool GameScene::sortNumBlocks(Block *a, Block *b)
{
	return a->stype() < b->stype() ? true : false;
}

bool GameScene::isCleanAll()
{
	int blocksize = _vecBlocks.size();
	return _overblockcount >= blocksize - _blankblockcount - _unavailableBlockCount;
}

void GameScene::shuffleAction()
{
	if (GlobalData::g_gamestatus == GAMEOVER && _pasttime <= _stageSecond)
		return;
	if (isCleanAll())
	{
		if (GlobalData::getChallengeMode() == CMBOSS)
		{
			GlobalData::g_gamestatus = GAMEOVER;
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ROLECRASH);
			int nextbossstage = GlobalData::getDefeatedBossCount() + 1;
			showBossCrashAnim();
			int maxbossstage = GlobalData::getMapCount();
			GlobalData::setDefeatedBossCount(nextbossstage);
			if (nextbossstage >= maxbossstage)
			{
				this->scheduleOnce(schedule_selector(GameScene::enterBossSettlementLayer), 1.5f);
			}
			else
			{
				this->scheduleOnce(schedule_selector(GameScene::delayNextBossStage), 1.5f);
			}
		}
		else if (GlobalData::getChallengeMode() == CMCLASIC)
		{
			GlobalData::g_gamestatus = GAMEOVER;
			if (_curChapter >= MAXCLASICCSTAGE)
				_curChapter = MAXCLASICCSTAGE - 1;
			this->scheduleOnce(schedule_selector(GameScene::resetNextClasicStage), 0.25f);
		}
		else if (GlobalData::getChallengeMode() == CMFLASH)
		{
			
		}
		else
		{
			GlobalData::g_gamestatus = GAMEOVER;

			if (isNewerGuide)
			{
				scheduleOnce(schedule_selector(GameScene::delayShowFreeGift), 0.5f);
			}
			else
			{
				delayShowSuccLayer();
			}

#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "stage%d", GlobalData::getPlayingStage() + 1);
			char p_str1[32] = { 0 };
			sprintf(p_str1, "%d", GlobalData::getStageStars());

			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				jstring para2 = methodInfo.env->NewStringUTF(p_str1);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, para2);
			}
#endif
		}
		
#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };

		const char* stagename[] = {"", "boss", "clasic", "flash"};
		int level[] = {GlobalData::getPlayingStage() + 1, _curChapter, clasicstage, flashAddtimes};
		sprintf(p_str, "%sstage%d", stagename[GlobalData::getChallengeMode()], level[GlobalData::getChallengeMode()]);
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "finishLevel", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
		return;
	}

	if (GlobalData::g_gamestatus != GAMESTART)
		return;
	if (!checkiflive())
		shuffle();
}

void GameScene::showBossCrashAnim()
{
	Spawn* spw = Spawn::create(RotateTo::create(1.2f, 360 * 3), MoveTo::create(1.2f, Vec2(540, -100)), NULL);
	Sequence* saction = Sequence::create(DelayTime::create(0.2f), spw, NULL);
	if (bossnormal != NULL)
		bossnormal->runAction(saction);
	if (bosshited != NULL)
		bosshited->runAction(saction->clone());
}
void GameScene::delayShowSuccLayer()
{
	if (isbossmode)
	{
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_ROLECRASH);
	}
	this->scheduleOnce(schedule_selector(GameScene::playWinAnim), 0.5f);

	this->scheduleOnce(schedule_selector(GameScene::delayPlayWinSound), 0.6f);
	showBossCrashAnim();
}

void GameScene::showLineAnimation()
{
	for (vector<Line*>::iterator it = _vecShowLines.begin(); it != _vecShowLines.end(); it++)
	{
		Line *line = *it;
		line->setVisible(true);
		line->StartRun();
	}
}


Block * GameScene::getBlock(int row, int col)
{
	if (row < 0 || row >= _stage.rows || col < 0 || col >= _stage.cols)
		return NULL;
	return _vecBlocks[row * _stage.cols + col];
}

Line * GameScene::getLine(int row, int col, LineType type)
{
	return _vecLines[((row + 1) * (_stage.cols + 2) + col + 1) * 4 + (LineType)type];
}

void GameScene::AddSameRowLines(int row, int col1, int col2, int sort)
{
	if (sort == 0)
		_vecShowLines.push_back(getLine(row, col1, LINE_RIGHT));
	else
		_vecShowLines.push_back(getLine(row, col2, LINE_LEFT));

	if (sort == 0)
	{
		for (int i = col1 + 1; i < col2; i++)
		{
			_vecShowLines.push_back(getLine(row, i, LINE_LEFT));
			_vecShowLines.push_back(getLine(row, i, LINE_RIGHT));
		}
	}
	else
	{
		for (int i = col2 - 1; i > col1; i--)
		{
			_vecShowLines.push_back(getLine(row, i, LINE_RIGHT));
			_vecShowLines.push_back(getLine(row, i, LINE_LEFT));
		}
	}
	if (sort == 0)
		_vecShowLines.push_back(getLine(row, col2, LINE_LEFT));
	else
		_vecShowLines.push_back(getLine(row, col1, LINE_RIGHT));
}

void GameScene::AddSameColLines(int col, int row1, int row2, int sort)
{
	if (sort == 0)
		_vecShowLines.push_back(getLine(row1, col, LINE_TOP));
	else
		_vecShowLines.push_back(getLine(row2, col, LINE_BOTTOM));

	if (sort == 0)
	{
		for (int i = row1 + 1; i < row2; i++)
		{
			_vecShowLines.push_back(getLine(i, col, LINE_BOTTOM));
			_vecShowLines.push_back(getLine(i, col, LINE_TOP));
		}
	}
	else
	{
		for (int i = row2 - 1; i > row1; i--)
		{
			_vecShowLines.push_back(getLine(i, col, LINE_TOP));
			_vecShowLines.push_back(getLine(i, col, LINE_BOTTOM));
		}
	}
	if (sort == 0)
		_vecShowLines.push_back(getLine(row2, col, LINE_BOTTOM));
	else
		_vecShowLines.push_back(getLine(row1, col, LINE_TOP));
}

bool GameScene::verify2points(Block * a, Block * b)
{
	//if (a->stype() == B_BOMB || b->stype() == B_BOMB)
	//	return true;

	if (a->type() != b->type() || a->state() == DESTROID || b->state() == DESTROID 
		|| (a->stype() != b->stype() && a->stype() != B_TURN && b->stype() != B_TURN && a->stype() != B_COVER && b->stype() != B_COVER)
		|| (a->stype() == B_UNAVAILABLE || b->stype() == B_UNAVAILABLE)
		|| (a->stype() == B_RHOMBICBARRIER || b->stype() == B_RHOMBICBARRIER)
		|| (a->stype() == B_HEXAGONBARRIER || b->stype() == B_HEXAGONBARRIER)
		|| (a->state() == DISABLE || b->state() == DISABLE)
		|| (a->sstate() != SS_NORMAL || b->sstate() != SS_NORMAL)
		)
		return false;


	int i;
	bool ret;

	_vecShowLines.clear();

	if (a->row() == b->row())
	{
		if (verifysamerow(a->row(), a->col(), b->col(), false))
		{
			if (a->row() == 0 || a->row() == _stage.rows - 1)
			{
				if (verifyoutlinesamerow(a->row(), a->col(), b->col()))
				{
					AddSameRowLines(a->row(), a->col(), b->col() , 0);
				}
				else
				{
					if (a->row() == 0)
					{
						_vecShowLines.push_back(getLine(a->row(), a->col(), LINE_BOTTOM));
						_vecShowLines.push_back(getLine(a->row() - 1, a->col(), LINE_TOP));
						AddSameRowLines(a->row() - 1, a->col(), b->col() , 0);
						_vecShowLines.push_back(getLine(a->row() - 1, b->col(), LINE_TOP));
						_vecShowLines.push_back(getLine(a->row(), b->col(), LINE_BOTTOM));
					}
					else
					{
						_vecShowLines.push_back(getLine(a->row(), a->col(), LINE_TOP));
						_vecShowLines.push_back(getLine(a->row() + 1, a->col(), LINE_BOTTOM));
						AddSameRowLines(a->row() + 1, a->col(), b->col() , 0);
						_vecShowLines.push_back(getLine(a->row() + 1, b->col(), LINE_BOTTOM));
						_vecShowLines.push_back(getLine(a->row(), b->col(), LINE_TOP));
					}
				}
			}
			else
				AddSameRowLines(a->row(), a->col(), b->col() , 0);
			return true;
		}
	}

	if (a->col() == b->col())
	{
		if (verifysamecol(a->col(), a->row(), b->row(), false))
		{
			if (a->col() == 0 || a->col() == _stage.cols - 1)
			{
				if (verifyoutlinesamecol(a->col(), a->row(), b->row()))
				{
					AddSameColLines(a->col(), a->row(), b->row() , 0);
				}
				else
				{
					if (a->col() == 0)
					{
						_vecShowLines.push_back(getLine(a->row(), a->col(), LINE_LEFT));
						_vecShowLines.push_back(getLine(a->row(), a->col() - 1, LINE_RIGHT));
						AddSameColLines(a->col() - 1, a->row(), b->row(), 0);
						_vecShowLines.push_back(getLine(b->row(), a->col() - 1, LINE_RIGHT));
						_vecShowLines.push_back(getLine(b->row(), a->col(), LINE_LEFT));
					}
					else
					{
						_vecShowLines.push_back(getLine(a->row(), a->col(), LINE_RIGHT));
						_vecShowLines.push_back(getLine(a->row(), a->col() + 1, LINE_LEFT));
						AddSameColLines(a->col() + 1, a->row(), b->row(), 0);
						_vecShowLines.push_back(getLine(b->row(), a->col() + 1, LINE_LEFT));
						_vecShowLines.push_back(getLine(b->row(), a->col(), LINE_RIGHT));
					}
				}
			}
			else
				AddSameColLines(a->col(), a->row(), b->row(), 0);
			return true;
		}
	}

	if (a->row() != b->row() && a->col() != b->col())
	{
		if (checkoneturn(a, b))
			return true;
	}

	_vecShowLines.clear();
	vector<Line *> vecTemp;
	//向北
	for (i = 0; i < _stage.rows - a->row() - 1; i++)
	{
		Block * block = getBlock(a->row() + i + 1, a->col());
		if (block->state() != DESTROID)
			break;
		vecTemp.push_back(getLine(a->row() + i, a->col(), LINE_TOP));
		vecTemp.push_back(getLine(a->row() + i + 1, a->col(), LINE_BOTTOM));
		if (block->row() != b->row())
		{
			if (block->row() < b->row())
				ret = checkoneturn(block, b);
			else
				ret = checkoneturn(b, block, true);
		}
		else
			ret = false;
		if (ret)
		{
			_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
			return true;
		}
		_vecShowLines.clear();
	}

	//向北走外围
	if (!_stage.blockup)
	{
		_vecShowLines.clear();
		vecTemp.clear();
		for (i = 0; i < _stage.rows - a->row(); i++)
		{
			Block * block = NULL;
			if (a->row() + i + 1 < _stage.rows)
			{
				block = getBlock(a->row() + i + 1, a->col());
				if (block->state() != DESTROID)
					break;
			}
			vecTemp.push_back(getLine(a->row() + i, a->col(), LINE_TOP));
			vecTemp.push_back(getLine(a->row() + i + 1, a->col(), LINE_BOTTOM));
			bool ischange = false;
			if (block != NULL)
			{
				if (block->row() != b->row())
				{
					if (block->row() < b->row())
						ret = checkoneturn(block, b);
					else
					{
						ischange = true;
						ret = checkoneturn(b, block, ischange);
					}
				}
				else
					ret = false;
				if (ret)
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					return true;
				}
			}
			else
			{
				_vecShowLines.clear();
				ret = true;
				for (int i = b->row() + 1; i < _stage.rows; i++)
				{
					if (_vecBlocks[b->col() + i * _stage.cols]->state() != DESTROID)
					{
						ret = false;
						break;
					}
				}
				if (ret)
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					if (a->col() < b->col())
						AddSameRowLines(_stage.rows, a->col(), b->col(), 0);
					else
					{
						if (ischange)
							AddSameRowLines(_stage.rows, b->col(), a->col(), 0);
						else
							AddSameRowLines(_stage.rows, b->col(), a->col(), 1);
						
					}
					AddSameColLines(b->col(), b->row(), _stage.rows, 1);
					return true;
				}
			}
			_vecShowLines.clear();
		}
	}

	//向南
	_vecShowLines.clear();
	vecTemp.clear();
	for (i = 0; i < a->row(); i++)
	{
		Block * block = getBlock(a->row() - i - 1, a->col());
		if (block->state() != DESTROID)
			break;
		vecTemp.push_back(getLine(a->row() - i, a->col(), LINE_BOTTOM));
		vecTemp.push_back(getLine(a->row() - i - 1, a->col(), LINE_TOP));
		if (checkoneturn(block, b))
		{
			_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
			return true;
		}
		_vecShowLines.clear();
	}

		//向南走外围
	if (!_stage.blockdown)
	{
		_vecShowLines.clear();
		vecTemp.clear();
		for (i = 0; i < a->row() + 1; i++)
		{
			Block * block = NULL;
			if (a->row() - i - 1 >= 0)
			{
				block = getBlock(a->row() - i - 1, a->col());
				if (block->state() != DESTROID)
					break;
			}
			vecTemp.push_back(getLine(a->row() - i, a->col(), LINE_BOTTOM));
			vecTemp.push_back(getLine(a->row() - i - 1, a->col(), LINE_TOP));
			if (block != NULL)
			{
				if (checkoneturn(block, b))
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					return true;
				}
			}
			else
			{
				_vecShowLines.clear();
				ret = true;
				for (int i = b->row() - 1; i >= 0; i--)
				{
					if (_vecBlocks[b->col() + i * _stage.cols]->state() != DESTROID)
					{
						ret = false;
						break;
					}
				}
				if (ret)
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					if (a->col() < b->col())
						AddSameRowLines(-1, a->col(), b->col(), 0);
					else
						AddSameRowLines(-1, b->col(), a->col(), 1);
					AddSameColLines(b->col(), -1, b->row(), 1);
					return true;
				}
			}
			_vecShowLines.clear();
		}
	}


	//向西
	_vecShowLines.clear();
	vecTemp.clear();
	for (i = 0; i < a->col(); i++)
	{
		Block * block = getBlock(a->row(), a->col() - i - 1);
		if (block->state() != DESTROID)
			break;
		vecTemp.push_back(getLine(a->row(), a->col() - i, LINE_LEFT));
		vecTemp.push_back(getLine(a->row(), a->col() - i - 1, LINE_RIGHT));
		if (checkoneturn(block, b))
		{
			_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
			return true;
		}
		_vecShowLines.clear();
	}

	//向西走外围
	if (!_stage.blockleft)
	{
		_vecShowLines.clear();
		vecTemp.clear();
		for (i = 0; i < a->col() + 1; i++)
		{
			Block * block = NULL;
			if (a->col() - i - 1 >= 0)
			{
				block = getBlock(a->row(), a->col() - i - 1);
				if (block->state() != DESTROID)
					break;
			}
			vecTemp.push_back(getLine(a->row(), a->col() - i, LINE_LEFT));
			vecTemp.push_back(getLine(a->row(), a->col() - i - 1, LINE_RIGHT));
			if (block != NULL)
			{
				if (checkoneturn(block, b))
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					return true;
				}
			}
			else
			{
				_vecShowLines.clear();
				ret = true;
				for (int i = b->col() - 1; i >= 0; i--)
				{
					if (_vecBlocks[i + b->row() * _stage.cols]->state() != DESTROID)
					{
						ret = false;
						break;
					}
				}
				if (ret)
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					AddSameColLines(-1, a->row(), b->row(), 0);
					AddSameRowLines(b->row(), -1, b->col(), 0);
					return true;
				}
			}
			_vecShowLines.clear();
		}
	}

	//向东
	_vecShowLines.clear();
	vecTemp.clear();
	for (i = 0; i < _stage.cols - a->col() - 1; i++)
	{
		Block * block = getBlock(a->row(), a->col() + i + 1);
		if (block->state() != DESTROID)
			break;
		vecTemp.push_back(getLine(a->row(), a->col() + i, LINE_RIGHT));
		vecTemp.push_back(getLine(a->row(), a->col() + i + 1, LINE_LEFT));
		//if (block->row() < b->row() || (block->row() == b->row() && block->col() < b->col()))
		if (block->col() != b->col())
		{
			if (block->row() < b->row())
				ret = checkoneturn(block, b);
			else
				ret = checkoneturn(b, block);
		}
		else
			ret = false;
		if (ret)
		{
			_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
			return true;
		}
		_vecShowLines.clear();
	}

	//向东走外围
	if (!_stage.blockright)
	{
		_vecShowLines.clear();
		vecTemp.clear();
		for (i = 0; i < _stage.cols - a->col(); i++)
		{
			Block * block = NULL;
			if (a->col() + i + 1 < _stage.cols)
			{
				block = getBlock(a->row(), a->col() + i + 1);
				if (block->state() != DESTROID)
					break;
			}
			vecTemp.push_back(getLine(a->row(), a->col() + i, LINE_RIGHT));
			vecTemp.push_back(getLine(a->row(), a->col() + i + 1, LINE_LEFT));
			if (block != NULL)
			{
				//if (block->row() < b->row() || (block->row() == b->row() && block->col() < b->col()))
				if (block->col() != b->col())
				{
					if (block->row() < b->row())
						ret = checkoneturn(block, b);
					else
						ret = checkoneturn(b, block);
				}
				else
					ret = false;
				if (ret)
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					return true;
				}
			}
			else
			{
				_vecShowLines.clear();
				ret = true;
				for (int i = b->col() + 1; i < _stage.cols; i++)
				{
					if (_vecBlocks[i + b->row() * _stage.cols]->state() != DESTROID)
					{
						ret = false;
						break;
					}
				}
				if (ret)
				{
					_vecShowLines.insert(_vecShowLines.begin(), vecTemp.begin(), vecTemp.end());
					AddSameColLines(_stage.cols, a->row(), b->row(), 0);
					AddSameRowLines(b->row(), b->col(), _stage.cols, 1);
					return true;
				}
			}
			_vecShowLines.clear();
		}
	}
	return false;
}

bool GameScene::checkiflive()
{
	if (isCleanAll() || _isAddingBlocks)
		return true;

	for (int i = 0; i < _stage.rows * _stage.cols - 1; i++)
	{
		for (int j = i + 1; j < _stage.rows * _stage.cols; j++)
		{
			if (_vecBlocks[i]->stype() == B_BOMB && _vecBlocks[i]->state() == NORMALBLOCK)
				return true;
			if (verify2points(_vecBlocks[i], _vecBlocks[j]))
				return true;
		}
	}
	
	return false;
}

bool GameScene::checkoneturn(Block * a, Block * b, bool ischange)
{
	//Block * block;
	if (a->col() < b->col())
	{
		if (getBlock(a->row(), b->col())->state() == DESTROID)
		{
			if (verifysamerow(a->row(), a->col(), b->col(), true))
			{
				AddSameRowLines(a->row(), a->col(), b->col() , 0);
				if (verifysamecol(b->col(), a->row(), b->row(), true))
				{
					AddSameColLines(b->col(), a->row(), b->row() , 0);
					return true;
				}
			}
		}
		_vecShowLines.clear();
		if (getBlock(b->row(), a->col())->state() == DESTROID)
		{
			if (verifysamecol(a->col(), a->row(), b->row(), true))
			{
				AddSameColLines(a->col(), a->row(), b->row() , 0);
				if (verifysamerow(b->row(), a->col(), b->col(), true))
				{
					AddSameRowLines(b->row(), a->col(), b->col() , 0);
					return true;
				}
			}
		}
	}
	else if (a->col() > b->col())
	{
		if (getBlock(a->row(), b->col())->state() == DESTROID)
		{
			if (verifysamerow(a->row(), b->col(), a->col(), true))
			{
				AddSameRowLines(a->row(), b->col(), a->col() , 1);
				if (verifysamecol(b->col(), a->row(), b->row(), true))
				{
					AddSameColLines(b->col(), a->row(), b->row() , 0);
					return true;
				}
			}
		}
		_vecShowLines.clear();
		if (getBlock(b->row(), a->col())->state() == DESTROID)
		{
			if (ischange)
			{
				if (verifysamerow(b->row(), b->col(), a->col(), true))
				{
					AddSameRowLines(b->row(), b->col(), a->col(), 0);
					if (verifysamecol(a->col(), a->row(), b->row(), true))
					{
						AddSameColLines(a->col(), a->row(), b->row(), 1);
						return true;
					}
				}
			}
			else
			{
				if (verifysamecol(a->col(), a->row(), b->row(), true))
				{
					AddSameColLines(a->col(), a->row(), b->row(), 0);
					if (verifysamerow(b->row(), b->col(), a->col(), true))
					{
						AddSameRowLines(b->row(), b->col(), a->col(), 1);
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool GameScene::verifysamerow(int row, int col1, int col2, bool strict)
{
	int step = col2 - col1 - 1;
	int start = row * _stage.cols + col1 + 1;
	bool ret = true;

	if (!strict)
	{
		if ((row == 0 && !_stage.blockdown) || (row == _stage.rows - 1 && !_stage.blockup))
		{
			return ret;
		}
	}
	for (int i = 0; i < step; i++)
	{
		if (_vecBlocks[start + i]->state() != DESTROID)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

bool GameScene::verifysamecol(int col, int row1, int row2, bool strict)
{
	int step = row2 - row1 - 1;
	int start = (row1 + 1) * _stage.cols + col;
	bool ret = true;

	if (!strict)
	{
		if ((col == 0 && !_stage.blockleft) || (col == _stage.cols - 1 && !_stage.blockright))
		{
			return ret;
		}
	}
	for (int i = 0; i < step; i++)
	{
		if (_vecBlocks[start + i * _stage.cols]->state() != DESTROID)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

//中间有阻隔则返回false
bool GameScene::verifyoutlinesamerow(int row, int col1, int col2)
{
	int step = col2 - col1 - 1;
	int start = row * _stage.cols + col1 + 1;
	bool ret = true;

	for (int i = 0; i < step; i++)
	{
		if (_vecBlocks[start + i]->state() != DESTROID)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

bool GameScene::verifyoutlinesamecol(int col, int row1, int row2)
{
	int step = row2 - row1 - 1;
	int start = (row1 + 1) * _stage.cols + col;
	bool ret = true;

	for (int i = 0; i < step; i++)
	{
		if (_vecBlocks[start + i * _stage.cols]->state() != DESTROID)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

void GameScene::parseXML(string fileName)
{
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	string filedata = FileUtils::getInstance()->getStringFromFile(fileName);

	int err = pDoc->Parse(filedata.c_str());
	if (err != 0)
	{
		delete pDoc;
		return;
	}

	string fn = fileName;
	int l = fn.find('-');
	string tmp = fn.substr(5, l-5);
	int chapter = atoi(tmp.c_str());
	tmp = fn.substr(l+1, 1);
	int stagen = atoi(tmp.c_str());
	tmp = fn.substr(l+3, 1);
	int extstage = atoi(tmp.c_str());
	XMLElement *rootEle = pDoc->RootElement();
	int total = rootEle->IntAttribute("total");
	XMLElement *element = rootEle->FirstChildElement();
	while (element != NULL)
	{
		if (strcmp(element->Name(), "s") == 0)
		{
			StageInfo stage;
			stage.chapter = chapter;
			stage.stage = stagen;
			stage.extstage = extstage;
			stage.snum = total;	//element->IntAttribute("n");
			stage.rows = element->IntAttribute("r");
			stage.cols = element->IntAttribute("l");
			stage.elements_num = element->IntAttribute("en");
			stage.time = element->IntAttribute("t");
			stage.time1 = element->IntAttribute("t1");
			stage.time2 = element->IntAttribute("t2");
			stage.time3 = element->IntAttribute("t3");
			string b = element->Attribute("b");

			stage.blockup = (convert16toint(b.at(0)) & 8) == 8 ? true : false;
			stage.blockleft = (convert16toint(b.at(0)) & 4) == 4 ? true : false;
			stage.blockdown = (convert16toint(b.at(0)) & 2) == 2 ? true : false;
			stage.blockright = (convert16toint(b.at(0)) & 1) == 1 ? true : false;
			stage.es = element->Attribute("e");
			_stage = stage;

		}
		element = element->NextSiblingElement();
	}
	delete pDoc;
}


void GameScene::playWinAnim(float dt)
{
	heroAminNode->stopAllActions();
	heroAminNode->setRotationSkewY(0);
	heroAminNode->setPosition(Vec2(270, 370));
	heroAminNode->setScale(0.8f);
	heroAction = CSLoader::createTimeline(heroAminNode->getName()); //加载动画：
	heroAminNode->runAction(heroAction);
	heroAction->play("win", true);
	this->scheduleOnce(schedule_selector(GameScene::enterSuccLayer), 1.5f);
}

void GameScene::enterSuccLayer(float dt)
{
	rettype = 0;
	heroAminNode->stopAllActions();
	heroAminNode->setVisible(false);
	_gameStateTextBox->setVisible(false);
	int stars = 0;
	for (int i = 0; i < 3; i++)
	{
		if (_starlv[i]->isVisible())
			stars++;
	}

	GlobalData::setStageStars(stars);
	SuccLayer* pLayer = SuccLayer::create();
	//pLayer->LoadingCSB(WIN, 0, 0);
	this->addChild(pLayer, 1000);
}

void GameScene::enterBossSettlementLayer(float dt)
{
	rettype = 0;
	_gameStateTextBox->setVisible(false);

	GlobalData::bossstasge++;
	if (GlobalData::bossstasge > 5)
		GlobalData::bossstasge = 5;

	BossSettlement* pLayer = BossSettlement::create();
	//pLayer->LoadingCSB(WIN, 0, 0);
	addChild(pLayer, 1000);
}

void GameScene::showReadyAnim()
{
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_READYGO);

	Sprite * ready = Sprite::createWithSpriteFrameName("ready.png");
	ready->setScale(0.01f);
	ready->setPosition(Point(SCREEN_WIDTH / 2, _go->getPositionY()));
	addChild(ready, 200);
	Spawn* spawn = Spawn::create(FadeIn::create(0.8f), ScaleTo::create(0.5f, 1.0f), NULL);
	ready->runAction(Sequence::create(spawn, CallFuncN::create(CC_CALLBACK_1(GameScene::removeReadySprite, this)), NULL));
	_go->setScale(0.01f);
	_go->setVisible(true);
	_go->runAction(Sequence::create(DelayTime::create(0.8f), spawn, Hide::create(), CallFunc::create(CC_CALLBACK_0(GameScene::GameStart, this)), NULL));
	this->scheduleOnce(schedule_selector(GameScene::showBlockGuide), 1.8f);
}

void GameScene::show321Anim()
{

	Sprite* ready[3];
	for (int i = 0; i < 3; i++)
	{
		string readystr = StringUtils::format("ready%d.png", 3-i);
		ready[i] = Sprite::createWithSpriteFrameName(readystr);
		ready[i]->setOpacity(0);
		ready[i]->setPosition(Point(SCREEN_WIDTH / 2, _go->getPositionY()));
		addChild(ready[i], 200);
		ready[i]->runAction(Sequence::create(DelayTime::create(i), FadeIn::create(0.6f), DelayTime::create(0.4f), CallFuncN::create(CC_CALLBACK_1(GameScene::removeReadySprite, this)), NULL));
	}
	Spawn* spawn = Spawn::create(FadeIn::create(0.8f), ScaleTo::create(0.6f, 1.0f), NULL);
	_go->setScale(0.1f);
	_go->runAction(Sequence::create(DelayTime::create(3.0f),Show::create(), spawn, Hide::create(), CallFunc::create(CC_CALLBACK_0(GameScene::GameStart, this)), NULL));

}

void GameScene::removeReadySprite(Node* node)
{
	node->setVisible(false);
	node->removeFromParentAndCleanup(true);
}

void GameScene::GameStart()
{
	if (GlobalData::getIsUseBomb())
	{
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_CLEAR);
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->stype() == B_BOMB)
			{
				_overblockcount++;
				isClickBomb = true;
				b->state(DESTROID);
				boom(b);
				isClickBomb = false;
			}
		}
	}
	GlobalData::g_gamestatus = GAMESTART;
#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	const char* stagename[] = {"", "boss", "clasic", "flash"};
	int level[] = {GlobalData::getPlayingStage() + 1, _curChapter, clasicstage, flashAddtimes};
	sprintf(p_str, "%sstage%d", stagename[GlobalData::getChallengeMode()], level[GlobalData::getChallengeMode()]);

	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "startLevel", "(Ljava/lang/String;)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
	}
#endif
}

void GameScene::showComboHitAnim()
{
	if (_keepingcount >= 2)
	{
		if (_combosprite != NULL)
		{
			_combosprite->removeFromParentAndCleanup(true);
			_combosprite = NULL;
		}

		_combosprite = Sprite::createWithSpriteFrameName("combo.png");
		_combosprite->setPosition(Vec2(270, 700));
		addChild(_combosprite, 200);
		_combolable = Label::createWithBMFont("fonts/combonum.fnt", "0123456789");
		_combolable->setAnchorPoint(Vec2(0, 0.5));
		_combolable->setPosition(Vec2(230, 40));
		_combosprite->addChild(_combolable);
		string str = StringUtils::format(" %d", _keepingcount);
		_combolable->setString(str);
		_combosprite->setScale(1.2f);
		_combosprite->runAction(Sequence::create(EaseSineIn::create(ScaleTo::create(0.1f, 0.8f)), EaseSineIn::create(ScaleTo::create(0.1f, 1.0f)), DelayTime::create(0.5f), Hide::create(), NULL));
		if (_keepingcount == 2)
			_comboscore += 40;
		else if (_keepingcount == 3)
			_comboscore += 50;
		else if (_keepingcount == 4)
			_comboscore += 60;
		else if (_keepingcount == 5)
			_comboscore += 70;
		else if (_keepingcount == 6)
			_comboscore += 80;
		else
			_comboscore += 90;

		int index = _keepingcount > 6?6:_keepingcount;

		if (_keepingcount <= 6)
		{
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_COMBO2 + index - 2);
			if (!isbossmode)
			{
				if (_heroComboWord != NULL)
				{
					_heroComboWord->removeFromParentAndCleanup(true);
					_heroComboWord = NULL;
				}
				string combowordstr = StringUtils::format("combo%d.png", index);
				_heroComboWord = Sprite::createWithSpriteFrameName(combowordstr);
				_heroComboWord->setAnchorPoint(Vec2(0, 0.5));
				_heroComboWord->setPosition(Vec2(315, 830));
				_heroComboWord->setScale(0.1f);
				_heroComboWord->runAction(Sequence::create(EaseSineIn::create(ScaleTo::create(0.1f, 0.8f)), EaseSineIn::create(ScaleTo::create(0.1f, 1.2f)), EaseSineIn::create(ScaleTo::create(0.1f, 1.0f)), DelayTime::create(0.5f), Hide::create(), NULL));
				addChild(_heroComboWord, 200);
			}
		}
	}
	else
	{
		_comboscore += 20;
	}
}

void GameScene::boom(Block* block)
{

	if (block->stype() == B_TURN)
		block->setFocus(true);

	if (block->sstate() == SS_GOLDLOCK || block->sstate() == SS_GOLDLOCKFROZEN)
	{
		MyParticle::Show(this, SpeciallyName[15], block->getPositionX(), block->getPositionY(), false, 0.3f);
		MyParticle::Show(this, SpeciallyName[16], block->getPositionX(), block->getPositionY(), true, 0.3f);
		return;
	}
	else if (block->sstate() == SS_SILVERLOCK || block->sstate() == SS_SILVERLOCKFROZEN)
	{
		MyParticle::Show(this, SpeciallyName[17], block->getPositionX(), block->getPositionY(), false, 0.3f);
		MyParticle::Show(this, SpeciallyName[18], block->getPositionX(), block->getPositionY(), true, 0.3f);
		return;
	}

	if (block->stype() == B_RHOMBICBARRIER || block->stype() == B_HEXAGONBARRIER)
	{
		MyParticle::Show(this, SpeciallyName[12], block->getPositionX(), block->getPositionY(), false, 0.3f);
		MyParticle::Show(this, SpeciallyName[13], block->getPositionX(), block->getPositionY(), true, 0.3f);
		MyParticle::Show(this, SpeciallyName[14], block->getPositionX(), block->getPositionY(), false, 0.3f);
	}
	else
	{
		if (isClickBomb)
		{
			MyParticle::Show(this, SpeciallyName[10], block->getPositionX(), block->getPositionY(), true, 0.3f);
			MyParticle::Show(this, SpeciallyName[11], block->getPositionX(), block->getPositionY(), true, 0.3f);
		}
		else
		{
			if (_lastBlock != NULL && _curBlock != NULL && (_curBlock->stype() != B_BOMB || _lastBlock->stype() != B_BOMB) )
			{
				Block* a = NULL;
				Block* b = NULL;
				if (_lastBlock->row() * _stage.cols + _lastBlock->col() > _curBlock->row() * _stage.cols + _curBlock->col())
				{
					a = _curBlock;
					b = _lastBlock;
				}
				else
				{
					b = _curBlock;
					a = _lastBlock;
				}
				int arraysize = _vecShowLines.size() / 2;

				if (boomlight1 == NULL)
				{
					boomlight1 = Sprite::createWithSpriteFrameName("boomlight.png");
					boomlight1->setPosition(a->getPosition());
					addChild(boomlight1, 200);
				}
				else
				{
					boomlight1->setPosition(a->getPosition());
				}
				MyParticle::Show(boomlight1, SpeciallyName[23], boomlight1->getContentSize().width / 2, boomlight1->getContentSize().height / 2, true, 1.0f);

				Vector<cocos2d::FiniteTimeAction*> actionmove1;
				for (int i = 0; i < arraysize; i++)
				{
					float dt = 0.15f / arraysize;
					MoveTo* move = MoveTo::create(dt, Vec2(_vecShowLines[i]->x(), _vecShowLines[i]->y()));
					actionmove1.pushBack(move);
				}
				Hide* hide1 = Hide::create();
				actionmove1.pushBack(hide1);
				boomlight1->stopAllActions();
				boomlight1->setVisible(true);
				boomlight1->runAction(Sequence::create(actionmove1));
				if (boomlight2 == NULL)
				{
					boomlight2 = Sprite::createWithSpriteFrameName("boomlight.png");
					boomlight2->setPosition(b->getPosition());
					addChild(boomlight2, 200);
				}

				else
				{
					boomlight2->setPosition(b->getPosition());
				}

				MyParticle::Show(boomlight2, SpeciallyName[23], boomlight2->getContentSize().width / 2, boomlight2->getContentSize().height / 2, true, 1.0f);

				Vector<cocos2d::FiniteTimeAction*> actionmove2;
				for (int i = _vecShowLines.size() - 1; i >= arraysize; i--)
				{
					float dt = 0.15f / arraysize;
					MoveTo* move = MoveTo::create(dt, Vec2(_vecShowLines[i]->x(), _vecShowLines[i]->y()));
					actionmove2.pushBack(move);
				}

				Hide* hide2 = Hide::create();
				actionmove2.pushBack(hide2);
				boomlight2->stopAllActions();
				boomlight2->setVisible(true);
				boomlight2->runAction(Sequence::create(actionmove2));
				boomx = _vecShowLines[arraysize]->x();
				boomy = _vecShowLines[arraysize]->y();
				if (!Director::getInstance()->getScheduler()->isScheduled(schedule_selector(GameScene::delayShowCleanBoomAnim), this))
					this->scheduleOnce(schedule_selector(GameScene::delayShowCleanBoomAnim), 0.3f);
			}
		}
	}
	//block->runAction(Sequence::create(FadeOut::create(0.15f), Hide::create(), NULL));

	block->runAction(Sequence::create(ScaleTo::create(0.05f, 0.0f),FadeOut::create(0.05f), Hide::create(), NULL));
}

void GameScene::playHitBossAnim()
{
	if (isbossmode)
	{
		heroAminNode->stopAllActions();
		heroAction = CSLoader::createTimeline(heroAminNode->getName()); //加载动画：
		heroAminNode->runAction(heroAction);
		heroAction->play("attack", false);
		this->scheduleOnce(schedule_selector(GameScene::delayShowBossHited), 0.5f);
	}
}

void GameScene::delayShowBossHited(float dt)
{
	delayShowHeroAnim(0);

	bossnormal->setVisible(false);
	bosshitedAnim = CSLoader::createTimeline(bossHitedLayer[_curChapter - 1]->getName());
	bosshited->setVisible(true);
	bosshited->runAction(bosshitedAnim);
	bosshitedAnim->gotoFrameAndPlay(0, false);
	this->scheduleOnce(schedule_selector(GameScene::delayShowBossAnim), 0.8f);

	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_HITBOSS);
}

void GameScene::delayShowCleanBoomAnim(float dt)
{
	//MyParticle::Show(this, SpeciallyName[24], boomx, boomy, true, 0.3f, 200);
	//MyParticle::Show(this, SpeciallyName[25], boomx, boomy, true, 0.3f, 50);
	//MyParticle::Show(this, SpeciallyName[26], boomx, boomy, true, 0.3f, 30);

	MyParticle::Show(this, SpeciallyName[37], boomx, boomy, true, 0.08f)->setScale(0.5f);
	MyParticle::Show(this, SpeciallyName[38], boomx, boomy, true, 0.1f)->setScale(0.5f);
	MyParticle::Show(this, SpeciallyName[39], boomx, boomy, true, 0.08f)->setScale(0.5f);
	MyParticle::Show(this, SpeciallyName[40], boomx, boomy, true, 0.1f)->setScale(0.5f);
	MyParticle::Show(this, SpeciallyName[41], boomx, boomy, true, 0.1f)->setScale(0.5f);
}

void GameScene::gamePause()
{
	GlobalData::g_gamestatus = GAMEPAUSE;
	_go->pause();
	//showBlockBack();

}
void GameScene::gameResume()
{
	gamePlayingShowShop = false;
	_go->resume();

	GlobalData::g_gamestatus = GAMESTART;
}

void GameScene::clickBombProp()
{
	Block* A = NULL;
	Block* B = NULL;
	find2RandomSameBlocks(&A, &B);
	if (A != NULL && B != NULL)
	{
		isClickBomb = true;
		_overblockcount += 2;
		_clickCleanBlockCount += 2;
		clasiccleancount += 2;
		_lastBlock = A;
		_curBlock = B;

		if (_lastBlock->type() >= 0 && _lastBlock->type() < ELECOUNT)
			animals[_lastBlock->type()] += 2;

		A->state(DESTROID);
		boom(A);
		B->state(DESTROID);
		boom(B);
		isClickBomb = false;
		extrasocre += 20;
	}
}
void GameScene::showBlockBack()
{
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* b = _vecBlocks[i];
		if (b->state() != DESTROID || b->stype() != B_NOTHING)
		{
			b->showBack();
		}
	}
}

void GameScene::hideBlockBack()
{
	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* b = _vecBlocks[i];
		if (b->state() != DESTROID || b->stype() != B_NOTHING)
		{
			b->hideBack();
		}
	}
}

void GameScene::gameRevive()
{
	GlobalData::g_gamestatus = GAMERESTART;
	if (isbossmode)
	{
		SoundManager::sharedManager()->playBackMusic("sound/bgboss.mp3");
	}
	else
	{
		SoundManager::sharedManager()->playBackMusic("sound/bg2.mp3");
	}

}

void GameScene::saveData()
{

}

void GameScene::clearData()
{

}

void GameScene::showPropPrice(float dt)
{
	if (GlobalData::getPromptProp() > 0)
	{
		promptPrice->setVisible(false);
		_promptcountLabel->setVisible(true);
	}
	else
	{
		promptPrice->setVisible(true);
		_promptcountLabel->setVisible(false);
	}

	if (GlobalData::getRefreshProp() > 0)
	{
		refreshPrice->setVisible(false);
		_refreshcountLabel->setVisible(true);
	}
	else
	{
		refreshPrice->setVisible(true);
		_refreshcountLabel->setVisible(false);
	}

	if (GlobalData::getAddtimeProp() > 0)
	{
		clockPrice->setVisible(false);
		_addtimecountLabel->setVisible(true);
	}
	else
	{
		clockPrice->setVisible(true);
		_addtimecountLabel->setVisible(false);
	}
}

void GameScene::NewerGuideClickCB(int step)
{
	vector<Node*> nodes;
	if (step == 2 && GlobalData::checkGuide(2))
	{
		nodes.push_back(_vecBlocks[6]);
		nodes.push_back(_vecBlocks[9]);
		g_NewerLayer = NewerGuideLayer::create(2);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
	else if (step == 3 && GlobalData::checkGuide(3))
	{
		nodes.push_back(_vecBlocks[15]);
		nodes.push_back(_vecBlocks[14]);
		g_NewerLayer = NewerGuideLayer::create(3);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
	else if (step == 4 && GlobalData::checkGuide(4))
	{
		g_NewerLayer = NewerGuideLayer::create(4);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
	else if (step == 5 && GlobalData::checkGuide(5))
	{
		nodes.push_back(timebarbg);
		nodes.push_back(progressTimer);
		nodes.push_back(_leftCountLabel);
		g_NewerLayer = NewerGuideLayer::create(5);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
	else if (step == 14 && GlobalData::checkGuide(14))
	{
		scheduleOnce(schedule_selector(GameScene::delayShowPromptGuide), 0.1f);
	}
	else if (step == 15 && GlobalData::checkGuide(15))
	{
		scheduleOnce(schedule_selector(GameScene::delayShowRefreshGuide), 0.1f);
	}
	else if (step == 16 && GlobalData::checkGuide(16))
	{
		scheduleOnce(schedule_selector(GameScene::delayShowAddTimeGuide), 0.1f);
	}
}

void GameScene::delayShowPromptGuide(float dt)
{
	if (g_NewerLayer == NULL)
	{
		vector<Node*> nodes;
		nodes.push_back(_promptBtn);
		g_NewerLayer = NewerGuideLayer::create(14);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
}

void GameScene::delayShowRefreshGuide(float dt)
{
	if (g_NewerLayer == NULL)
	{
		vector<Node*> nodes;
		nodes.push_back(_refreshBtn);
		g_NewerLayer = NewerGuideLayer::create(15);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
}
void GameScene::delayShowAddTimeGuide(float dt)
{
	if (g_NewerLayer == NULL)
	{
		vector<Node*> nodes;
		nodes.push_back(_clockBtn);
		g_NewerLayer = NewerGuideLayer::create(16);
		rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
}

void GameScene::delayPlayWinSound(float dt)
{
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_WIN);
}

void GameScene::delayPlayLossSound(float dt)
{
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_LOSS);
	
}

void GameScene::showBlockGuide(float dt)
{
	vector<Node*> nodes;
	int guide = 0;

	if (GlobalData::checkGuide(46))
	{
		if ((GlobalData::getPlayingStage() + 1) == BOSSSTAGEPOS && GlobalData::getChallengeMode() == CMNORMAL)
		{
			nodes.push_back(bossnormal);
			guide = 46;
		}

		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(36))
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->stype() == B_UNAVAILABLE)
			{
				nodes.push_back(b);
				guide = 36;
			}
		}
		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(37))
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->stype() == B_RHOMBICBARRIER || b->stype() == B_HEXAGONBARRIER)
			{
				nodes.push_back(b);
				guide = 37;
			}
		}

		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(44))
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->sstate() == SS_FROZEN)
			{
				nodes.push_back(b);
				guide = 44;
			}
		}
		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}

	if (GlobalData::checkGuide(43))
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->stype() >= B_ONE && b->stype() <= B_NINE)
			{
				nodes.push_back(b);
				guide = 43;
			}
		}
		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}

	if (GlobalData::checkGuide(41))
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->sstate() == SS_GOLDLOCK || b->sstate() == SS_SILVERLOCK)
			{
				nodes.push_back(b);
				guide = 41;
			}
		}
		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(40))
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->stype() == B_TURN)
			{
				nodes.push_back(b);
				guide = 40;
			}
		}

		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(39))
	{
		for (unsigned int i = 0; i < _vecBlocks.size(); i++)
		{
			Block* b = _vecBlocks[i];
			if (b->stype() == B_COVER)
			{
				nodes.push_back(b);
				guide = 39;
			}
		}

		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
	if (GlobalData::checkGuide(45))
	{
		if (_SNUp != NULL)
			nodes.push_back(_SNUp);
		if (_SNDown != NULL)
			nodes.push_back(_SNDown);
		if (_SNLeft != NULL)
			nodes.push_back(_SNLeft);
		if (_SNRight != NULL)
			nodes.push_back(_SNRight);

		if (_wallleft != NULL)
			nodes.push_back(_wallleft);
		if (_wallright = NULL)
			nodes.push_back(_wallright);
		guide = 45;

		if (nodes.size() > 0)
		{
			GlobalData::g_gamestatus = GAMEPAUSE;
			g_NewerLayer = NewerGuideLayer::create(guide);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
}

void GameScene::showNextBlocksGuide(int step)
{
	vector<Node*> nodes;
	if (step == 37)
	{
		if (GlobalData::checkGuide(38))
		{
			for (unsigned int i = 0; i < _vecBlocks.size(); i++)
			{
				Block* b = _vecBlocks[i];
				if (b->stype() == B_RHOMBIC || b->stype() == B_HEXAGON)
				{
					nodes.push_back(b);
				}
			}
			g_NewerLayer = NewerGuideLayer::create(38);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
		}
	}

	else if (step == 41)
	{
		if (GlobalData::checkGuide(42))
		{
			for (unsigned int i = 0; i < _vecBlocks.size(); i++)
			{
				Block* b = _vecBlocks[i];
				if (b->stype() == B_GOLDKEY || b->stype() == B_SILVERKEY)
				{
					nodes.push_back(b);
				}
			}
			g_NewerLayer = NewerGuideLayer::create(42);
			rootCsbLayer->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
		}
	}
}

int GameScene::doGamePause()
{
	if (g_gameScene != NULL)
	{
		if (GlobalData::g_gamestatus == GAMESTART || (GlobalData::g_gamestatus == GAMEPAUSE && g_gameScene->icefrozen->isVisible()))
		{
			if (pauselayer == NULL)
			{
				g_gameScene->scheduleOnce(schedule_selector(GameScene::backKeyShowPauseLayer), 0.05f);
				return 1;
			}
		}
	}
	return 0;
}

void GameScene::backKeyShowPauseLayer(float dt)
{
	g_gameScene->onPauseClick(pauseBtn, ui::Widget::TouchEventType::ENDED);
}

void GameScene::exitGame()
{
	Director::getInstance()->end();
}
int GameScene::getCleanCount()
{
	return _overblockcount;
}

int GameScene::getclasiccleancount()
{
	return clasiccleancount;
}

int GameScene::getCleanAnimalCount(int type)
{
	return animals[type];
}

void GameScene::doFlashModeRule(int index)
{
	vector<Block*> addblocks;

	int addcount = GlobalData::FRuleData[index].ecount;
	int addele = GlobalData::FRuleData[index].ele;

	findDestroyBlocks(addblocks, addcount);

	addEleBlocks(addblocks, addele);

	for (unsigned int i = 0; i < addblocks.size(); i++)
	{
		Block* block = addblocks[i];
		block->state(DISABLE);

		block->runFadeIn(1.2f);
	}
	this->runAction(Sequence::create(DelayTime::create(1.25f), CallFuncN::create(CC_CALLBACK_1(GameScene::flashModeAddedBlockNormal, this, addblocks)), NULL));

	_overblockcount -= addcount;
}

void GameScene::flashModeAddedBlockNormal(Ref* pSender, vector<Block *> vec)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		vec[i]->setOpacity(255);
		vec[i]->setVisible(true);
		vec[i]->state(NORMALBLOCK);
	}
	_isAddingBlocks = false;
	if (!checkiflive())
		shuffle();
}

void GameScene::initFlashModeData()
{
	flashAddtimes = 0;
	clasiccleancount = 0;
	cmpasttime = 0;

	for (unsigned int i = 0; i < GlobalData::FRuleData.size(); i++)
	{
		GlobalData::FRuleData[i].isdone = false;
	}

	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		Block* block = _vecBlocks[i];
		if (block->stype() == B_NOTHING)
		{
			block->changeSpecialType(B_NORMAL, 0);
			block->setVisible(false);
			block->state(DESTROID);
			_blankblockcount--;
			_overblockcount++;
		}
	}
}


void GameScene::sendStageData()
{
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	string idstr = StringUtils::format("%s", GlobalData::getMyID().c_str());
	writedoc.AddMember("uid", rapidjson::Value(idstr.c_str(), allocator), allocator);

	string macstr = "123456";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "GetLocalMacAddress", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		macstr = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif
	writedoc.AddMember("mac", rapidjson::Value(macstr.c_str(), allocator), allocator);

	ChallengeMode mode = GlobalData::getChallengeMode();

	writedoc.AddMember("cmtype", mode, allocator);

	int stars = 0;
	string stagestr;
	if (mode == CMNORMAL)
	{
		stars = GlobalData::getStageStars();
		writedoc.AddMember("stage", GlobalData::getPlayingStage() + 1, allocator);
	}
	else if (mode == CMBOSS)
	{
		stars = GlobalData::getDefeatedBossCount();
	}
	else if (mode == CMCLASIC)
	{
		stars = _curChapter;
	}

	writedoc.AddMember("star", stars, allocator);
	writedoc.AddMember("resulttype", rettype, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	string url;
	url.append(ANALYTICS_HTTPURL);
	url.append("llk_insertstagedata");
	url.append("&param=");
	url.append(buffer.GetString());
	HttpUtil::getInstance()->doData(url);
}

int GameScene::convert16toint(char a)
{
	if (a >= '0' && a <= '9') {
		return a - '0';
	}
	else if (a >= 'A' && a <= 'F') {
		return a - 'A' + 10;
	}
	else if (a >= 'a' && a <= 'f') {
		return a - 'a' + 10;
	}
	else {
		return -1;
	}
}

int GameScene::converToDecimal(char a)
{

	if (a >= '0' && a <= '9')
	{
		return a - '0';
	}

	else if (a >= 'A' && a <= 'Z') {
		return  a - 'A' + 10;
	}
	else if (a >= 'a' && a <= 'z')
		return a - 'a' + 10;
	return -1;
}

char GameScene::converTolowerCase(int val)
{
	if (val >= 10)
		return val - 10 + 'a';
	else
		return val + '0';

}

void GameScene::showBuyFrame(int assign)
{
	if (GlobalData::checkPayCode(sizeof(gemNum) / sizeof(gemNum[0]) + GAME_PROPGIFT, assign))
	{
		FourTypeGift::AnimType = A_UPTODOWN;
		FourTypeGift::_type = GAME_PROPGIFT;
		FourTypeGift* giftLayer = FourTypeGift::create();
		Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
	}
	else
	{
		gamePlayingShowShop = true;
		ShopLayer::whichClicked = 0;
		ShopLayer::AnimType = A_UPTODOWN;
		Director::getInstance()->getRunningScene()->addChild(ShopLayer::create(), 0, "giftlayer");
		
	}
}

bool GameScene::isfirstStageGuide()
{
	if (GlobalData::getHighestStage() < 0 && (GlobalData::checkGuide(1) || GlobalData::checkGuide(2) || GlobalData::checkGuide(3) || GlobalData::checkGuide(4) || GlobalData::checkGuide(5)))
		return true;
	else
		return false;
}

void GameScene::changeGuideBlocks()
{
	int blocktype[] = { 1,0,2,3,0,
						1,3,2,4,3,
						0,2,1,3,4,
						4,1,0,2,4};

	for (unsigned int i = 0; i < _vecBlocks.size(); i++)
	{
		_vecBlocks[i]->changeType(blocktype[i]);
	}

}
