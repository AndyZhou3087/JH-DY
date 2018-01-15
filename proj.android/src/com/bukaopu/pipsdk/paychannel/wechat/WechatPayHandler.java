/**
 * 
 */
package com.bukaopu.pipsdk.paychannel.wechat;

import org.json.JSONObject;

import android.app.Activity;
import android.util.Log;
import android.webkit.WebView;

import com.bukaopu.pipsdk.PayChannelHandler;
import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

/**
 * @author zeal
 *
 */
public class WechatPayHandler extends PayChannelHandler {
	
	public static final String TAG = "WechatPayHandler";
	
	public static final String PAY_CHANNEL_NAME = "wechatOpenPay";
	public static final String APPID_KEY = "appid";
	public static final String PARTNER_ID_KEY = "partnerid";
	public static final String PREPAY_ID_KEY = "prepayid";
	public static final String NONCE_STR_KEY = "noncestr";
	public static final String TIMESTAMP_KEY = "timestamp";
	public static final String PACKAGE_KEY = "package";
	public static final String SIGN_KEY = "sign";
	
	public static final String APP_ID = "wx496430d3cf56f5b5";
	
	private IWXAPI api = null;

	
	public void initialize(Activity ctx, WebView webView, JSONObject payParam) throws Exception {
		String appid = payParam.getString(APPID_KEY);
		//if (appid == null || appid.length() <= 0 || !APP_ID.equals(appid)) {
		if (appid == null || (appid != null && appid.length() <= 0)){
			throw new IllegalStateException("appid is required");
		}
		this.ctx = ctx;
		this.webView = webView;
		api = WXAPIFactory.createWXAPI(ctx, null);
		boolean ret = api.registerApp(appid);
		//api.unregisterApp();
		//Log.d(TAG, "api.registerApp=" +  ret + ",appid=" + appid);
		//this.init = true;
	}

	/* (non-Javadoc)
	 * @see com.mdiameter.pipsdk.PayChannelHandler#pay(org.json.JSONObject)
	 */
	@Override
	public void pay(JSONObject payParam) throws Exception {
		
		PayReq req = new PayReq();
		req.appId = payParam.getString(APPID_KEY);
		req.partnerId = payParam.getString(PARTNER_ID_KEY);
		req.prepayId = payParam.getString(PREPAY_ID_KEY);
		req.nonceStr = payParam.getString(NONCE_STR_KEY);
		req.timeStamp = payParam.getString(TIMESTAMP_KEY);
		req.packageValue = payParam.getString(PACKAGE_KEY);
		req.sign = payParam.getString(SIGN_KEY);
		
		api.registerApp(req.appId);
		boolean ret = api.sendReq(req);
	}

//	@Override
//	public void onReq(BaseReq req) {
//		// TODO Auto-generated method stub
//		Log.d(TAG, "BaseReq=" + req.toString());
//	}
//
//	@Override
//	public void onResp(BaseResp resp) {
//		// TODO Auto-generated method stub
//		Log.d(TAG, "onPayFinish, errCode = " + resp.errCode);
//		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
//			AlertDialog.Builder builder = new AlertDialog.Builder(this.ctx);
//			builder.setTitle("");
//			//builder.setMessage("", String.valueOf(resp.errCode));
//			builder.setMessage(String.valueOf(resp.errCode));
//			builder.show();
//		}
//		else {
//			AlertDialog.Builder builder = new AlertDialog.Builder(this.ctx);
//			builder.setTitle("ffff");
//			//builder.setMessage("", String.valueOf(resp.errCode));
//			builder.setMessage(String.valueOf(resp.getType()));
//			builder.show();
//		}
//		
//	}

}
