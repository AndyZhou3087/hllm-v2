package com.kuxx.hllm;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;

import com.og.gameconfig.OGLoadParamsCallBack;
import com.og.unite.charge.OGSdkIPayCenter;
import com.og.unite.main.OGSdkPlatform;
import com.og.unite.shop.OGSdkIShopCenter;
import com.og.unite.shop.bean.*;

public class OGThranPay {
	public static Activity context;
	public static ArrayList<OGSDKMall> goodsList = null;
	public static final String paramKey = "gameparam";
	public static final String paramKey1 = "gameinit";
	public static int giftbtnmode = 0;
	public static Map<String,Integer> keyMap = null;
	public static String payType = null;
	public static String[] bypcode = {"hllm.lb.8","hllm.lb.10.1", "hllm.lb.15","hllm.lb.20","hllm.lb.30"};
	public static String[] pcode0 = {"hllm.lb.30.1", "hllm.lb.20.2", "hllm.lb.15.3", "hllm.lb.8.2", "hllm.lb.6.3", "hllm.lb.10.2"};
	public static String[] pcode1 = {"hllm.lb.30.3", "hllm.lb.20.3", "hllm.lb.15.4", "hllm.lb.8.3", "hllm.lb.6.4","hllm.lb.10.3"};

	public static void init(Activity act)
	{
		context = act;
		//OGSdkPlatform.setConnectLog(true);
		OGSdkPlatform.initSDK(act);
		
		String otAppId = Utils.getMetaData(context, "OT_APPID");
		OGSdkPlatform.initGameParamsByKey(act, otAppId, paramKey + "|" + paramKey1);
	}
	
	public static void destroy()
	{
		OGSdkPlatform.destroy();
	}
	
    public static void getShopList()
    {
		String uuid = Utils.UUID();
	
		OGSdkPlatform.getShopList((Activity)context, uuid, uuid,  new OGSdkIShopCenter(){
			
			public void onGetShopListResult(OGSDKShopData resultData) {
				ArrayList<?> malls = resultData.getMallList();
				if (malls != null && malls.size() > 0) {
					
					if(goodsList == null)
						goodsList = new ArrayList<OGSDKMall>();
					else
						goodsList.clear();
					for (Object mall : malls) {
						goodsList.add((OGSDKMall)mall);
					}
					Collections.sort(goodsList, new SortBySortNum());
				}
			}
		});
    }
    
    public static void getGoodsList(final String pcode)
    {
		String uuid = Utils.UUID();
	
		OGSdkPlatform.getShopList((Activity)context, uuid, uuid,  new OGSdkIShopCenter(){
			
			public void onGetShopListResult(OGSDKShopData resultData) {
				ArrayList<?> malls = resultData.getMallList();
				if (malls != null && malls.size() > 0) {
					
					if(goodsList == null)
						goodsList = new ArrayList<OGSDKMall>();
					else
						goodsList.clear();
					for (Object mall : malls) {
						goodsList.add((OGSDKMall)mall);
					}
					Collections.sort(goodsList, new SortBySortNum());
					checkPayTypeAndPay(pcode);

				}else{
			        Message msg = hllm.handler.obtainMessage();
			        msg.what = 5;
			        msg.sendToTarget();
				}
				
			}
		});
    }
   
    public static String checkBYPCode()
    {
    	if (goodsList != null)
    	{
    		OGSDKMall mall;
			for (int i=0;i < goodsList.size(); i++)
			{
				mall = goodsList.get(i);

				for (int j = 0; j < bypcode.length; j++)	
				{
					if (mall.getProductId().equalsIgnoreCase(bypcode[j]))
					{
						return bypcode[j];
					}
				}
			}
    	}
    	return "";
    }
    
    public static String checkPCode(int where)
    {
    	if (goodsList != null)
    	{
    		OGSDKMall mall;

			for (int i=0; i < goodsList.size(); i++)
			{
				mall = goodsList.get(i);
	    		if (where == 0)//进入游戏
	    		{
					for (int j = 0; j < pcode0.length; j++)	
					{
						if (mall.getProductId().equalsIgnoreCase(pcode0[j]))
						{
							return pcode0[j];
						}
					}
	    		}
	    		else if (where == 1)//每3关
	    		{
					for (int j = 0; j < pcode1.length; j++)	
					{
						if (mall.getProductId().equalsIgnoreCase(pcode1[j]))
						{
							return pcode1[j];
						}
					}
	    		}
			}
    	}
    	return "";
    }
   
    public static boolean checkPCodeExist(String pcode, int assign)
    {
    	if (goodsList != null)
    	{
    		OGSDKMall mall;

			for (int i=0; i < goodsList.size(); i++)
			{
				mall = goodsList.get(i);
	
				if (assign == 0)
				{
					if (mall.getProductId().equalsIgnoreCase(pcode))
					{
						return true;
					}
				}
				else
				{
					if (mall.getProductId().equalsIgnoreCase(pcode) && mall.getSdkPackageKey().equalsIgnoreCase("SENDSMS"))
					{
						return true;
					}
				}
			}
    	}
    	return true;
    }
    
    public static void checkPayTypeAndPay(String pcode)
    {
		for (int i=0;i < goodsList.size(); i++)
		{
			if (goodsList.get(i).getProductId().equals(pcode))
			{
		        Message msg = hllm.handler.obtainMessage();
		        msg.what = 7;
		        Bundle data = new Bundle();
		        data.putString("pcode", pcode);
		        data.putString("type", goodsList.get(i).getSdkPackageKey());
		        msg.setData(data);
		        msg.sendToTarget();
				return;
			}
		}
		
        Message msg = hllm.handler.obtainMessage();
        msg.what = 5;
        msg.sendToTarget();
    }
    
	public static void pay(final String pcode, String type)
	{
		String uuid = Utils.UUID();
		String mtime = new Date().getTime() + "";
		payType = type;

		OGSdkPlatform.payUI(context, type, uuid, pcode, mtime, uuid, new OGSdkIPayCenter(){
		//OGSdkPlatform.pay(context, type, uuid, pcode, mtime, uuid, new OGSdkIPayCenter(){

		@Override
			public void onPayResult(int result, String desc) {
				if (result == 0)
				{
			        Message msg = hllm.handler.obtainMessage();
			        msg.what = 4;
			        msg.sendToTarget();
				}
				else if (result == 1001)
				{
			        Message msg = hllm.handler.obtainMessage();
			        msg.what = 3;
			        msg.sendToTarget();
				}
				else
				{
			        Message msg = hllm.handler.obtainMessage();
			        msg.what = 5;
			        msg.sendToTarget();
				}
			}
		});
	}
	
	public static void getGameConfirmParam()
	{
		OGSdkPlatform.getGameParamByKey(context, paramKey,new  OGLoadParamsCallBack(){
			public void  onLoad(String  param){
		    JSONObject json;
			try 
			{
				json = new JSONObject(param);
				JSONArray retArray = json.getJSONArray("sdkpackage");
				for (int i=0;i<retArray.length();i++)
				{
					JSONObject arrinfo = retArray.getJSONObject(i);
					String type = arrinfo.getString("SdkPackageId");
					
					keyMap = new HashMap<String,Integer>();
					int value = arrinfo.getBoolean("IsConfirm")?1:0;
					keyMap.put(type, value);
				}
			}catch(Exception ex)
			{
				if (keyMap != null)
					keyMap.clear();
				keyMap = null;
			}
			//Log.d("", "zhou isComfirm = " + isComfirm);
	} 
	});

	}
	
	public static int getIsComfirm()
	{
		if (keyMap != null)
		{
		   for (Map.Entry<String,Integer> entry : keyMap.entrySet()) 
		   {
			    String key = entry.getKey();

			    int value = entry.getValue().intValue();
			    if (key.equalsIgnoreCase(payType))
			    	return value;
			}

		}
		return 0;
	}
	
	public static void getGameGiftTaggleParam()
	{
		OGSdkPlatform.getGameParamByKey(context, paramKey1, new OGLoadParamsCallBack(){
			public void  onLoad(String  param){
		    JSONObject json;
			try 
			{
				json = new JSONObject(param);
				JSONObject confjson = json.getJSONObject("button_config");
				String btnMode = confjson.getString("VIPBtnMode");
				Log.d("", "zho btnmode = " + btnMode);
				JNI.setBtnMode(Integer.valueOf(btnMode));
			}catch(Exception ex)
			{
			}
		} 
	});

	}
	
	public static int getGiftBtnMode()
	{
		return giftbtnmode;
	}
}

class SortBySortNum implements Comparator<OGSDKMall> 
{
	 public int compare(OGSDKMall mall1, OGSDKMall mall2) 
	 {
	  if (mall1.getOrderNum() > mall2.getOrderNum())
		  return -1;
	  else if (mall1.getOrderNum() == mall2.getOrderNum())
		  return 0;
	  return 1;
	 }
}
