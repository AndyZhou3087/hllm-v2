#include "ComfirmBuyLayer.h"
#include "Const.h"
#include "SoundManager.h"
#include "ShopLayer.h"
#include "CommonLayer.h"

ComfirmBuyLayer::ComfirmBuyLayer()
{
}


ComfirmBuyLayer::~ComfirmBuyLayer()
{

}
bool ComfirmBuyLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	auto layer = CSLoader::createNode("ComfirmLayer.csb");
	layer->setPosition(0, 0);
	addChild(layer);

	cocos2d::ui::Widget * okbtn = (cocos2d::ui::Widget*)layer->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(ComfirmBuyLayer::ok, this));

	cocos2d::ui::Widget * cancelbtn = (cocos2d::ui::Widget*)layer->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(ComfirmBuyLayer::close, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void ComfirmBuyLayer::ok(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		ShopLayer::setMessage(0);
	}
}
void ComfirmBuyLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}