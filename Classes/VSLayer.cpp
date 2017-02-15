#include "VSLayer.h"

VSLayer::VSLayer()
{
}


VSLayer::~VSLayer()
{

}
bool VSLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	auto acitonNode = CSLoader::createNode("VSLayer.csb");
	acitonNode->setPosition(270, 480);
	addChild(acitonNode);

	auto action = CSLoader::createTimeline("VSLayer.csb");
	acitonNode->runAction(action);
	action->gotoFrameAndPlay(0, 25, false);

	boss = (cocos2d::ui::ImageView*)acitonNode->getChildByName("pk_boss")->getChildByName("boss");
	boss->setVisible(false);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleOnce(schedule_selector(VSLayer::removeSelf), 2.0f);

	return true;
}
void VSLayer::setBoss(int b)
{
	std::string str = StringUtils::format("vsboss%d.png", b);
	boss->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	boss->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
	boss->setVisible(true);
}

void VSLayer::removeSelf(float)
{
	this->removeFromParentAndCleanup(true);
}