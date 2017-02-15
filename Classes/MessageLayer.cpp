#include "MessageLayer.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "MyMenu.h"
#include "MessageDetailsLayer.h"
#include "WaitingProgress.h"
#include "LoadingScene.h"
#include "HttpUtil.h"
MessageLayer* g_messageLayer = NULL;
MessageLayer::MessageLayer()
{
	scrollview = NULL;
}

MessageLayer::~MessageLayer()
{
	scrollview = NULL;
	g_messageLayer = NULL;
}

void MessageLayer::onExit()
{
	Layer::onExit();
}

bool MessageLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	Node * layer = CSLoader::createNode("messageLayer.csb");
	addChild(layer);

	Node * hamsterAnim = CSLoader::createNode("hamsterAnim.csb");
	hamsterAnim->setPosition(Vec2(88, 160));
	hamsterAnim->setScale(0.8f);
	addChild(hamsterAnim);
	auto action = CSLoader::createTimeline("hamsterAnim.csb");
	hamsterAnim->runAction(action);
	action->gotoFrameAndPlay(0);

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(MessageLayer::close, this));

	scrollview = (cocos2d::ui::ScrollView*)layer->getChildByName("ScrollView");

	WaitingProgress* waitbox = WaitingProgress::create("数据加载中...");
	addChild(waitbox, 10000, "waitbox");

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleOnce(schedule_selector(MessageLayer::getMessageData), 0.1f);
	return true;
}

void MessageLayer::getMessageData(float dt)
{
	std::string url;
	url.append(HTTPURL);
	url.append("llk_MessageList");
	url.append("&id=");
	url.append(GlobalData::getMyID());
	url.append("&vercode=");
	std::string vercodestr = StringUtils::format("%d",GlobalData::getVersionCode());
	url.append(vercodestr);
	url.append("&cid=");
	url.append(GlobalData::getCid());
	HttpUtil::getInstance()->doData(url, httpMessageDataCompleteCB);
}

void MessageLayer::httpMessageDataCompleteCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		LoadingScene::parseMessageJson(retdata);

		if (GlobalData::mapMessageData[0].size() > 0)
			g_messageLayer->addMesages();
		else
		{
			Sprite* desc = Sprite::createWithSpriteFrameName("UI/msgnotips.png");
			desc->setPosition(Vec2(270, 510));
			g_messageLayer->addChild(desc, 2);
		}
	}
	else
	{
		Label* desclbl = Label::createWithSystemFont(CommonFuncs::gbk2utf("获取数据失败，请检查网络连接！"), FONT_NAME, 25);
		desclbl->setColor(Color3B(109, 44, 21));
		desclbl->setPosition(Vec2(270, 480));
		if (g_messageLayer != NULL)
			g_messageLayer->addChild(desclbl, 2);
	}
	if (g_messageLayer != NULL)
		g_messageLayer->removeChildByName("waitbox");
}


void MessageLayer::addMesages()
{
	sortMessageData(0);

	int msgsize = GlobalData::mapMessageData[0].size();
	int itemheight = 115;
	int scrollinnerheight = msgsize * itemheight;

	if (scrollview == NULL)
		return;

	int scorellheight = scrollview->getContentSize().height;
	if (scrollinnerheight <= scorellheight)
		scrollinnerheight = scorellheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, scrollinnerheight));
	for (int i = 0; i < msgsize; i++)
	{
		MessageItem* item0 = MessageItem::createWithData(&GlobalData::mapMessageData[0][i]);
		MessageItem* item1 = MessageItem::createWithData(&GlobalData::mapMessageData[0][i]);
		MenuItemSprite *pItem = MenuItemSprite::create(
			item0,
			item1,
			item0,
			CC_CALLBACK_1(MessageLayer::clickMessageCallback, this));
		pItem->setTag(i);
		pItem->setPosition(Vec2(270, scrollinnerheight - itemheight / 2 - i * itemheight));
		item1->setScale(0.95f);
		float scale = 1.0f - item1->getScale();
		item1->setPosition(item1->getContentSize().width*scale / 2.0f, item1->getContentSize().height*scale / 2.0f);
		MyMenu* menu = MyMenu::create();
		menu->addChild(pItem, 1, "item");
		menu->setPosition(0, 0);
		std::string str = StringUtils::format("itemmenu%d", i);
		scrollview->addChild(menu, 0, str);
	}

}

void MessageLayer::clickMessageCallback(Ref* pSender)
{
	SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_BUTTON);
	MessageItem* item = (MessageItem*)pSender;

	Layer* layer = MessageDetailsLayer::create(item->getTag());
	this->addChild(layer, 2);
}

void MessageLayer::sortMessageData(int tab)
{
	sort(GlobalData::mapMessageData[tab].begin(), GlobalData::mapMessageData[tab].end(), bystatus);
}


bool MessageLayer::bystatus(Message a, Message b)
{
	if (a.status < b.status)
		return true;
	return false;
}

void MessageLayer::updataMessage()
{
	sortMessageData(0);
	std::string str;
	for (unsigned int i = 0; i < GlobalData::mapMessageData[0].size(); i++)
	{
		str = StringUtils::format("itemmenu%d", i);
		MenuItemSprite* item = (MenuItemSprite*)scrollview->getChildByName(str)->getChildByName("item");
		MessageItem* normalbtn = (MessageItem*)item->getNormalImage();
		MessageItem* selectbtn = (MessageItem*)item->getSelectedImage();
		normalbtn->updatedata(&GlobalData::mapMessageData[0][i]);
		selectbtn->updatedata(&GlobalData::mapMessageData[0][i]);
	}
}

void MessageLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

MessageItem::MessageItem()
{

}

MessageItem::~MessageItem()
{
}

MessageItem* MessageItem::createWithData(Message *data)
{
	MessageItem* item = new MessageItem;
	item->init(data);
	item->autorelease();
	return item;
}

bool MessageItem::init(Message *data)
{
	if (Sprite::initWithSpriteFrameName("UI/mesgitem.png"))
	{
		std::string iconstr = StringUtils::format("UI/micon%d.png", data->type);
		icon = Sprite::createWithSpriteFrameName(iconstr);
		icon->setPosition(Vec2(icon->getContentSize().width/2 + 10, this->getContentSize().height / 2));
		addChild(icon);

		title = Label::createWithSystemFont("", FONT_NAME, 22);
		title->setColor(Color3B(109, 44, 21));
		title->setAnchorPoint(Vec2(0, 0.5));
		title->setPosition(Vec2(120, this->getContentSize().height / 2));
		addChild(title);

		datetime = Label::createWithSystemFont("2016-01-01", FONT_NAME, 18);
		datetime->setColor(Color3B(109, 44, 21));
		datetime->setAnchorPoint(Vec2(0, 0.5));
		datetime->setPosition(Vec2(this->getContentSize().width - datetime->getContentSize().width - 30, this->getContentSize().height / 2));
		addChild(datetime);

		status = Sprite::createWithSpriteFrameName("UI/startredpoint.png");
		status->setPosition(Vec2(this->getContentSize().width - status->getContentSize().width/2, this->getContentSize().height - status->getContentSize().height/2));
		status->setVisible(false);
		addChild(status);
		updatedata(data);
		return true;
	}
	return false;
}

void MessageItem::updatedata(Message *data)
{
	mdata = data;

	std::string iconstr = StringUtils::format("UI/micon%d.png", mdata->type);
	SpriteFrame* newIcon = SpriteFrameCache::getInstance()->getSpriteFrameByName(iconstr);
	icon->setSpriteFrame(newIcon);

	title->setString(mdata->title);

	long long int timeSec = mdata->datetime;
	char str[64] = { 0 };
	time_t timep = timeSec;
	tm* timeStuct = gmtime(&timep);
	sprintf(str, "%04d-%02d-%02d", timeStuct->tm_year + 1900, timeStuct->tm_mon + 1, timeStuct->tm_mday);
	datetime->setString(str);

	status->setVisible(mdata->status <= 0);
}