#ifndef __LOGINREWARD_LAYER_H__
#define __LOGINREWARD_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;

class LoginRewardLayer : public Layer
{
public:
	LoginRewardLayer();
	~LoginRewardLayer();
	virtual bool init();
	virtual void onExit();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(LoginRewardLayer);
	static int AnimType;
private:
	void getOKAnim(const char* pic, Vec2 spoint, Vec2 dpoint);
	void closeSelf(float dt);
	void changeGray(Node* node);
	void removeGray(Node* node);

	void delayShowNewerGuide(float t);
	cocos2d::ui::Button* getBtn;
	cocos2d::ui::Widget*  bgitemselect;
	cocos2d::ui::Widget*  getIcon;
	int logindays;
	int count;
};

//extern Node* g_LoginRewardLayer;
#endif