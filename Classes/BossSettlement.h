#ifndef __BOSSSETTLEMENT_H__
#define __BOSSSETTLEMENT_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;

class BossSettlement : public Layer
{
public:
	BossSettlement();
	~BossSettlement();
	CREATE_FUNC(BossSettlement);

private:
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void parseBossRewardConfig();
private:
	int bossbestscore;
	Node* _layer;
	cocos2d::ui::Widget* startBtn;
	std::vector<int> rewards;
};
#endif