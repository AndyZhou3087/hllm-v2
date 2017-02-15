package com.kuxx.hllm;

import java.util.Random;

import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;
import cn.sharesdk.onekeyshare.ShareContentCustomizeCallback;
import cn.sharesdk.tencent.qq.QQ;
import cn.sharesdk.tencent.qzone.QZone;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.wechat.moments.WechatMoments;

public class Share {

	public static void showShare() {
		 ShareSDK.initSDK(hllm.getTheApp());
		 OnekeyShare oks = new OnekeyShare();
		 //关闭sso授权
		 oks.disableSSOWhenAuthorize(); 
		 String url = JNI.getShareUrl();
		 if (url.equals(""))
			 url = "http://121.199.57.69:7777/applist/hllm_share.apk";
		 
		 String imgurl = url.substring(0, url.lastIndexOf("/") + 1) + "icon.png";
		 
		 String[]  comments = {"超萌搭档，所向披靡", "每日登陆，豪礼拿不停", "萌宠来袭，嗨翻全场", "百变关卡，邀你来战", "捍卫家园，更多挑战"};
		 Random ra =new Random();
		 int rindex = ra.nextInt(comments.length);
		// 分享时Notification的图标和文字  2.5.9以后的版本不调用此方法
		 //oks.setNotification(R.drawable.ic_launcher, getString(R.string.app_name));
		 // title标题，印象笔记、邮箱、信息、微信、人人网和QQ空间使用
		 oks.setTitle(hllm.getTheApp().getString(R.string.app_name));
		 // titleUrl是标题的网络链接，仅在人人网和QQ空间使用
		 oks.setTitleUrl(url);
		 // text是分享文本，所有平台都需要这个字段
		 oks.setText("我在欢乐连萌，一起来玩吧！");
		 // imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
		 //oks.setImagePath("/sdcard/test.jpg");//确保SDcard下面存在此张图片
		 // url仅在微信（包括好友和朋友圈）中使用
		 oks.setImageUrl(imgurl);
		 oks.setUrl(url);
		 // comment是我对这条分享的评论，仅在人人网和QQ空间使用
		 oks.setComment(comments[rindex]);
		 // site是分享此内容的网站名称，仅在QQ空间使用
		 oks.setSite(hllm.getTheApp().getString(R.string.app_name));
		 // siteUrl是分享此内容的网站地址，仅在QQ空间使用
		 oks.setSiteUrl(url);

		// 启动分享GUI
		 oks.show(hllm.getTheApp());
		 }
	
	public static void shareImg()
	{
		 String url = JNI.getShareUrl();
		 if (url.equals(""))
			 url = "http://121.199.57.69:7777/applist/hllm_share.apk";
		OnekeyShare share = new OnekeyShare();
        share.disableSSOWhenAuthorize();
        share.setText(hllm.getTheApp().getString(R.string.app_name));
        // text是分享文本，所有平台都需要这个字段
        share.setTitle(hllm.getTheApp().getString(R.string.app_name));
        // url仅在微信（包括好友和朋友圈）中使用
        share.setUrl(url);
        share.setTitleUrl(url);
        share.setSiteUrl(url);
        share.setShareContentCustomizeCallback(new ShareContentCustomizeCallback() {
            @Override
            public void onShare(Platform platform, Platform.ShareParams paramsToShare) {
                if(platform.getName().equalsIgnoreCase(Wechat.NAME) || platform.getName().equalsIgnoreCase(WechatMoments.NAME)){
                    paramsToShare.setShareType(Platform.SHARE_IMAGE);
                    paramsToShare.setImagePath(JNI.getShareImgPath());
                }else if(platform.getName().equalsIgnoreCase(QQ.NAME)){
                    paramsToShare.setText(null);
                    paramsToShare.setTitle(null);
                    paramsToShare.setTitleUrl(null);
                    paramsToShare.setImagePath(JNI.getShareImgPath());
                }
                else if (platform.getName().equalsIgnoreCase(QZone.NAME))
                {
                	paramsToShare.setText("我在欢乐连萌，一起来玩吧！");
                	paramsToShare.setImagePath(JNI.getShareImgPath());
                }
            }
        });
        share.show(hllm.getTheApp());
	}
}
