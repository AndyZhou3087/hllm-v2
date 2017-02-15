#ifndef __MESSAGE_LAYER_H__
#define __MESSAGE_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GlobalData.h"
USING_NS_CC;
class MessageLayer : public Layer
{
public:
	MessageLayer();
	~MessageLayer();
	virtual bool init();
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static void sortMessageData(int tab);
	static bool bystatus(Message a, Message b);
	CREATE_FUNC(MessageLayer);
	void updataMessage();
private:
	cocos2d::ui::ScrollView* scrollview;
	EventListenerTouchOneByOne* listener;
	void clickMessageCallback(Ref* pSender);
	void addMesages();
	void getMessageData(float dt);
	static void httpMessageDataCompleteCB(std::string retdata, int code, std::string tag);
};
class MessageItem : public Sprite
{
public:
	MessageItem();
	~MessageItem();
	bool init(Message *data);
	void updatedata(Message *data);
	void get(Ref* sender);
	static MessageItem* createWithData(Message *data);

private:
	Label* title;
	Sprite* icon;
	Label* datetime;
	Sprite* status;
	Message *mdata;
};
extern MessageLayer* g_messageLayer;
#endif