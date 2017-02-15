#ifndef __TASK_LAYER_H__
#define __TASK_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GlobalData.h"
USING_NS_CC;
class TaskLayer : public Layer
{
public:
	TaskLayer();
	~TaskLayer();
	virtual bool init();
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void tagBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static void sortTaskData();
	static void sortAchieveData();
	static bool byfinish(Achieve a, Achieve b);
	void removeRwdLayer(float dt);
	CREATE_FUNC(TaskLayer);
	void showRwdLayer(std::string path, int count);
	static bool isgeting;
private:
	cocos2d::ui::ScrollView* taskscrollview;
	cocos2d::ui::ScrollView* achivescrollview;
	cocos2d::ui::Widget* tagBtn0;
	cocos2d::ui::Widget* tagBtn1;
	void updatelblcount();
	cocos2d::ui::TextBMFont* tasklbl;
	cocos2d::ui::TextBMFont* achivelbl;
	LayerColor* color;
	EventListenerTouchOneByOne* listener;
	void showParticle(float dt);
	void loadAchiveScrollView();
	int selecttag;
	bool isloadedachive;
};
extern TaskLayer* g_TaskLayer;
class TaskItem : public Sprite
{
public:
	TaskItem();
	~TaskItem();
	bool init(Achieve *data);
	void updatedata(Achieve *data);
	static TaskItem* createWithData(Achieve *data);
	int status;//-1:已领取；0可以领取；1进行中
private:
	Label* mname;
	Label* mcount;
	Label* mdesc;
	Label* progresslbl;
	Sprite* rwdSprite;
	Sprite* finishStatusSprite;
	Sprite* getbtnword;
	MenuItemSprite* getbtn;
	ProgressTimer* progressbar;
	Achieve *mdata;
	void getCallback(Ref* sender);
};
#endif