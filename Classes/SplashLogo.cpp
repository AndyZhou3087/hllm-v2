#include "SplashLogo.h"
#include "LoadingScene.h"

USING_NS_CC;

Scene* SplashLogo::scene()
{
    // 'scene' is an autorelease object
	Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
	SplashLogo *layer = SplashLogo::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool SplashLogo::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	_bg = Sprite::create("splashlogo.jpg");
	_bg->setPosition(Vec2(270, 480));
	this->addChild(_bg);
	Sprite* logo;
#ifdef LZLOGO
	logo = Sprite::create("lzlgo.jpg");
	logo->setPosition(Vec2(270, 650));
	this->addChild(logo);
#else
	logo = Sprite::create("jzjm_logo.png");
	logo->setPosition(Vec2(270, 650));
	logo->setScale(0.9f);
	this->addChild(logo);
#endif
	return true;
}

void SplashLogo::onEnterTransitionDidFinish()
{
	this->scheduleOnce(schedule_selector(SplashLogo::showNextScene), 2.0f);
}
void SplashLogo::showNextScene(float dt)
{
	Director::getInstance()->replaceScene(LoadingScene::scene());
}