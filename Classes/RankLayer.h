#ifndef __RANK_LAYER_H__
#define __RANK_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GlobalData.h"
USING_NS_CC;
class RankLayer : public Layer
{
public:
	RankLayer();
	~RankLayer();
	virtual bool init();
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void cmbtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(RankLayer);
	static void showRankData();
	void getRankData(float dt);
	static int rankMode;
private:
	static void httpCompleteCB(std::string retdata, int code, std::string tag);
	static void httpDownImgCB(std::string retdata, int code, std::string tag);
private:
	static cocos2d::ui::ScrollView* scrollview;
	static Label * desclbl;
	static cocos2d::ui::TextBMFont * myranklbl;
	float pasttime;

};
class RankItem : public Sprite
{
public:
	RankItem();
	~RankItem();
	bool init(RankData data, int index);
	static RankItem* createWithData(RankData data, int index);
	void upataheadimg(int index);

private:
	RankData mdata;
	Sprite* head;
};
#endif