#include "SettingLayer.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "HintBox.h"
#include "CommonLayer.h"
#include "LoadingScene.h"
#include "WaitingProgress.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif


int SettingLayer::setChangelogin = LOGIN_NONE;
SettingLayer::SettingLayer()
{
	setChangelogin = LOGIN_NONE;
}


SettingLayer::~SettingLayer()
{
	setChangelogin = LOGIN_NONE;
}

bool SettingLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	Node * layer = CSLoader::createNode("settingLayer.csb");
	addChild(layer);

	_musicBtn = (cocos2d::ui::Button*)layer->getChildByName("settingmusicbtn");
	_musicBtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::musicOption, this));
	_musicoff = (cocos2d::ui::Widget*)layer->getChildByName("settingmusicoff");

	bool musicon = UserDefault::getInstance()->getBoolForKey("ismusicon", true);
	_musicoff->setVisible(!musicon);
	_musicBtn->setBright(musicon);

	_soundBtn = (cocos2d::ui::Button*)layer->getChildByName("settingsoundbtn");
	_soundBtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::soundOption, this));
	_soundoff = (cocos2d::ui::Widget*)layer->getChildByName("settingsoundoff");

	bool soundon = UserDefault::getInstance()->getBoolForKey("issoundon", true);
	_soundoff->setVisible(!soundon);
	_soundBtn->setBright(soundon);

	cocos2d::ui::EditBox* editName = cocos2d::ui::EditBox::create(Size(200, 32), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("blank.png"));
	editName->setAnchorPoint(Vec2(0, 0.5));
	editName->setPosition(Point(183, 635));
	editName->setFontColor(Color3B::WHITE);
	editName->setPlaceHolder("请输入昵称:（回车键确认输入，返回关闭）");
	editName->setPlaceholderFontColor(Color3B::WHITE);
	editName->setMaxLength(14);
	editName->setTag(0);
	editName->setText(GlobalData::getMyName().c_str());
	//editName->setReturnType(EditBox::KeyboardReturnType::DONE);
	editName->setDelegate(this);
	layer->addChild(editName);

	cocos2d::ui::Widget* motifyBtn = (cocos2d::ui::Widget*)layer->getChildByName("settingmotifybtn");
	motifyBtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::motifyName, this));

	cocos2d::ui::Text* myid = (cocos2d::ui::Text*)layer->getChildByName("idtext");

	if (GlobalData::getMyID().compare(0, 2, "loc", 0, 2) == 0)
		myid->setString(CommonFuncs::gbk2utf("请检查网络"));
	else 
		myid->setString("ID:" + GlobalData::getMyID());

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("close");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::close, this));

	cocos2d::ui::Widget* qqloginbtn = (cocos2d::ui::Widget*)layer->getChildByName("loginqqbtn");
	qqloginbtn->setTag(LOGIN_QQ);
	qqloginbtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::login, this));

	cocos2d::ui::Widget* wxloginbtn = (cocos2d::ui::Widget*)layer->getChildByName("loginwxbtn");
	wxloginbtn->setTag(LOGIN_WX);
	wxloginbtn->addTouchEventListener(CC_CALLBACK_2(SettingLayer::login, this));

#ifndef THIRDLOGIN
	qqloginbtn->setVisible(false);
	wxloginbtn->setVisible(false);
	_musicBtn->setPosition(qqloginbtn->getPosition());
	_musicoff->setPosition(_musicBtn->getPosition());
	_soundBtn->setPosition(wxloginbtn->getPosition());
	_soundoff->setPosition(_soundBtn->getPosition());
#endif

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	layer->setAnchorPoint(Vec2(0.5f, 0.5f));
	layer->setPosition(Vec2(270, 480));
	layer->setScale(0.1f);
	layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));

	_myhead = (cocos2d::ui::ImageView*)layer->getChildByName("head");

	cocos2d::ui::Widget* settingheadbox = (cocos2d::ui::Widget*)layer->getChildByName("settingheadbox");

	int logintype = GlobalData::getLoginType();
	if (logintype > 0)
	{
		std::string headimgname = "qhead00.png";
		if (logintype == 2)
			headimgname = "wxhead00.png";

		string path = FileUtils::getInstance()->getWritablePath() + headimgname;
		bool isHasfile = FileUtils::getInstance()->isFileExist(path);
		if (isHasfile)
		{
			_myhead->loadTexture(path, cocos2d::ui::TextureResType::LOCAL);
			_myhead->setContentSize(Size(78,78));
			_myhead->setPosition(Vec2(settingheadbox->getPositionX() - 1, settingheadbox->getPositionY() + 1));
			settingheadbox->setLocalZOrder(1);
		}
		editName->setEnabled(false);
		motifyBtn->setVisible(false);
	}
	else
	{
		editName->setEnabled(true);
		motifyBtn->setVisible(true);
	}
	return true;
}

void SettingLayer::login(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	cocos2d::ui::Widget* nodebtn = (cocos2d::ui::Widget*)pSender;
	int tag = nodebtn->getTag();
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		setChangelogin = tag;
		WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 10000, "waitbox");
		LoadingScene::login(tag);
	}
}

void SettingLayer::musicOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (!_musicoff->isVisible())
		{
			SoundManager::sharedManager()->setIsMusicOn(false);
			_musicoff->setVisible(true);
			SoundManager::sharedManager()->pauseBackMusic();
			UserDefault::getInstance()->setBoolForKey("ismusicon", false);
			_musicBtn->setBright(false);
		}
		else
		{
			SoundManager::sharedManager()->setIsMusicOn(true);
			_musicoff->setVisible(false);

			if (SoundManager::sharedManager()->getMusicPlayed())
				SoundManager::sharedManager()->resumeBackMusic();
			else
				SoundManager::sharedManager()->playBackMusic("sound/bg.mp3");
			UserDefault::getInstance()->setBoolForKey("ismusicon", true);
			_musicBtn->setBright(true);
		}
	}
}

void SettingLayer::soundOption(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
		if (!_soundoff->isVisible())
		{
			SoundManager::sharedManager()->setIsSoundOn(false);
			_soundoff->setVisible(true);
			UserDefault::getInstance()->setBoolForKey("issoundon", false);
			_soundBtn->setBright(false);
		}
		else
		{
			SoundManager::sharedManager()->setIsSoundOn(true);
			_soundoff->setVisible(false);
			UserDefault::getInstance()->setBoolForKey("issoundon", true);
			_soundBtn->setBright(true);
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
		}
	}
}

void SettingLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}


void SettingLayer::motifyName(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
}


void SettingLayer::textFiledEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType eventtype)
{
	switch (eventtype)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:

		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:

		break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:

		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:

		break;
	default:
		break;
	}
}


void SettingLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{

}

void SettingLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	if (editBox->getTag() == 0)
	{
		std::string myname = GlobalData::getMyName();
		if (myname.compare(editBox->getText()) != 0)
		{
			int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			char p_str[64] = { 0 };
			sprintf(p_str, "%s", editBox->getText());
			JniMethodInfo methodInfo;
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/hllm", "isSensitiveWord", "(Ljava/lang/String;)I"))
			{
				jstring str1 = methodInfo.env->NewStringUTF(p_str);
				ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, str1);
			}
#endif
			if (ret == 0)
				GlobalData::setMyName(editBox->getText());
			else
			{
				editBox->setText(myname.c_str());
			}
		}
	}
}

void SettingLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
}

void SettingLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{
}
