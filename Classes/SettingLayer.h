#ifndef __SETTING_LAYER_H__
#define __SETTING_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;
class SettingLayer : public Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	SettingLayer();
	~SettingLayer();
	virtual bool init();
	void musicOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void soundOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void motifyName(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void textFiledEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType eventtype);
	void login(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	CREATE_FUNC(SettingLayer);

	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);

	static int setChangelogin;
private:
	cocos2d::ui::Button* _musicBtn;
	cocos2d::ui::Widget* _musicoff;
	cocos2d::ui::Button* _soundBtn;
	cocos2d::ui::Widget* _soundoff;
	cocos2d::ui::ImageView* _myhead;
};
#endif