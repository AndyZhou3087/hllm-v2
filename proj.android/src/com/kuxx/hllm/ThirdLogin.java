package com.kuxx.hllm;

import java.util.HashMap;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.Toast;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.tencent.qq.QQ;
import cn.sharesdk.wechat.friends.Wechat;

public class ThirdLogin {

	private static final int LOGIN_OK = 0;
	private static final int LOGIN_ERROR = 1;
	private static final int LOGIN_CANCEL = 2;
	
	public static final int LOGIN_NONE = 0;
	public static final int LOGIN_QQ = 1;
	public static final int LOGIN_WECHAT = 2;
	private Activity mcontext;
	private String platformname = null;
	public ThirdLogin(Activity context)
	{
		mcontext = context;
	}
	
	public static ThirdLogin init(Activity context)
	{
		ShareSDK.initSDK(context);
		return new ThirdLogin(context);
	}
	
	public void login(final int logintype)
	{
		Platform platform = null;
		if (logintype == LOGIN_QQ)
		{
			platformname = QQ.NAME;
		}
		else if (logintype == LOGIN_WECHAT)
		{
			platformname = Wechat.NAME;
		}

		if (platformname != null)
		{
			platform = ShareSDK.getPlatform(mcontext, platformname);
	        if (platform.isValid()) 
	        {
	        	platform.removeAccount(true);
	        }
	        platform.setPlatformActionListener(new PlatformActionListener() {
				@Override
				public void onComplete(Platform platform, int i, HashMap<String, Object> hashmap) {
					// TODO Auto-generated method stub
					Message msg = new Message();
					msg.what = LOGIN_OK;
					Bundle data = new Bundle();
					data.putString("nickname", platform.getDb().getUserName());
					data.putString("headurl", platform.getDb().getUserIcon());
					data.putString("uid", platform.getDb().getUserId());
					data.putString("platform", platformname);
					msg.setData(data);
					mHandler.sendMessage(msg);
				}
	            @Override
	            public void onError(Platform platform, int i, Throwable throwable) {
	            	hllm.dismissProgressDialog();
					Message msg = new Message();
					msg.what = LOGIN_ERROR;
					mHandler.sendMessage(msg);
	            }
	
	            @Override
	            public void onCancel(Platform platform, int i) {
	            	hllm.dismissProgressDialog();
					Message msg = new Message();
					msg.what = LOGIN_CANCEL;
					mHandler.sendMessage(msg);
	            }
	
	
	        });
	        platform.SSOSetting(false);
	        platform.authorize();
		}
	}
	
	static Handler mHandler = new Handler(Looper.getMainLooper()) {

		@Override
		public void handleMessage(Message msg) {
			switch(msg.what)
			{
			case LOGIN_OK:
				Bundle data = msg.getData();
				String nickname = data.getString("nickname");
				String openid = data.getString("uid");
				String headurl = data.getString("headurl");
				String platform = data.getString("platform");
				JNI.setMyHeadImg(headurl);
				JNI.getUserData(openid, nickname, headurl, Utils.getChannelID(), Utils.getPkgName());
				Analysis.SignIn(platform, openid, nickname);
				break;
			case LOGIN_ERROR:
				Toast.makeText(hllm.getTheApp(), "登录失败", Toast.LENGTH_SHORT).show();
				break;
			case LOGIN_CANCEL:
				Toast.makeText(hllm.getTheApp(), "登录取消", Toast.LENGTH_SHORT).show();
				JNI.cancelThirdLogin();
				break;	
			}
		}

	};
}
