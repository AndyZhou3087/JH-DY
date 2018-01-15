/**
 * 
 */
package com.bukaopu.pipsdk.paychannel.alipay;

import org.json.JSONObject;

import android.app.Activity;

import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.webkit.WebView;
import android.widget.Toast;

import com.alipay.sdk.app.PayTask;
import com.bukaopu.pipsdk.PayChannelHandler;

/**
 * @author zeal
 *
 */
public class AlipayHandler extends PayChannelHandler {
	
	private static final String TAG = "AlipayHandler";
	
	public static final String PAY_CHANNEL_NAME = "alipay";
	
	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;

	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				PayResult payResult = new PayResult((String) msg.obj);
				
				// 支付宝返回此次支付结果及加签，建议对支付宝签名信息拿签约时支付宝提供的公钥做验签
				String resultInfo = payResult.getResult();
				
				String resultStatus = payResult.getResultStatus();

				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "9000")) {
					Toast.makeText(ctx, "支付成功", Toast.LENGTH_SHORT).show();
					AlipayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_SUCCESS);
				} 
				// 判断resultStatus 为非“9000”则代表可能支付失败
				// “8000”代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
				else if (TextUtils.equals(resultStatus, "8000")) {
					Toast.makeText(ctx, "支付结果确认中", Toast.LENGTH_SHORT).show();
					AlipayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_PAYING);
				} 
				else {
					// 其他值就可以判断为支付失败，包括用户主动取消支付，或者系统返回的错误
					Toast.makeText(ctx, "支付失败"+payResult.toString(), Toast.LENGTH_SHORT).show();
					AlipayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(ctx, "检查结果为：" + msg.obj, Toast.LENGTH_SHORT).show();
				AlipayHandler.this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
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
	}

	/* (non-Javadoc)
	 * @see com.mdiameter.pipsdk.PayChannelHandler#pay(org.json.JSONObject)
	 */
	@Override
	public void pay(JSONObject payParam) throws Exception {
		
		String urlParams = payParam.getString("urlParams");
		final String payInfo = urlParams;
		
		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// 构造PayTask 对象
				PayTask alipay = new PayTask(ctx);
				// 调用支付接口，获取支付结果
				String result = alipay.pay(payInfo);

				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		// 必须异步调用
		Thread payThread = new Thread(payRunnable);
		payThread.start();

	}
	
	

}
