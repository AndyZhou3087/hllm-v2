#include "FreeGift.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "NewerGuideLayer.h"
#include "MyTransitionScene.h"
int FreeGift::AnimType = A_SMALLTONORMAL;
FreeGift::FreeGift()
{
}


FreeGift::~FreeGift()
{
}

bool FreeGift::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	this->addChild(color);

	_layer = CSLoader::createNode("freegiftLayer.csb");
	addChild(_layer);

	auto layerAnim = CSLoader::createTimeline("freegiftLayer.csb");
	_layer->runAction(layerAnim);
	layerAnim->gotoFrameAndPlay(0, 30, true);

	getBtn = (cocos2d::ui::Widget*)_layer->getChildByName("giftbtn");
	getBtn->addTouchEventListener(CC_CALLBACK_2(FreeGift::get, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	if (AnimType == A_SMALLTONORMAL)
	{
		_layer->setAnchorPoint(Vec2(0.5f, 0.5f));
		_layer->setPosition(Vec2(270, 480));
		_layer->setScale(0.1f);
		_layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	}
	else
	{
		_layer->setPosition(0, 960);
		_layer->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))), NULL));
		AnimType = A_SMALLTONORMAL;
	}
	this->scheduleOnce(schedule_selector(FreeGift::delayShowNewerGuide), 0.5f);
	return true;
}

void FreeGift::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void FreeGift::get(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_NewerLayer != NULL)
			g_NewerLayer->removeSelf();
		UserDefault::getInstance()->setBoolForKey("isgetfreegift", true);
		GlobalData::setGoldCount(GlobalData::getGoldCount() + 100);
		GlobalData::setPromptProp(GlobalData::getPromptProp() + 1);
		GlobalData::setRefreshProp(GlobalData::getRefreshProp() + 1);
		Director::getInstance()->replaceScene(MyTransitionScene::scene());
	}

}

void FreeGift::delayShowNewerGuide(float dt)
{
	vector<Node*> nodes;
	int size = _layer->getChildren().size();
	for (int i = 0; i < size; i++)
	{
		Node* node = _layer->getChildren().at(i);
		nodes.push_back(node);
	}
	g_NewerLayer = NewerGuideLayer::create(6);
	_layer->addChild(g_NewerLayer, NEWERLAYERZOER);
	g_NewerLayer->setData(nodes);

}