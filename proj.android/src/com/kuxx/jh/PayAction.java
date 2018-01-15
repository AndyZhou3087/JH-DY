package com.kuxx.jh;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Message;
import android.view.Gravity;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.webkit.WebChromeClient;
import android.webkit.WebView;

public class PayAction {
	static Activity context;
	static String SERVERURL = "http://pay.p-access.com/simple-pay-api/submitOrder";
	static String BACKURL = "https://www.stormnet.cn/api/wx_payresultnotify";
	static String MERCHANTID = "10000075";
	static String MERCHANTKEY ="0bfee619cf4d297867b87f0d5f18615d";
	static Runnable PayUIThread;
	static String payURL;
	
	static int buyprice[] = { 0, 6, 12, 30, 6, 12, 30, 68, 6, 30, 68, 18};
	static String goodsDesc[] = { "", "三少爷角色", "江小白角色", "阿青角色", "60元宝", "140元宝", "400元宝", "1080元宝", "荣耀月卡", "贵族月卡", "尊贵月卡", "限时礼包"};
	public static void pay(String payCode, int payindex)
	{ 
		 if (!Utils.getNetworkAvailable())
		 {
	        Message msg = AppActivity.handler.obtainMessage();
	        msg.what = -1;
	        msg.sendToTarget();
			return;
		 }
		
		 int ret = submitOrder(payCode, buyprice[payindex] * 100, goodsDesc[payindex]);
		 if (ret != 0)
		 {
	        Message msg = AppActivity.handler.obtainMessage();
	        msg.what = -2;
	        msg.sendToTarget();
		 }
		 else
		 {
			 //context.runOnUiThread(PayUIThread);
		    Intent intent =new Intent(context, PayActivity.class);
		    Bundle bundle=new Bundle();
		    bundle.putString("payurl", payURL);
		    intent.putExtras(bundle);
		    
		    context.startActivity(intent);
		 }
	}

	public static void init(Activity act)
	{
		context = act;
		PayUIThread = new Runnable()      
		{     
			 public void run()
			 {
				showPayView(payURL);
			 }
		};
	}
	
	private static int submitOrder(String payCode, int price, String desc)
	{
		StringBuffer buffer = new StringBuffer();
		String systimeStr = new Date().getTime() + "";
		String orderidStr = Utils.getChannelID() + systimeStr;
		String priceStr =  price + "";
		String orderDesc =  desc;
		String orderLifeStr = "60";
		//String payChannel = "wechat_app";
		String typeStr = "app";
		
		List<NameValuePair> paierList = new ArrayList<NameValuePair>();
		paierList.add(new BasicNameValuePair("backendUrl", BACKURL));
		paierList.add(new BasicNameValuePair("merchantNo", MERCHANTID));
		paierList.add(new BasicNameValuePair("merchantOrderNo", orderidStr));
		paierList.add(new BasicNameValuePair("orderAmt", priceStr));
		paierList.add(new BasicNameValuePair("orderDesc", orderDesc));
		paierList.add(new BasicNameValuePair("orderLife", orderLifeStr));
		//paierList.add(new BasicNameValuePair("payChannel", payChannel));
		paierList.add(new BasicNameValuePair("productNo", payCode));
		paierList.add(new BasicNameValuePair("terminalType", typeStr));
		
		buffer.append("backendUrl=");
		buffer.append(BACKURL);
		buffer.append("&merchantNo=");
		buffer.append(MERCHANTID);
		buffer.append("&merchantOrderNo=");
		buffer.append(orderidStr);
		buffer.append("&orderAmt=");
		buffer.append(priceStr);
		buffer.append("&orderDesc=");
		buffer.append(orderDesc);
		
		buffer.append("&orderLife=");
		buffer.append(orderLifeStr);
		
//		buffer.append("&payChannel=");
//		buffer.append(payChannel);
		
		buffer.append("&productNo=");
		buffer.append(payCode);
		
		buffer.append("&terminalType=");
		buffer.append(typeStr);
		buffer.append("&key=");
		buffer.append(MERCHANTKEY);
		
		String md5str = null;
		try {
			String md5str1 = MD5Util.getMD5String(buffer.toString()).toUpperCase(Locale.US);
			md5str = MD5Util.getMD5String(md5str1).toUpperCase(Locale.US);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return -1;
		}
		String ret = null;
		if (md5str != null)
		{
			paierList.add(new BasicNameValuePair("sign", md5str.toUpperCase(Locale.US)));

			HttpUtil hp = new HttpUtil(SERVERURL, "POST", null, paierList);
			ret = hp.getSimpleString();
		}
		JSONObject json;
		try {
			json = new JSONObject(ret);
			payURL = json.getString("payUrl");
		} catch (Exception e) {
			e.printStackTrace();
			return -2;
		}
		return 0;
	}
	
	public static void showPayView(String payurl)
	{
		WebView mywebview = new WebView(context);
		mywebview.getSettings().setJavaScriptEnabled(true);                   //设置允许Js
		mywebview.getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
		/*设置webview控件背景透明*/
		mywebview.setBackgroundColor(Color.TRANSPARENT);	
		mywebview.setWebChromeClient(new WebChromeClient());               
		mywebview.requestFocus();
		mywebview.addJavascriptInterface(context, "pipClient");
		mywebview.loadUrl(payurl); 
		WindowManager wm =(WindowManager)context.getSystemService("window");
		LayoutParams wmlay = new WindowManager.LayoutParams(); 
		wmlay.type= LayoutParams.TYPE_APPLICATION;                      //当前悬浮窗口位于phone层
		wmlay.format=PixelFormat.RGBA_8888;                      //悬浮窗口背景设为透明
		wmlay.gravity=Gravity.LEFT|Gravity.TOP;
		wmlay.flags = LayoutParams.FLAG_NOT_TOUCH_MODAL;	//属性设置
		wmlay.x = 0;
		wmlay.y = 0;
		wm.addView(mywebview, wmlay);
	}
}

















