/**
 * 
 */
package com.bukaopu.pipsdk.paychannel.baifubao;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import android.app.Activity;
import android.util.Log;
import android.webkit.WebView;
import android.widget.Toast;

import com.baidu.android.pay.PayCallBack;
import com.baidu.paysdk.api.BaiduPay;
import com.bukaopu.pipsdk.PayChannelHandler;

/**
 * @author zeal
 *
 */
public class BaifubaoHandler extends PayChannelHandler {
	
	private static final String TAG = "BaifubaoHandler";
	
	public static final String PAY_CHANNEL_NAME = "baifubao";

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
		String orderInfo = urlParams;
		Map<String, String> map = new HashMap<String, String>();
        map.put(BaiduPay.CASHIER_TYPE, BaiduPay.CASHIER_TYPE_LOGIN);
        Log.d(TAG, "orderInfo = "+orderInfo);
        BaiduPay.getInstance().doPay(this.ctx, orderInfo, new PayCallBack() {
            public void onPayResult(int stateCode, String payDesc) {
                // TODO Auto-generated method stub
                Log.d(TAG, "rsult=" + stateCode + "#desc=" + payDesc);
                handlepayResult(stateCode, payDesc);
            }

            public boolean isHideLoadingDialog() {
                return false;
            }
        }, map);

	}
	
    /**
     * 支付结果处理
     * 
     * @param stateCode
     * @param payDesc
     */
    private void handlepayResult(int stateCode, String payDesc) {
        String tradeStatus = "statecode={";
        try {
            Log.d(TAG, "msg:" + payDesc);
            int imemoStart = payDesc.indexOf("statecode=");
            imemoStart += tradeStatus.length();
            int imemoEnd = payDesc.indexOf("};order_no=");
            tradeStatus = payDesc.substring(imemoStart, imemoEnd);
            if ("0".equals(tradeStatus)) {
                /*if (ResultChecker.checkSign(mSpPri.getText().toString(),payDesc) == 2) {
                    Toast.makeText(this.ctx, "支付成功", Toast.LENGTH_SHORT).show();
                } 
                else {
                    Toast.makeText(this.ctx, "返回结果验签不通过", Toast.LENGTH_SHORT).show();
                }*/
            	//FIXME 由于客户端不便保存商户Key不严格的就算是支付成功了,如果想严格验正签名则需要后台服务器接口配合
            	Toast.makeText(this.ctx, "支付成功", Toast.LENGTH_SHORT).show();
            	this.payCallback(PayChannelHandler.PAY_RESULT_SUCCESS);
            } 
            else if ("1".equals(tradeStatus)) {
                // TODU 需要主动查询
                Toast.makeText(this.ctx, "支付处理中", Toast.LENGTH_SHORT).show();
                this.payCallback(PayChannelHandler.PAY_RESULT_PAYING);
            } 
            else if ("2".equals(tradeStatus)) {
                // TODU 需要主动查询
                Toast.makeText(this.ctx, "取消", Toast.LENGTH_SHORT).show();
                this.payCallback(PayChannelHandler.PAY_RESULT_CANCEL);
            } 
            else if ("3".equals(tradeStatus)) {
                // TODU 需要主动查询
                Toast.makeText(this.ctx, "不支持该种支付方式", Toast.LENGTH_SHORT).show();
                this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
            } 
            else if ("4".equals(tradeStatus)) {
                // TODU 需要主动查询
                Toast.makeText(this.ctx, "无效的登陆状态", Toast.LENGTH_SHORT).show();
                this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
            }
            else {
            	Toast.makeText(this.ctx, "支付失败", Toast.LENGTH_SHORT).show();
            	this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
            }
        } catch (Exception e) {
            Log.d(TAG, "Exception", e);
            Toast.makeText(this.ctx, "支付失败", Toast.LENGTH_SHORT).show();
            this.payCallback(PayChannelHandler.PAY_RESULT_FAIL);
        }
    }
	

}
