

#ifndef __TOPNODE__
#define __TOPNODE__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;


class TopComNode : public Layer
{
public:
    virtual bool init();
	virtual void update(float dt);
	CREATE_FUNC(TopComNode);
private:
	cocos2d::ui::Widget* m_hpicon;
	cocos2d::ui::Widget* m_diamondBtn;
	cocos2d::ui::Widget* m_goldBtn;
	cocos2d::ui::Widget* m_HpBtn;


	cocos2d::ui::TextBMFont* m_time;//时间
	cocos2d::ui::TextBMFont* m_hpNum;//体力
	cocos2d::ui::TextBMFont* m_goldNum;//金币
	cocos2d::ui::TextBMFont* m_diamondNum;//钻石

	cocos2d::ui::Widget* timeBg;
	int _powerSec;
	int _powertime;//时间
	int _powercount;//体力
	int _goldcount;//金币
	int _diamondcount;//钻石
	int _nowday;
	int maxHpCount;

	float _visualhp;
	float _visualgold;
	float _visualgem;
private:
	void initTime();//初始化时间
	void Save();//保存
	void updateTime(float t);//更新时间
	void HpAction(float t);
	void updateUI();

	void updateHp(float dt);
	void updateGold(float dt);
	void updateGem(float dt);
	void Hp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Gold(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Diamond(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
