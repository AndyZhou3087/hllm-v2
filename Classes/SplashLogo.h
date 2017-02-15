#ifndef __SPLASHLOGO_H__
#define __SPLASHLOGO_H__

#include "cocos2d.h"
USING_NS_CC;
class SplashLogo : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	virtual void onEnterTransitionDidFinish();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
	CREATE_FUNC(SplashLogo);
	void showNextScene(float dt);
private:
	Sprite* _bg;
};

#endif

