/**
 * 
 */
package com.bukaopu.pipsdk;

import org.json.JSONObject;

import com.kuxx.jh.AppActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Message;
import android.webkit.WebView;

/**
 * @author zeal
 *
 */
public abstract class PayChannelHandler {
	//支付成功
	public static final int PAY_RESULT_SUCCESS = 0;
	//支付失败
	public static final int PAY_RESULT_FAIL = 1;
	//支付中
	public static final int PAY_RESULT_PAYING = 2;
	//用户取消退出支付
	public static final int PAY_RESULT_CANCEL = 3;
	
	
	
	protected Activity ctx = null;
	protected WebView webView = null;
	
//	protected boolean init = false;
	
	public abstract void initialize(Activity ctx, WebView webView, JSONObject payParam) throws Exception;
	
	public abstract void pay(JSONObject payParam) throws Exception;
	
	public void payCallback(int payResult) {
//		Toast.makeText(ctx, "支付结果返回码" + payResult, Toast.LENGTH_SHORT).show();
		final int _payResult = payResult;
//		webView.post(new Runnable() {
//			@Override
//			public void run() {
//				webView.loadUrl("javascript:payCallback(" + _payResult + ")");
//			}
//		});
//		Intent intent = ctx.getIntent();
//		intent.putExtra("respCode", _payResult);  
//        //设置返回数据  
//		ctx.setResult(1, intent); 
//        //关闭Activity
		ctx.finish();
        Message msg = AppActivity.handler.obtainMessage();
        msg.what = _payResult;
        msg.sendToTarget();
	}
	
//	public boolean isInitialized() {
//		return this.init;
//	}

}
