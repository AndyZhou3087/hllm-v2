#include "ModifyNameLayer.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "CommonLayer.h"
#include "ThemeScene.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "HttpUtil.h"
#include "HintBox.h"


cocos2d::ui::EditBox* ModifyNameLayer::editName = NULL;
ModifyNameLayer::ModifyNameLayer()
{
}


ModifyNameLayer::~ModifyNameLayer()
{
	editName = NULL;
}
bool ModifyNameLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	Node * layer = CSLoader::createNode("ModifyNameLayer.csb");
	addChild(layer);

	editName = cocos2d::ui::EditBox::create(Size(250, 30), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("blank.png"));
	editName->setPosition(Point(290, 525));
	editName->setFontColor(Color3B::WHITE);

	if (GlobalData::getMyName().compare("") == 0 || strstr(GlobalData::getMyName().c_str(), CommonFuncs::gbk2utf("游客").c_str()) != NULL)
	{
		editName->setPlaceHolder(CommonFuncs::gbk2utf("请输入昵称").c_str());
		editName->setText("");
	}
	else
	{
		editName->setText(GlobalData::getMyName().c_str());
	}

	editName->setPlaceholderFontColor(Color3B::WHITE);
	editName->setMaxLength(14);
	
	
	//editName->setReturnType(EditBox::KeyboardReturnType::DONE);
	editName->setDelegate(this);
	layer->addChild(editName);

	cocos2d::ui::Widget* motifyBtn = (cocos2d::ui::Widget*)layer->getChildByName("okbtn");
	motifyBtn->addTouchEventListener(CC_CALLBACK_2(ModifyNameLayer::motifyName, this));

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(ModifyNameLayer::close, this));


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ModifyNameLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(ThemeScene::Scene());
	}
}


void ModifyNameLayer::motifyName(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string myname = editName->getText();

		if (myname.length() <= 0)
		{
			HintBox* hbox = HintBox::create("昵称不能为空", 0);
			Director::getInstance()->getRunningScene()->addChild(hbox, 1);
			return;
		}
		string namestr;
		const char* retstr = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo methodInfo;
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Utils", "gbkToUTF8", "(Ljava/lang/String;)Ljava/lang/String;"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(myname.c_str());
			jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, para1);

			retstr = methodInfo.env->GetStringUTFChars(jstr, 0);
		}
		namestr = retstr;
#else
		namestr = StringUtils::format("%s", myname.c_str());
#endif

		rapidjson::Document writedoc;
		writedoc.SetObject();
		rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
		writedoc.AddMember("id", rapidjson::Value(GlobalData::getMyID().c_str(), allocator), allocator);
		writedoc.AddMember("name", rapidjson::Value(namestr.c_str(), allocator), allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		writedoc.Accept(writer);
		string url;
		url.append(HTTPURL);
		url.append("llk_UpdateUserInfo");
		url.append("&param=");
		url.append(buffer.GetString());
		HttpUtil::getInstance()->doData(url, ModifyNameLayer::httpUpdateUserInfoCB);

	}
}

void ModifyNameLayer::httpUpdateUserInfoCB(std::string retdata, int code, std::string tag)
{
	rapidjson::Document doc;

	if (code == 0)
	{
		doc.Parse<0>(retdata.c_str());
		if (doc.HasParseError())  //打印解析错误
		{
		}
		else
		{
			if (doc.IsObject() && doc.HasMember("ret"))
			{
				rapidjson::Value& objectData = doc["ret"];

				std::string desc;
				if (objectData.GetInt() == 0)
				{
					GlobalData::setMyName(editName->getText());
					Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(ModifyNameLayer::showThemeScene), 1.2f);
					
					desc = "修改昵称成功";
				}
				else if (objectData.GetInt() == 313)
				{
					desc = "此昵称已被使用";
				}
				else
				{
					desc = "修改昵称失败";
				}
				HintBox* hbox = HintBox::create(desc, 0);
				Director::getInstance()->getRunningScene()->addChild(hbox, 1);
			}
		}
	}

}

void ModifyNameLayer::showThemeScene(float dt)
{
	Director::getInstance()->replaceScene(ThemeScene::Scene());
}

void ModifyNameLayer::textFiledEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType eventtype)
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


void ModifyNameLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	editName->setPlaceHolder(CommonFuncs::gbk2utf("请输入昵称:（回车键确认输入，返回关闭）").c_str());
}

void ModifyNameLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
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
	}
}

void ModifyNameLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
}

void ModifyNameLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{
}
