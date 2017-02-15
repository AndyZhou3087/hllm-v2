/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.kuxx.hllm;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.kuxx.hllm.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.widget.Toast;

public class hllm extends Cocos2dxActivity{
	
	public static HandlerThread handlerThread;
    public static THandler handler;

    private static ProgressDialog updateProgressDialog = null;
    private static hllm theOnly = null;
    private static ThirdLogin mThirdLogin = null;
	public static hllm getTheApp()
    {
        return (hllm) hllm.theOnly;
    }
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        theOnly = this;
    	handlerThread = new HandlerThread("handle_thread");
        handlerThread.start();
        handler = new THandler(handlerThread.getLooper());
		GMAction.init(this);
		Analysis.init(this);
		mThirdLogin = ThirdLogin.init(this);
	}
    
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// llk should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }   
    
    @Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{

		if (keyCode == KeyEvent.KEYCODE_BACK )
		{
		
		int showval = JNI.doGamePause();
		if (showval == 0)
		{
			final Activity act = (Activity)theOnly;
			new AlertDialog.Builder(act)
	        .setMessage("确认退出"+ act.getString(R.string.app_name) + "吗？")
	        .setPositiveButton("确认", new DialogInterface.OnClickListener() {
		        public void onClick(DialogInterface dialog, int whichButton) 
		        {
	        		String postdata = JNI.getPostData();
		        	if (postdata.length() > 0)
		        	{
			            SharedPreferences userData = theOnly.getSharedPreferences("config", MODE_PRIVATE);
			     	     Editor editor = userData.edit();
			     	     editor.putString("postdata", postdata);
			     	     editor.commit();
			        	Intent intent = new Intent(theOnly, PostDataService.class);
			        	theOnly.startService(intent);
		        	}
		            JNI.exitGame();
		            act.finish();
		        }
	        })
	        .setNegativeButton("取消", new DialogInterface.OnClickListener() {
		        public void onClick(DialogInterface dialog, int whichButton) {
		         
		        }
	        })
	        .show();
			
			}
		}
		return false;
	}
    
    public static int checkOperator()
    {
    	String imsi = Utils.GetIMSI();
    	if(imsi!=null)
    	{//中国移动
            if(imsi.startsWith("46000") || imsi.startsWith("46002") || imsi.startsWith("46007") || imsi.startsWith("46020"))
            {//因为移动网络编号46000下的IMSI已经用完，所以虚拟了一个46002编号，134/159号段使用了此编号

				return 1;
            
            }else if(imsi.startsWith("46001") || imsi.startsWith("46006")){
            //中国联通
            	return 2;
            }else if(imsi.startsWith("46003") || imsi.startsWith("46005") || imsi.startsWith("46011")){
            //中国电信
            	return 3;
            }
    	}
    	return 1;
    }

	@Override
	protected void onPause() {
		super.onPause();
		Analysis.onPause();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		Analysis.onResume();
		dismissProgressDialog();
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		GMAction.destroy();
		System.exit(0);
		super.onDestroy();
	}
	
	public static String getResFileContent(String filename, Context context) {
		InputStream is = context.getClass().getClassLoader()
				.getResourceAsStream(filename);

		if (is == null) {
			return null;
		}

		StringBuilder builder = new StringBuilder();
		String content = "";
		BufferedReader bufferedReader = new BufferedReader(
				new InputStreamReader(is));

		try {
			while (bufferedReader.ready()) {
				content = bufferedReader.readLine();
				builder.append(content);
			}
			bufferedReader.close();
		} catch (IOException e) {
			return null;
		}

		return builder.toString();
	}
	
	public static void showProgressDialog(String text)
    {
    	updateProgressDialog = ProgressDialog.show(theOnly, "", text, true, false); 
    }
    
    public static void dismissProgressDialog() 
    {
		if (updateProgressDialog != null && updateProgressDialog.isShowing()) 
		{
			updateProgressDialog.dismiss();
			
		}
	}

    
    public static int isSensitiveWord(String name)
    {
    	for (int i=0; i < SensitiveWord.Key.length; i++)
    	{
	    	if (name.contains(SensitiveWord.Key[i]))
	    	{
		        Message msg = hllm.handler.obtainMessage();
		        msg.what = 0;
		        msg.sendToTarget();
	    		return 1;
	    	}
    	}
    	return 0;
    }
    
    
    public static void login(int type)
    {
    	if (type == ThirdLogin.LOGIN_NONE)
    	{
    		String localid = Utils.UUID();
    		JNI.getUserData(localid, "", "", Utils.getChannelID(), Utils.getPkgName());
    		Analysis.SignIn("none", localid, "游客");
    	}
    	else
    	{
    		showloginloading(type);
    		mThirdLogin.login(type);
    	}
    }

    public static void showloginloading(int type)
    {
        Message msg = hllm.handler.obtainMessage();
        msg.what = 2;
        msg.arg1 = type;
        msg.sendToTarget();
    }
	
    public static class THandler extends Handler

	{
		  public THandler()
		  {
		   
		  }
		  
		  public THandler(Looper looper)
		  {
			  super(looper);
		  }
		  
		  @Override
		  public void handleMessage(Message msg) 
		  {
			  if (!Thread.currentThread().isInterrupted())
			  {
				  switch(msg.what)
				  {
				  case 0:
					  Toast.makeText(theOnly, "非常抱歉，您的输入中包含敏感词，不允许修改", Toast.LENGTH_SHORT).show();
					  break;
				  case 1:
					  Toast.makeText(theOnly, "网络不可用，请检查您的网络设置", Toast.LENGTH_SHORT).show();
					  //StarJNI.sendMessage(-1);
					  break;
				  case 2:
					  	String text = "";
					  	if (msg.arg1 == 1)
					  		text="启动QQ中，请稍等...";
					  	else if (msg.arg1 == 2)
					  		text="启动微信中，请稍等...";
					  	if(msg.arg1 == 1 || msg.arg1 == 2)
					  		showProgressDialog(text);
					  	if (msg.arg1 == 3)
					  		Toast.makeText(theOnly, "微信客户端未安装，请确认！", Toast.LENGTH_SHORT).show();
					  break;
				  case 3:
					  OGThranPay.getGoodsList((String)msg.obj);
					  break;
				  case 4:
						Toast.makeText(theOnly, "购买成功", Toast.LENGTH_SHORT).show();
						JNI.sendMessage(0);
					  break;
				  case 5:
						Toast.makeText(theOnly, "购买失败", Toast.LENGTH_SHORT).show();
						JNI.sendMessage(-1);
						break;
				  case 6:
						Toast.makeText(theOnly, "购买取消", Toast.LENGTH_SHORT).show();
						JNI.sendMessage(-1);
					  break;
				  case 7:
					  Bundle data = msg.getData();
				      String pcode = data.getString("pcode");
				      String type = data.getString("type");
					  OGThranPay.pay(pcode, type);
					  break;
				  default:
					  break;
				  }
			  } 
		}
	}
}


