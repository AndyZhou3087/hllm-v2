#include "WaitingProgress.h"

#include "CommonFuncs.h"
bool WaitingProgress::init(std::string text)
{
    if (Layer::init()) 
	{
		LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 100));
		this->addChild(color);

		boxbg = Sprite::createWithSpriteFrameName("UI/waitingbg.png");
		boxbg->setPosition(Vec2(270, 480));
		addChild(boxbg);

		Sprite* progress = Sprite::createWithSpriteFrameName("UI/waitingprogress.png");
		progress->setPosition(Vec2(progress->getContentSize().width, boxbg->getContentSize().height/2));
		boxbg->addChild(progress);
		progress->runAction(RepeatForever::create(RotateTo::create(2, 720)));

		textstr = text;
		//this->scheduleOnce(schedule_selector(WaitingProgress::delayShowText), 0.01f);
		delayShowText(0);
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

void WaitingProgress::delayShowText(float dt)
{
	Label* textlbl = Label::createWithBMFont("fonts/tips.fnt", CommonFuncs::gbk2utf(textstr.c_str()));
	textlbl->setPosition(Vec2(boxbg->getContentSize().width / 2, boxbg->getContentSize().height / 2));
	textlbl->setColor(Color3B(255, 255, 255));
	textlbl->setScale(0.85f);
	boxbg->addChild(textlbl);
}

WaitingProgress* WaitingProgress::create(std::string content)
{
	WaitingProgress *pRet = new WaitingProgress();
	if (pRet && pRet->init(content))
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