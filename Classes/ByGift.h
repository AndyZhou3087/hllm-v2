#ifndef __BYGIFTLAYER_H__
#define __BYGIFTLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;

class ByGift : public Layer
{
public:
	ByGift();
	~ByGift();
	virtual bool init();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void buy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(ByGift);
	static int AnimType;
	static int checkprice();
	static bool isClickPop;
private:
	Node* _layer;
	cocos2d::ui::Widget* getBtn;
	int pindex;
};

#endif