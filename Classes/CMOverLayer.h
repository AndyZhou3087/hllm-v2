#ifndef __CMOVER_LAYER_H__
#define __CMOVER_LAYER_H__
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
USING_NS_CC;
class CMOverLayer : public Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // implement the "static node()" method manually
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(CMOverLayer);
	void delayPlayAnim(float dt);
	void showGiftLayer(int type);
private:
	void restart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void share(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void showStarParticle(float dt);
	void updatescore(float dt);
	void screenShot(bool b, const std::string& name);
	void showBtn();
	int getBonuseGold();
	Node* _root;
	cocos2d::ui::Widget* m_restartBtn;
	cocos2d::ui::Widget* m_shareBtn;
	static int lastbestscore;
	float _visualscore;
	cocos2d::ui::TextBMFont* scorebmlbl;
	cocos2d::ui::TextBMFont* usehpbmlbl;
};
#endif // __HELLOWORLD_SCENE_H__
