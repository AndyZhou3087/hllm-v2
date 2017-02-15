package com.kuxx.hllm;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo.State;
import android.os.IBinder;

/**
 * Filename : NotifyService.java
 * 
 */
public class PostDataService extends Service {

	public static final String HTTPURL = "http://115.29.224.123:9191/api.php?action=";
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	@Override
	public void onCreate() {
		super.onCreate();
	}
	
	@SuppressLint("NewApi")
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
        SharedPreferences userData = this.getSharedPreferences("config", MODE_PRIVATE);
        String data = userData.getString("postdata", "");
		
		getPostResult(data);
		return super.onStartCommand(intent, flags, startId);
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
	}
	
	public int getPostResult(String content)
	{
		if (!getNetworkAvailable())
		{
			stopSelf();
			return -1;
		}
		
		if (content == null ||(content != null && content.equals("")))
		{
			stopSelf();
			return -2;
		}
		String uid = content.substring(0, content.indexOf(";"));

		if (uid.equals(""))
		{
			stopSelf();
			return -2;
		}
		
		StringBuffer strb = new StringBuffer();
		strb.append(HTTPURL);
		strb.append("llk_updatedata");
		strb.append("&param=");
		
		String datas[] = content.split(";");
		JSONObject json = new JSONObject();
		try {
			json.put("id", Integer.valueOf(datas[0]));
			json.put("level", Integer.valueOf(datas[1]));
			json.put("skill_level", datas[2]);
			json.put("coin", Integer.valueOf(datas[3]));
			json.put("diamond", Integer.valueOf(datas[4]));
			json.put("life", Integer.valueOf(datas[5]));
			json.put("lifeupperlimit", Integer.valueOf(datas[6]));
			json.put("find", Integer.valueOf(datas[7]));
			json.put("reset", Integer.valueOf(datas[8]));
			json.put("addtime", Integer.valueOf(datas[9]));
			json.put("finished_pass", datas[10]);
			json.put("bestscore", Integer.valueOf(datas[11]));
			json.put("bossstatus", Integer.valueOf(datas[12]));
			json.put("bossbestscore", Integer.valueOf(datas[13]));
			json.put("login_days", Integer.valueOf(datas[14]));
			json.put("higheststage", Integer.valueOf(datas[15]));
			json.put("lasttakeawardtime", Integer.valueOf(datas[16]));
			json.put("achievement", datas[17]);
			json.put("nickname", datas[18]);
			json.put("classicscore", Integer.valueOf(datas[19]));
			json.put("flashscore", Integer.valueOf(datas[20]));
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		//List<NameValuePair> params = new ArrayList<NameValuePair>();
		//params.add(new BasicNameValuePair("data", json.toString()));
		//HttpUtil hp = new HttpUtil(strb.toString(), "POST", null, params);
		try {
			strb.append(URLEncoder.encode(json.toString(), "utf-8"));
		} catch (UnsupportedEncodingException e1) {
			e1.printStackTrace();
		}
		
		HttpUtil hp = new HttpUtil(strb.toString());
		String ret = hp.getSimpleString();
		if (ret == null)
		{
			stopSelf();
			return -4;
		}
		
        //JSONObject json;
        int code = -5;
		try 
		{
			json = new JSONObject(ret);
			ret = json.getString("ret");
			code = Integer.valueOf(ret);
		}
		catch(JSONException e)
		{
			stopSelf();
			e.printStackTrace();
			return -5;
		}
		stopSelf();
		//StarJNI.sendPostResult(code);
		return code;
	}
	
    public boolean getNetworkAvailable() 
    { 
    	boolean success = false;
    	//获得网络连接服务   
    	ConnectivityManager connManager = (ConnectivityManager) this.getSystemService(Context.CONNECTIVITY_SERVICE);   
    	State state = connManager.getNetworkInfo(   
    	ConnectivityManager.TYPE_WIFI).getState(); // 获取网络连接状态   
    	if (State.CONNECTED == state) 
    	{ // 判断是否正在使用WIFI网络   
    		success = true;   
    	}
    	state = connManager.getNetworkInfo(   
    	ConnectivityManager.TYPE_MOBILE).getState(); // 获取网络连接状态   
    	if (State.CONNECTED == state) 
    	{ // 判断是否正在使用GPRS网络   
    		success = true;   
    	}
    	return success;
    	  
    }
}
