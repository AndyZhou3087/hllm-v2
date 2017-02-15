#include "UnlockLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "HintBox.h"
#include "ThemeScene.h"
int UnlockLayer::AnimType = A_SMALLTONORMAL;
UnlockLayer::UnlockLayer()
{
}

UnlockLayer::~UnlockLayer()
{
}


UnlockLayer* UnlockLayer::create(int chapter, int starsnum)
{
	UnlockLayer *pRet = new UnlockLayer();
	if (pRet && pRet->init(chapter, starsnum))
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

bool UnlockLayer::init(int chapter, int starsnum)
{
	if (!Layer::init()) 
	{
		return false;
	}
	m_chapter = chapter;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	Node * layer = CSLoader::createNode("UnlockLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(UnlockLayer::close, this));


	cocos2d::ui::Widget* unlockbtn = (cocos2d::ui::Widget*)layer->getChildByName("unlockbtn");
	unlockbtn->addTouchEventListener(CC_CALLBACK_2(UnlockLayer::onUnlock, this));

	cocos2d::ui::Widget* colletbtn = (cocos2d::ui::Widget*)layer->getChildByName("colletbtn");
	colletbtn->addTouchEventListener(CC_CALLBACK_2(UnlockLayer::onCllect, this));

	cocos2d::ui::TextBMFont* needStarlbl = (cocos2d::ui::TextBMFont*)layer->getChildByName("unlocklbl");
	std::string starstr = StringUtils::format("%d", starsnum);
	needStarlbl->setString(starstr);

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

void UnlockLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void UnlockLayer::onUnlock(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getGemCount() < 50)
		{
			HintBox * hintbox = HintBox::create("钻石不足", 1);
			addChild(hintbox);
		}
		else
		{
			GlobalData::setGemCount(GlobalData::getGemCount() - 50);
			GlobalData::setChapterUnlock(m_chapter, true);
			this->removeFromParentAndCleanup(true);
		}
	}
}

void UnlockLayer::onCllect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		int stageCount = 10;
		int index = stageCount - 1;
		for (int i = stageCount - 1; i >= 0; i--)
		{
			std::string pagestr = StringUtils::format("page%dstar", m_chapter);
			std::string pagestarsStr = GlobalData::getChapterStageStar(pagestr);
			long long pageStars = atoll(pagestarsStr.c_str());
			int star = pageStars >> (i * 2) & 3;
			if (star < 3)
			{
				index = i;
				break;
			}
		}

		GlobalData::setPlayingStage(m_chapter *stageCount + index);
		this->removeFromParentAndCleanup(true);
		Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(ThemeScene::openSureStageLayer), 0.2f);
	}
}