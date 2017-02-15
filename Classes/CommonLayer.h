#ifndef __COMMON_LAYER_H__
#define __COMMON_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
#include "CommonFuncs.h"
USING_NS_CC;

class CommonLayer : public Layer
{
public:
	enum DISPLAYNUMTYPE
	{
		HPTYPE,
		GOLDTYPE,
		DIAMONDTYPE
	};
	CommonLayer();
	~CommonLayer();
	virtual bool init();

	void more(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void share(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onevent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void vip(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updateRedPoint(float dt);
	static void Action(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void vipAnim(float dt);
	CREATE_FUNC(CommonLayer);
	void updateUI();
	void delayShowNewerGuide(float t);
	bool isnewerguiding;
	void setMorebtnVisible(bool val);
	void setMoreCMbtnVisible(bool val);
	void delayShowGift(float dt);
	static void showGift(int pos);
	bool hasNewEvent();
	void checkShowNewerGuide();
	void AutoclickMoreBtn();
	void delayShowLoginReward(float dt);
private:
	cocos2d::ui::Widget* light;
	cocos2d::ui::Widget* m_onsaleGiftBtn;
	cocos2d::ui::Widget* m_vipBtnNode;
	cocos2d::ui::Widget* m_vipBtn;
	cocos2d::ui::Widget* m_moreBtn;
	cocos2d::ui::Widget* m_shareBtn;
	cocos2d::ui::Widget* m_eventBtn;
	cocos2d::ui::Widget* m_shopBtn;


	cocos2d::ui::Widget* onsaleRedPoint;
	cocos2d::ui::Widget* moreRedPoint;
	cocos2d::ui::Widget* eventRedPoint;
	cocos2d::ui::Widget* vipRedPoint;


	cocos2d::ui::Widget* m_BossChallengeBtn;
	cocos2d::ui::Widget* m_ClasicBtn;
	cocos2d::ui::Widget* m_flashBtn;
	cocos2d::ui::Widget* clasicRedPoint;
	cocos2d::ui::Widget* flashRedPoint;

	void BossChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void ClasicChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void FlashChallenge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void Shop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	Node * _bgNode;

	void resetBtnPos();
	void showCMStartFrame(int mode);
};
extern CommonLayer* g_commomLayer;
#endif