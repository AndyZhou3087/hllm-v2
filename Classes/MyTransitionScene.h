
#ifndef __MYTRANSITION_SCENE_H__
#define __MYTRANSITION_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class MyTransitionScene : public Layer
{
public:
	MyTransitionScene();
	~MyTransitionScene();
	virtual bool init();
	static Scene* scene();
	virtual void onExit();
	CREATE_FUNC(MyTransitionScene);
	virtual void onEnterTransitionDidFinish();
	static bool isloadingUpgrade;

private:
	Scene * themScene;
	int m_serverChapter;
	void loadthemeScene(float dt);
	void loadSDPng(int index);
	void loadBossPng(int index);
	void loadBossPlist(cocos2d::Texture2D* texture);
	void LoadpSDList(cocos2d::Texture2D* texture);
	void loadLayer(float dt);
};

#endif
