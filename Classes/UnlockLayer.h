#ifndef __UNLOCKLAYER_H__
#define __UNLOCKLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
class UnlockLayer : public Layer
{
public:
	UnlockLayer();
	~UnlockLayer();
	 bool init(int chapter, int starsnum);
	void onUnlock(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onCllect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static UnlockLayer* create(int chapter, int starsnum);
	static int AnimType;
private:
	int m_chapter;
};
#endif