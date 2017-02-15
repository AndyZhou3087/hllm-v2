#include "HintBox.h"
#include "ShopLayer.h"
#include "HpBuyLayer.h"
#include "GlobalData.h"
#include "FourTypeGift.h"
#include "CommonFuncs.h"
bool HintBox::init(std::string text, int type)
{
	
    if (Layer::init()) 
	{
		Label* textlbl = Label::createWithBMFont("fonts/tips.fnt", CommonFuncs::gbk2utf(text.c_str()));
		textlbl->setPosition(Vec2(270, 480));
		int textwidth = textlbl->getContentSize().width;
		int fontsize = textlbl->getLineHeight();
		textlbl->setColor(Color3B(255, 255, 255));
		addChild(textlbl, 1);

		boxbg = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("hintbg.png", Rect(80, 25, 130, 20));
		int boxbgOriginalheight = boxbg->getContentSize().height;

		int linecount = 1;

		if (textwidth > 200 && textwidth <= 400)
		{
			boxbg->setContentSize(Size(textwidth + 70, boxbgOriginalheight));
		}

		if (textwidth > 400)
		{
			linecount = textwidth % 400 == 0 ? textwidth / 400 : (textwidth / 400 + 1);
			textlbl->setWidth(400);
			textlbl->setHeight(linecount * fontsize);
			boxbg->setContentSize(Size(470, boxbgOriginalheight + (linecount - 1) * fontsize));
		}

		boxbg->setPosition(Vec2(270, 480));
		addChild(boxbg);
    }

	_type = type;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create(CC_CALLBACK_0(HintBox::removSelf, this)), NULL));
    return true;
}

HintBox* HintBox::create(std::string content, int type)
{
	HintBox *pRet = new HintBox();
	if (pRet && pRet->init(content, type))
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

void HintBox::removSelf()
{
	this->removeFromParentAndCleanup(true);
	if (_type == 0)
	{
	}
	else if (_type == 1)//钻石
	{
		if (GlobalData::checkPayCode(sizeof(gemNum) / sizeof(gemNum[0]) + LESS_GEMGIFT, 1))
		{
			FourTypeGift::AnimType = A_UPTODOWN;
			FourTypeGift::_type = LESS_GEMGIFT;
			FourTypeGift* giftLayer = FourTypeGift::create();
			Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
		}
		else
		{
			ShopLayer::whichClicked = 0;
			ShopLayer::AnimType = A_UPTODOWN;
			Layer* layer = ShopLayer::create();
			Director::getInstance()->getRunningScene()->addChild(layer, 0);
		}
	}
	else if (_type == 2)//金币
	{

		if (GlobalData::checkPayCode(sizeof(gemNum) / sizeof(gemNum[0]) + LESS_COINGIFT, 1))
		{
			FourTypeGift::AnimType = A_UPTODOWN;
			FourTypeGift::_type = LESS_COINGIFT;
			FourTypeGift* giftLayer = FourTypeGift::create();
			Director::getInstance()->getRunningScene()->addChild(giftLayer, 0, "giftlayer");
		}
		else
		{
			ShopLayer::whichClicked = 0;
			ShopLayer::AnimType = A_UPTODOWN;
			Layer* layer = ShopLayer::create();
			Director::getInstance()->getRunningScene()->addChild(layer, 0);
		}
	}
	else if (_type == 3)
	{
		HpBuyLayer::AnimType = A_UPTODOWN;
		Layer* layer = HpBuyLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}