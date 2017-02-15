#ifndef __SKILLGROW_LAYER_H__
#define __SKILLGROW_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
class SkillGrowLayer : public Layer
{
public:
	SkillGrowLayer();
	~SkillGrowLayer();
	virtual bool init();
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void growUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void skillbtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void showHeroNormalAmin(float t);
	CREATE_FUNC(SkillGrowLayer);
private:
	Node * _layer;
	cocos2d::ui::Widget* _growUpBtn;
	cocos2d::ui::Button* _closeBtn;
	cocos2d::ui::Widget* _select;
	cocos2d::ui::Widget* selectlock;
	cocos2d::ui::ImageView* selectprop;
	cocos2d::ui::TextBMFont* _needGoldlbl;
	int skilltype;
	int needGold;
	cocos2d::ui::Widget* _smallgoldicon;
	void delayShowNewerGuide(float dt);
	cocos2d::ui::Text* desctext0;
	cocos2d::ui::Text* desctext1;
	cocos2d::ui::Text* nextlvtext;
	cocos2d::ui::ImageView* nameimage;
	void updatadesc(int level);
	cocostudio::timeline::ActionTimeline* heroAction;
	cocos2d::ui::Widget* lvmaxtext;
	cocos2d::ui::Widget* growbtnpropbox;
	cocos2d::ui::Widget* s_tipbox1;
	void selectskill(Node* btnnode);
};
extern SkillGrowLayer* g_skillGrowlayer;
#endif