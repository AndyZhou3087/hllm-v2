#ifndef __SU_LAYER_H__
#define __SU_LAYER_H__
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
USING_NS_CC;
class SuccLayer : public Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // implement the "static node()" method manually
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(SuccLayer);
	void showStarParticle(Node* node);
	static void saveData();
	void delayPlayAnim(Ref* pSender);
	static void updataUserData();
	static void httpUpdateScoreCB(std::string retdata, int code, std::string tag);
	void showGiftLayer(int type);
private:
	void WinBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void WinNext(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void ok(Ref* sender);
	void showBtn();
	void ShowNewerGuide(float dt);
	cocos2d::ui::Widget* _wintitle;
	Node* _root;
	cocos2d::ui::Widget* m_winNextBtn;
	cocos2d::ui::Widget* m_winBackBtn;
	int m_stage;
	static int lastbestscore;
	bool isfirstpass;
};
#endif // __HELLOWORLD_SCENE_H__
