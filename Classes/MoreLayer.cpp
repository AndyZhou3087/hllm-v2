#include "MoreLayer.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "SettingLayer.h"
#include "LoginRewardLayer.h"
#include "CommonLayer.h"
#include "ShopLayer.h"
#include "RankLayer.h"
#include "TaskLayer.h"
#include "MyParticle.h"
#include "MessageLayer.h"
#include "SkillGrowLayer.h"
#include "NewerGuideLayer.h"
#include "HintBox.h"

using namespace std;

MoreLayer::MoreLayer()
{
}

MoreLayer::~MoreLayer()
{
}

bool MoreLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	this->addChild(color);

	_bgNode = CSLoader::createNode("moreLayer.csb");
	addChild(_bgNode);

	auto action = CSLoader::createTimeline("moreLayer.csb");
	_bgNode->runAction(action);
	action->gotoFrameAndPlay(0, 35, true);

	cocos2d::ui::Widget* m_moreBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("morebtn");
	m_moreBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::more, this));

	m_loginrewardBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("loginrewardbtn");
	m_loginrewardBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::loginReward, this));

	cocos2d::ui::Widget* m_settingBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("settingBtn");
	m_settingBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::Setting, this));

	m_taskBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("taskbtn");
	m_taskBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::task, this));

	m_taskAnimBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("taskanimbtn");
	m_taskAnimBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::task, this));
	m_taskAnimBtn->setVisible(false);

	m_rankBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("rankbtn");
	m_rankBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::rank, this));

	cocos2d::ui::Widget* m_messageBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("messagebtn");
	m_messageBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::message, this));

#ifndef USENETWORK
	m_messageBtn->setVisible(false);
	m_rankBtn->setVisible(false);
#endif
	loginRewardRedPoint = (cocos2d::ui::Widget*)m_loginrewardBtn->getChildByName("startredpoint");
	loginRewardRedPoint->setVisible(!GlobalData::getIsGetLoginReward());

	taskRewardRedPoint = (cocos2d::ui::Widget*)m_taskBtn->getChildByName("startredpoint");

	messageRedPoint = (cocos2d::ui::Widget*)m_messageBtn->getChildByName("startredpoint");

	m_growUpBtn = (cocos2d::ui::Widget*)_bgNode->getChildByName("growUpBtn");
	m_growUpBtn->addTouchEventListener(CC_CALLBACK_2(MoreLayer::GrowUp, this));

	growRedPoint = (cocos2d::ui::Widget*)m_growUpBtn->getChildByName("startredpoint");

	this->schedule(schedule_selector(MoreLayer::checkRedPoint), 0.2f);

	this->scheduleOnce(schedule_selector(MoreLayer::delayShowNewerGuide), 0.5f);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		more(m_moreBtn, ui::Widget::TouchEventType::ENDED);
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool MoreLayer::isHasRedPoint()
{
	bool ishasAchie = checkHasAchie() > 0 ? true : false;
	bool ishasTask = checkHasFinishTask() > 0 ? true : false;
	bool ishasmessage = checkHasMessage() > 0 ? true : false;
	return ishasAchie || ishasTask || ishasmessage;
}

void MoreLayer::checkRedPoint(float dt)
{
	bool isGrowup = checkifGrowUp() && GlobalData::getHighestStage() >= 8;
	growRedPoint->setVisible(isGrowup);

	if (isGrowup)
	{
		showCanGrowUpAnim();
	}

	loginRewardRedPoint->setVisible(!GlobalData::getIsGetLoginReward());

	bool ishasAchie = checkHasAchie() > 0 ? true : false;
	bool ishasTask = checkHasFinishTask() > 0 ? true : false;

	m_taskAnimBtn->setVisible(ishasTask || ishasAchie);
	m_taskBtn->setVisible(!m_taskAnimBtn->isVisible());


	bool ishasmessage = checkHasMessage() > 0 ? true : false;
	messageRedPoint->setVisible(ishasmessage);
}

int MoreLayer::checkHasAchie()
{
	int count = 0;
	for (unsigned int i = 0; i < GlobalData::achieveData.size(); i++)
	{
		if (GlobalData::achieveData[i].achType != AchType::ACH_WORLD_RANK && GlobalData::achieveData[i].achType != AchType::ACH_FRIEND_RANK)
		{
			if (GlobalData::achieveData[i].finish >= GlobalData::achieveData[i].count)
				count++;
		}
		else
		{
			if ((GlobalData::achieveData[i].finish <= GlobalData::achieveData[i].count && GlobalData::achieveData[i].finish > 0) || GlobalData::achieveData[i].finish == INT_MAX)
				count++;
		}
	}
	return count;
}

int MoreLayer::checkHasFinishTask()
{
	int count = 0;
	for (unsigned int i = 0; i < GlobalData::taskData.size(); i++)
	{
		if (GlobalData::taskData[i].finish >= GlobalData::taskData[i].count)
			count++;
	}
	return count;
}

int MoreLayer::checkHasMessage()
{
	int count = 0;
	for (unsigned int i = 0; i < GlobalData::mapMessageData.size(); i++)
	{
		for (unsigned int j = 0; j < GlobalData::mapMessageData[i].size(); j++)
		{
			if (GlobalData::mapMessageData[i][j].status <= 0)
				count++;
		}
	}
	return count;
}

void MoreLayer::showCanGrowUpAnim()
{
	MyParticle::Show(m_growUpBtn, SpeciallyName[31], 45, 60, true, 0.1f);
	MyParticle::Show(m_growUpBtn, SpeciallyName[32], 45, 60, true, 0.1f);
	MyParticle::Show(m_growUpBtn, SpeciallyName[33], 45, 60, true, 0.1f);
}

bool MoreLayer::checkifGrowUp()
{
	for (int i = S_AUTOPROMT; i <= S_ADDONESECOND; i++)
	{
		if (GlobalData::getHighestStage() + 1 >= unlockSkillstage[i] && GlobalData::getMySkillLevel(i) < GlobalData::mapSkillData[i].maxleve)
		{
			if (GlobalData::getGoldCount() >= 100 * (GlobalData::getMySkillLevel(i) + 1))
			{
				return true;
			}
		}
	}
	return false;
}

void MoreLayer::Setting(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Layer* layer = SettingLayer::create();
		this->addChild(layer);
	}
}

void MoreLayer::loginReward(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		LoginRewardLayer::AnimType = A_SMALLTONORMAL;
		LoginRewardLayer* layer = LoginRewardLayer::create();
		this->addChild(layer, 100);
	}
}

void MoreLayer::more(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_commomLayer != NULL)
			g_commomLayer->setMorebtnVisible(true);
		this->removeFromParentAndCleanup(true);
	}
}

void MoreLayer::task(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_TaskLayer = TaskLayer::create();
		this->addChild(g_TaskLayer);
	}
}


void MoreLayer::rank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_commomLayer->isnewerguiding = false;
		if (g_NewerLayer != NULL)
			g_NewerLayer->removeSelf();

		Layer* layer = RankLayer::create();
		this->addChild(layer);
	}
}

void MoreLayer::message(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		MessageLayer* layer = MessageLayer::create();
		g_messageLayer = layer;
		this->addChild(layer);
	}
}

void MoreLayer::GrowUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getHighestStage() >= 8)
		{
			g_commomLayer->isnewerguiding = false;
			if (g_NewerLayer != NULL)
				g_NewerLayer->removeSelf();

			SkillGrowLayer *layer = SkillGrowLayer::create();
			g_skillGrowlayer = layer;
			addChild(layer);
		}
		else
		{
			HintBox* hint = HintBox::create("通关第9关解锁", 0);
			addChild(hint, 1000);
		}
	}
}

void MoreLayer::delayShowNewerGuide(float t)
{
	std::vector<Node*> nodes;
	if (GlobalData::checkGuide(21))
	{
		if (GlobalData::getHighestStage() == 8)
		{
			nodes.push_back(m_growUpBtn);
			g_NewerLayer = NewerGuideLayer::create(21);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
#ifdef USENETWORK
	if (GlobalData::checkGuide(17))
	{
		if (GlobalData::getHighestStage() == 6)
		{
			nodes.push_back(m_rankBtn);
			g_NewerLayer = NewerGuideLayer::create(17);
			_bgNode->addChild(g_NewerLayer, NEWERLAYERZOER);
			g_NewerLayer->setData(nodes);
			return;
		}
	}
#endif
}


