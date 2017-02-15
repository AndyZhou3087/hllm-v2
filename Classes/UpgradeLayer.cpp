#include "UpgradeLayer.h"

bool UpgradeLayer::init()
{
    if (!Layer::init()) 
	{
		return false;
    }

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	Node * layer = CSLoader::createNode("UpgradeLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* bgbox = (cocos2d::ui::Widget*)layer->getChildByName("bg");
	percentText = (cocos2d::ui::Text*)bgbox->getChildByName("percenttext");
	llText = (cocos2d::ui::Text*)bgbox->getChildByName("lltext");
	loadingBar = (cocos2d::ui::LoadingBar*)bgbox->getChildByName("loadingbar");

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void UpgradeLayer::updataUI(double downedbyte, double totalbyte)
{
	int percent = (int)downedbyte / totalbyte * 100;
	std::string str = StringUtils::format("%d%%", percent);
	percentText->setString(str);
	loadingBar->setPercent(percent);
	str = StringUtils::format("%.2fkb/%.2fkb", downedbyte/1024, totalbyte/1024);
	llText->setString(str);
}