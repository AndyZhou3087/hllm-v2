#include "LotteryLayer.h"
#include "HintBox.h"
#include "math.h"
#include "GlobalData.h"
#include "SoundManager.h"
#include "CommonLayer.h"
#include "TopComNode.h"

int lotteryoncegem = 10;
int lottery5gem = 40;
int kindrand[] = {15, 42, 67,86, 100, 100};
int showrand[] = {20, 30, 65, 90, 100, 15, 30 ,60, 90, 100, 20, 35, 55, 80, 100, 20, 30, 50, 75, 100, 10, 35, 60, 75, 100, 40, 50, 60, 90, 100};
int image[] = {2,1,0,3,5,2,1,0,4,4,2,1,0,3,5,2,1,3,4,5,2,3,4,1,5,0,3,4,0,5};
int PorpNum[] = { 3, 100, 8, 2, 2, 5, 200, 10, 1, 2, 10, 500, 20, 1, 1, 20, 1000, 3,3,3,25,5,5,300,5,166,9,9,100,9 };

typedef struct
{
	std::vector<int> image;
	std::vector<int> propnum;
	std::vector<int> rand;
}LotteryRewardData;

std::vector<LotteryRewardData> vec_RewardData;
LotteryLayer::LotteryLayer() :m_Rotate(0)
{
	xlayer = NULL;
	m_isfree = false;
	vec_RewardData.clear();
	int size = sizeof(kindrand) / sizeof(kindrand[0]);
	for (int i = 0; i < size; i++)
	{
		LotteryRewardData data;
		for (int j = 0; j < 5; j++)
		{
			int index = j + i * 5;
			data.image.push_back(image[index]);
			data.propnum.push_back(PorpNum[index]);
			data.rand.push_back(showrand[index]);
		}
		vec_RewardData.push_back(data);
	}
}


LotteryLayer::~LotteryLayer()
{
}

bool LotteryLayer::init(bool isfree)
{
	if (!Layer::init())
	{
		return false;
	}
	m_isfree = isfree;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 220));
	color->setPosition(Vec2(0, 0));
	this->addChild(color);

	LoadLottery();

#ifdef ANALYTICS
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", "poplottery");
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
	}
#endif

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

LotteryLayer* LotteryLayer::create(bool isfree)
{
	LotteryLayer *pRet = new LotteryLayer();
	if (pRet && pRet->init(isfree))
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

void LotteryLayer::LoadLottery()
{
	lotteryCsbLayer = CSLoader::createNode("LotteryLayer.csb");
	addChild(lotteryCsbLayer);

	TopComNode* topnode = TopComNode::create();
	topnode->setPosition(Vec2(270, 910));
	lotteryCsbLayer->addChild(topnode);

	m_lotteryBtn = (cocos2d::ui::Widget*)lotteryCsbLayer->getChildByName("lotterybtn");
	m_lotteryBtn->addTouchEventListener(CC_CALLBACK_2(LotteryLayer::LotteryOnce, this));

	m_lottery5Btn = (cocos2d::ui::Widget*)lotteryCsbLayer->getChildByName("lottery5btn");
	m_lottery5Btn->addTouchEventListener(CC_CALLBACK_2(LotteryLayer::Lottery5, this));
	m_lotteryPointer = (cocos2d::ui::Widget*)lotteryCsbLayer->getChildByName("lotteryPointer");

	m_lotteryclose = (cocos2d::ui::Widget*)lotteryCsbLayer->getChildByName("closebtn");
	m_lotteryclose->addTouchEventListener(CC_CALLBACK_2(LotteryLayer::Lotteryclose, this));

#ifdef ZFTEXTCLEAR
	m_lotteryclose->setVisible(false);
	cocos2d::ui::Widget* colseBtnclear = (cocos2d::ui::Widget*)lotteryCsbLayer->getChildByName("closebtnclear");
	colseBtnclear->addTouchEventListener(CC_CALLBACK_2(LotteryLayer::Lotteryclose, this));
	colseBtnclear->setVisible(true);
#endif

	m_lotterychange = (cocos2d::ui::Widget*)lotteryCsbLayer->getChildByName("lotterychangebtn");
	m_lotterychange->addTouchEventListener(CC_CALLBACK_2(LotteryLayer::Lotterychange, this));

	m_btnGemIcon = (cocos2d::ui::Widget*)m_lotteryBtn->getChildByName("diamond");
	m_btnGemLbl = (cocos2d::ui::Widget*)m_lotteryBtn->getChildByName("gemlbl");
	m_btnFreetext = (cocos2d::ui::Widget*)m_lotteryBtn->getChildByName("freetext");

	showFreeText();

	resetLottery();
}

void LotteryLayer::showFreeText()
{
	if (m_isfree)
	{
		m_btnGemIcon->setVisible(true);
		m_btnGemLbl->setVisible(false);
		m_btnFreetext->setVisible(true);
	}
	else
	{
		m_btnGemIcon->setVisible(true);
		m_btnGemLbl->setVisible(true);
		m_btnFreetext->setVisible(false);
	}
}

void LotteryLayer::resetLottery()
{
	m_Rotate = 0;
	m_lotteryPointer->setRotation(0);
	vecpropindex.clear();

	int syssec = GlobalData::getSystemSecTime();
	int static randNum = 0;
	randNum += 60 * 60 * 1000;
	syssec += randNum;
	srand(syssec);

	for (unsigned int i = 0; i < vec_RewardData.size(); i++)
	{
		int r = rand() % 100;
		int j = 0;

		int randsize = vec_RewardData[i].rand.size();
		for (j = 0; j < randsize; j++)
		{
			if (r <= vec_RewardData[i].rand[j])
				break;
		}

		vecpropindex.push_back(j + i * randsize);
	}

	randNum += 60 * 60;
	syssec += randNum;
	srand(syssec);
	std::random_shuffle(vecpropindex.begin(), vecpropindex.end());
	for (unsigned int i = 0; i < vecpropindex.size(); i++)
	{
		std::string propstr = StringUtils::format("Image_%d", i + 1);

		cocos2d::ui::ImageView* propimage = (cocos2d::ui::ImageView*)lotteryCsbLayer->getChildByName(propstr);
		std::string str = StringUtils::format("UI/rprop%d.png", image[vecpropindex[i]]);
		propimage->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		propimage->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

		std::string proplblstr = StringUtils::format("lbl_%d", i + 1);
		cocos2d::ui::TextBMFont* proplbl = (cocos2d::ui::TextBMFont*)lotteryCsbLayer->getChildByName(proplblstr);

		str = StringUtils::format("x%d", PorpNum[vecpropindex[i]]);
		proplbl->setString(str);
	}
}
void LotteryLayer::LotteryOnce(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_isfree)
		{
			m_isfree = false;
			showFreeText();
			lotteryStart(-1);

#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "freelottery");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
#endif
		}
		else if (GlobalData::getGemCount() >= lotteryoncegem)
		{
			GlobalData::setGemCount(GlobalData::getGemCount() - lotteryoncegem);
			lotteryStart(-1);
#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "gemlottery");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}
#endif
		}
		else
		{
			HintBox* hint = HintBox::create("钻石不足", 1);
			addChild(hint, 10000);
		}
	}
}

void LotteryLayer::Lotteryclose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#ifdef ANALYTICS
		JniMethodInfo methodInfo;
		char p_str[32] = { 0 };
		sprintf(p_str, "%s", "closeottery");
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
		}
#endif
		this->removeFromParentAndCleanup(true);
	}
}

void LotteryLayer::Lotterychange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getGoldCount() >= 100)
		{
			resetLottery();
			GlobalData::setGoldCount(GlobalData::getGoldCount() - 100);
#ifdef ANALYTICS
			JniMethodInfo methodInfo;
			char p_str[32] = { 0 };
			sprintf(p_str, "%s", "changelottery");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "onEvent", "(Ljava/lang/String;)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
			}

			sprintf(p_str, "%s", "coin");
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "use", "(Ljava/lang/String;I)V"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, 100);
			}
#endif
		}
		else
		{
			HintBox* hint = HintBox::create("金币不足", 2);
			addChild(hint, 10000);
		}
	}
}

void LotteryLayer::Lottery5(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (GlobalData::getGemCount() >= lottery5gem)
		{
			GlobalData::setGemCount(GlobalData::getGoldCount() - lottery5gem);
			lotteryStart(0);
		}
		else
		{
			HintBox* hint = HintBox::create("钻石不足", 1);
			addChild(hint, 10000);
		}
	}
}

void LotteryLayer::lotteryStart(int index)
{
	float aciontime = 10.0f;
	if (index >= 0)
		aciontime = 3.0f;
	else
	{
		SoundManager::sharedManager()->playSound(SoundManager::SOUND_ID_LOTTERY);
	}

	m_lotteryBtn->setEnabled(false);
	m_lottery5Btn->setEnabled(false);
	m_lotteryclose->setEnabled(false);
	m_lotterychange->setEnabled(false);
	int syssec = GlobalData::getSystemSecTime();
	int static randNum = 0;
	randNum += 60 * 60 * 1000;
	syssec += randNum;
	srand(syssec);

	int r = rand() % 100;
	int kindsize = sizeof(kindrand) / sizeof(kindrand[0]);
	int rewarindex = 0;
	for (int i = 0; i < kindsize; i++)
	{
		if (r <= kindrand[i])
		{
			rewarindex = i;
			break;
		}
	}

	for (unsigned int i = 0; i < vecpropindex.size(); i++)
	{
		int rewardindex = vecpropindex[i];
		if (rewardindex / 5 == rewarindex)
		{
			m_Rotate = i + 1;
		}
	}

	int y = (int)(m_lotteryPointer->getRotation()) % 360;

	int r1 = rand() % (360 / (kindsize - 20)) + 10;
	int t = (m_Rotate * 60 - r1) + 360 * 40 + 360 - y;


	RotateBy *x = RotateBy::create(aciontime, t);
	ActionInterval * easeSineInOut = EaseExponentialInOut::create(x);
	m_lotteryPointer->runAction(Sequence::create(easeSineInOut, CallFuncN::create(CC_CALLBACK_1(LotteryLayer::lotteryover, this, index)), NULL));
	
}


void LotteryLayer::lotteryover(Ref* pSender, int index)
{
	Node* pNode;
	if (xlayer == NULL)
	{
		xlayer = Layer::create();
		xlayer->setPosition(Vec2(0, 0));
		this->addChild(xlayer, 3);

		LayerColor* color1 = LayerColor::create(Color4B(11, 32, 22, 192));
		xlayer->addChild(color1);

		pNode = CSLoader::createNode("LotteryActionLayer.csb");
		xlayer->addChild(pNode);
		auto acitonlignt = pNode->getChildByName("action1");
		acitonlignt->setVisible(false);
		if (index < 0)
		{
			acitonlignt->setVisible(true);
			auto action = CSLoader::createTimeline("LotteryActionLayer.csb");
			pNode->runAction(action);
			action->gotoFrameAndPlay(0, 280, true);
		}
		
		m_lotteryActionBtn = (cocos2d::ui::Widget*)pNode->getChildByName("LotteryActionBtn");
		m_lotteryActionBtn->addTouchEventListener(CC_CALLBACK_2(LotteryLayer::closeRewardLottery, this));
		m_lotteryActionBtn->setEnabled(false);
	}
	std::string font = StringUtils::format("UI/%d.png", image[vecpropindex[m_Rotate-1]]);
	auto sprite = Sprite::createWithSpriteFrameName(font);

	sprite->setScale(0.01f);
	sprite->setPosition(Vec2(270, 700));
	pNode->addChild(sprite);
	
	std::string numstr = StringUtils::format("x%d", PorpNum[vecpropindex[m_Rotate - 1]]);
	Label* rewardlbl = Label::createWithBMFont("fonts/lotterypropnum.fnt", numstr);
	
	rewardlbl->setPosition(Vec2(sprite->getContentSize().width/2, -30));
	
	rewardlbl->setString(numstr);
	sprite->addChild(rewardlbl);

	Vec2 movetopos;
	if (index < 0)
	{
		movetopos = Vec2(270, 500);
		sprite->runAction(Spawn::create(ScaleTo::create(0.3f, 1), MoveTo::create(0.3f, movetopos), NULL));
		m_lotteryActionBtn->setEnabled(true);
	}
	else
	{
		movetopos = Vec2(70 + 100 * index, 500);
		sprite->runAction(Spawn::create(ScaleTo::create(0.3f, 0.6f), MoveTo::create(0.3f, movetopos), NULL));	
	}
	if (index < 4 && index >= 0)
	{
		lotteryStart(index + 1);
	}
	else if (index == 4)
	{
		auto acitonlignt = pNode->getChildByName("action1");
		acitonlignt->setVisible(true);

		auto action = CSLoader::createTimeline("LotteryActionLayer.csb");
		pNode->runAction(action);
		action->gotoFrameAndPlay(0, 280, true);
		m_lotteryActionBtn->setEnabled(true);
	}
	GiveRewards(vecpropindex[m_Rotate-1]);
}

void LotteryLayer::closeRewardLottery(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonLayer::Action(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_lotteryBtn->setEnabled(true);
		m_lottery5Btn->setEnabled(true);
		m_lotteryclose->setEnabled(true);
		m_lotterychange->setEnabled(true);
		xlayer->removeAllChildrenWithCleanup(true);
		xlayer = NULL;
		resetLottery();
	}
}

void LotteryLayer::GiveRewards(int type)
{
	int index = image[type];
	switch (index)
	{
	case 0:
		GlobalData::setGemCount(GlobalData::getGemCount() + PorpNum[type]);
		break;
	case 1:
		GlobalData::setGoldCount(GlobalData::getGoldCount() + PorpNum[type]);
		break;
	case 2:
		GlobalData::setHpCount(GlobalData::getHpCount() + PorpNum[type]);
		break;
	case 3:
		GlobalData::setPromptProp(GlobalData::getPromptProp() + PorpNum[type]);
		break;
	case 4:
		GlobalData::setRefreshProp(GlobalData::getRefreshProp() + PorpNum[type]);
		break;
	case 5:
		GlobalData::setAddtimeProp(GlobalData::getAddtimeProp() + PorpNum[type]);
		break;
	default:
		break;
	}

#ifdef ANALYTICS
	const char* popstr[] = { "gem", "coin", "hp", "prompt", "refresh", "addtime" };
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", popstr[index]);
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/hllm/Analysis", "bonus", "(Ljava/lang/String;II)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1, PorpNum[type], BONUSE_LOTTERY);
	}
#endif
}