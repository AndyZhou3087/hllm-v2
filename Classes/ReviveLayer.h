#ifndef __REVIVELAYER_H_
#define __REVIVELAYER_H_
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
USING_NS_CC;

class ReviveLayer : public Layer
{
public:
	ReviveLayer();
	~ReviveLayer();
	virtual bool init();
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(ReviveLayer);
	static int AnimType;
private:
	cocos2d::ui::Widget* m_restart;
	cocos2d::ui::Widget* m_close;
	Node * _failNode;
	void FailExit(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void restart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSale(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void delayShowBossSettlement();
	void delayPlayWinSound(float dt);
	void enterBossSettlementLayer(float dt);
	void delayShowReviveGift(float dt);

};
#endif
