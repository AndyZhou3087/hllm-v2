#ifndef __MODIFYNAME_LAYER_H__
#define __MODIFYNAME_LAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
using namespace std;
class ModifyNameLayer : public Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	ModifyNameLayer();
	~ModifyNameLayer();
	virtual bool init();
	void motifyName(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void textFiledEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType eventtype);
	CREATE_FUNC(ModifyNameLayer);

	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);

private:
	static cocos2d::ui::EditBox* editName;
	static void httpUpdateUserInfoCB(std::string retdata, int code, std::string tag);
	void showThemeScene(float dt);
};
#endif