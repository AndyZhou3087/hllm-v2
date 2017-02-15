#ifndef __VSLAYER_H__
#define __VSLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
class VSLayer : public Layer
{
public:
	VSLayer();
	~VSLayer();
	virtual bool init();
	CREATE_FUNC(VSLayer);
	void setBoss(int boss);
private:
	void removeSelf(float dt);
	cocos2d::ui::ImageView* boss;
};
#endif