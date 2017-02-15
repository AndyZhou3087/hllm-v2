package com.kuxx.hllm;

public class JNI {

	public static native void sendMessage(int msg);
	
	public static native int doGamePause();
	
	public static native void exitGame();
	
	public static native void getUserData(String localid, String nickname, String headurl, String channelid, String pkgname);
	
	public static native void setMyHeadImg(String url);
	
	public static native String getPostData();
	
	public static native String getShareUrl();
	
	public static native boolean getIsBuyGift(int index);
	
	public static native String getShareImgPath();
	
	public static native void setBtnMode(int value);
	
	public static native void cancelThirdLogin();
}
