/**
 * 
 */
package com.bukaopu.pipsdk.paychannel.cmpay;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.webkit.WebView;
import android.widget.Toast;

import com.bukaopu.pipsdk.PayChannelHandler;
import com.hisun.b2c.api.core.IPOSUtils;
import com.hisun.b2c.api.util.IPOSID;
import com.hisun.b2c.api.util.OrderBean;
import com.hisun.b2c.api.util.SignUtils;

/**
 * @author zeal
 *
 */
public class CmpayHandler extends PayChannelHandler {
	
	private static final String TAG = "CmpayHandler";
	
	private IPOSUtils ipos;
	
	public static final String PAY_CHANNEL_NAME = "cmpay";
	
	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;

	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case IPOSID.PAY_REQUEST: {
				PayResult payResult = new PayResult((String) msg.obj);
				
				// 支付宝返回此次支付结果及加签，建议对支付宝签名信息拿签约时支付宝提供的公钥做验签
				String resultInfo = (String) msg.obj;
				Log.d("resultInfo", resultInfo);
				String resultStatus = payResult.getResultStatus();

				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "0000")) {
					Toast.makeText(ctx, "支付成功", Toast.LENGTH_SHORT).show();
					CmpayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_SUCCESS);
				} 
				// 判断resultStatus 为非“9000”则代表可能支付失败
				// “8000”代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
				else{
					Toast.makeText(ctx, payResult.getResult(), Toast.LENGTH_SHORT).show();
					CmpayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_PAYING);
				} 
//				else {
//					// 其他值就可以判断为支付失败，包括用户主动取消支付，或者系统返回的错误
//					Toast.makeText(ctx, "支付失败"+payResult.toString(), Toast.LENGTH_SHORT).show();
//					CmpayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
//				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(ctx, "检查结果为：" + msg.obj, Toast.LENGTH_SHORT).show();
				CmpayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
				break;
			}
			default:
				break;
			}
		};
	};
	
	

	/* (non-Javadoc)
	 * @see com.mdiameter.pipsdk.PayChannelHandler#initialize(android.content.Context, android.content.Intent, org.json.JSONObject)
	 */
	@Override
	public void initialize(Activity ctx, WebView webView, JSONObject payParam) throws Exception {
		this.ctx = ctx;
		this.webView = webView;
		ipos = new IPOSUtils(this.ctx);
	}

	/* (non-Javadoc)
	 * @see com.mdiameter.pipsdk.PayChannelHandler#pay(org.json.JSONObject)
	 */
	@Override
	public void pay(JSONObject payParam) throws Exception {
		
		String urlParams = payParam.getString("urlParams");
//		final String payInfo = urlParams;
		Log.i(TAG, urlParams);
		
		String requestXmlData = build(payParam);
		Log.i(TAG, requestXmlData);
		try {
			ipos.iPay(requestXmlData, IPOSID.PAY_REQUEST, mHandler);
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	public String build(JSONObject payParam) throws JSONException {
		SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
		OrderBean orderBean = new OrderBean();   
		orderBean.setCharacter("00");// 对应字段：CHARACTER 描述:字符集
		orderBean.setNotifyUrl(payParam.getString("notifyUrl"));// 对应字段：NOTIFYURL
														// 描述:后台通知URL
		orderBean.setPartner(payParam.getString("partner"));// 对应字段：PARTNER 描述:商户编号
		orderBean.setRequestId(payParam.getString("requestId"));// 对应字段：REQUESTID
		orderBean.setSignType("MD5");// 对应字段：SIGNTYPE 描述:签名方式
		orderBean.setType("CASDirectPayConfirm");// 对应字段：TYPE 描述:接口类型
		orderBean.setItfVer("2.0.0");// 对应字段：ITFVER 描述:接口版本号
		orderBean.setTxnAmt(payParam.getString("amount"));
		orderBean.setCcy("00");// 对应字段:CCY 描述:币种
		orderBean.setOrderDate(sdf.format(new Date()));
		orderBean.setOrderNo(payParam.getString("merchantOrderNo"));
		orderBean.setAcDate(sdf.format(new Date()));
		orderBean.setPeriod(payParam.getString("orderLife"));// 对应字段:PERIOD 描述：有效期数量
		orderBean.setPeriodUnit("00");// 对应字段:PERIODUNIT 描述：有效期单位
		orderBean.setProDesc(payParam.getString("orderDesc"));// 对应字段:PRODESC 描述：商品描述
		orderBean.setProId(payParam.getString("productNo"));// 对应字段:PROID 描述:商品编号
		orderBean.setProName("通卡联城");// 对应字段:PRONAME 描述:商品名称
		orderBean.setProNum("1");// 对应字段:PRONUM 描述:商品数量
		orderBean.setUserToken(payParam.getString("phone")); // 商户指定支付手机号
		orderBean.setCouponsFlag("00");// 对应字段:COUPONSFLAG 描述:是否支持营销工具
//		orderBean.setBusinessType(businessType); // 业务类型
		
		orderBean.setMerId(payParam.getString("merchantOrderNo")); // 商户ID
		String signStr = orderBean.getSignStr();
		signStr = SignUtils.MD5SignData(signStr, payParam.getString("merchantKey"));
		orderBean.setSign(signStr);

		return orderBean.getSignedXml();   
	}
	

}
