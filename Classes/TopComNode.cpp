#include "TopComNode.h"
#include "ShopLayer.h"
#include "HpBuyLayer.h"
#include "GlobalData.h"
#include "FourTypeGift.h"
#include "MyParticle.h"
#include "CommonLayer.h"
bool TopComNode::init()
{
	if (Node::init())
	{
		Node * node = CSLoader::createNode("CommonTopNode.csb");
		addChild(node);

		m_hpicon = (cocos2d::ui::Widget*)node->getChildByName("hp");
		m_HpBtn = (cocos2d::ui::Widget*)node->getChildByName("HpBtn");
		m_HpBtn->addTouchEventListener(CC_CALLBACK_2(TopComNode::Hp, this));
		m_goldBtn = (cocos2d::ui::Widget*)node->getChildByName("goldBtn");
		m_goldBtn->addTouchEventListener(CC_CALLBACK_2(TopComNode::Gold, this));

		m_diamondBtn = (cocos2d::ui::Widget*)node->getChildByName("diamondBtn");
		m_diamondBtn->addTouchEventListener(CC_CALLBACK_2(TopComNode::Diamond, this));

		initTime();
		std::string goldStr = StringUtils::format("%d", _goldcount);
		m_goldNum = (cocos2d::ui::TextBMFont*)node->getChildByName("goldbmlbl");
		m_goldNum->setString(goldStr);

		std::string diamondStr = StringUtils::format("%d", _diamondcount);
		m_diamondNum = (cocos2d::ui::TextBMFont*)node->getChildByName("gembmlbl");
		m_diamondNum->setString(diamondStr);

		std::string hpStr = StringUtils::format("%d/%d", _powercount, maxHpCount);
		m_hpNum = (cocos2d::ui::TextBMFont*)node->getChildByName("hpbmlbl");
		m_hpNum->setString(hpStr);

		timeBg = (cocos2d::ui::Widget*)node->getChildByName("timebg");

		std::string timeStr = StringUtils::format("%02d:%02d", _powerSec / 60, _powerSec % 60);
		m_time = (cocos2d::ui::TextBMFont*)node->getChildByName("timebmlbl");
		m_time->setString(timeStr);

		updateUI();

		if (_powercount >= maxHpCount)
		{
			timeBg->setVisible(false);
			m_time->setVisible(false);
		}
	}

	return true;
}

void TopComNode::HpAction(float t)
{
	MyParticle::Show(m_hpicon, SpeciallyName[22], 30, 5, true, 2.0f);
}

void TopComNode::updateTime(float t)
{
	maxHpCount = GlobalData::getMyMaxHp();
	_powercount = GlobalData::getHpCount();
	_powertime = GlobalData::getHpTime();
	if (_powercount < maxHpCount)
	{
		//struct timeval now;
		//gettimeofday(&now, NULL);
		//int nowtimestamp = now.tv_sec;
		int nowtimestamp = GlobalData::getSystemSecTime();

		if (nowtimestamp > 0)
		{
			char timeStr[6] = { 0 };
			int tmp = nowtimestamp - _powertime;
			if (tmp >= 600)
			{
				_powertime += 10 * 60;
				_powercount++;
				tmp = nowtimestamp - _powertime;
				Save();
			}
			tmp = 600 - tmp;
			if (_powercount < maxHpCount)
			{
				sprintf(timeStr, "%02d:%02d", tmp / 60, tmp % 60);
				m_time->setString(timeStr);
			}
		}
	}

	if (_powercount < maxHpCount)
	{
		m_time->setVisible(true);
		timeBg->setVisible(true);
	}
	else
	{
		m_time->setVisible(false);
		timeBg->setVisible(false);
	}
}


void TopComNode::updateUI()
{
	_visualhp = GlobalData::getHpCount();
	_visualgold = GlobalData::getGoldCount();
	_visualgem = GlobalData::getGemCount();

	std::string hpCount = StringUtils::format("%d/%d", GlobalData::getHpCount(), maxHpCount);
	m_hpNum->setString(hpCount);

	std::string diamondCount = StringUtils::format("%d", GlobalData::getGemCount());
	m_diamondNum->setString(diamondCount);

	std::string GoldCount = StringUtils::format("%d", GlobalData::getGoldCount());
	m_goldNum->setString(GoldCount);

	scheduleUpdate();
	schedule(schedule_selector(TopComNode::HpAction), 5.0f);
}

void TopComNode::initTime()
{
	//struct timeval now;
	//gettimeofday(&now, NULL);
	//int nowtimestamp = now.tv_sec;
	int nowtimestamp = GlobalData::getSystemSecTime();
	int maxSec = 600;
	_powerSec = 0;

	_powertime = GlobalData::getHpTime();
	_goldcount = GlobalData::getGoldCount();
	_diamondcount = GlobalData::getGemCount();
	_powercount = GlobalData::getHpCount();
	maxHpCount = GlobalData::getMyMaxHp();

	if (nowtimestamp < _powertime)
		_powertime = nowtimestamp;

	if (_powercount < maxHpCount)
	{
		int c = floor((nowtimestamp - _powertime) / (10 * 60));
		if (c > 0)
		{
			_powercount += c;
			if (_powercount >= maxHpCount)
			{
				_powercount = maxHpCount;
				_powertime = nowtimestamp;
			}
			else
			{
				_powertime += c * 10 * 60;
				_powerSec = maxSec - (nowtimestamp - _powertime);
			}

		}
		else
		{
			_powerSec = maxSec - (nowtimestamp - _powertime);
		}
	}
	else
	{
		_powertime = nowtimestamp;
	}

	Save();
}

void TopComNode::Save()
{
	GlobalData::setHpCount(_powercount);
	GlobalData::setHpTime(_powertime);
}


void TopComNode::update(float dt)
{
	updateTime(dt);
	updateHp(dt);
	updateGold(dt);
	updateGem(dt);
}

void TopComNode::updateHp(float dt)
{
	int count = GlobalData::getHpCount();
	if (count == 0)
	{
		_visualhp = 0;
	}
	else
	{
		float step = (count - _visualhp) * 0.04f;
		if (fabs(step) < 0.05)
		{
			step = 0.05 * ((step > 0) ? 1 : -1);
		}

		_visualhp += step;

		if (fabs(count - _visualhp) < 0.1f) {
			_visualhp = count;
		}
	}

	std::string hpCountStr = StringUtils::format("%d/%d", (int)_visualhp, maxHpCount);
	m_hpNum->setString(hpCountStr);
}

void TopComNode::updateGold(float dt)
{
	int count = GlobalData::getGoldCount();
	if (count == 0)
	{
		_visualgold = 0;
	}
	else
	{
		float step = (count - _visualgold) * 0.04f;
		if (fabs(step) < 0.05)
		{
			step = 0.05 * ((step > 0) ? 1 : -1);
		}

		_visualgold += step;

		if (fabs(count - _visualgold) < 0.1f) {
			_visualgold = count;
		}
	}

	std::string GoldCount = StringUtils::format("%d", (int)_visualgold);
	m_goldNum->setString(GoldCount);
}

void TopComNode::updateGem(float dt)
{
	int count = GlobalData::getGemCount();
	if (count == 0)
	{
		_visualgem = 0;
	}
	else
	{
		float step = (count - _visualgem) * 0.04f;
		if (fabs(step) < 0.05)
		{
			step = 0.05 * ((step > 0) ? 1 : -1);
		}

		_visualgem += step;

		if (fabs(count - _visualgem) < 0.1f) {
			_visualgem = count;
		}
	}

	std::string GemCount = StringUtils::format("%d", (int)_visualgem);
	m_diamondNum->setString(GemCount);
}

void TopComNode::Hp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		HpBuyLayer* layer = HpBuyLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}

void TopComNode::Gold(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer::whichClicked = 0;
		Layer* layer = ShopLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}

void TopComNode::Diamond(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer::whichClicked = 0;
		Layer* layer = ShopLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}