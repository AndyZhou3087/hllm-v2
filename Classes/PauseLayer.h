#ifndef __PAUSE_LAYER_H__
#define __PAUSE_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
class PauseLayer : public Layer
{
public:
	PauseLayer();
	~PauseLayer();
	virtual bool init();
	virtual void onExit();
	void musicOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void soundOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void gohome(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void restartGame(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(PauseLayer);
private:
	cocos2d::ui::Button* _musicBtn;
	cocos2d::ui::Widget* _musicoff;
	cocos2d::ui::Button* _soundBtn;
	cocos2d::ui::Widget* _soundoff;
};
#endif