
#include "NewerGuideLayer.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "SkillGrowLayer.h"
#include "CommonLayer.h"
NewerGuideLayer* g_NewerLayer = NULL;
int NewerGuideLayer::_step = 0;
//我们需要在这个倒计时结束前消掉下方\n所有的图形，不然你就开心不起来啦。
string descText[] = {"欢迎来到“欢乐连萌”，让我们一起冒险吧！", "两个相同的图形如果能用不超过2个拐角的线相连就能消除。", "按住图案滑到另一个图案上也可以消除哦~", "拐角超过2个就不能消除啦~一定要注意哦！",
"好了，规则都学会了吗？那剩下的就交给你把，一定要加油哦！", "在规定的时间内消除完所有的图案就成功啦！努力达到三星可以获得更多奖励哦~", "恭喜你顺利闯过新手关，送给你几个道具作为奖励哦~", "恭喜你顺利闯过新手关，接下来的冒险就要靠你自己啦~",
"这是一种非常有趣的魔法，可以把5对图案变为同一种图案，快来试试吧!", "这是一种威力十分强大的炸弹，一次可以消除3对图案，快来试试吧！", "这是一种古老的法术，可以把时间冻结15秒，快来试试吧！", "",
"每天来签到领取丰富小礼物，连续签到七天赢取超级大礼。","遇到小怪兽千万不要害怕，但是要注意小怪兽施放的法术，打败他，嘿嘿嘿！", "找不到消除的图案了吗？不要慌！使用提示道具试一试。", "又找不到消除的图案了吗？不要慌！使用重排道具试一试。", 
"这是加时道具，使用它可以增加15秒的闯关时间","快来和其他小伙伴一决高低下吧！前三名可是会获得钻石奖励的哦~", "好了，让我们抓紧时间继续前进吧。", "时间结束了？没关系，使用加时道具试一试。", 
"呜呜呜~~~挑战失败了，我可不想重新开始，让我们再来一次吧。", "现在可以进入成长界面，学习新的技能了哦~", "这里有六种技能可以提升你的实力。", "凶恶的小怪兽守护着无穷无尽的宝藏，打败小怪兽，宝藏抱回家！",
"这就是游戏界面了，让我给你介绍一下基础游戏规则。", "看这2个闪闪发亮的图标，来试试滑动消除它们。", "太棒了，当局面较为复杂时，使用提示道具能帮你节省很多时间。", "看，所有的图标都重新排列了，这样我们能更容易找到可以消除的图形。",
"有时候离过关就差一点点，加时道具能帮你顺利过关哦。", "太棒了，这后面还有好多技能，我们一起来看看。", "每次消除都有几率加时，太棒了，但是我们需要通关第二幕才能领悟。", "后面还有更多有用的技能，让我们继续努力前进吧。",
"看，这里有个宝箱在闪闪发光，点进去看看。", "过关获得的星星累计到一定数量就能领取奖励，这是我们的第一个奖励,真是太棒了。", "后面还有更多的奖励，我们继续努力吧。", "这是我们通关第一幕后解锁的道具，来试试看吧。",
"此种石块无法进行消除，会阻挡你的消除路线。", "此种石块只要把宝石消除掉就可进行消除。","", "云会不定时散开，你要找机会记住被遮挡住的是什么图案。", 
"点击问号牌即可翻开查看，但是你点击其它图案时又会翻过去哦。","被锁住的图案是无法进行消除的，你要先找到对应的钥匙进行消除才能解锁哦。","", "移动要按照数字的顺序进行消除才能进行消除哦。", 
"被冰冻图案无法进行消除，必须先消除相邻的其它图案才能破碎冰块。", "被边框圈起来的图案组无法进行最外围的消除，只能从内部消除。", "打败小怪兽就能得到丰富的奖励，加油吧，我的小勇士！", "经典挑战，挑战你的极限！",
"天下武功，唯快不破，勇士敢不敢来挑战一下！"
};

NewerGuideLayer::NewerGuideLayer()
{

}
NewerGuideLayer::~NewerGuideLayer()
{
	iscanclick = false;
}
NewerGuideLayer* NewerGuideLayer::create(int type)
{
	NewerGuideLayer *pRet = new NewerGuideLayer();
	if (pRet && pRet->init(type))
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

bool NewerGuideLayer::init(int step)
{
	_step = step;
	_sj = NULL;
	isCanClick = true;
    if (!Layer::init()) 
	{
		return false;
    }
	color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color, -2);

	heroGuider = Sprite::createWithSpriteFrameName("guidehero.png");
	heroGuider->setPosition(Vec2(80, 320));
	addChild(heroGuider);

	guidetextbox = Sprite::createWithSpriteFrameName("hintbg.png");
	guidetextbox->setPosition(Vec2(300, 280));
	addChild(guidetextbox);

	std::string desc = CommonFuncs::gbk2utf(StringUtils::format("%s", descText[_step].c_str()).c_str());
	_guideText = Label::createWithBMFont("fonts/guidetext.fnt", desc);
	_guideText->setWidth(280);
	_guideText->setAnchorPoint(Vec2(0, 0.5f));
	_guideText->setAlignment(TextHAlignment::LEFT);
	_guideText->setColor(Color3B(255, 255, 255));
	_guideText->setPosition(Vec2(18, guidetextbox->getContentSize().height/2));

	if (descText[_step].length() <= 0)
	{
		_guideText->setVisible(false);
		guidetextbox->setVisible(false);
		heroGuider->setVisible(false);
	}

	if (_step == 0)
	{
		heroGuider->setPositionY(140);
		guidetextbox->setPositionY(100);
	}
	else if ( _step == 11 || _step == 21||_step == 23 || _step == 26 || _step == 31 || _step == 32 )
	{
		heroGuider->setPositionY(520);
		guidetextbox->setPositionY(480);
	}
	else if (_step == 1 || _step == 2 || _step == 3)
	{
		Sprite* line = Sprite::createWithSpriteFrameName("guideline1.png");
		line->setPosition(Vec2(300, 230));
		addChild(line, -1);

		if (_step == 1)
		{
			heroGuider->setPositionY(140);
			guidetextbox->setPositionY(100);
		}
		else if (_step == 2)
		{
			heroGuider->setPositionY(210);
			guidetextbox->setPositionY(170);

			line->setPositionY(300);
			GlobalData::setIsShowNewerGuidePromp(true);
		}
		else if (_step == 3)
		{
			heroGuider->setPositionY(210);
			guidetextbox->setPositionY(170);

			line->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("guideline2.png"));
			line->setPosition(Vec2(310, 520));
		}
		line->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0f, Vec2(0, 10)), MoveBy::create(1.0f, Vec2(0, -10)), NULL)));
	}
	else if (_step == 4)
	{
		heroGuider->setPositionY(460);
		guidetextbox->setPositionY(420);
	}
	else if (_step == 7)
	{
		heroGuider->setPositionY(520);
		guidetextbox->setPositionY(480);
	}
	else if (_step == 5)
	{
		heroGuider->setPositionY(670);
		guidetextbox->setPositionY(630);
	}
	else if (_step == 6)
	{
		heroGuider->setPositionY(140);
		guidetextbox->setPositionY(100);
	}
	else if (_step == 47 || _step == 48)
	{
		heroGuider->setPositionY(340);
		guidetextbox->setPositionY(300);
	}
	else if (_step == 46)
	{
		heroGuider->setPositionY(690);
		guidetextbox->setPositionY(650);
	}

	else if (_step == 17)
	{
		heroGuider->setPositionY(440);
		guidetextbox->setPositionY(400);
	}

	else if ( _step == 19 || _step == 20 || _step == 22 || _step == 29 || _step == 30 || _step == 33)
	{
		heroGuider->setPositionY(400);
		guidetextbox->setPositionY(360);
	}
	else if (_step == 2 || _step == 12 || _step == 38 || _step == 42)
	{
		heroGuider->setVisible(false);
		guidetextbox->setVisible(false);
	}
	else if (_step >= 36)
	{
		heroGuider->setPositionY(820);
		guidetextbox->setPositionY(780);
	}

	GlobalData::setNewerGuideStep(_step);

	guidetextbox->addChild(_guideText);

	iscanclick = false;

	this->scheduleOnce(schedule_selector(NewerGuideLayer::canClick), 1.0f);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		if (!iscanclick)
			return true;

		if (_step == 0)
		{
			removeSelf();
			GlobalData::setPlayingStage(0);
			Director::getInstance()->replaceScene(GameScene::scene());
		}

		if ((_step == 26 || _step == 27 || _step == 28) || _step == 24)
		{
			removeSelf();
			if (g_gameScene != NULL)
			{
				if (_step == 24)
				{
					_step = 3;
					g_gameScene->NewerGuideClickCB(_step);
				}
				else if (_step == 26)
				{
					g_gameScene->NewerGuideClickCB(15);
				}
				else if (_step == 27)
				{
					g_gameScene->NewerGuideClickCB(16);
				}
				else if (_step == 28)
				{
					g_gameScene->NewerGuideClickCB(18);
				}

				else
				{
					g_gameScene->NewerGuideClickCB(_step + 1);
				}
			}
		}
		else if (_step == 3)
		{
			removeSelf();
			if (g_gameScene != NULL)
				g_gameScene->NewerGuideClickCB(4);
		}
		else if (_step == 4)
		{
			removeSelf();
			if (g_gameScene != NULL)
				g_gameScene->NewerGuideClickCB(5);
		}
		else if (_step == 5)
		{
			removeSelf();
			if (g_gameScene != NULL)
			{
				GlobalData::g_gamestatus = GAMEREADY;
				g_gameScene->showReadyAnim();
			}
		}
		else if (_step == 30 || _step == 34 || _step == 23 || _step == 47 || _step == 48)
		{
			removeSelf();
		}

		else if (_step >= 36 && _step <= 46)
		{
			removeSelf();
			if (_step == 37 || _step == 41)
			{
				if (g_gameScene != NULL)
				{
					g_gameScene->showNextBlocksGuide(_step);
				}
			}
			else
				GlobalData::g_gamestatus = GAMESTART;
		}

		if (_step == 2 || _step == 1)
		{
			Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
			if (_hightNodes.size() >= 2)
			{
				if ((location.x >= _hightNodes[0]->getPositionX() - _hightNodes[0]->getContentSize().width / 2 && location.x <= _hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width / 2
					&& location.y >= _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2 && location.y <= _hightNodes[0]->getPositionY() + _hightNodes[0]->getContentSize().height / 2)
					|| (location.x >= _hightNodes[1]->getPositionX() - _hightNodes[1]->getContentSize().width / 2 && location.x <= _hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2
					&& location.y >= _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2 && location.y <= _hightNodes[1]->getPositionY() + _hightNodes[1]->getContentSize().height / 2))
				{
					for (unsigned int i = 0; i < _hightNodes.size(); i++)
					{
						_hightNodes[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
					}
					g_gameScene->onTouchBegan(touch, event);
				}

			}
		}
		return true;
	};
	listener->onTouchMoved = [=](Touch* touch, Event* event)
	{
		if (!iscanclick)
			return;
		if (_step == 2 || _step == 1)
		{
			Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
			if (_hightNodes.size() >= 2)
			{
				if ((location.x >= _hightNodes[0]->getPositionX() - _hightNodes[0]->getContentSize().width / 2 && location.x <= _hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width / 2
					&& location.y >= _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2 && location.y <= _hightNodes[0]->getPositionY() + _hightNodes[0]->getContentSize().height / 2)
					|| (location.x >= _hightNodes[1]->getPositionX() - _hightNodes[1]->getContentSize().width / 2 && location.x <= _hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2
					&& location.y >= _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2 && location.y <= _hightNodes[1]->getPositionY() + _hightNodes[1]->getContentSize().height / 2))
				{
					for (unsigned int i = 0; i < _hightNodes.size(); i++)
					{
						_hightNodes[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
					}
					g_gameScene->onTouchMoved(touch, event);
				}
			}
		}
	};
	
	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		if (!iscanclick)
			return;
		if (_step == 2 || _step == 1)
		{
			Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
			if (_hightNodes.size() >= 2)
			{
				if ((location.x >= _hightNodes[0]->getPositionX() - _hightNodes[0]->getContentSize().width / 2 && location.x <= _hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width / 2
					&& location.y >= _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2 && location.y <= _hightNodes[0]->getPositionY() + _hightNodes[0]->getContentSize().height / 2)
					|| (location.x >= _hightNodes[1]->getPositionX() - _hightNodes[1]->getContentSize().width / 2 && location.x <= _hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2
					&& location.y >= _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2 && location.y <= _hightNodes[1]->getPositionY() + _hightNodes[1]->getContentSize().height / 2))
				{
					for (unsigned int i = 0; i < _hightNodes.size(); i++)
					{
						_hightNodes[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
					}
					g_gameScene->onTouchEnded(touch, event);
				}
			}
		}
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
    return true;
}

void NewerGuideLayer::canClick(float dt)
{
	iscanclick = true;
}

void NewerGuideLayer::setData(std::vector<Node*> hiligt)
{
	_sj = Sprite::createWithSpriteFrameName("guidesj.png");
	if (hiligt.size() > 0)
	{
		for (unsigned int i = 0; i < hiligt.size(); i++)
		{

			_hightNodes.push_back(hiligt[i]);
			_Ozoder.push_back(hiligt[i]->getLocalZOrder());
			//_clipNode->setStencil(hiligt[i]);
			hiligt[i]->setLocalZOrder(NEWERLAYERZOER + 1 + i);
			_sj->setPosition(Vec2(hiligt[0]->getPositionX() + hiligt[0]->getContentSize().width / 2, hiligt[0]->getPositionY() - hiligt[0]->getContentSize().height / 2));

		}
	}
	else
	{
		_sj->setPosition(450, 350);
	}
	if (_step == 1)
	{
		Vec2 pos1 = Vec2(_hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width / 2, _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2);
		Vec2 pos2 = Vec2(_hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2, _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2);

		_sj->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.05f, pos1), Show::create(), RotateTo::create(0.5f, 20), RotateTo::create(0.5f, -20),
			Hide::create(), DelayTime::create(0.5f), MoveTo::create(0.05f, pos2), Show::create(), RotateTo::create(0.5f, 20), RotateTo::create(0.5f, -20), Hide::create(), DelayTime::create(0.5f), NULL)));
	}

	if (_step == 32)
	{
		_sj->setPosition(200, 280);
	}
	if (_step != 2)
	{
		_sj->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.8f, 20), RotateTo::create(0.8f, -20), NULL)));

		if (_step == 5)
			_sj->setPositionY(_sj->getPositionY() - 20);
	}
	else
	{
		Vec2 point0 = Vec2(_hightNodes[0]->getPositionX() + _hightNodes[0]->getContentSize().width/2, _hightNodes[0]->getPositionY() - _hightNodes[0]->getContentSize().height / 2);
		Vec2 point1 = Vec2(_hightNodes[1]->getPositionX() + _hightNodes[1]->getContentSize().width / 2, _hightNodes[1]->getPositionY() - _hightNodes[1]->getContentSize().height / 2);
		_sj->setPosition(point1);
		_sj->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.5f, point0), MoveTo::create(1.5f, point1), NULL)));
	}
	if (_step == 46 || _step == 3)
	{
		_sj->setVisible(false);
	}

	if (_step == 0)
	{
		_sj->setPosition(450, 50);
	}
	this->getParent()->addChild(_sj, NEWERLAYERZOER + 100, "newersj");
}

void NewerGuideLayer::removeSelf()
{
	iscanclick = false;
	if (_Ozoder.size() > 0)
	{
		for (unsigned int i = 0; i < _hightNodes.size(); i++)
		{
			_hightNodes[i]->setLocalZOrder(_Ozoder[i]);
		}
	}

	GlobalData::setGuide(_step, false);

	if (_step == 8 || _step == 9 || _step == 10 || _step == 11)
	{
		GlobalData::setGuide(_step, true);
	}
	if (_step == 2)
		GlobalData::setIsShowNewerGuidePromp(false);

	this->getParent()->removeChildByName("newersj");
	this->removeFromParentAndCleanup(true);
	g_NewerLayer = NULL;
}

int NewerGuideLayer::getType()
{
	return _step;
}