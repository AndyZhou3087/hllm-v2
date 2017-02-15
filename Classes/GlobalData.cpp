#include "GlobalData.h"
#include "CommonFuncs.h"
int GlobalData::myRefreshCount = 0;
int GlobalData::myPromptCount = 0;
int GlobalData::myAddtimeCount = 0;

int GlobalData::hpCount = 25;
int GlobalData::myMaxHp = 25;
int GlobalData::goldCount = 0;
int GlobalData::gemCount = 0;

int GlobalData::lotteryCount = 1;

int GlobalData::mapcount = 0;
int GlobalData::stars = -1;
int GlobalData::curStage = -1;
int GlobalData::mySkillLevel[LEVELTYPECOUNT] = { 0, 0, 0, 0, 0, 0 };

GameStatus GlobalData::g_gamestatus = GAMEREADY;

bool GlobalData::isClickNext = false;
int GlobalData::curStageUseHpCount = 0;
int GlobalData::myReviveCount = 0;
int GlobalData::highestStage = -1;

int GlobalData::hpTime = 0;
int GlobalData::buyHpTimes = 0;
bool GlobalData::isFirstEnterGame = true;
bool GlobalData::isSuccOrReviveLayer = false;
int GlobalData::freeReviveCount = 0;
bool GlobalData::isShowReviveGuide = false;
std::string GlobalData::myName = "";
std::string GlobalData::myId = "";
std::string GlobalData::headurl = "";
bool GlobalData::guide[NEWERGUIDECOUNT];
std::map<int, SkillData> GlobalData::mapSkillData;
std::map<int, RewardData> GlobalData::mapRewardData;
std::map<int, std::vector<std::string>> GlobalData::mapBossWords;
std::vector<int> GlobalData::ckRewards;
std::vector<Achieve> GlobalData::achieveData;
std::vector<Achieve> GlobalData::taskData;
std::vector<RankData> GlobalData::rankData;
std::map<int, BossSkillData> GlobalData::mapBossSkillData;

std::map<int, std::vector<Message>> GlobalData::mapMessageData;

std::vector<MEvent> GlobalData::EventsData;

std::vector<ClasicPropData> GlobalData::CPData;

std::vector<FlashRuleData> GlobalData::FRuleData;

bool GlobalData::isShowNewerGuidePromp = false;

int GlobalData::lastLoginRewardSecond = 0;
int GlobalData::lastLoginRewardDaySecond = 0;
int  GlobalData::continueLoginDay = 1;

int GlobalData::totalLoginDay = 0;
int GlobalData::newerGuideStep = 0;

bool GlobalData::isGetLoginReward = false;
int GlobalData::serverChapterCount = 0;

int GlobalData::myTotalSkillLevel = 0;
ChallengeMode GlobalData::challengemode = CMNORMAL;
int GlobalData::defeatedBossCount = 0;
int GlobalData::bestDefeatedBossCount = 0;

bool GlobalData::isUseChange = false;
bool GlobalData::isUseBomb = false;
bool GlobalData::isUseIce = false;
std::string GlobalData::chapterStageStarStr = "0";

std::string GlobalData::postdata = "";
int GlobalData::logintype = 0;
bool GlobalData::isHomeAutoPopGift = false;

int GlobalData::buyTextType = 0;
int GlobalData::buyOnsaleGiftDay = -1;
int GlobalData::buyPropGiftDay = -1;
std::string GlobalData::shareurl = "";
std::string GlobalData::shareimgpath = "";
bool GlobalData::isGiftBuy[ENDGIFT];

std::vector<bool> GlobalData::vec_isunlock;

std::vector<bool> GlobalData::vec_isShowUnlockAnim;

std::vector<int> GlobalData::vec_unlockstars;
std::vector<int> GlobalData::vec_chapternum;

int GlobalData::cmcount[CMFLASH + 1];
bool GlobalData::isCurStagePass = false;

int GlobalData::bestscore[CMFLASH + 1];

const static char* scorenamestr[] = { "", "clasic", "boss", "flash" };

int GlobalData::bossstasge = 1;

std::string GlobalData::upgradeurl = "";

int GlobalData::totalstage = 100;

bool GlobalData::isump = false;

bool GlobalData::isrestart = false;

void GlobalData::init()
{
	myRefreshCount = UserDefault::getInstance()->getIntegerForKey(addUidString("refresh").c_str(), 0);
	myPromptCount = UserDefault::getInstance()->getIntegerForKey(addUidString("prompt").c_str(), 0);
	myAddtimeCount = UserDefault::getInstance()->getIntegerForKey(addUidString("addtime").c_str(), 0);

	hpCount = UserDefault::getInstance()->getIntegerForKey(addUidString("HpCount").c_str(), 25);
	goldCount = UserDefault::getInstance()->getIntegerForKey(addUidString("goldNum").c_str(), 0);
	gemCount = UserDefault::getInstance()->getIntegerForKey(addUidString("diamondNum").c_str(), 0);

	lotteryCount = UserDefault::getInstance()->getIntegerForKey(addUidString("lotteryCount").c_str(), 1);

	myReviveCount = UserDefault::getInstance()->getIntegerForKey(addUidString("revivecount").c_str(), 0);
	highestStage = UserDefault::getInstance()->getIntegerForKey(addUidString("maxstage").c_str(), -1);
	myMaxHp = UserDefault::getInstance()->getIntegerForKey(addUidString("maxhp").c_str(), 25);
	hpTime = UserDefault::getInstance()->getIntegerForKey(addUidString("hptime").c_str(), 0);
	buyHpTimes = UserDefault::getInstance()->getIntegerForKey(addUidString("buyhptimes").c_str(), 0);

	myTotalSkillLevel = UserDefault::getInstance()->getIntegerForKey(addUidString("tskill").c_str(), 0);
	for (int i = 0; i < LEVELTYPECOUNT; i++)
	{
		std::string str = StringUtils::format("skill%d", i);
		mySkillLevel[i] = UserDefault::getInstance()->getIntegerForKey(addUidString(str).c_str(), 0);
	}
	for (int i = 0; i < NEWERGUIDECOUNT; i++)
	{
		std::string str = StringUtils::format("guide%d", i);
		guide[i] = UserDefault::getInstance()->getBoolForKey(str.c_str(), true);
	}
	myName = UserDefault::getInstance()->getStringForKey(addUidString("name").c_str(), "");
	headurl = UserDefault::getInstance()->getStringForKey(addUidString("hid").c_str(), "");
	freeReviveCount = UserDefault::getInstance()->getIntegerForKey(addUidString("freerevivecount").c_str(), 0);

	lastLoginRewardSecond = UserDefault::getInstance()->getIntegerForKey(addUidString("lastloginsecond").c_str(), 0);
	lastLoginRewardDaySecond = UserDefault::getInstance()->getIntegerForKey(addUidString("lastlogindaysec").c_str(), 0);
	continueLoginDay = UserDefault::getInstance()->getIntegerForKey(addUidString("continueloginday").c_str(), 1);
	totalLoginDay = UserDefault::getInstance()->getIntegerForKey(addUidString("totalloginday").c_str(), 0);
	isGetLoginReward = UserDefault::getInstance()->getBoolForKey(addUidString("isgetloginreward").c_str(), false);

	bestDefeatedBossCount = UserDefault::getInstance()->getIntegerForKey(addUidString("bestdefeatedbosscount").c_str(), 0);

	for (int i = 0; i < ENDGIFT; i++)
	{
		std::string str = StringUtils::format("g%dbuy", i);
		isGiftBuy[i] = UserDefault::getInstance()->getBoolForKey(addUidString(str).c_str(), false);
	}

	buyOnsaleGiftDay = UserDefault::getInstance()->getIntegerForKey(addUidString("getonsalegiftday").c_str(), -1);
	buyPropGiftDay = UserDefault::getInstance()->getIntegerForKey(addUidString("getpropgiftday").c_str(), -1);

	//saveHeadDataTofile("http:\/\/q.qlogo.cn\/qqapp\/1105233824\/EF95C27003021EBA5F65B86F205948EE\/40");
	initUnlockData();

	for (int i = 0; i <= CMFLASH; i++)
	{
		std::string cmstr = StringUtils::format("cm%dcount", i);
		cmcount[i] = UserDefault::getInstance()->getIntegerForKey(addUidString(cmstr).c_str(), 1);

		std::string scorestr = StringUtils::format("best%sscore", scorenamestr[i]);
		bestscore[i] = UserDefault::getInstance()->getIntegerForKey(addUidString(scorestr).c_str(), 0);
	}
}

int GlobalData::getRefreshProp()
{
	return myRefreshCount;
}

int GlobalData::getLotteryCount()
{
	return lotteryCount;
}

void GlobalData::setLotteryCount(int val)
{
	lotteryCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("lotteryCount").c_str(), lotteryCount);
}

void GlobalData::setRefreshProp(int val)
{
	myRefreshCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("refresh").c_str(), myRefreshCount);
}
int GlobalData::getPromptProp()
{
	return myPromptCount;
}
void GlobalData::setPromptProp(int val)
{
	myPromptCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("prompt").c_str(), myPromptCount);
}

void GlobalData::setAddtimeProp(int val)
{
	myAddtimeCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("addtime").c_str(), myAddtimeCount);
}
int GlobalData::getAddtimeProp()
{
	return myAddtimeCount;
}


void GlobalData::setHpCount(int val)
{
	hpCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("HpCount").c_str(), hpCount);
}
int GlobalData::getHpCount()
{
	return hpCount;
}

void GlobalData::setGoldCount(int val)
{
	goldCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("goldNum").c_str(), goldCount);
}

int GlobalData::getGoldCount()
{
	return goldCount;
}

void GlobalData::setGemCount(int val)
{
	gemCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("diamondNum").c_str(), gemCount);
}
int GlobalData::getGemCount()
{
	return gemCount;
}

bool GlobalData::checkGuide(int index)
{
	return guide[index];
}
void GlobalData::setGuide(int index, bool val)
{
	guide[index] = val;
	std::string str = StringUtils::format("guide%d", index);
	UserDefault::getInstance()->setBoolForKey(str.c_str(), val);
}


void GlobalData::setMapCount(int val)
{
	mapcount = val;
}

int GlobalData::getMapCount()
{
	return mapcount;
}

int GlobalData::getPlayingStage()
{
	return curStage;
}

void GlobalData::setPlayingStage(int val)
{
	curStage = val;
}

int GlobalData::getstagebestscore(int stage)
{
	std::string str = StringUtils::format("stagebestscore%d", stage);
	return UserDefault::getInstance()->getIntegerForKey(addUidString(str).c_str(), 0);
}

void GlobalData::setstagebestscore(int stage, int val)
{
	std::string str = StringUtils::format("stagebestscore%d", stage);
	UserDefault::getInstance()->setIntegerForKey(addUidString(str).c_str(), val);
}

int GlobalData::getStageStars()
{
	return stars;
}

void GlobalData::setStageStars(int val)
{
	stars = val;
}

int GlobalData::getReviveProp()
{
	return myReviveCount;
}

void GlobalData::setReviveProp(int val)
{
	myReviveCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("revivecount").c_str(), myReviveCount);
}

void GlobalData::setHighestStage(int val)
{
	highestStage = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("maxstage").c_str(), highestStage);
}
int GlobalData::getHighestStage()
{
	return highestStage;
}

bool GlobalData::getIsClickNext()
{
	return isClickNext;
}
void GlobalData::setIsClickNext(bool val)
{
	isClickNext = val;
}

int GlobalData::getMySkillLevel(int index)
{
	return mySkillLevel[index];
}
void GlobalData::setMySkilLevel(int index, int val)
{
	mySkillLevel[index] = val;
	std::string str = StringUtils::format("skill%d", index);
	UserDefault::getInstance()->setIntegerForKey(addUidString(str).c_str(), mySkillLevel[index]);
}

int GlobalData::getMyTotalSkillLevel()
{
	return myTotalSkillLevel;
}

void GlobalData::setMyTotalSkillLvel(int val)
{
	myTotalSkillLevel = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("tskill").c_str(), myTotalSkillLevel);
}


int GlobalData::getCurStageUseHp()
{
	return curStageUseHpCount;
}
void GlobalData::setCurStageUseHp(int val)
{
	curStageUseHpCount = val;
}

int GlobalData::getMyFreeReviveCount()
{
	/*
	int day = getDayOfMonth();

	if (day != UserDefault::getInstance()->getIntegerForKey("skillreviveday", -1))
	{
		int myReviveSkillLv = GlobalData::getMySkillLevel(S_FREEREVIVE);

		if (myReviveSkillLv > 0)
		{
			freeReviveCount = GlobalData::mapSkillData[S_FREEREVIVE].data[myReviveSkillLv - 1];
		}
	}
	else
	{
		freeReviveCount = UserDefault::getInstance()->getIntegerForKey("skillrevivecount", 0);
	}
	*/
	return freeReviveCount;
}
void GlobalData::setMyFreeReviveCount(int val)
{
	/*
	UserDefault::getInstance()->setIntegerForKey("skillrevivecount", val);

	UserDefault::getInstance()->setIntegerForKey("skillreviveday", getDayOfMonth());
	*/
	freeReviveCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("freerevivecount").c_str(), val);
}

int GlobalData::getDayOfMonth()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int day = tm->tm_mday;
	return day;
}

int GlobalData::getDayOfYear()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int day = tm->tm_yday;
	return day;
}

int GlobalData::getMonthOfYear()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int month = tm->tm_mon + 1;
	return month;
}

int GlobalData::getSystemSecTime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	return timep;
}

int GlobalData::getLastLoginDayOfYear()
{
	return UserDefault::getInstance()->getIntegerForKey(addUidString("lastlogindayofyear").c_str(), -1);
}

void GlobalData::setLastLoginDayOfYear(int val)
{
	UserDefault::getInstance()->setIntegerForKey(addUidString("lastlogindayofyear").c_str(), val);
}

int GlobalData::getMyMaxHp()
{
	return myMaxHp;
}
void GlobalData::setMyMaxHp(int val)
{
	myMaxHp = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("maxhp").c_str(), myMaxHp);
}

int GlobalData::getHpTime()
{
	return hpTime;
}
void GlobalData::setHpTime(int val)
{
	hpTime = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("hptime").c_str(), hpTime);
}

int GlobalData::getBuyHpTimes()
{
	return buyHpTimes;
}
void GlobalData::setBuyHpTimes(int val)
{
	buyHpTimes = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("buyhptimes").c_str(), buyHpTimes);
}

std::string GlobalData::getMyName()
{
	return myName;
}

void GlobalData::setMyName(std::string name)
{
	myName = name;
	UserDefault::getInstance()->setStringForKey(addUidString("name").c_str(), name);

}

void GlobalData::setMyID(std::string id)
{
	myId = id;
	UserDefault::getInstance()->setStringForKey("id", id);
}
std::string GlobalData::getMyID()
{
	return UserDefault::getInstance()->getStringForKey("id", "");
}

void GlobalData::setHeadImg(std::string headimg)
{
	headurl = headimg;
	UserDefault::getInstance()->setStringForKey(addUidString("hurl").c_str(), headurl);
}

std::string GlobalData::getHeadImg()
{
	return headurl;
}

bool GlobalData::getIsShowNewerGuidePromp()
{
	return isShowNewerGuidePromp;
}
void GlobalData::setIsShowNewerGuidePromp(bool val)
{
	isShowNewerGuidePromp = val;
}

int GlobalData::getLastLoginRewardSecond()
{
	return lastLoginRewardSecond;
}
void GlobalData::setLastLoginRewardSecond(int val)
{
	lastLoginRewardSecond = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("lastloginsecond").c_str(), lastLoginRewardSecond);
}

int GlobalData::getLastLoginRewardDaySecond()
{
	return lastLoginRewardDaySecond;
}
void GlobalData::setLastLoginRewardDaySecond(int val)
{
	lastLoginRewardDaySecond = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("lastlogindaysec").c_str(), lastLoginRewardDaySecond);
}

int GlobalData::getContinueLoginDay()
{
	return continueLoginDay;
}
void GlobalData::setContinueLoginDay(int val)
{
	continueLoginDay = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("continueloginday").c_str(), continueLoginDay);
}

int GlobalData::getTotalLoginDay()
{
	return totalLoginDay;
}

void GlobalData::setTotalLoginDay(int val)
{
	totalLoginDay = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("totalloginday").c_str(), totalLoginDay);
}


bool GlobalData::getIsGetLoginReward()
{
	return isGetLoginReward;
}
void GlobalData::setIsGetLoginReward(bool val)
{
	isGetLoginReward = val;
	UserDefault::getInstance()->setBoolForKey(addUidString("isgetloginreward").c_str(), isGetLoginReward);
}

int GlobalData::getBuyPropGiftDay()
{
	return buyPropGiftDay;
}

void GlobalData::setBuyPropGiftDay(int val)
{
	buyPropGiftDay = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("getpropgiftday").c_str(), val);
}

int GlobalData::getBuyOnsaleGiftDay()
{
	return buyOnsaleGiftDay;
}

void GlobalData::setBuyOnsaleGiftDay(int val)
{
	buyOnsaleGiftDay = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("getonsalegiftday").c_str(), val);
}

void GlobalData::setServerChapterCount(int val)
{
	serverChapterCount = val;
}
int GlobalData::getServerChapterCount()
{
	return serverChapterCount;
}

std::string GlobalData::getBasePath()
{
	std::string path;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "getBaseDir", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		path = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#else
	path = FileUtils::getInstance()->getWritablePath() + "hllm";
#endif
	return path;
}

std::string GlobalData::getDownloadMapPath()
{
	std::string path = getBasePath() + "/download";
	return path;
}

std::string GlobalData::getUpgradeUrl()
{
	return upgradeurl;
}

void GlobalData::setUpgradeUrl(std::string url)
{
	upgradeurl = url;
}

int GlobalData::getNewerGuideStep()
{
	return newerGuideStep;
}

void GlobalData::setNewerGuideStep(int step)
{
	newerGuideStep = step;
}

std::string GlobalData::getFormatDateTime()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	std::string ret = StringUtils::format("%d%02d%02d%02d%02d%02d", (int)tm->tm_year + 1900, (int)tm->tm_mon + 1, (int)tm->tm_mday, (int)tm->tm_hour, (int)tm->tm_min, (int)tm->tm_sec);
	return ret;
}

void GlobalData::setChallengeMode(ChallengeMode val)
{
	challengemode = val;
}

ChallengeMode GlobalData::getChallengeMode()
{
	return challengemode;
}

void GlobalData::setDefeatedBossCount(int val)
{
	defeatedBossCount = val;
}

int GlobalData::getDefeatedBossCount()
{
	return defeatedBossCount;
}

int GlobalData::getBestDefeatedBossCount()
{
	return bestDefeatedBossCount;
}

void GlobalData::setBestDefeatedBossCount(int val)
{
	bestDefeatedBossCount = val;
	UserDefault::getInstance()->setIntegerForKey(addUidString("bestdefeatedbosscount").c_str(), val);
}

void GlobalData::setIsUseChange(bool val)
{
	isUseChange = val;
}

bool GlobalData::getIsUseChange()
{
	return isUseChange;
}
void GlobalData::setIsUseBomb(bool val)
{
	isUseBomb = val;
}
bool GlobalData::getIsUseBomb()
{
	return isUseBomb;
}
void GlobalData::setIsUseIce(bool val)
{
	isUseIce = val;
}
bool GlobalData::getIsUseIce()
{
	return isUseIce;
}

void GlobalData::setIsFirstPass(std::string key, bool val)
{
	UserDefault::getInstance()->setBoolForKey(addUidString(key).c_str(), val);
}

bool GlobalData::getIsFirstPass(std::string key)
{
	return UserDefault::getInstance()->getBoolForKey(addUidString(key).c_str(), true);
}

void GlobalData::setChapterStageStar(std::string key, std::string val)
{
	chapterStageStarStr = val;
	UserDefault::getInstance()->setStringForKey(addUidString(key).c_str(), val.c_str());
}

std::string GlobalData::getChapterStageStar(std::string key)
{
	return UserDefault::getInstance()->getStringForKey(addUidString(key).c_str(), "0");
}

int GlobalData::getCurStageContinueFailCount()
{
	std::string str = StringUtils::format("cfailcount%d", GlobalData::getPlayingStage());
	return UserDefault::getInstance()->getIntegerForKey(addUidString(str).c_str(), 0);
}

void GlobalData::setCurStageContinueFailCount(int val)
{
	std::string str = StringUtils::format("cfailcount%d", GlobalData::getPlayingStage());
	UserDefault::getInstance()->setIntegerForKey(addUidString(str).c_str(), val);
}

void GlobalData::SaveAchieveData()
{
	std::string strdata;
	int size = achieveData.size();
	for (int i = 0; i < size; i++)
	{
		std::string str = StringUtils::format("%d,%s,%s,%d,%d,%d,%d,%d,%ld", achieveData[i].id, achieveData[i].name, achieveData[i].desc, achieveData[i].achType, achieveData[i].count,
			achieveData[i].rwdtype, achieveData[i].rcount, achieveData[i].finish, achieveData[i].datetime);
		strdata.append(str);
		if (i < size - 1)
			strdata.append(";");
	}
	UserDefault::getInstance()->setStringForKey(addUidString("achieve").c_str(), strdata);
}

std::string GlobalData::getAchieveData()
{
	return UserDefault::getInstance()->getStringForKey(addUidString("achieve").c_str(), "");
}

Achieve* GlobalData::getAchieveDataByID(int id)
{
	for (unsigned int i = 0; i < achieveData.size(); i++)
	{
		if (achieveData[i].id == id)
			return &achieveData[i];
	}
	return NULL;
}

void GlobalData::SaveTaskData()
{
	std::string strdata;
	int size = taskData.size();
	for (int i = 0; i < size; i++)
	{
		std::string str = StringUtils::format("%d,%s,%s,%d,%d,%d,%d,%d,%ld", taskData[i].id, taskData[i].name, taskData[i].desc, taskData[i].achType, taskData[i].count,
			taskData[i].rwdtype, taskData[i].rcount, taskData[i].finish, taskData[i].datetime);
		strdata.append(str);
		if (i < size - 1)
			strdata.append(";");
	}
	UserDefault::getInstance()->setStringForKey(addUidString("task").c_str(), strdata);
}

std::string GlobalData::getTaskData()
{
	return UserDefault::getInstance()->getStringForKey(addUidString("task").c_str(), "");
}

void GlobalData::clearTaskData()
{
	UserDefault::getInstance()->setStringForKey(addUidString("task").c_str(), "");
}

Achieve* GlobalData::getTaskDataByID(int id)
{
	for (unsigned int i = 0; i < taskData.size(); i++)
	{
		if (taskData[i].id == id)
			return &taskData[i];
	}
	return NULL;
}

void GlobalData::setBYMonth(int val)
{
	UserDefault::getInstance()->setIntegerForKey(addUidString("mmonth").c_str(), val);
}
int GlobalData::getBYMonth()
{
	return UserDefault::getInstance()->getIntegerForKey(addUidString("mmonth").c_str(), -1);
}

void GlobalData::setBYDay(int val)
{
	UserDefault::getInstance()->setIntegerForKey(addUidString("mday").c_str(), val);
}
int GlobalData::getBYDay()
{
	return UserDefault::getInstance()->getIntegerForKey(addUidString("mday").c_str(), -1);
}

void GlobalData::setBYPrice(int val)
{
	UserDefault::getInstance()->setIntegerForKey(addUidString("mprice").c_str(), val);
}
int GlobalData::getBYPrice()
{
	return UserDefault::getInstance()->getIntegerForKey(addUidString("mprice").c_str(), 0);
}

void GlobalData::setBYTime(int val)
{
	UserDefault::getInstance()->setIntegerForKey(addUidString("mbuytime").c_str(), val);
}
int GlobalData::getBYTime()
{
	return UserDefault::getInstance()->getIntegerForKey(addUidString("mbuytime").c_str(), 0);
}
std::string GlobalData::addUidString(std::string str)
{
	return myId + str;
}

void GlobalData::setLoginType(int val)
{
	logintype = val;
	UserDefault::getInstance()->setIntegerForKey("logintype", val);
}
int GlobalData::getLoginType()
{
	return UserDefault::getInstance()->getIntegerForKey("logintype", 0);;
}

bool GlobalData::getIsHomeAutoPopGift()
{
	return isHomeAutoPopGift;
}
void GlobalData::setIsHomeAutoPopGift(bool val)
{
	isHomeAutoPopGift = val;
}

void GlobalData::sortAchieveDatabyId()
{
	sort(GlobalData::achieveData.begin(), GlobalData::achieveData.end(), sortByIdCb);
}

bool GlobalData::sortByIdCb(Achieve a, Achieve b)
{
	if (a.id < b.id)
		return true;
	return false;
}

int GlobalData::getBuyTextType()
{
	return buyTextType;
}
void GlobalData::setBuyTextType(int val)
{
	buyTextType = val;
}

const char* GlobalData::getShareUrl()
{
	return shareurl.c_str();
}

void GlobalData::setShareUrl(std::string val)
{
	shareurl = val;
}

void GlobalData::setShareImgPath(std::string val)
{
	shareimgpath = val;
}

const char* GlobalData::getShareImgPath()
{
	return shareimgpath.c_str();
}

void GlobalData::setIsBuyGift(int index, bool val)
{
	isGiftBuy[index] = val;
	std::string str = StringUtils::format("g%dbuy", index);
	UserDefault::getInstance()->setBoolForKey(addUidString(str).c_str(), val);
}

bool GlobalData::getIsBuyGift(int index)
{
	return isGiftBuy[index];
}

void GlobalData::setChapterUnlock(int chapter, bool val)
{
	vec_isunlock[chapter] = val;
	std::string str = StringUtils::format("unlock%d", chapter);
	UserDefault::getInstance()->setBoolForKey(addUidString(str).c_str(), val);
}

bool GlobalData::getChapterUnlock(int chapter)
{
	return vec_isunlock[chapter];
}

void GlobalData::setShowChapterUnlock(int chapter, bool val)
{
	vec_isShowUnlockAnim[chapter] = val;
	
	std::string str = StringUtils::format("showunlock%d", chapter);
	UserDefault::getInstance()->setBoolForKey(addUidString(str).c_str(), val);
}

bool GlobalData::getShowChapterUnlock(int chapter)
{
	return vec_isShowUnlockAnim[chapter];
}

int GlobalData::getCModeCount(ChallengeMode mode)
{
	return cmcount[mode];
}

void GlobalData::setCModeCount(ChallengeMode mode, int val)
{
	cmcount[mode] = val;

	std::string cmstr = StringUtils::format("cm%dcount", mode);
	UserDefault::getInstance()->setIntegerForKey(addUidString(cmstr).c_str(), val);
}

void GlobalData::setCModeYearOfDay(ChallengeMode mode, int val)
{
	std::string cmstr = StringUtils::format("cm%dday", mode);
	UserDefault::getInstance()->setIntegerForKey(addUidString(cmstr).c_str(), val);
}

int GlobalData::getCModeYearOfDay(ChallengeMode mode)
{
	std::string cmstr = StringUtils::format("cm%dday", mode);
	return UserDefault::getInstance()->getIntegerForKey(addUidString(cmstr).c_str(), -1);
}

void GlobalData::setBestCMScore(ChallengeMode mode, int val)
{
	bestscore[mode] = val;
	std::string scorestr = StringUtils::format("best%sscore", scorenamestr[mode]);
	UserDefault::getInstance()->setIntegerForKey(addUidString(scorestr).c_str(), val);
}

int GlobalData::getBestCMScore(ChallengeMode mode)
{
	return bestscore[mode];
}

void GlobalData::setIsFirst(ChallengeMode mode, int index, bool val)
{
	std::string str = StringUtils::format("cm%d_%d", mode, index);
	UserDefault::getInstance()->setBoolForKey(addUidString(str).c_str(), val);
}

bool GlobalData::getIsFirst(ChallengeMode mode, int index)
{
	std::string str = StringUtils::format("cm%d_%d", mode, index);
	return UserDefault::getInstance()->getBoolForKey(addUidString(str).c_str(), true);
}

void GlobalData::setIsCurStagePass(bool val)
{
	isCurStagePass = val;
}

bool GlobalData::getIsCurStagePass()
{
	return isCurStagePass;
}

const char* GlobalData::getPostData()
{
	if (GlobalData::getMyID().length() > 0)
	{
		std::string myskillstr;
		std::string temp;
		int skillsize = sizeof (mySkillLevel) / sizeof (mySkillLevel[0]);
		for (int i = 0; i < skillsize; i++)
		{
			if (i == skillsize - 1)
				temp = StringUtils::format("%d", getMySkillLevel(i));
			else
				temp = StringUtils::format("%d,", getMySkillLevel(i));

			myskillstr.append(temp);
		}
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
		std::string achieveStr;
		sortAchieveDatabyId();
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
		postdata = StringUtils::format("%s;%d;%s;%d;%d;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d;%s;%s;%d;%d", getMyID().c_str(), getMyTotalSkillLevel(), myskillstr.c_str(), getGoldCount(), getGemCount(), getHpCount(), getMyMaxHp(),
			getPromptProp(), getRefreshProp(), getAddtimeProp(), stagestr.c_str(), getBestCMScore(CMNORMAL), getBestDefeatedBossCount(), getBestCMScore(CMBOSS), getContinueLoginDay(), getHighestStage(), getLastLoginRewardSecond(),
			achieveStr.c_str(), getMyName().c_str(), getBestCMScore(CMCLASIC), getBestCMScore(CMFLASH));
	}
	return postdata.c_str();
}

void GlobalData::saveEventsIds()
{
	std::string ids;
	for (unsigned int i = 0; i < GlobalData::EventsData.size(); i++)
	{
		std::string temp = StringUtils::format("%d", GlobalData::EventsData[i].id);
		ids.append(temp);
		ids.append("-");
	}
	UserDefault::getInstance()->setStringForKey(addUidString("eids").c_str(), ids);
}
std::string GlobalData::getEventsIds()
{
	return UserDefault::getInstance()->getStringForKey(addUidString("eids").c_str(), "");
}

MEvent* GlobalData::getEventByType(int type)
{
	for (unsigned int i = 0; i < GlobalData::EventsData.size(); i++)
	{
		MEvent mevent = GlobalData::EventsData[i];
		if (mevent.type == type)
			return &GlobalData::EventsData[i];
	}
	return NULL;
}

int GlobalData::getEventstatus(int type)
{
	int sta = 0;
	MEvent* mevent = GlobalData::getEventByType(type);
	if (mevent != NULL)
	{
		std::string key = StringUtils::format("%s-%d%d", addUidString("esta").c_str(), type, mevent->startime);
		sta = UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
	}
	return sta;
}

void GlobalData::setEventstatus(int type, int val)
{
	MEvent* mevent = GlobalData::getEventByType(type);
	if (mevent != NULL)
	{
		std::string key = StringUtils::format("%s-%d%d", addUidString("esta").c_str(), type, mevent->startime);
		UserDefault::getInstance()->setIntegerForKey(key.c_str(), val);
	}
}

std::string GlobalData::getXMLDataPath(std::string filename)
{
	if (filename.size() <= 0)
		return "";

	std::string path = getBasePath() + "/";
	std::string xmlfile = path + filename;
	bool isexist = FileUtils::getInstance()->isFileExist(xmlfile);
	if (isexist)
	{
		return xmlfile;
	}
	else
	{
		return filename;
	}
}

void GlobalData::initUnlockData()
{
	vec_isunlock.clear();
	vec_isShowUnlockAnim.clear();
	vec_unlockstars.clear();
	for (int i = 0; i < GlobalData::getMapCount(); i++)
	{
		std::string str = StringUtils::format("unlock%d", i);
		vec_isunlock.push_back(UserDefault::getInstance()->getBoolForKey(addUidString(str).c_str(), false));

		str = StringUtils::format("showunlock%d", i);
		vec_isShowUnlockAnim.push_back(UserDefault::getInstance()->getBoolForKey(addUidString(str).c_str(), false));
	}
}

std::string GlobalData::getCid()
{
	const char* str = "bbg";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "getChannelID", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		str = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
#endif

	return str;
}

int GlobalData::covertChapterStage(int decimalStage, int &stage)
{
	int stagecount = 0;
	int chapter = 0;
	for (unsigned int i = 0; i < GlobalData::vec_chapternum.size(); i++)
	{
		stagecount += GlobalData::vec_chapternum.at(i);
		if (stagecount - 1 >= decimalStage)
		{
			chapter = i;
			break;
		}
	}
	stagecount = 0;
	for (int i = 0; i < chapter; i++)
	{
		stagecount += GlobalData::vec_chapternum.at(i);
	}
	stage = decimalStage - stagecount;
	return chapter;
}

void GlobalData::setTotalStage(int val)
{
	totalstage = val;
}
int GlobalData::getTotalStage()
{
	return totalstage;
}

bool GlobalData::checkPayCode(int payindex, int assign)
{
	bool ret = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char p_str[64] = { 0 };
	sprintf(p_str, "%s", PayCode[payindex].c_str());
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/OGThranPay", "checkPCodeExist", "(Ljava/lang/String;I)Z"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, str1, assign);
	}
#endif
	return ret;
}

int GlobalData::getVersionCode()
{
	int vercode = 110;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "getVersionCode", "()I"))
	{
		vercode = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	}
#endif
	return vercode;
}

bool GlobalData::getIsUmp()
{
	return isump;
}

void GlobalData::setIsUmp(bool val)
{
	isump = val;
}

bool GlobalData::getIsRestart()
{
	return isrestart;
}
void GlobalData::setIsRestart(bool val)
{
	isrestart = val;
}
