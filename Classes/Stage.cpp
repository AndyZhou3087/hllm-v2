#include "Stage.h"
#include "CommonFuncs.h"
Stage::Stage()
{

}

Stage::Stage(Node* node)
{
	m_isboss = false;
	lockNode = NULL;
	m_node = node;
	initData();
}

Stage::~Stage()
{
	m_isboss = false;
	if (lockNode != NULL)
	{
		lockNode->removeFromParentAndCleanup(true);
		lockNode = NULL;
	}
}

void Stage::initData()
{
	m_starnum = 0;
	m_stageIcon = (cocos2d::ui::ImageView*)m_node->getChildByName("image");
	std::string str;
	for (int i = 0; i < 3; i++)
	{
		str = StringUtils::format("star%d", i);
		m_star[i] = (cocos2d::ui::Widget*)m_node->getChildByName(str);
		str = StringUtils::format("star%dbg", i);
		m_starbg[i] = (cocos2d::ui::Widget*)m_node->getChildByName(str);
	}
	m_stagenumlbl = (cocos2d::ui::TextBMFont*)m_node->getChildByName("stagenum");
}

void Stage::setStar(int num, bool isboss)
{
	m_starnum = num;
	m_isboss = isboss;
	std::string stagestr;
	if (num > 0)
	{ 
		stagestr = "stage1.png";
		for (int i = 0; i < 3; i++)
		{
			m_starbg[i]->setVisible(true);
		}
		for (int i = 0; i < num; i++)
		{
			m_star[i]->setVisible(true);
		}
	}
	else
	{
		stagestr = "stage0.png";
		for (int i = 0; i < 3; i++)
		{
			m_star[i]->setVisible(false);
			m_starbg[i]->setVisible(false);
		}
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
	if (isboss)
	{
		int index = num > 0 ? 1 : 0;
		std::string bossstr = StringUtils::format("sboss%d.png", index);
		m_stageIcon->loadTexture(bossstr, cocos2d::ui::TextureResType::PLIST);
		m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(bossstr)->getContentSize());
	}
}

void Stage::setStageNum(int stage)
{
	std::string numstr = StringUtils::format("%d", stage);
	m_stagenumlbl->setString(numstr);
	if (m_isboss)
		m_stagenumlbl->setVisible(false);
}

void Stage::hilight()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr;
	if (m_isboss)
	{
		stagestr = "sboss1.png";
	}
	else
	{
		stagestr = "stage2.png";
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}

void Stage::disable()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr = "stage0.png";
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}
void Stage::nomal()
{
	m_stageIcon->stopAllActions();
	m_node->setScale(1);
	std::string stagestr;
	if (m_isboss)
	{
		stagestr = "sboss1.png";
	}
	else
	{
		stagestr = "stage1.png";
	}
	m_stageIcon->loadTexture(stagestr, cocos2d::ui::TextureResType::PLIST);
	m_stageIcon->setContentSize(Sprite::createWithSpriteFrameName(stagestr)->getContentSize());
}

void Stage::showLock(int starnum)
{
	if (lockNode == NULL)
	{
		lockNode = Sprite::createWithSpriteFrameName("lock.png");
		lockNode->setPosition(Vec2(m_node->getContentSize().width / 2, 10));
		m_node->addChild(lockNode, 2);

		//Sprite* lockbox = Sprite::createWithSpriteFrameName("unlockbox.png");
		//lockbox->setPosition(Vec2(lockNode->getContentSize().width / 2, lockNode->getContentSize().height - 60));
		//lockNode->addChild(lockbox);

		//Sprite* star = Sprite::createWithSpriteFrameName("star1.png");
		//star->setPosition(Vec2(lockbox->getContentSize().width / 2, lockbox->getContentSize().height / 2));
		//lockbox->addChild(star);

		//std::string desc = StringUtils::format("%3d     开启", starnum);
		//Label* stardesclbl = Label::createWithBMFont("fonts/tips.fnt", CommonFuncs::gbk2utf(desc.c_str()));
		//stardesclbl->setPosition(Vec2(lockbox->getContentSize().width / 2, lockbox->getContentSize().height / 2));
		//stardesclbl->setScale(0.6f);
		//lockbox->addChild(stardesclbl);
	}
}

void Stage::removeLock()
{
	if (lockNode != NULL)
	{
		lockNode->removeFromParentAndCleanup(true);
		lockNode = NULL;
	}
}
