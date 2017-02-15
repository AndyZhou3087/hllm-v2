#include "MessageDetailsLayer.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "MyMenu.h"
#include "MessageLayer.h"
#include "HttpUtil.h"

MessageDetailsLayer::MessageDetailsLayer()
{
}

MessageDetailsLayer::~MessageDetailsLayer()
{

}

void MessageDetailsLayer::onExit()
{
	Layer::onExit();
}

MessageDetailsLayer* MessageDetailsLayer::create(int index)
{
	MessageDetailsLayer *pRet = new MessageDetailsLayer();
	if (pRet && pRet->initwithtype(index))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool MessageDetailsLayer::initwithtype(int index)
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	Node * layer = CSLoader::createNode("messagDetailsLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(MessageDetailsLayer::close, this));

	getBtn = (cocos2d::ui::Widget*)layer->getChildByName("getbtn");
	getBtn->addTouchEventListener(CC_CALLBACK_2(MessageDetailsLayer::get, this));

	cocos2d::ui::Widget* oktext = (cocos2d::ui::Widget*)getBtn->getChildByName("oktext");
	cocos2d::ui::Widget* lqtext = (cocos2d::ui::Widget*)getBtn->getChildByName("lqtext");
	cocos2d::ui::Widget* yilqtext = (cocos2d::ui::Widget*)getBtn->getChildByName("yilqutext");
	message = &GlobalData::mapMessageData[0][index];
	if (message->type == 0)
	{
		oktext->setVisible(true);
		lqtext->setVisible(false);
		yilqtext->setVisible(false);
	}
	else if (message->type == 1)
	{
		oktext->setVisible(false);
		if (message->status == 0)
		{
			lqtext->setVisible(true);
			yilqtext->setVisible(false);
		}
		else
		{
			lqtext->setVisible(false);
			yilqtext->setVisible(true);
		}
	}

	cocos2d::ui::Text* content = (cocos2d::ui::Text*)layer->getChildByName("content");
	content->setString(message->content);
	content->ignoreContentAdaptWithSize(false);
	content->setContentSize(Size(400, 100));

	std::vector<std::string> vec_temp;
	CommonFuncs::split(message->prize, vec_temp, "-");
	
	for (unsigned int i = 0; i < vec_temp.size(); i++)
	{
		vec_prize.push_back(atoi(vec_temp[i].c_str()));
	}

	int count = 0;
	for (unsigned int i = 0; i < vec_prize.size(); i++)
	{
		if (vec_prize[i] > 0)
		{
			std::string str = StringUtils::format("UI/rprop%d.png", i);
			Sprite* icon = Sprite::createWithSpriteFrameName(str);
			icon->setScale(0.8f);
			icon->setPosition(Vec2(100 + count * 82, 470));
			addChild(icon);

			str = StringUtils::format("x%d", vec_prize[i]);
			Label* num = Label::createWithBMFont("fonts/buynum.fnt", str);
			num->setAnchorPoint(Vec2(0.5f, 0.5f));
			num->setPosition(Vec2(100 + count * 82, 430));
			addChild(num);
			count++;
		}
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void MessageDetailsLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		getBtn->setEnabled(false);
		if (message->type == 0)
		{
			g_messageLayer->updataMessage();
			updataStatusToServer(message);
			message->status = 1;
		}
		this->removeFromParentAndCleanup(true);
	}
}

void MessageDetailsLayer::get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		getBtn->setEnabled(false);
		if (message->status == 0)
		{
			g_messageLayer->updataMessage();
			updataStatusToServer(message);
			message->status = 1;
		}
		this->removeFromParentAndCleanup(true);
	}
}

void MessageDetailsLayer::updataStatusToServer(Message* message)
{
	if (message->status == 0)
	{
		std::string url;
		url.append(HTTPURL);
		url.append("llk_take");
		url.append("&id=");
		url.append(GlobalData::getMyID());
		url.append("&mid=");
		std::string midstr = StringUtils::format("%d", message->id);
		url.append(midstr);
		HttpUtil::getInstance()->doData(url, httpCompleteCB, "", HTTPTYPE::GET, "", message->prize);
	}
}

void MessageDetailsLayer::httpCompleteCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		std::vector<std::string> vec_temp;
		CommonFuncs::split(tag, vec_temp, "-");

		if (vec_temp.size() >= 6)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() + atoi(vec_temp[0].c_str()));
			GlobalData::setGoldCount(GlobalData::getGoldCount() + atoi(vec_temp[1].c_str()));
			GlobalData::setHpCount(GlobalData::getHpCount() + atoi(vec_temp[2].c_str()));
			GlobalData::setPromptProp(GlobalData::getPromptProp() + atoi(vec_temp[3].c_str()));
			GlobalData::setRefreshProp(GlobalData::getRefreshProp() + atoi(vec_temp[4].c_str()));
			GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + atoi(vec_temp[5].c_str()));
		}
	}
}