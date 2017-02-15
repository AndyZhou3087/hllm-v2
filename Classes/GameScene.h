#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include "Block.h"
#include "Line.h"
#include "MyParticle.h"
#include "ui/UIScale9Sprite.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace std;

typedef struct
{
	int chapter;
	int stage;
	int extstage;
	int snum;
	int rows;
	int cols;
	int elements_num;
	int time;
	int time1;
	int time2;
	int time3;
	bool blockup;
	bool blockdown;
	bool blockleft;
	bool blockright;
	string es;	//所有元素
}StageInfo;

typedef enum
{
	P_PROMPT,
	P_REFRESH,
	P_CLOCK
}PropsType;

class GameScene : public Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onExit();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* scene();
	static void addLayer(Scene* scene);
    // a selector callback
    void menuCloseCallback(Ref* pSender);
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch *touch, Event *event);
	// implement the "static node()" method manually
	GameScene();
	~GameScene();
    CREATE_FUNC(GameScene);
	void showReadyAnim();
	void show321Anim();
	float getUseSecondTime();
	int getLeftSecondTime();
	int getBonuesTime();
	void saveData();
	void loadData();
	void clearData();
	void gamePause();
	void gameResume();
	void usePrompt();
	void useRefresh();
	void useClock();
	void onPauseClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void removeCSB();
	int getBonuseGold();
	void NewerGuideClickCB(int step);
	static int doGamePause();

	static void exitGame();
	void showBlockGuide(float dt);
	void showNextBlocksGuide(int type);
	void delayShowHeroAnim(float dt);
	void delayShowBossAnim(float dt);
	int getScore();
	int getCleanCount();
	int getCleanAnimalCount(int type);
	void gameRevive();
	void restart();
	void restartClasic();
	int getClasicStage();
	int getclasiccleancount();

	void resetBossStage();

	int getFlashAddTimes();

	void restartFlash();

	bool gamePlayingShowShop;
private:
	void update(float dt);
	void parseXML(string fileName);
	void makeMap();
	bool verify2points(Block * a, Block * b);
	bool verifysamerow(int row, int col1, int col2, bool strict);
	bool verifysamecol(int col, int row1, int row2, bool strict);
	bool checkoneturn(Block * a, Block * b, bool ischange = false);
	Block * getBlock(int row, int col);
	Line * getLine(int row, int col, LineType type);
	bool checkiflive();
	void shuffle();
	void showLineAnimation();
	void AddSameRowLines(int row, int col1, int col2, int sort);//0--小到大，1--大到小
	void AddSameColLines(int col, int row1, int row2, int sort);//0--小到大，1--大到小

	bool verifyoutlinesamerow(int row, int col1, int col2);
	bool verifyoutlinesamecol(int col, int row1, int row2);

	void onPromptClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRefreshClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onClockClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void showCountDownTime(float dt);

	void showUseChange(float dt);
	void revive();

	void playWinAnim(float dt);
	void playFailAnim(float dt);
	void delayShowSuccLayer();
	void enterSuccLayer(float dt);
	void enterBossSettlementLayer(float dt);
	void enterReviveLayer(float dt);
	void enterAddTimeLayer(float dt);
	void removeReadySprite(Node* node);
	void GameStart();
	void showComboHitAnim();
	void shuffleAction();
	void shuffleOver();
	void stopFireAction();
	void reMakeMap();
	void resetNextSmallStageData();
	int convert16toint(char a);
	int converToDecimal(char a);
	char converTolowerCase(int val);
	void clearSNBlocks();
	void cleanSpecialBlock(Block* a, Block* b);
	void changeNumBlocks();
	bool allMove(int row, int col);
	static bool lesser_callback(Block *a, Block *b);
	static bool sortNumBlocks(Block *a, Block *b);
	void shuffleBlocks(vector<Block*> &ret);
	void shuffleBlocksOther(vector<Block*> &ret);
	bool isObstruction(Block* block);
	bool checkMoveOrder(int row, int col);
	bool isCanShuffle(Block* block);
	void beginmove();
	void showCloudBlocks(float dt);
	void eraseCloudBlock(Block *a, Block *b);
	void shuffeOverGameStart();
	void showBlockBack();
	void hideBlockBack();
	void find2RandomSameBlocks(Block **a, Block **b);
	void delayShuffle(float dt);
	void useCleanAfterSkills(Block *a, Block *b);
	void checkUseProp(float dt);
	void showChangePropAnim(Ref* pSender, int type);
	void showBombPropAnim(Ref* pSender);
	void setBombBlockZoder(Ref* pSender);
	void resetBlockZorder();
	void changeBombMove(Ref* pSender);
	void delayShowReadyGo(float dt);
	void delayShowSkillAddTime(float dt);
	void clickBombProp();
	void boom(Block* block);
	void showChangeGoldBlock(Ref* pSender);
	bool checkMoveable();
	void showVsLayer(float dt);
	void removeAddTimeText(Ref* pSender);
	void remove1SText(Ref* pSender);
	void showAdd1SAnim();

	void delayPlayWinSound(float dt);
	void delayPlayLossSound(float dt);

	void showPropPrice(float dt);
	void backKeyShowPauseLayer(float dt);

	void delayShowPromptGuide(float dt);
	void delayShowRefreshGuide(float dt);
	void delayShowAddTimeGuide(float dt);
	void delayShowCleanBoomAnim(float dt);
	void updateStarlv();
	void playHitBossAnim();
	void delayShowBossHited(float dt);
	void delayShowAutoCleanSkill(float dt);
	void delayNextBossStage(float dt);
	void showBossStage();
	void resetNextClasicStage(float dt);

	void bossSkill(float dt);
	void removeSelf(Ref* pSender);
	void removeGray(float dt);

	void findDestroyBlocks(std::vector<Block *> &vec, int &count);
	void findRandNormalBlocks(std::vector<Block *> &vec, int count);

	void exeBossSkill(Ref* pSender, vector<Block *> vec);

	void addEleBlocks(vector<Block *> vec, int Eletype);

	void flashModeAddedBlockNormal(Ref* pSender, vector<Block *> vec);

	void bossskillAmin(vector<Block *> vec);
	void removebossskillAmin(Block *A, Block *B);
	void createRunLight(Node* A);

	void showBossCrashAnim();
	void reset();
	void updateTimeUi(int time);
	void delayShowFreeGift(float dt);
	void enterCMOverLayer(float dt);

	void addBlockProp(float dt);
	void exeBlockProp(Block *a, Block *b);
	bool isCleanAll();

	void initFlashModeData();
	void doFlashModeRule(int index);

	void sendStageData();
	int rettype;
	void showBuyFrame(int assign);
	bool isfirstStageGuide();
	void changeGuideBlocks();

	void showUsePropPaticle(float dt);

	void playPromptAnim(Node* node);

	void playUsePromptParticle(Node* node, Vec2 despos);

	void useRefreshShuffle(float dt);
private:
	int _curStage;
	int _curChapter;
	vector<Block *> _vecBlocks;
	StageInfo _stage;
	Block * _lastBlock;
	Block * _curBlock;
	Block * _promptBlock1;
	Block * _promptBlock2;
	vector<Line *> _vecLines;
	vector<Line *> _vecShowLines;
	SpriteBatchNode * batch;
	float _leftoffset;
	float _topoffset;

	Node * rootCsbLayer;


	Sprite * _go;
	Sprite* _refreshword;
	Sprite * _fire;
	Sprite * _fire1;
	bool _fireBlink;
	static float _pasttime;
	cocos2d::ui::TextBMFont* _leftCountLabel;
	cocos2d::ui::Widget * timebarbg;
	cocos2d::ui::LoadingBar* progressTimer;

	Sprite* _saddTimeText;
	Sprite* _gameStateTextBox;
	Sprite* _gameStateText;
	float _lastlinkpasttime;	//上次成功连接以来过了多少秒
	int _keepingcount;	//第多少次连击
	int _overblockcount;	//已经消了多少个
	int _blankblockcount;	//开始有多少个空白块和不可消除块
	int _unavailableBlockCount;

	int _canCleanBarriers;//可消除障碍物个数
	int _clickCleanBlockCount;//点击消除个数，用于BOSS技能触发条件

	float _nolinktime;	//多少秒没有消除行为发生

	static int leftTime;

	cocos2d::ui::TextBMFont* _chapterLbl;
	double BOXWIDTH;
	double BOXHEIGHT;

	MoveRule _mr[100];
	cocos2d::ui::Widget* _starbg[3];
	cocos2d::ui::Widget* _starlv[3];
	Sprite* _SNUp;
	Sprite* _SNDown;
	Sprite* _wallleft;
	Sprite * _SNLeft;
	Sprite * _SNRight;
	Sprite* _wallright;
	vector<Block*> _vecNumBlocks;
	vector<Block*> _vecCloudBlocks;
	cocos2d::ui::Widget* icefrozen;
	int _lastRandom;
	int _isMoving;
	int _isMovingClean;
	int _cleanBlockTimes;
	int _cleanGlodBlockTimes;
	float _noCleanTime;
	int _autopromptCount;
	int _timeupShowAddTime;
	float icesec;
	int skillAdd1SecondCount;
	bool _isPlayTimeUp;

	cocos2d::ui::Button* pauseBtn;
	cocos2d::ui::Button *_promptBtn;
	cocos2d::ui::Button *_refreshBtn;
	cocos2d::ui::Button *_clockBtn;
	cocos2d::ui::TextBMFont* _promptcountLabel;
	cocos2d::ui::TextBMFont* _refreshcountLabel;
	cocos2d::ui::TextBMFont* _addtimecountLabel;
	cocos2d::ui::Widget* promptPrice;
	cocos2d::ui::Widget* refreshPrice;
	cocos2d::ui::Widget* clockPrice;
	int useAddTime;
	bool isctogoldskill;
	Sprite* boomlight1;
	Sprite* boomlight2;
	bool isClickBomb;
	int boomx;
	int boomy;
	Node* bosshited;
	Node* bossnormal;
	ActionTimeline* heroAction;
	ActionTimeline* bosshitedAnim;
	ActionTimeline* bossNoramlAnim;
	float _keeptime;
	Sprite* _combosprite;
	Label* _combolable;
	int _comboscore;
	int extrasocre;
	int animals[ELECOUNT];
	Sprite* _heroComboWord;
	bool isbossmode;
	int clasicstage;
	int clasiccleancount;
	float cmpasttime;
	int flashAddtimes;
	bool _isAddingBlocks;
	bool isNewerGuide;
public:
	static int g_whattobuy;	//准备买什么 0:提示 1：重排 2：加时
	static int _stageSecond;	//本局秒数

};
extern GameScene* g_gameScene;
extern Layer* pauselayer;
#endif // __GAME_SCENE_H__
