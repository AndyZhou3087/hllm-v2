#ifndef __EVENT_LAYER_H__
#define __EVENT_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GlobalData.h"
USING_NS_CC;
class EventLayer : public Layer
{
public:
	EventLayer();
	~EventLayer();
	virtual bool init();
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onevent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(EventLayer);
	static void doEventNotice(int type);
private:
	cocos2d::ui::ScrollView* scrollview;
	cocos2d::ui::Widget* contentnode;
	cocos2d::ui::Widget* descnode;

	cocos2d::ui::ImageView* imageTitle;

	cocos2d::ui::Widget* rewardnode;
	cocos2d::ui::Widget* rewardnode1;//Ö»ÓÐÒ»¸öÀñ°üÕ¹Ê¾
	EventListenerTouchOneByOne* listener;
	void addEvents();
	void getEventsData(float dt);
	void clickEventCallback(Ref* pSender);
	static void httpEventDataCompleteCB(std::string retdata, int code, std::string tag);
	static void httpEventNoticeCB(std::string retdata, int code, std::string tag);
	void updateFinishProgress(float dt);
	MenuItemSprite *pItem[MAXEVENTTYPE + 1];
	int tag;
	MEvent curEventData;
	cocos2d::ui::Widget* finishbg;
	cocos2d::ui::LoadingBar* finishbar;

	cocos2d::ui::Text* finishtext;

	void selectEvent(Node* btnnode);
};
extern EventLayer* g_EventLayer;
#endif