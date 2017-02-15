#include "ChooseInfoLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "CommonLayer.h"

int ChooseInfoLayer::AnimType = A_SMALLTONORMAL;
ChooseInfoLayer::ChooseInfoLayer()
{
}


ChooseInfoLayer::~ChooseInfoLayer()
{
}
bool ChooseInfoLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	Node * layer = CSLoader::createNode("ChooseInfoLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("close");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(ChooseInfoLayer::close, this));

	cocos2d::ui::TextBMFont *stagelbl = (cocos2d::ui::TextBMFont*)layer->getChildByName("stagelbl");
	cocos2d::ui::TextBMFont *slv = (cocos2d::ui::TextBMFont*)layer->getChildByName("slv");
	cocos2d::ui::TextBMFont *ranklbl = (cocos2d::ui::TextBMFont*)layer->getChildByName("ranklbl");

	cocos2d::ui::Widget* stagebtn = (cocos2d::ui::Widget*)layer->getChildByName("stagebtn");
	stagebtn->addTouchEventListener(CC_CALLBACK_2(ChooseInfoLayer::onStage, this));

	cocos2d::ui::Widget* clasicbtn = (cocos2d::ui::Widget*)layer->getChildByName("clasicbtn");
	clasicbtn->addTouchEventListener(CC_CALLBACK_2(ChooseInfoLayer::onClasic, this));

	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (AnimType == A_SMALLTONORMAL)
	{
		layer->setAnchorPoint(Vec2(0.5f, 0.5f));
		layer->setPosition(Vec2(270, 480));
		layer->setScale(0.1f);
		layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		layer->setPosition(0, 960);
		layer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}
	return true;
}

void ChooseInfoLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void ChooseInfoLayer::onStage(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
	}
}

void ChooseInfoLayer::onClasic(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

	}
}