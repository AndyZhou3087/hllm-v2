
#ifndef __THEME_SCENE_H__
#define __THEME_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Const.h"
#include "UpgradeManager.h"
#include "UpgradeLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

const int TOUCH_DISTANCE = 50;

class ThemeScene : public Layer, public ScrollViewDelegate, public UpgradeManagerDelegateProtocol
{
public:
	ThemeScene();
	~ThemeScene();
    static cocos2d::Scene* Scene();
  
    virtual bool init();
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(ThemeScene);
	void delayShowSureStageLayer(float dt);
	void openSureStageLayer(float dt);
	static bool isShowingUnlock;
	void showHereMove(int stage);
	void checkShowUnlockAnim(float dt);
	void delayShowNewerGuide(float t);
	void restartShowSureStageLayer(float dt);
private:
	Layer* commonLayer;
	Size visibleSize;
	int offset = 0;//¹ö¶¯Æ«ÒÆ
	Layer* _scrolllayer;
	Sprite* yun;
	Layer *_spritesContainer;
    ScrollView *scrollView;
	Sprite* here;
	int themecount;
	int maxs;
	MenuItemSprite *upgradebtn;
	UpgradeManager* upgradeManager;
	UpgradeLayer* upgradeLayer;
	Node* hereqqNode;
	cocostudio::timeline::ActionTimeline* hereqqAction;
	Node* stageboomnode;

private:
	void addScrollView();
	void scrollViewDidScroll(ScrollView* view);

	void onTouchEnded(Touch*touch,Event *unused_event);
	bool onTouchBegan(Touch * touch,Event *unused_event);//¿ªÊ¼×´Ì¬
    void onTouchMoved(Touch *touch,Event *unused_event);//ÒÆ¶¯×´Ì¬
	void clickStageCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void doBYData();
	void checkMyHead(float dt);
	void showMyHeadAnim(Vec2 pos);

	void removeUnlockAnim(float dt);
	void ShowUnlockAnim();
	void showStageboom(Vec2 pos);
	void delayShowStageboom(float dt);
	void HideStageBoom(float dt);
	void parseUnlockstarsXml(std::string filename);
	void parseChapterNum(std::string filename);
	void delayShowParticle(float dt);
	void delayShowHeadUpAndDown(float dt);
	void getUpdateCallback(Ref* pSender);
	void onError(UpgradeManager::ErrorCode errorCode);
	void onProgress(double downedbytesize, double totalbytesize);
	void onSuccess();
};

extern vector<Sprite*> mapbg;
extern vector<Node*> mapStageLayer;
extern vector<Node*> bossNormalLayer;
extern vector<Node*> bossHitedLayer;
extern ThemeScene* g_ThemeScene;
extern Node* heroAminNode;
#endif



