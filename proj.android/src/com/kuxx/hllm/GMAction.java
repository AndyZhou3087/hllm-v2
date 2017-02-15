package com.kuxx.hllm;

import android.app.Activity;
import android.os.Message;

public class GMAction {
	static Activity context;
	static int isfinished = 0;
	static Runnable getServerDataThread;
	public static void goby(String payCode)
	{ 
//		 if (!Utils.getNetworkAvailable())
//		 {
//	         Message msg = hllm.handler.obtainMessage();
//	         msg.what = 1;
//	         msg.sendToTarget();
//			 return;
//		 }
//         Message msg = hllm.handler.obtainMessage();
//         msg.what = 3;
//         msg.obj = payCode;
//         msg.sendToTarget();
        Message msg = hllm.handler.obtainMessage();
        msg.what = 4;
        msg.sendToTarget();

	}

	public static void init(Activity act)
	{
		context = act;
		OGThranPay.init(act);
		getServerDataThread = new Runnable()      
		{     
		 public void run()
		 {
			 getconfigData();
		 }                
		};
	}
	
	public static void destroy()
	{
		OGThranPay.destroy();
	}
	
	public static void getPayData()
	{
		context.runOnUiThread(getServerDataThread);
	}
	
	public static int isFinished()
	{
		return isfinished;
	}
	
	public static void getconfigData()
	{
        OGThranPay.getShopList();
        OGThranPay.getGameGiftTaggleParam();
		isfinished = 1;
	}
}
