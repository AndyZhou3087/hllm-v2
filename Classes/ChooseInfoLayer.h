#ifndef __CHOOSEINFO_LAYER_H__
#define __CHOOSEINFO_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
class ChooseInfoLayer : public Layer
{
public:
	ChooseInfoLayer();
	~ChooseInfoLayer();
	virtual bool init();
	void onStage(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onClasic(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(ChooseInfoLayer);
	static int AnimType;
private:
};
#endif