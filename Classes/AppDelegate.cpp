#include "AppDelegate.h"
#include "LoadingScene.h"
#include "SoundManager.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "Const.h"
#include "SplashLogo.h"
#include "SuccLayer.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
#ifdef USENETWORK
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	SuccLayer::updataUserData();
#endif
#endif
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}
// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview->setFrameSize(540.f, 960.f);
#endif
	glview->setDesignResolutionSize(540.f, 960.f, ResolutionPolicy::EXACT_FIT);
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    register_all_packages();

    // create a scene. it's an autorelease object

	Scene *scene = SplashLogo::scene();

	//return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
	    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
	
	if (g_gameScene != NULL)
	{
		g_gameScene->doGamePause();
	}

	SoundManager::sharedManager()->pauseBackMusic();
	Director::getInstance()->stopAnimation();
#ifdef USENETWORK
	SuccLayer::updataUserData();
#endif
	//Director::getInstance()->pause();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
	//Director::getInstance()->resume();
	SoundManager::sharedManager()->resumeBackMusic();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
