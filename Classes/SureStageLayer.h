#ifndef __SURESTAGE_LAYER_H__
#define __SURESTAGE_LAYER_H__
#include "cocos2d.h"
#include "Const.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GlobalData.h"
#include "GameScene.h"
USING_NS_CC;

class SureStageLayer : public  Layer
{
public:
	SureStageLayer();
	~SureStageLayer();
	virtual bool init();
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	void PlaySpecially();
	void ReplaceScene(float t);
	static bool isShowing;
	CREATE_FUNC(SureStageLayer);
private:
	Node * pNode;
	Size visibleSize;
	bool isClick;
	void SureStart(ChallengeMode mode);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void start(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void selectProp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void passstage(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void passstage_close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void passstage_ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	int ReadConfig(string fileName);

	void parseCPConfig(string filepath);

	void parseFlashRuleConfig(string filepath);

	void parseBossSkillConfig(string filepath);

	int _timeNum;
	int _useHpCount;
	cocos2d::ui::Widget* m_sureStartBtn;
	cocos2d::ui::Widget* propBtn[3];
	cocos2d::ui::Widget* goldIcon[3];
	cocos2d::ui::TextBMFont* goldNum[3];
	cocos2d::ui::Widget* isbuyText[3];
	cocos2d::ui::Widget* freeText[3];
	cocos2d::ui::Widget* getgou[3];
	int usenum[3];
	Node *passlayernode;
	Layer* passColorLayer;
	static Label* desclbl;
	static cocos2d::ui::ScrollView* scrollview;
	void getStageRankData(int stage);
	static void httpCompleteCB(std::string retdata, int code, std::string tag);
	static void httpDownImgCB(std::string retdata, int code, std::string tag);
	static void showRankData();
	void isAnimFinished(float dt);
	void delayShowNewerGuide(float t);
	void showStartGuide();

	void delayShowSelectAnim(float dt);
	int selectindex;
	cocos2d::ui::TextBMFont* tipsbmlbl;
	ChallengeMode cmode;
	bool isPropGuide[3];
};

class SRankItem : public Sprite
{
public:
	SRankItem();
	~SRankItem();
	bool init(RankData data, int index);
	static SRankItem* createWithData(RankData data, int index);
	void upataheadimg(int index);
private:
	RankData mdata;
	Sprite* head;
};
#endif