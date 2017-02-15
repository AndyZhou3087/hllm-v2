#ifndef __FOURTYPELAYER_H__
#define __FOURTYPELAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;
class FourTypeGift : public Layer
{
public:
	FourTypeGift();
	~FourTypeGift();
	virtual bool init();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(FourTypeGift);
	static int AnimType;
	static int _type;
	static bool isshowinggift;
	static bool isClickPop;
	void showAnim(float dt);
private:
	Node* _layer;
	cocos2d::ui::Widget* getBtn;
};
#endif