#ifndef __COMFIRMBUYLAYER_H__
#define __COMFIRMBUYLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
class ComfirmBuyLayer : public Layer
{
public:
	ComfirmBuyLayer();
	~ComfirmBuyLayer();
	virtual bool init();
	CREATE_FUNC(ComfirmBuyLayer);
private:
	void ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif