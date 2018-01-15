/**
 * 
 */
package com.bukaopu.pipsdk;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.util.Base64;
import android.util.Log;
import android.webkit.WebView;

import com.bukaopu.pipsdk.paychannel.alipay.AlipayHandler;
import com.bukaopu.pipsdk.paychannel.baifubao.BaifubaoHandler;
import com.bukaopu.pipsdk.paychannel.cmpay.CmpayHandler;
import com.bukaopu.pipsdk.paychannel.wechat.WechatPayHandler;

/**
 * @author zeal
 *
 */
public class PipClient {
	
	public static final String TAG = "MdiameterClient";
	
	
	public static final String PAY_CHANNEL_KEY = "payChannel";
	private static Map<String, PayChannelHandler> payChannelHandlers = new HashMap<String, PayChannelHandler>();
	//public static final String PAY_CHANNEL_TYPE_WECHAT_WAP = "";
	
	private static PipClient instance = new PipClient();
	
	private PayChannelHandler currentPayHandler = null;
	
	static {
		//System.loadLibrary("pipsdk");
		WechatPayHandler wechat = new WechatPayHandler();
		payChannelHandlers.put(WechatPayHandler.PAY_CHANNEL_NAME, wechat);
		BaifubaoHandler baifubao = new BaifubaoHandler();
		payChannelHandlers.put(BaifubaoHandler.PAY_CHANNEL_NAME, baifubao);
		AlipayHandler alipay = new AlipayHandler();
		payChannelHandlers.put(AlipayHandler.PAY_CHANNEL_NAME, alipay);
		CmpayHandler cmpay = new CmpayHandler();
		payChannelHandlers.put(CmpayHandler.PAY_CHANNEL_NAME, cmpay);
	}
	
	public static PipClient getInstance() {
		return instance;
	}
	
	/**
	 * Decrypt paraeters
	 * @param ctx
	 * @param encryptString
	 * @return
	 */
	public native String decrypt(Context ctx, String encryptString); 
	
	public void pay(Activity ctx, WebView webView, String encryptString) throws Exception {
		//有空可使用JNI自定义算法解码和验证APK公钥
		String json = new String(Base64.decode(encryptString.getBytes("UTF-8"), Base64.NO_WRAP), "UTF-8");
		Log.d(TAG, "JS传递JSON = " + json);
		JSONObject jsonObj = new JSONObject(json);
		String payChannel = jsonObj.getString(PAY_CHANNEL_KEY);
		if (payChannel == null || payChannel.length() <= 0) {
			throw new IllegalStateException("Pay channel parameter is required");
		}
		PayChannelHandler handler = payChannelHandlers.get(payChannel);
		if (handler == null) {
			throw new IllegalStateException("Pay channel" + payChannel + " is not supported");
		}
//		if (!handler.isInitialized()) {
//			handler.initialize(ctx, webView, jsonObj);
//		}
		currentPayHandler = handler;
		handler.initialize(ctx, webView, jsonObj);
		handler.pay(jsonObj);
	}
	
	public void payCallback(int payResult) {
		if (currentPayHandler == null) {
			payChannelHandlers.get(WechatPayHandler.PAY_CHANNEL_NAME).payCallback(payResult);
			return;
		}
		else {
			Log.i(TAG, "currentPayHandler =  is not null" );
			currentPayHandler.payCallback(payResult);
		}
	}
	

}
