#ifndef __STAGE_H_
#define __STAGE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

class Stage
{
public:
	Stage(Node* node);
	~Stage();
	void setStar(int num, bool isboss = false);
	void hilight();
	void disable();
	void nomal();
	void setStageNum(int stage);
	void showLock(int starnum);
	void removeLock();
private:
	Stage();
	void initData();
	Node* m_node;
	cocos2d::ui::ImageView* m_stageIcon;
	cocos2d::ui::TextBMFont* m_stagenumlbl;
	cocos2d::ui::Widget* m_star[3];
	cocos2d::ui::Widget* m_starbg[3];
	int m_starnum;
	bool m_isboss;
	Sprite* lockNode;
};

#endif
