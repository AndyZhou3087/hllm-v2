#ifndef _SHOPLAYER_H_
#define _SHOPLAYER_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Const.h"
USING_NS_CC;
static int gemNum[] = {280, 160, 110, 70, 20 };
static float gemprice[] = {15.00f, 10.00f, 8.00f, 6.00f, 2.00f };

static int goldNum[] = { 2000, 1200, 500, 200 };
static int goldprice[] = { 100, 80, 40, 20 };

static int buyPropGemOrGoldNum[] = { 800, 80, 80 };
static int propNum[] = { 5, 5, 5 }; 

static int giftNum[][5] = { { 500, 2000, 2, 2, 2 }, { 288, 2888, 0, 0, 0 }, { 100, 1000, 3, 3, 3 }, { 100, 1, 1, 0, 0 }, { 1000, 1, 1, 0, 0 }, { 3, 3, 3, 0, 0 } };

class ShopLayer : public  Layer
{
public:
	ShopLayer();
	~ShopLayer();
	virtual bool init();
	void LoadingShop();
	void ShopClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Exchang(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void BuyGameCurrency(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void BuyProp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void BuyPackage(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	static void beginPayment(int index);
	static void setMessage(int ret);
	static int whichClicked;
	CREATE_FUNC(ShopLayer);
	static void showBuyOKAnim(const char* pic,Vec2 spoint, Vec2 dpoint);
	static int AnimType;
private:
	void delayRemoveGiftLayer(float dt);
	void delayReviveGame(float dt);
private:

	cocos2d::ui::Widget* m_shopExchangBtn[3];
	cocos2d::ui::Widget* m_pTag;
	cocos2d::ui::ScrollView* m_ScrollView[3];

	cocos2d::ui::Widget* m_buyDiamondItem[sizeof(gemNum) / sizeof(gemNum[0]) + sizeof(goldNum) / sizeof(goldNum[0])];
	cocos2d::ui::Widget* m_buyDiamondBtn[sizeof(gemNum) / sizeof(gemNum[0]) + sizeof(goldNum) / sizeof(goldNum[0])];
	cocos2d::ui::Widget* m_buyPropBtn[3];
	cocos2d::ui::Widget* m_buyGiftBtn[sizeof(giftNum) / sizeof(giftNum[0])];

	static cocos2d::ui::TextBMFont* myPrompLbl;
	static cocos2d::ui::TextBMFont* myRefreshLbl;
	static cocos2d::ui::TextBMFont* myAddTimeLbl;
	cocos2d::ui::Widget* m_shop[3];
	static void reFreshPropLable();
	bool m_isScrolling;
	void scrollFinish();
	static int _payIndex;
	bool isFirstEnter;
	static bool isPaying;
	void checkGemPCode();
	void checkGiftPCode();
	std::vector<int> vec_gemindex;
	std::vector<int> vec_giftindex;
};
//extern Node* g_shopCsbLayer;
#endif
