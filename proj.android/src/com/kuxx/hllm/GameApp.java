package com.kuxx.hllm;

import com.tendcloud.tenddata.TalkingDataGA;

import android.app.Application;
public class GameApp extends Application {
	
	@Override
	public void onCreate() {
		super.onCreate();
		
		String strdata = Utils.getMetaData(this, "OT_APPCHANNEL");
		TalkingDataGA.init(this, "22E00DF2CA42EED6EC3F558D1386B0FA", strdata);
		
		}
}
