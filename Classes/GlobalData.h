
#ifndef __GLOBALDATA_H_
#define __GLOBALDATA_H_
#include "cocos2d.h"
#include "Const.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

typedef enum
{
	GAMEREADY = 0,
	GAMESTART,
	GAMEPAUSE,
	GAMEOVER,
	GAMERESTART
}GameStatus;

typedef struct
{
	int id;
	std::string name;
	std::string desc;
	int maxleve;
	std::vector<float> data;
	//std::vector<int> needglod;
}SkillData;

typedef struct
{
	int id;
	std::string name;
	std::string desc;
	int type;
	std::vector<int> tdata;//触发条件
	std::vector<int> edata;//影响
}BossSkillData;

typedef struct
{
	int needstars[3];
	int propnum[3][7];
}RewardData;

typedef enum
{
	S_AUTOPROMT = 0,
	S_CTOBGLOD,//图形变为金币
	S_ADDTIME,
	S_ADDCOMBOTIME,
	S_AUTOCLEAN,
	S_ADDONESECOND
}SkillType;

typedef enum
{
	A_SMALLTONORMAL = 0,
	A_UPTODOWN
}AnimType;

typedef enum
{
	ACH_CLEAN = 0,
	ACH_CHANGE, //变身
	ACH_BOMB,//炸弹
	ACH_ICETIME,//时间冻结
	ACH_PROMPT,//放大镜
	ACH_REFRESH,//重排
	ACH_ADDTIME,//加时
	ACH_GEM,//钻石
	ACH_COIN,//金币
	ACH_HEART,//体力
	ACH_MONKEY,//猴子
	ACH_TIGER,//老虎
	ACH_WORLD_RANK,//全国排行榜
	ACH_FRIEND_RANK,//好友排行榜
	ACH_FRIEND_COUNT,//好友数量
	ACH_LELVE,//角色等级
	ACH_STARCOUNT,//星星数量
	ACH_PROPCOUNT,//道具数量
	ACH_STAGE,//游戏关卡
}AchType;

typedef enum
{
	BS_TOBACK = 0,
	BS_TOTRUN,
	BS_COVER,
	BS_FROZEN,
	BS_ADDELE,
	BS_TOGRAY,//

}BossSkillType;

typedef struct{
	int id;
	char name[64];
	char desc[128];
	int achType;
	int count;
	int rwdtype;
	int rcount;
	int finish;
	long datetime;
}Achieve;

typedef struct{
	int rank;
	char name[64];
	char headimg[150];
	int score;
}RankData;

typedef struct{
	int id;
	int type;
	char title[64];
	char content[512];
	char prize[20];
	int status;
	int exdata;
	int datetime;
}Message;

typedef struct{
	int id;
	int type;
	char title[64];
	char rulestr[512];
	char timestr[64];
	int propid[3];
	int propnum[3];
	int startime;
	int endtime;
	char data[32];//需要指定的数据
}MEvent;

typedef enum
{
	RANK_WORLD = 0,
	RANK_BOSS_WORLD,
	RANK_STAGE_WORLD
}RankType;

typedef enum
{
	ONSALEGIFT = 0,
	RICHGIFT,
	CGGIFT,
	GEMGIFT,
	COINGIFT,
	PROPGIFT,
	NEWERGIFT,
	REVIVEGIFT,
	FIRSTGIFT,
	ENTER_ONSALEGIFT,
	ENTER_RICHGIFT,
	ENTER_CGGIFT,
	ENTER_GEMGIFT,
	ENTER_COINGIFT,
	ENTER_PROPGIFT,
	P3_ONSALEGIFT,
	P3_RICHGIFT,
	P3_CGGIFT,
	P3_GEMGIFT,
	P3_COINGIFT,
	P3_PROPGIFT,
	HOME_ONSALE,
	LESS_GEMGIFT,
	LESS_COINGIFT,
	GAME_PROPGIFT,
	GJGIFT,
	ZXGIFT,
	TQGIFT,
	ZZGIFT,
	HHGIFT,
	ENDGIFT
}GiftType;

typedef enum
{
	CMNORMAL = 0,
	CMBOSS,
	CMCLASIC,
	CMFLASH
}ChallengeMode;

typedef struct
{
	int tcount;
	int etype;
}ClasicPropData;

typedef struct
{
	float ttime;
	int ecount;
	int ele;
	bool isdone;
}FlashRuleData;

typedef enum
{
	LOGIN_NONE = 0,//游客
	LOGIN_QQ,
	LOGIN_WX
}LOGINTYPE;

typedef enum
{
	BONUSE_DEFAULT = 0,
	BONUSE_SYSTEM,
	BONUSE_NORMAL,
	BONUSE_BOSSS,
	BONUSE_CLACIS,
	BONUSE_FLASH,
	BONUSE_LOGIN,
	BONUSE_LOTTERY,
	BONUSE_ACHI,
	BONUSE_TASK
}BONUSETYPE;

class GlobalData
{
public:
	static void init();

	static int getDayOfMonth();

	static int getDayOfYear();

	static int getMonthOfYear();

	static int getSystemSecTime();

	static int getLastLoginDayOfYear();

	static void setLastLoginDayOfYear(int val);

	//重排
	static int getRefreshProp();
	static void setRefreshProp(int val);
	//提示
	static int getPromptProp();
	static void setPromptProp(int val);
	//加时
	static void setAddtimeProp(int val);
	static int getAddtimeProp();
	//复活
	static void setReviveProp(int val);
	static int getReviveProp();

	static void setHpCount(int val);
	static int getHpCount();

	static void setGoldCount(int val);
	static int getGoldCount();

	static void setGemCount(int val);
	static int getGemCount();

	static bool isShowGuide();
	static bool checkGuide(int index);
	static void setGuide(int index, bool val);

	static void setMapCount(int val);
	static int getMapCount();

	static int getPlayingStage();
	static void setPlayingStage(int val);

	static int getStageStars();
	static void setStageStars(int val);

	static void setHighestStage(int val);
	static int getHighestStage();

	static bool getIsClickNext();
	static void setIsClickNext(bool val);

	static int getMySkillLevel(int index);
	static void setMySkilLevel(int index, int val);

	static int getMyTotalSkillLevel();
	static void setMyTotalSkillLvel(int val);

	static int getLotteryCount();
	static void setLotteryCount(int val);

	static int getCurStageUseHp();
	static void setCurStageUseHp(int val);

	static int getMyFreeReviveCount();
	static void setMyFreeReviveCount(int val);

	static int getMyMaxHp();
	static void setMyMaxHp(int val);

	static int getHpTime();
	static void setHpTime(int val);

	static int getBuyHpTimes();
	static void setBuyHpTimes(int val);

	static void setMyName(std::string name);
	static std::string getMyName();

	static void setMyID(std::string id);
	static std::string getMyID();

	static void sortAchieveDatabyId();
	static bool sortByIdCb(Achieve a, Achieve b);

	static void setHeadImg(std::string headimg);
	static std::string getHeadImg();

	static int getDefeatedBossCount();
	static void setDefeatedBossCount(int val);
	static int getBestDefeatedBossCount();
	static void setBestDefeatedBossCount(int val);

	static bool getIsShowNewerGuidePromp();
	static void setIsShowNewerGuidePromp(bool val);

	static int getLastLoginRewardSecond();
	static void setLastLoginRewardSecond(int val);

	static int getLastLoginRewardDaySecond();
	static void setLastLoginRewardDaySecond(int val);

	static int getContinueLoginDay();
	static void setContinueLoginDay(int val);

	static int getTotalLoginDay();
	static void setTotalLoginDay(int val);
	static bool getIsGetLoginReward();
	static void setIsGetLoginReward(bool val);
	static void setServerChapterCount(int val);
	static int getServerChapterCount();

	
	static int getNewerGuideStep();
	static void setNewerGuideStep(int step);

	static std::string getBasePath();
	static std::string getUpgradeUrl();
	static void setUpgradeUrl(std::string url);
	static std::string getFormatDateTime();

	static void SaveAchieveData();
	static std::string getAchieveData();
	static Achieve* getAchieveDataByID(int id);

	static void SaveTaskData();
	static std::string getTaskData();
	static void clearTaskData();
	static Achieve* getTaskDataByID(int id);

	static void setIsUseChange(bool val);
	static bool getIsUseChange();
	static void setIsUseBomb(bool val);
	static bool getIsUseBomb();
	static void setIsUseIce(bool val);
	static bool getIsUseIce();

	static void setIsFirstPass(std::string key, bool val);
	static bool getIsFirstPass(std::string key);

	static void setChapterStageStar(std::string key, std::string val);
	static std::string getChapterStageStar(std::string key);

	static std::string addUidString(std::string str);
	static int getCurStageContinueFailCount();
	static void setCurStageContinueFailCount(int val);

	static void setBYMonth(int val);
	static int getBYMonth();

	static void setBYDay(int val);
	static int getBYDay();

	static void setBYPrice(int val);
	static int getBYPrice();

	static void setBYTime(int val);
	static int getBYTime();

	static void setLoginType(int val);
	static int getLoginType();

	static int getstagebestscore(int stage);
	static void setstagebestscore(int stage, int val);

	static bool getIsHomeAutoPopGift();
	static void setIsHomeAutoPopGift(bool val);

	static const char* getPostData();

	static int getBuyTextType();
	static void setBuyTextType(int val);

	static int getBuyPropGiftDay();
	static void setBuyPropGiftDay(int val);

	static int getBuyOnsaleGiftDay();
	static void setBuyOnsaleGiftDay(int val);

	static const char* getShareUrl();

	static void setShareUrl(std::string val);

	static void setIsBuyGift(int index, bool val);

	static bool getIsBuyGift(int index);

	static void setBestCMScore(ChallengeMode mode, int val);
	static int getBestCMScore(ChallengeMode mode);

	static void setChallengeMode(ChallengeMode val);
	static ChallengeMode getChallengeMode();

	static void setShareImgPath(std::string val);
	static const char* getShareImgPath();

	static void setPlayClasicDay(int val);
	static int getPlayClasicDay();

	static void setChapterUnlock(int chapter, bool val);
	static bool getChapterUnlock(int chapter);

	static void setShowChapterUnlock(int chapter, bool val);
	static bool getShowChapterUnlock(int chapter);

	static void setCModeCount(ChallengeMode mode, int val);
	static int getCModeCount(ChallengeMode mode);

	static void setCModeYearOfDay(ChallengeMode mode, int val);
	static int getCModeYearOfDay(ChallengeMode mode);

	static void setIsFirst(ChallengeMode mode, int index, bool val);
	static bool getIsFirst(ChallengeMode mode, int index);

	static void setIsCurStagePass(bool val);
	static bool getIsCurStagePass();

	static void saveEventsIds();
	static std::string getEventsIds();
	static MEvent* getEventByType(int type);
	static int getEventstatus(int type);
	static void setEventstatus(int type, int val);

	static std::string getDownloadMapPath();
	static std::string getXMLDataPath(std::string filename);

	static int covertChapterStage(int decimalStage, int &stage);
	static void initUnlockData();
	static std::string getCid();
	static int bossstasge;
	static std::vector<int> vec_unlockstars;
	static std::vector<int> vec_chapternum;

	static void setTotalStage(int val);
	static int getTotalStage();

	static bool checkPayCode(int payindex, int assign);

	static int getVersionCode();

	static bool getIsUmp();
	static void setIsUmp(bool val);

	static bool getIsRestart();
	static void setIsRestart(bool val);

private:
	static int myRefreshCount;
	static int myPromptCount;
	static int myAddtimeCount;
	static int myReviveCount;

	static int hpCount;
	static int goldCount;
	static int gemCount;

	static int nowday;

	static bool showGuide;
	static int curStage;
	static int score;
	static bool curStageIsPass;
	static int stars;
	static int highestStage;
	static bool isClickNext;
	static bool isSuccOrReviveLayer;
	static int lotteryCount;
	static int mapcount;

	static int mySkillLevel[LEVELTYPECOUNT];
	static int curStageUseHpCount;
	static int myMaxHp;
	static int hpTime;
	static int buyHpTimes;

	static std::string myName;
	static std::string myId;
	static std::string headurl;
	static bool guide[NEWERGUIDECOUNT];
	static bool isFirstEnterGame;

	static bool isShowReviveGuide;
	static int freeReviveCount;

	static int lastLoginRewardSecond;
	static int lastLoginRewardDaySecond;
	static int continueLoginDay;
	static int totalLoginDay;
	static bool isGetLoginReward;
	static int serverChapterCount;
	static bool isShowNewerGuidePromp;
	static int newerGuideStep;
	static int myTotalSkillLevel;

	static int defeatedBossCount;
	static int bestDefeatedBossCount;

	static bool isUseChange;
	static bool isUseBomb;
	static bool isUseIce;
	static std::string chapterStageStarStr;

	static std::string postdata;

	static int logintype;
	static bool isHomeAutoPopGift;
	static int buyTextType;

	static int buyOnsaleGiftDay;
	static int buyPropGiftDay;
	static std::string shareurl;
	static std::string shareimgpath;

	static bool isGiftBuy[ENDGIFT];

	static ChallengeMode challengemode;

	static std::vector<bool> vec_isunlock;
	static std::vector<bool> vec_isShowUnlockAnim;
	static int cmcount[CMFLASH + 1];

	static int bestscore[CMFLASH + 1];

	static bool isCurStagePass;
	static std::string upgradeurl;
	static int totalstage;
	static bool isump;
	static bool isrestart;
public:
	static GameStatus g_gamestatus;
	static std::map<int, SkillData> mapSkillData;
	static std::map<int, RewardData> mapRewardData;
	static std::vector<int> ckRewards;
	static std::map<int, std::vector<std::string>> mapBossWords;
	static std::vector<Achieve> achieveData;
	static std::vector<Achieve> taskData;
	static std::map<int, BossSkillData> mapBossSkillData;
	static std::map<int, std::vector<Message>> mapMessageData;
	static std::vector<MEvent> EventsData;
	static std::vector<RankData> rankData;
	static std::vector<ClasicPropData> CPData;
	static std::vector<FlashRuleData> FRuleData;
};


#endif /* defined(__GLOBALDATA_H_) */
