#ifndef __MESSAGEDETAILS_LAYER_H__
#define __MESSAGEDETAILS_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GlobalData.h"
USING_NS_CC;
class MessageDetailsLayer : public Layer
{
public:
	MessageDetailsLayer();
	~MessageDetailsLayer();
	bool initwithtype(int index);
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static MessageDetailsLayer* create(int index);
private:
	EventListenerTouchOneByOne* listener;
	cocos2d::ui::Widget* getBtn;
	Message* message;
	std::vector<int> vec_prize;
	static void updataStatusToServer(Message* message);
	static void httpCompleteCB(std::string retdata, int code, std::string tag);
	
};

#endif