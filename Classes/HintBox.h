

#ifndef __HINTBOX__
#define __HINTBOX__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class HintBox : public Layer
{
public:
    bool init(std::string text, int type);
	static HintBox* create(std::string content, int type);
	void removSelf();
private:
	int _type;
	cocos2d::ui::Scale9Sprite* boxbg;
};

#endif
