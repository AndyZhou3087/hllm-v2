#ifndef __CONTINUE_LAYER_H__
#define __CONTINUE_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "GameScene.h"
USING_NS_CC;
class ContinueLayer : public Layer
{
public:
	ContinueLayer();
	~ContinueLayer();
	virtual bool init();
	void ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(ContinueLayer);
	static int AnimType;
private:
	void resumeGame();
	void useProp(PropsType type);
	cocos2d::ui::Widget* okBtn;
	cocos2d::ui::Widget* colseBtn;

	cocos2d::ui::Widget* usetext;
	cocos2d::ui::Widget* buyusetext;
	Node * _rootlayer;
	void delayShowNewerGuide(float dt);
};
#endif