

#ifndef __UPGRADE_LAYER__
#define __UPGRADE_LAYER__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;


class UpgradeLayer : public Layer
{
public:
    bool init();
	CREATE_FUNC(UpgradeLayer);
	void updataUI(double downedbyte, double totalbyte);
private:
	cocos2d::ui::Text* percentText;
	cocos2d::ui::Text* llText;
	cocos2d::ui::LoadingBar* loadingBar;
};

#endif
