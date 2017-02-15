#include "TaskLayer.h"
#include "SoundManager.h"
#include "MyParticle.h"
#include "CommonLayer.h"
#include "CommonFuncs.h"
#include "MyMenu.h"
#include "MoreLayer.h"
TaskLayer* g_TaskLayer = NULL;
bool TaskLayer::isgeting = false;
#define ITEMHEIGHT 135

std::vector<std::string> rwdPath;
TaskLayer::TaskLayer()
{
	g_TaskLayer = NULL;
	isgeting = false;

	rwdPath.push_back("UI/rprop3.png");
	rwdPath.push_back("UI/rprop4.png");
	rwdPath.push_back("UI/rprop5.png");
	rwdPath.push_back("UI/rprop0.png");
	rwdPath.push_back("UI/rprop1.png");
	rwdPath.push_back("UI/rprop2.png");
}


TaskLayer::~TaskLayer()
{
	g_TaskLayer = NULL;
	isgeting = false;
	rwdPath.clear();
}

void TaskLayer::onExit()
{
	Layer::onExit();
}

bool TaskLayer::init()
{
	if (!Layer::init()) 
	{
		return false;
	}
	selecttag = 0;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	Node * layer = CSLoader::createNode("taskLayer.csb");
	addChild(layer);

	cocos2d::ui::Widget* closeBtn = (cocos2d::ui::Widget*)layer->getChildByName("closebtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(TaskLayer::close, this));

	tagBtn0 = (cocos2d::ui::Widget*)layer->getChildByName("taskbtn");
	tagBtn0->addTouchEventListener(CC_CALLBACK_2(TaskLayer::tagBtn, this));
	tagBtn0->setTag(0);

	tagBtn1 = (cocos2d::ui::Widget*)layer->getChildByName("achivebtn");
	tagBtn1->addTouchEventListener(CC_CALLBACK_2(TaskLayer::tagBtn, this));
	tagBtn1->setTag(1);
	tagBtn1->setBright(false);

	tasklbl = (cocos2d::ui::TextBMFont*)tagBtn0->getChildByName("redpoint")->getChildByName("numlbl");
	achivelbl = (cocos2d::ui::TextBMFont*)tagBtn1->getChildByName("redpoint")->getChildByName("numlbl");

	taskscrollview = (cocos2d::ui::ScrollView*)layer->getChildByName("taskscrollview");

	achivescrollview = (cocos2d::ui::ScrollView*)layer->getChildByName("achivescrollview");
	achivescrollview->setVisible(false);

	for (unsigned int i = 0; i < GlobalData::taskData.size(); i++)
	{
		if (GlobalData::taskData[i].finish >= GlobalData::taskData[i].count)
			GlobalData::taskData[i].finish = INT_MAX / 2;
	}

	sortTaskData();

	int size = GlobalData::taskData.size();

	if (size * ITEMHEIGHT > taskscrollview->getContentSize().height)
	{
		taskscrollview->setInnerContainerSize(Size(taskscrollview->getContentSize().width, size * ITEMHEIGHT));
	}

	int startY = taskscrollview->getInnerContainerSize().height;
	for (int i = 0; i < size; i++)
	{
		TaskItem* titem = TaskItem::createWithData(&GlobalData::taskData[i]);

		titem->setPosition(Vec2(270, startY - ITEMHEIGHT / 2 - i * ITEMHEIGHT));

		std::string str = StringUtils::format("titem%d", i);
		taskscrollview->addChild(titem, 0, str);
	}
	updatelblcount();
	isloadedachive = false;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TaskLayer::sortTaskData()
{
	sort(GlobalData::taskData.begin(), GlobalData::taskData.end(), byfinish);
}

void TaskLayer::showParticle(float dt)
{
	MyParticle::Show(color, SpeciallyName[25], 270, 480, true, -1.0f);
	MyParticle::Show(color, SpeciallyName[26], 270, 480, true, -1.0f);

	MyParticle::Show(color, SpeciallyName[27], 270, 480, true, -1.0f);
	MyParticle::Show(color, SpeciallyName[28], 270, 480, true, -1.0f);
}

void TaskLayer::showRwdLayer(std::string path, int count)
{
	color = LayerColor::create(Color4B(11, 32, 22, 150));
	color->setPosition(Vec2(0, 0));
	addChild(color, 1, 999);

	Sprite * s = Sprite::createWithSpriteFrameName(path);
	s->setPosition(Vec2(270, 500));
	color->addChild(s, 2000);

	std::string str = StringUtils::format("x%d", count);
	Label *num = Label::createWithBMFont("fonts/giftnum.fnt", str);
	num->setPosition(Vec2(s->getPositionX(), 450));
	color->addChild(num, 2000);
	scheduleOnce(schedule_selector(TaskLayer::showParticle), 0.1f);

	scheduleOnce(schedule_selector(TaskLayer::removeRwdLayer), 2.0f);
	updatelblcount();
}

void TaskLayer::removeRwdLayer(float dt)
{
	if (g_TaskLayer != NULL)
	{
		unschedule(schedule_selector(TaskLayer::showParticle));
		color->removeFromParentAndCleanup(true);

		if (selecttag == 0)
		{
			//保存数据
			GlobalData::SaveTaskData();
			//排序
			sortTaskData();

			for (unsigned int i = 0; i < GlobalData::taskData.size(); i++)
			{
				std::string str = StringUtils::format("titem%d", i);
				TaskItem * item = (TaskItem*)taskscrollview->getChildByName(str);
				item->updatedata(&GlobalData::taskData[i]);
			}
		}
		else
		{
			//保存数据
			GlobalData::SaveAchieveData();
			//排序
			sortAchieveData();
			for (unsigned int i = 0; i < GlobalData::achieveData.size(); i++)
			{
				std::string str = StringUtils::format("titem%d", i);
				TaskItem * item = (TaskItem*)achivescrollview->getChildByName(str);
				item->updatedata(&GlobalData::achieveData[i]);
			}
		}
	}
	isgeting = false;
}

bool TaskLayer::byfinish(Achieve a, Achieve b)
{
	if (a.finish > b.finish)
		return true;
	return false;
}

void TaskLayer::loadAchiveScrollView()
{
	int size = GlobalData::achieveData.size();
	for (int i = 0; i < size; i++)
	{
		if (GlobalData::achieveData[i].achType != AchType::ACH_WORLD_RANK && GlobalData::achieveData[i].achType != AchType::ACH_FRIEND_RANK)
		{
			if (GlobalData::achieveData[i].finish >= GlobalData::achieveData[i].count)
				GlobalData::achieveData[i].finish = INT_MAX / 2;
		}
		else
		{
			if ((GlobalData::achieveData[i].finish <= GlobalData::achieveData[i].count && GlobalData::achieveData[i].finish > 0) || GlobalData::achieveData[i].finish == INT_MAX)
				GlobalData::achieveData[i].finish = INT_MAX;
		}
	}
	sortAchieveData();

	if (size * ITEMHEIGHT > achivescrollview->getContentSize().height)
	{
		achivescrollview->setInnerContainerSize(Size(achivescrollview->getContentSize().width, size * ITEMHEIGHT));
	}

	int startY = achivescrollview->getInnerContainerSize().height;

	for (int i = 0; i < size; i++)
	{
		TaskItem* titem = TaskItem::createWithData(&GlobalData::achieveData[i]);

		titem->setPosition(Vec2(270, startY - ITEMHEIGHT / 2 - i * ITEMHEIGHT));

		std::string str = StringUtils::format("titem%d", i);
		achivescrollview->addChild(titem, 0, str);
	}
	isloadedachive = true;
}

void TaskLayer::sortAchieveData()
{
	sort(GlobalData::achieveData.begin(), GlobalData::achieveData.end(), byfinish);
}

void TaskLayer::updatelblcount()
{
	int count = MoreLayer::checkHasFinishTask();
	std::string str;
	if (count <= 0)
		tasklbl->getParent()->setVisible(false);
	else
	{
		str = StringUtils::format("%d", count);
		tasklbl->setString(str);
	}
	count = MoreLayer::checkHasAchie();
	if (count <= 0)
		achivelbl->getParent()->setVisible(false);
	else
	{
		str = StringUtils::format("%d", count);
		achivelbl->setString(str);
	}
}

void TaskLayer::close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void TaskLayer::tagBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	int tag = node->getTag();
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (selecttag == tag)
			return;
		selecttag = tag;
		if (tag == 0)
		{
			taskscrollview->setVisible(true);
			achivescrollview->setVisible(false);
			tagBtn0->setBright(!tagBtn0->isBright());
			tagBtn1->setBright(!tagBtn1->isBright());
		}
		else
		{
			taskscrollview->setVisible(false);
			achivescrollview->setVisible(true);
			tagBtn0->setBright(!tagBtn0->isBright());
			tagBtn1->setBright(!tagBtn1->isBright());
			if (!isloadedachive)
			{
				loadAchiveScrollView();
			}
		}
	}
}

TaskItem::TaskItem()
{

}

TaskItem::~TaskItem()
{
}

TaskItem* TaskItem::createWithData(Achieve *data)
{
	TaskItem* item = new TaskItem;
	item->init(data);
	item->autorelease();
	return item;
}

bool TaskItem::init(Achieve *data)
{
	if (Sprite::initWithSpriteFrameName("UI/taskitem.png"))
	{
		mname = Label::createWithSystemFont(data->name, FONT_NAME, 25);
		mname->setColor(Color3B(255, 255, 255));
		mname->setAnchorPoint(Vec2(0, 0.5));
		mname->setPosition(Vec2(30, 108));
		addChild(mname);


		rwdSprite = Sprite::createWithSpriteFrameName(rwdPath[data->rwdtype - 4]);
		rwdSprite->setPosition(Vec2(370, 105));
		rwdSprite->setScale(0.5f);
		addChild(rwdSprite);

		std::string countstr = StringUtils::format("x%d", data->rcount);

		mcount = Label::createWithBMFont("fonts/tasknum.fnt", countstr);
		mcount->setAnchorPoint(Vec2(0, 0.5));
		mcount->setPosition(Vec2(388, 95));
		addChild(mcount);
		
		mdesc = Label::createWithSystemFont(data->desc, FONT_NAME, 20);
		mdesc->setAnchorPoint(Vec2(0, 0.5));
		mdesc->setColor(Color3B(112, 40, 0));
		mdesc->setPosition(Vec2(30, 70));
		addChild(mdesc);

		Sprite* progressbarbg = Sprite::createWithSpriteFrameName("UI/taskprogressbg.png");
		progressbarbg->setPosition(Vec2(160, 35));
		this->addChild(progressbarbg);

		progressbar = ProgressTimer::create(Sprite::createWithSpriteFrameName("UI/taskprogressbar.png"));
		progressbar->setType(ProgressTimer::Type::BAR);
		progressbar->setBarChangeRate(Vec2(1, 0));
		progressbar->setMidpoint(Vec2(0, 0));
		progressbar->setPosition(progressbarbg->getPosition());
		progressbar->setPercentage(0);
		this->addChild(progressbar);


		progresslbl = Label::createWithBMFont("fonts/taskprogressnum.fnt", "0/0");

		progresslbl->setPosition(Vec2(progressbarbg->getPositionX(), progressbarbg->getPositionY() - 5));
		addChild(progresslbl);

		finishStatusSprite = Sprite::createWithSpriteFrameName("UI/taskstatus-1.png");
		finishStatusSprite->setAnchorPoint(Vec2(1, 0.5));
		finishStatusSprite->setPosition(Vec2(this->getContentSize().width - 30, 45));
		addChild(finishStatusSprite);
		finishStatusSprite->setVisible(false);

		Sprite* item0 = Sprite::createWithSpriteFrameName("UI/taskgetbtn.png");
		Sprite* item1 = Sprite::createWithSpriteFrameName("UI/taskgetbtn.png");
		Sprite* item2 = Sprite::createWithSpriteFrameName("UI/taskgetbtn0.png");
		getbtn = MenuItemSprite::create(
			item0,
			item1,
			item2,
			CC_CALLBACK_1(TaskItem::getCallback, this));
		getbtn->setPosition(Vec2(this->getContentSize().width - getbtn->getContentSize().width / 2 - 10, 40));
		item1->setScale(0.9f);
		float scale = 1.0f - item1->getScale();
		item1->setPosition(item1->getContentSize().width*scale / 2.0f, item1->getContentSize().height*scale / 2.0f);
		MyMenu* menu = MyMenu::create();
		menu->addChild(getbtn);
		menu->setPosition(0, 0);
		this->addChild(menu);

		getbtnword = Sprite::createWithSpriteFrameName("UI/taskstatus1.png");
		getbtnword->setPosition(Vec2(getbtn->getContentSize().width / 2, getbtn->getContentSize().height / 2));
		getbtn->addChild(getbtnword);

		updatedata(data);
		return true;
	}
	return false;
}

void TaskItem::updatedata(Achieve *data)
{
	mdata = data;
	mname->setString(data->name);
	mdesc->setString(data->desc);

	std::string countstr = StringUtils::format("x%d", data->rcount);
	mcount->setString(countstr);

	int finishcount = mdata->finish;

	if (data->achType != AchType::ACH_WORLD_RANK && data->achType != AchType::ACH_FRIEND_RANK)
	{
		if (finishcount == -1)
			status = -1;
		else if (finishcount >= data->count)
		{
			status = 0;
			mdata->finish = INT_MAX / 2;
		}
		else
			status = 1;

	}
	else
	{
		if (finishcount == -1)
			status = -1;
		else if ((finishcount <= data->count && finishcount > 0) || finishcount == INT_MAX)
		{
			status = 0;
			mdata->finish = INT_MAX;
		}
		else
			status = 1;
	}

	int progressfinish = 0;
	if (status == -1)
	{
		mdata->finish = -1;
		progressfinish = data->count;
	}
	else if (status == 0)
	{
		progressfinish = data->count;
	}
	else
	{
		progressfinish = finishcount;
	}
	std::string str = StringUtils::format("%d/%d", progressfinish, data->count);
	progresslbl->setString(str);
	progressbar->setPercentage(progressfinish * 100 / data->count);

	SpriteFrame* newSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(rwdPath[data->rwdtype - 4]);
	rwdSprite->setSpriteFrame(newSprite);

	if (status == -1)
	{
		finishStatusSprite->setVisible(true);
		getbtn->setVisible(false);
	}
	else
	{
		finishStatusSprite->setVisible(false);
		getbtn->setVisible(true);
		std::string btnwordstr;
		if (status == 1)
		{
			getbtn->setEnabled(false);
			btnwordstr = "UI/taskstatus1.png";
		}
		else
		{
			btnwordstr = "UI/lqtext.png";
			getbtn->setEnabled(true);
		}
		SpriteFrame* textFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(btnwordstr);
		getbtnword->setSpriteFrame(textFrame);
	}
}


void TaskItem::getCallback(Ref* sender)
{
	if (TaskLayer::isgeting)
		return;

	if (status == 0)
	{
		TaskLayer::isgeting = true;
		mdata->finish = -1;
		int index = mdata->rwdtype - 4;

		rwdPath.push_back("UI/rprop3.png");
		rwdPath.push_back("UI/rprop4.png");
		rwdPath.push_back("UI/rprop5.png");
		rwdPath.push_back("UI/rprop0.png");
		rwdPath.push_back("UI/rprop1.png");
		rwdPath.push_back("UI/rprop2.png");
		if (index == 0)
		{
			GlobalData::setPromptProp(GlobalData::getPromptProp() + mdata->rcount);
		}
		else if (index == 1)
		{
			GlobalData::setRefreshProp(GlobalData::getRefreshProp() + mdata->rcount);
		}
		else if (index == 2)
		{
			GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + mdata->rcount);
		}
		else if (index == 3)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() + mdata->rcount);
		}
		else if (index == 4)
		{
			GlobalData::setGoldCount(GlobalData::getGoldCount() + mdata->rcount);
		}
		else if (index == 5)
		{
			GlobalData::setHpCount(GlobalData::getHpCount() + mdata->rcount);
		}

		g_TaskLayer->showRwdLayer(rwdPath[index], mdata->rcount);

#ifdef ANALYTICS
		const char* popstr[] = { "prompt", "refresh", "addtime", "gem", "coin", "hp" };
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", popstr[index]);
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, mdata->rcount, BONUSE_ACHI);
		}
#endif
	}

}