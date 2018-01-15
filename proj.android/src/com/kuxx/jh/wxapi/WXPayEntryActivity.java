package com.kuxx.jh.wxapi;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.bukaopu.pipsdk.PayChannelHandler;
import com.bukaopu.pipsdk.PipClient;
import com.bukaopu.pipsdk.paychannel.wechat.WechatPayHandler;
import com.kuxx.jh.AppActivity;
import com.kuxx.jh.R;
import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
//import android.app.AlertDialog;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler {
	
	private static final String TAG = "WXPayEntryActivity";
	
    private IWXAPI api;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.wx_pay_result);
    	api = WXAPIFactory.createWXAPI(this, WechatPayHandler.APP_ID);
        api.handleIntent(getIntent(), this);
    }

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
        api.handleIntent(intent, this);
	}

	@Override
	public void onReq(BaseReq req) {
	}

	@Override
	public void onResp(BaseResp resp) {
		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
			if (resp.errCode == 0) {
				Toast.makeText(AppActivity.theOnly, "支付成功", Toast.LENGTH_SHORT).show();
				PipClient.getInstance().payCallback(PayChannelHandler.PAY_RESULT_SUCCESS);
			}
			else if (resp.errCode == -2) {
				PipClient.getInstance().payCallback(PayChannelHandler.PAY_RESULT_CANCEL);
			}
			else {
				Toast.makeText(AppActivity.theOnly, "支付失败", Toast.LENGTH_SHORT).show();
				PipClient.getInstance().payCallback(PayChannelHandler.PAY_RESULT_FAIL);
			}
//			Toast.makeText(this, "Wechat pay result, error code is " + resp.errCode, Toast.LENGTH_LONG).show();
			this.finish();
		}
		else {
			Toast.makeText(AppActivity.theOnly, "支付失败", Toast.LENGTH_SHORT).show();
			PipClient.getInstance().payCallback(PayChannelHandler.PAY_RESULT_FAIL);
			this.finish();
		}
	}
}