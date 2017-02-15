#ifndef __CONST_H__
#define __CONST_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

#define SCREEN_WIDTH 540
#define SCREEN_HEIGHT 960

#define Button_ACTION (Sequence::create(ScaleTo::create(0.07f,0.8f),NULL))
#define Button_ACTION1 (Sequence::create(ScaleTo::create(0.07f,1),NULL))

#define FONT_NAME	"Arial"
//#define LEFTOFFSET	120
//#define TOPOFFSET	350
//#define BOXWIDTH	61//77
//#define BOXHEIGHT	70//81

#define GAMETIME	30
#define ADDGAMETIME	10

//#define DOWNLOADINGMODE

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define ANALYTICS
#endif

#define USENETWORK

#define ELECOUNT	33
#define PREELECOUNT 16
#ifndef DOWNLOADINGMODE
#define LOCALTHEMECOUNT	9
#else
#define LOCALTHEMECOUNT	6
#endif
#define TOPOFFSET		42
#define NEWERGUIDECOUNT 50
#define TOSMALLCOLS	10

#define ICETIME 15.0f

#define HTTPURL	"http://115.29.224.123:9191/api.php?action="
#define ANALYTICS_HTTPURL	"http://115.29.224.123:9191/api.php?action="
#define LEVELTYPECOUNT 6

#define MAXCLASICCSTAGE 15

#define CLASICUSEHP	3
#define BOSSSTAGEPOS 10
#define MAXEVENTTYPE 3

#define LZLOGO
#ifdef USENETWORK
#define THIRDLOGIN
#define BUYFREETEXT
#endif
#define AUTOPOPGIFT
//#define ZFTEXTCLEAR

#define CLASICOPENSTAGE 13
#define FLASHOPENSTAGE 18
#define BOSSOPENSTAGE 25


#define UPGRADETIMEFILE "sysdat"

static std::string SpeciallyName[] = { "specially/hp1", "specially/hp2", "specially/card", "specially/nextcard1", "specially/nextcard2", 
										"specially/nextcard3", "specially/fail", "specially/win", "specially/win1", "specially/win2", 
										"specially/bomb","specially/bomb1","specially/barrier1", "specially/barrier2", "specially/barrier3", 
										"specially/glock1", "specially/glock2", "specially/slock1", "specially/slock2", "specially/frozen1", 
										"specially/frozen2", "specially/frozen3","specially/life","specially/boomtrail", "specially/boomover1",
										"specially/lq1", "specially/lq2", "specially/lq3", "specially/lq4", "specially/bossskill", 
										"specially/loadingshine", "specially/vip1", "specially/vip2", "specially/vip3", "specially/vip4",
										"specially/buybtn", "specially/hereqq", "specially/boomover1", "specially/boomover2", "specially/boomover3",
										"specially/boomover4", "specially/boomover5", "specially/useprop1", "specially/useprop2", "specially/useprop3",
										"specially/useprop4", "specially/useprop5", "specially/tuowei", "specially/xuehua" };

static std::string tips[] = { "关卡开始前，使用合适道具更容易过关。",
"找不到能消除的图形了？用“提示”道具试试吧~",
"使用“重排”道具能获得新生。",
"想要更多的时间，就试试“加时”道具吧~",
"升级角色可以学习技能，更加轻松应对关卡。",
"“自动提示”是个不错的技能，快去升级角色学习技能吧~",
"连续登陆能获得超多的奖励，千万别忘了领取。",
"商城里的道具，多买多送哦。",
"别忘了升级体力上限，这样每天就能回复更多体力了。",
"优惠大礼包里有你想要的东西，去看看吧~",
"游戏过程中合理的使用道具可以更加轻松过关。",
"每日去挑战BOSS可以获得丰厚奖励哦^^",
"每个BOSS都有技能，会给你增加游戏难度。",
"满足条件即可达成成就并获得成就奖励。",
"点石成金技能可以让你获得更多的金币。",
"时间加成技能可以让你的游戏时间变得更长。",
"连击延长技能可以让你轻松触发连击。",
"学习了自动消除技能后不用点击也可以消除哦^^",
"学习了消除加时技能后消除就有几率增加时间！",
"获得VIP资格后每日都可以获取固定奖励。",
"消耗钻石直接跳过关卡可以获得三星评分哦^^"
};
static int useGoldGemNum[] = { 20, 20, 20 };
static int unlockSkillstage[] = { 9, 20, 30, 40, 50, 60 };

static std::string PayCode[] = { "hllm.zs.15", "hllm.zs.10", "hllm.zs.8", "hllm.zs.6", "hllm.zs.2","hllm.lb.30.2", "hllm.lb.20.1", "hllm.lb.15.2", "hllm.lb.8.1", "hllm.lb.6",
"hllm.lb.10", "hllm.lb.15.1", "hllm.lb.6.2", "hllm.lb.6.1", "hllm.lb.30.1", "hllm.lb.20.2", "hllm.lb.15.3", "hllm.lb.8.2", "hllm.lb.6.3", "hllm.lb.10.2", 
"hllm.lb.30.3", "hllm.lb.20.3", "hllm.lb.15.4", "hllm.lb.8.3", "hllm.lb.6.4","hllm.lb.10.3", "hllm.lb.30.4", "hllm.lb.8.4", "hllm.lb.6.5", "hllm.lb.10.4", "hllm.lb.8", "hllm.lb.10.1", "hllm.lb.15", "hllm.lb.20", "hllm.lb.30" };

static const char* packagename[] = { "pyh", "pcf", "pcg", "pzs", "pjb", "pdj", "pxs", "phf", "psc", "enterpyh", "enterpcf", "enterpcg", "enterpzs", "enterpjb", "enterpdj",
"p3pyh", "p3pcf", "p3pcg", "p3pzs", "p3pjb", "p3pdj", "phomeyh", "plesszs", "plessjb", "pgamedj", "pgj", "pzx", "ptq", "pzz", "phh" };

static const int gemAmount[] = { 500, 288, 100, 100, 0, 0, 300, 0, 88, 500, 288, 100, 100, 0, 0,
500, 288, 100, 100, 500, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const int pgiftprice[] = { 30, 20, 15, 8, 6, 10, 15, 6, 6, 30, 20, 15, 8, 6, 10,
30, 20, 15, 8, 6, 10, 30, 8, 6, 10, 8, 10, 15, 20, 30 };
static const int pgiftyuanprice[] = { 45, 30, 22, 12, 10, 15, 22, 10, 10, 45, 30, 22, 12, 10, 15,
45, 30, 22, 12, 10, 15, 45, 12, 10, 15, 8, 10, 15, 20, 30 };
#endif
