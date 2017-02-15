#include "SkillGrowLayer.h"
#include "SoundManager.h"
#include "GlobalData.h"
#include "GameScene.h"
#include "HintBox.h"
#include "NewerGuideLayer.h"
#include "CommonLayer.h"
#include "CommonFuncs.h"
#include "ThemeScene.h"
#include "TopComNode.h"

SkillGrowLayer* g_skillGrowlayer = NULL;
SkillGrowLayer::SkillGrowLayer()
{
	skilltype = 0;
}

SkillGrowLayer::~SkillGrowLayer()
{
	g_skillGrowlayer = NULL;
}
bool SkillGrowLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 192));
	this->addChild(color);

	_layer = CSLoader::createNode("skillGrowLayer.csb");
	addChild(_layer);

	TopComNode* topnode = TopComNode::create();
	topnode->setPosition(Vec2(270, 910));
	_layer->addChild(topnode);

	heroAminNode->setRotationSkewY(0);
	heroAminNode->setPosition(Vec2(270, 610));
	heroAminNode->setScale(0.7f);
	_layer->addChild(heroAminNode);

	_closeBtn = (cocos2d::ui::Button*)_layer->getChildByName("growclosebtn");
	_closeBtn->addTouchEventListener(CC_CALLBACK_2(SkillGrowLayer::close, this));

	_growUpBtn = (cocos2d::ui::Button*)_layer->getChildByName("growupbtn");
	_growUpBtn->addTouchEventListener(CC_CALLBACK_2(SkillGrowLayer::growUp, this));

	lvmaxtext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvmaxtext");
	growbtnpropbox = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("usepropbox");

	s_tipbox1 = (cocos2d::ui::Widget*)_layer->getChildByName("s_tipbox1");

	_smallgoldicon = (cocos2d::ui::Button*)_layer->getChildByName("growupbtn")->getChildByName("smallgold");
	_needGoldlbl = (cocos2d::ui::TextBMFont*)_layer->getChildByName("growupbtn")->getChildByName("growgoldbmlbl");
	_select = (cocos2d::ui::Widget*)_layer->getChildByName("select");

	selectlock = (cocos2d::ui::Widget*)_layer->getChildByName("select_sbox")->getChildByName("skilllock");
	selectprop = (cocos2d::ui::ImageView*)_layer->getChildByName("select_sbox")->getChildByName("image");

	nameimage = (cocos2d::ui::ImageView*)_layer->getChildByName("nameimage");

	cocos2d::ui::Text* nametext = (cocos2d::ui::Text*)_layer->getChildByName("nametext");
	nametext->setString(GlobalData::getMyName());

	desctext0 = (cocos2d::ui::Text*)_layer->getChildByName("desctext0");
	desctext1 = (cocos2d::ui::Text*)_layer->getChildByName("desctext1");
	nextlvtext = (cocos2d::ui::Text*)_layer->getChildByName("nextlvtext");
	int myTotalSkillLevel = GlobalData::getMyTotalSkillLevel();

	for (int i = 0; i < LEVELTYPECOUNT; i++)
	{
		std::string str = StringUtils::format("s_%d_btn", i + 1);
		cocos2d::ui::Widget* skillbox = (cocos2d::ui::Widget*)_layer->getChildByName(str);
		skillbox->setTag(i);
		skillbox->addTouchEventListener(CC_CALLBACK_2(SkillGrowLayer::skillbtn, this));
		int level = GlobalData::getMySkillLevel(i);
		if ((GlobalData::getHighestStage() + 1) >= unlockSkillstage[i])
			skillbox->getChildByName("skilllock")->setVisible(false);
		if (i == 0)
		{
			skilltype = 0;
			selectskill(skillbox);
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_layer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_layer->setPosition(Vec2(270, 480));
	_layer->setScale(0.1f);
	_layer->runAction(Speed::create(EaseSineIn::create(ScaleTo::create(0.5f, 1)), 2));
	this->scheduleOnce(schedule_selector(SkillGrowLayer::delayShowNewerGuide), 0.5f);

	this->scheduleOnce(schedule_selector(SkillGrowLayer::showHeroNormalAmin), 0.4f);
	return true;
}

void SkillGrowLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);

		g_ThemeScene->scheduleOnce(schedule_selector(ThemeScene::delayShowNewerGuide), 0.5f);
	}
}

void SkillGrowLayer::skillbtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	Node* btn = (Node*)pSender;
	skilltype = btn->getTag();
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		selectskill(btn);
	}
}

void SkillGrowLayer::selectskill(Node* btnnode)
{
	_select->setPosition(Vec2(btnnode->getPositionX(), btnnode->getPositionY() + 12));


	int level = GlobalData::getMySkillLevel(skilltype);

	for (int i = 0; i < GlobalData::mapSkillData[skilltype].maxleve; i++)
	{
		std::string starstr = StringUtils::format("star_%d", i + 1);
		_layer->getChildByName("starnode")->getChildByName(starstr)->setVisible(false);

	}
	cocos2d::ui::Widget* lvuptext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvuptext");
	lvuptext->setVisible(true);
	_smallgoldicon->setVisible(true);
	_needGoldlbl->setVisible(true);
	growbtnpropbox->setVisible(true);
	lvmaxtext->setVisible(false);

	s_tipbox1->setPositionX(btnnode->getPositionX());
	if (skilltype == LEVELTYPECOUNT - 1)
		s_tipbox1->setRotationSkewY(180);
	else
		s_tipbox1->setRotationSkewY(0);

	string valuestr = StringUtils::format("UI/s%d_0.png", skilltype + 1);
	selectprop->loadTexture(valuestr, cocos2d::ui::TextureResType::PLIST);
	selectprop->setContentSize(Sprite::createWithSpriteFrameName(valuestr)->getContentSize());

	valuestr = StringUtils::format("UI/s%d_name.png", skilltype + 1);
	nameimage->loadTexture(valuestr, cocos2d::ui::TextureResType::PLIST);
	nameimage->setContentSize(Sprite::createWithSpriteFrameName(valuestr)->getContentSize());

	if ((GlobalData::getHighestStage() + 1) >= unlockSkillstage[skilltype])
	{
		if (level > 0)
		{
			selectlock->setVisible(false);
			updatadesc(level);
		}
		else
		{
			desctext0->setVisible(false);
			valuestr = StringUtils::format(GlobalData::mapSkillData[skilltype].desc.c_str(), GlobalData::mapSkillData[skilltype].data[level]);
			nextlvtext->setVisible(true);
			desctext1->setVisible(true);
			desctext1->setString(valuestr);
		}
	}
	else
	{
		selectlock->setVisible(true);
		_layer->getChildByName("starnode")->setVisible(false);
		valuestr = StringUtils::format("通过%d关解锁", unlockSkillstage[skilltype]);
		desctext0->setVisible(true);
		desctext0->setString(CommonFuncs::gbk2utf(valuestr.c_str()));
		valuestr = StringUtils::format(GlobalData::mapSkillData[skilltype].desc.c_str(), GlobalData::mapSkillData[skilltype].data[level]);
		nextlvtext->setVisible(true);
		desctext1->setVisible(true);
		desctext1->setString(valuestr);
	}

	needGold = 100 * (GlobalData::getMySkillLevel(skilltype) + 1);
	valuestr = StringUtils::format("x%d", needGold);
	_needGoldlbl->setString(valuestr);

}
void SkillGrowLayer::updatadesc(int level)
{
	std::string valuestr = StringUtils::format(GlobalData::mapSkillData[skilltype].desc.c_str(), GlobalData::mapSkillData[skilltype].data[level - 1]);
	desctext0->setVisible(true);
	desctext0->setString(valuestr);

	for (int i = 0; i < GlobalData::mapSkillData[skilltype].maxleve; i++)
	{
		std::string starstr = StringUtils::format("star_%d", i + 1);
		_layer->getChildByName("starnode")->setVisible(true);
		cocos2d::ui::Widget* star = (cocos2d::ui::Widget*)_layer->getChildByName("starnode")->getChildByName(starstr);
		if (i < level)
		{
			star->setVisible(true);
		}
		else
		{
			star->setVisible(false);
		}
	}

	if (level >= GlobalData::mapSkillData[skilltype].maxleve)
	{
		nextlvtext->setVisible(false);
		desctext1->setVisible(false);
		cocos2d::ui::Widget* lvuptext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvuptext");
		lvuptext->setVisible(false);
		_smallgoldicon->setVisible(false);
		_needGoldlbl->setVisible(false);
		growbtnpropbox->setVisible(false);
		lvmaxtext->setVisible(true);

	}
	else
	{
		valuestr = StringUtils::format(GlobalData::mapSkillData[skilltype].desc.c_str(), GlobalData::mapSkillData[skilltype].data[level]);
		nextlvtext->setVisible(true);
		desctext1->setVisible(true);
		desctext1->setString(valuestr);

		cocos2d::ui::Widget* lvuptext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvuptext");
		lvuptext->setVisible(true);
		_smallgoldicon->setVisible(true);
		_needGoldlbl->setVisible(true);
		growbtnpropbox->setVisible(true);
		lvmaxtext->setVisible(false);

	}
}

void SkillGrowLayer::growUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (GlobalData::checkGuide(22))
		{
			if (g_NewerLayer != NULL)
				g_NewerLayer->removeSelf();
		}
		if ((GlobalData::getHighestStage() + 1) < unlockSkillstage[skilltype])
			return;

		int nextlevel = GlobalData::getMySkillLevel(skilltype) + 1;

		if (nextlevel > GlobalData::mapSkillData[skilltype].maxleve)
			return;

		if (needGold <= GlobalData::getGoldCount())
		{
			Achieve* data = GlobalData::getAchieveDataByID(19);
			if (data->finish != -1)
			{
				data->finish += needGold;
				GlobalData::SaveAchieveData();
			}

			data = GlobalData::getTaskDataByID(4);
			if (data->finish != -1)
			{
				data->finish += needGold;
				GlobalData::SaveTaskData();
			}

#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			char p_str1[32] = { 0 };
			sprintf(p_str, "%s", "coin");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, needGold);
			}
			sprintf(p_str, "skill%d", skilltype);
			sprintf(p_str1, "%d", nextlevel);
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				jstring para2 = methodInfo.env->NewStringUTF(p_str1);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, para2);
			}
#endif
			GlobalData::setGoldCount(GlobalData::getGoldCount() - needGold);

			needGold = 100 * (nextlevel + 1);

			std::string goldstr = StringUtils::format("x%d", needGold);
			_needGoldlbl->setString(goldstr);

			int maxlevel = GlobalData::mapSkillData[skilltype].maxleve;
			if (nextlevel >= maxlevel)
			{
				GlobalData::setMySkilLevel(skilltype, maxlevel);
				cocos2d::ui::Widget* lvuptext = (cocos2d::ui::Widget*)_layer->getChildByName("growupbtn")->getChildByName("lvuptext");
				lvuptext->setVisible(false);
				_smallgoldicon->setVisible(false);
				_needGoldlbl->setVisible(false);
				growbtnpropbox->setVisible(false);
				lvmaxtext->setVisible(true);
			}
			else
			{
				GlobalData::setMySkilLevel(skilltype, nextlevel);
			}

			updatadesc(nextlevel);
			SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_SKILLSUCC);
			HintBox* hint = HintBox::create("升级成功", 0);
			addChild(hint, 1000);

			heroAction = CSLoader::createTimeline(heroAminNode->getName()); //加载动画：
			heroAminNode->runAction(heroAction);
			heroAction->play("jump", false);

			this->scheduleOnce(schedule_selector(SkillGrowLayer::showHeroNormalAmin), 0.5f);

			data = GlobalData::getAchieveDataByID(20);
			if (data->finish != -1)
			{
				data->finish = GlobalData::getMyTotalSkillLevel();
				GlobalData::SaveAchieveData();
			}

			data = GlobalData::getAchieveDataByID(21);
			if (data->finish != -1)
			{
				data->finish = GlobalData::getMyTotalSkillLevel();
				GlobalData::SaveAchieveData();
			}

			data = GlobalData::getAchieveDataByID(22);
			if (data->finish != -1)
			{
				data->finish = GlobalData::getMyTotalSkillLevel();
				GlobalData::SaveAchieveData();
			}
		}
		else
		{
			HintBox* hint = HintBox::create("金币不足", 2);
			addChild(hint, 1000);
		}

	}
}
void SkillGrowLayer::showHeroNormalAmin(float t)
{
	heroAminNode->stopAllActions();
	heroAction = CSLoader::createTimeline(heroAminNode->getName()); //加载动画：
	heroAminNode->runAction(heroAction);
	heroAction->play("stand", true);
}

void SkillGrowLayer::delayShowNewerGuide(float dt)
{
	if (GlobalData::checkGuide(22) && GlobalData::getHighestStage() == 8)
	{
		std::vector<Node*> nodes;
		nodes.push_back(_growUpBtn);
		g_NewerLayer = NewerGuideLayer::create(22);
		_layer->addChild(g_NewerLayer, NEWERLAYERZOER);
		g_NewerLayer->setData(nodes);
	}
}