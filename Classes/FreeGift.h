#ifndef __FREELAYER_H__
#define __FREELAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;
class FreeGift : public Layer
{
public:
	FreeGift();
	~FreeGift();
	virtual bool init();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(FreeGift);
	static int AnimType;
private:
	Node* _layer;
	cocos2d::ui::Widget* getBtn;
	void delayShowNewerGuide(float dt);
};
#endif