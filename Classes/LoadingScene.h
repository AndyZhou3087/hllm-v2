#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "CommonLayer.h"
#include "Const.h"
#include "MyParticle.h"
USING_NS_CC;
using namespace std;
class LoadingScene : public Layer
{
public:
	LoadingScene();
	~LoadingScene();

	virtual bool init();
	virtual void update(float dt);
	static cocos2d::Scene* scene();
	void LoadpList(cocos2d::Texture2D* texture);
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(LoadingScene);
	static int folderCount(string folderPath);
	static void getUserData(std::string localid, std::string nickname, std::string headurl, std::string channelid, std::string pkgname);
	static void parseMessageJson(std::string retdata);
	static void parseEventJson(std::string retdata);
	static void cancelThirdLogin();
	static void login(int logintype);
private:
	ProgressTimer* loadingBar;
	int m_pngType;
	int m_serverChapter;
	Sprite* loadprogress;
	void parseSkillConfig();
	void parseAchieveConfig(std::string filepath);
	void getPayData();
	void loadPng(float dt);
	void loadnextListPng(int index);
	void loadSDPng(int index);
	void loadBossPng(int index);
	void loadBossPlist(cocos2d::Texture2D* texture);
	void LoadpSDList(cocos2d::Texture2D* texture);
	void updateBarLight();
	void delayShowAnim(float dt);
	void delayLoadData(float dt);
	void delayEnterStartLayer(float dt);
	void preLoadLayer(float dt);
	void delayLoadHeroPng(float dt);
	void loadHeroPlist(cocos2d::Texture2D* texture);
	void setLoginData();
	void getMessageData(float dt);

	void getEventData(float dt);

	void sendUserAnalyticsData();

	void parseTaskConfig(std::string filepath);
	void getAchieveData();
	void getTaskData();

	void loadUserData(float dt);
	static std::string getUpgradeTime();

	void getUmpData();
	static void httpCompleteCB(std::string retdata, int code, std::string tag);
	static void httpMessageDataCompleteCB(std::string retdata, int code, std::string tag);
	static void httpEventDataCompleteCB(std::string retdata, int code, std::string tag);
	static void httpUmpDataCompleteCB(std::string retdata, int code, std::string tag);
};
#endif