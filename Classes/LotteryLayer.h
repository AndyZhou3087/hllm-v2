#ifndef _LOTTERY_H_
#define _LOTTERY_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CommonLayer.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
class LotteryLayer : public  Layer
{
public:
	LotteryLayer();
	~LotteryLayer();
	
	void LoadLottery();
	void closeRewardLottery(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void LotteryOnce(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Lotteryclose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Lottery5(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void Lotterychange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void GiveRewards(int index);

	void lotteryStart(int index);
	bool init(bool isfree);
	static LotteryLayer* create(bool isfree);
private:
	cocos2d::ui::Widget* m_lotteryPointer;

	cocos2d::ui::Widget* m_lotteryBtn;

	cocos2d::ui::Widget* m_lottery5Btn;

	cocos2d::ui::Widget* m_lotteryclose;
	cocos2d::ui::Widget* m_lotterychange;
	cocos2d::ui::Widget* m_btnGemIcon;
	cocos2d::ui::Widget* m_btnGemLbl;
	cocos2d::ui::Widget* m_btnFreetext;
	std::vector<int> vecpropindex;
	cocos2d::ui::Widget* m_lotteryActionBtn;
	void lotteryover(Ref* pSender, int type);
	void resetLottery();
	Layer *xlayer;
	Node *lotteryCsbLayer;
	int m_Rotate;
	void showFreeText();
	bool m_isfree;
};
#endif