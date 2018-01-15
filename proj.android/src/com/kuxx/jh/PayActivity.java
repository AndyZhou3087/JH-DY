package com.kuxx.jh;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Message;
import android.view.KeyEvent;
import android.view.Window;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import com.bukaopu.pipsdk.PipClient;

public class PayActivity extends Activity {

	private WebView webView;
	private PipClient pipClient = new PipClient();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);      
//		 //全屏      
//		 getWindow().setFlags(WindowManager.LayoutParams. FLAG_FULLSCREEN ,        
//	                      WindowManager.LayoutParams. FLAG_FULLSCREEN);  
//		requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);  
		setContentView(R.layout.payactivity);
		 Intent intent= this.getIntent();
		 Bundle bundle = intent.getExtras();
		 String payUrl = bundle.getString("payurl");
		 //下单后返回的支付地址
	    if(payUrl != null && !"".equals(payUrl.trim())){
		webView=(WebView)findViewById(R.id.showMerchant);
        loadUrlPage(payUrl);
        }else{
        	Toast.makeText(this, "支付地址为空", Toast.LENGTH_SHORT).show();
        }
	}
	private void loadUrlPage(String showUrl) {
		
	       webView.setWebViewClient(new WebViewClient(){
				@Override
				public boolean shouldOverrideUrlLoading(WebView view, String url) {
//					return super.shouldOverrideUrlLoading(view, url);
					//view.loadUrl(url);
		            if (url.startsWith("weixin://wap/pay?")) {
		                Intent intent = new Intent();
		                intent.setAction(Intent.ACTION_VIEW);
		                intent.setData(Uri.parse(url));
		                startActivity(intent);
		                return true;
		            }
		            return super.shouldOverrideUrlLoading(view, url);
				}
	        });
	       webView.requestFocus();
	       WebSettings webSettings = webView.getSettings();
	       webSettings.setJavaScriptEnabled(true);
	       webSettings.setJavaScriptCanOpenWindowsAutomatically(true);
	       webSettings.setDomStorageEnabled(true);
	       webView.setWebChromeClient(new WebChromeClient());
	       webView.addJavascriptInterface(this, "pipClient");
		   webView.loadUrl(showUrl);
	}
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
        Message msg = AppActivity.handler.obtainMessage();
        msg.what = 3;
        msg.sendToTarget();
	}
	public void pay(String encryptJson) {
		try {
			this.pipClient.pay(this, this.webView, encryptJson);
		} 
		catch (Exception e) {
			e.printStackTrace();
	        Message msg = AppActivity.handler.obtainMessage();
	        msg.what = -3;
	        msg.sendToTarget();
		}
	}
	
	public void closeWebView(){
		this.finish();
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
			return super.onKeyDown(keyCode, event);
	}
	
/*	public void toCreateCode(){
		 String mask = URLEncoder.encode(application.encryptedKey());
			
			String consume="&category=1&commitCommand=6&sessionId="+sessionId+"&merchantId="+merchantId+"&points=10&cardId="+cardId+"&passwd=123456";
			String content=HttpsHandler.get(consume,mask);
			Intent resultIntent = new Intent();
			resultIntent.setClass(WxPayWebActivity.this, ConsumeResultActivity.class);
			Bundle bundle = new Bundle();
			bundle.putString("response", content);
			bundle.putString("balance", "");
			bundle.putString("sessionId", sessionId);
			resultIntent.putExtra("bundle", bundle);
			startActivity(resultIntent);
			WxPayWebActivity.this.finish();
			overridePendingTransition(android.R.anim.slide_in_left,android.R.anim.slide_out_right);
	}*/
}
