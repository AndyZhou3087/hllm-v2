package com.kuxx.hllm;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import android.app.Activity;

import com.tendcloud.tenddata.TDGAAccount;
import com.tendcloud.tenddata.TDGAItem;
import com.tendcloud.tenddata.TDGAMission;
import com.tendcloud.tenddata.TDGAVirtualCurrency;
import com.tendcloud.tenddata.TalkingDataGA;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.MobclickAgent.EScenarioType;
import com.umeng.analytics.game.UMGameAgent;
public class Analysis {
	public static Activity mcontext;
	private static TDGAAccount account;
	private static String payOrderId;
	public static void init(Activity context) {
		//UMGameAgent.setDebugMode(true);// 设置输出运行时日志
		mcontext = context;
		UMGameAgent.init(mcontext);
		MobclickAgent.setScenarioType(mcontext, EScenarioType.E_UM_GAME);
	}

	public static void startLevel(String level) {
		UMGameAgent.startLevel(level);
		TDGAMission.onBegin(level);
	}

	public static void failLevel(String level) {
		UMGameAgent.failLevel(level);
		TDGAMission.onFailed(level, "");
	}

	public static void finishLevel(String level) {
		UMGameAgent.finishLevel(level);
		TDGAMission.onCompleted(level);
	}
	
	public static void pay(String item ,int money, int number)  {
		UMGameAgent.pay((double)money, item, number, (double)0, 5);
		payOrderId = new Date().getTime() + "";
		TDGAVirtualCurrency.onChargeRequest(payOrderId, item, (double)money, "CNY", (double)number, "sms");
	}
	
	public static void payok()  {
	    TDGAVirtualCurrency.onChargeSuccess(payOrderId);
	}
	
	public static void buy(String item, int number, float gem)
	{
		UMGameAgent.buy(item, number, (double)gem);
		TDGAItem.onPurchase(item, number, (double)gem);
	}
	
	public static void use(String item, int number) {
		UMGameAgent.use(item, number, (double)0);
		TDGAItem.onUse(item, number);
	}

	public static void bonus(String item, int num, int type)
	{
		UMGameAgent.bonus(item, num, (double)0, type);
		
		if (item.equalsIgnoreCase("gem"))
			TDGAVirtualCurrency.onReward((double)num, type + "");
		else
			buy(item, num, 0);	
	}
	
	public static void onEvent(String eventId) {
		MobclickAgent.onEvent(mcontext, eventId);
		Map<String, Object> map = new HashMap<String, Object>();
		TalkingDataGA.onEvent(eventId, map);
	}
	
	public static void onEvent(String eventId, String data) {
		
		HashMap<String,String> umap = new HashMap<String,String>();
		umap.put("star", data);
		MobclickAgent.onEvent(mcontext, eventId, umap);  
		
		Map<String, Object> tmap = new HashMap<String, Object>();
		tmap.put("star", data);
		TalkingDataGA.onEvent(eventId, tmap);
	}
	
	public static void SignIn(String thirdplat, String uid, String uname) {
		UMGameAgent.onProfileSignIn(thirdplat, uid);
		
	    account = TDGAAccount.setAccount(uid);
	    account.setAccountName(uname);
	    if (thirdplat.equalsIgnoreCase("qq"))
	    	account.setAccountType(TDGAAccount.AccountType.QQ);
	    else if (thirdplat.equalsIgnoreCase("wechat"))
	    	account.setAccountType(TDGAAccount.AccountType.TYPE1);
	    else
	    	account.setAccountType(TDGAAccount.AccountType.ANONYMOUS);
	}
	
	public static void setPlayerLevel(int level)
	{
		UMGameAgent.setPlayerLevel(level);
	    account.setLevel(level); 
	}
	
	public static void onResume()
	{
		UMGameAgent.onResume(mcontext);
		TalkingDataGA.onPause(mcontext);
	}
	
	public static void onPause()
	{
		UMGameAgent.onPause(mcontext);
		TalkingDataGA.onResume(mcontext);
	}
	
	public static void exit()
	{
		MobclickAgent.onKillProcess(mcontext);
	}
}
