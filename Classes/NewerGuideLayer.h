

#ifndef __NEWERGUIDELAYER__
#define __NEWERGUIDELAYER__

#include "cocos2d.h"
USING_NS_CC;

#define NEWERLAYERZOER 999
class NewerGuideLayer : public Layer
{
public:
	NewerGuideLayer();
	~NewerGuideLayer();
    bool init(int type);
	static NewerGuideLayer* create(int step);
	void setData(std::vector<Node*> hiligt);
	void removeSelf();
	int getType();
private:
	static int _step;
	Layer* color;
	std::vector<Node*> _hightNodes;
	std::vector<int> _Ozoder;
	Label* _guideText;
	ClippingNode* _clipNode;
	Sprite* _blockcontent;
	bool isCanClick;

	void canClick(float dt);
	Sprite* guidetextbox;
	Sprite* heroGuider;
	Sprite* _sj;
	bool iscanclick;
};
extern NewerGuideLayer* g_NewerLayer;
#endif
