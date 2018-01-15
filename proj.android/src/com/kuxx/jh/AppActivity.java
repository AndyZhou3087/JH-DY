/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.kuxx.jh;

import java.text.DecimalFormat;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.kuxx.jh.DownloadApkTask.onDownloadApkListener;
import com.umeng.analytics.mobclick.game.MobClickCppHelper;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.widget.Toast;


public class AppActivity extends Cocos2dxActivity implements onDownloadApkListener {
	
	
	public static HandlerThread handlerThread;
    public static THandler handler;
    public static AppActivity theOnly = null;
    private ProgressDialog updateProgressDialog = null;
    static Runnable updateApkThread;
    
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		theOnly = this;
    	handlerThread = new HandlerThread("handle_thread");
        handlerThread.start();
        handler = new THandler(handlerThread.getLooper());
        Utils.init(this);
        MobClickCppHelper.init(this,"59f979bca40fa359fe000066", Utils.getChannelID());
		PayAction.init(this);
    }
    
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
    
 
    @Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		MobClickCppHelper.onPause(this);
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		MobClickCppHelper.onResume(this);
	}

    @Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if (keyCode == KeyEvent.KEYCODE_BACK )
		{
			JNI.changeGameStates(1);
			new AlertDialog.Builder(theOnly)
		    .setMessage("确认退出"+ theOnly.getString(R.string.app_name) + "吗？")
		    .setPositiveButton("确认", new DialogInterface.OnClickListener() {
		        public void onClick(DialogInterface dialog, int whichButton) 
		        {
		        	JNI.saveData();
		        	theOnly.finish();
		        	System.exit(0);
		        }
		    })
		    .setNegativeButton("取消", new DialogInterface.OnClickListener() {
		        public void onClick(DialogInterface dialog, int whichButton) {
		        	JNI.changeGameStates(0);
		        }
		    })
		    .setOnCancelListener(new OnCancelListener() {

				@Override
				public void onCancel(DialogInterface dialog) {
					// TODO Auto-generated method stub
					JNI.changeGameStates(0);
				}
	        })
		    .show();
		}
		return false;
	}
    public static void copyToClipboard(String text)
    {
        Message msg = AppActivity.handler.obtainMessage();
        msg.what = -4;
        msg.obj = text;
        msg.sendToTarget();
    }
    
    public static void updateApk(final String url)
    {
    	updateApkThread = new Runnable()      
		{     
		 public void run()
		 {
			 downloadApk(url);
		 }                
		};
		theOnly.runOnUiThread(updateApkThread);
    }
    
    private static void downloadApk(final String url)
    {
    	JNI.changeGameStates(1);
    	new AlertDialog.Builder(theOnly)
	    .setTitle("软件更新")
	    .setMessage("检测到有新版本，是否确认更新？")
	    .setPositiveButton("确认", new DialogInterface.OnClickListener() {
	        public void onClick(DialogInterface dialog, int whichButton) 
	        {
                DownloadApkTask tsk = new DownloadApkTask(theOnly);
                String downloadUrl = url;
                String destFilePath = Utils.getExternalStorage();
                if (destFilePath != null)
                {
                	Toast.makeText(theOnly, "开始下载...", Toast.LENGTH_LONG).show();
                	destFilePath = destFilePath + "/jh.apk";
                	tsk.execute(downloadUrl, destFilePath);
                }
                else
                {
                	Toast.makeText(theOnly, "无法下载，未检查到内存卡。", Toast.LENGTH_LONG).show();
                }
	        }
	    })
	    .setNegativeButton("取消", new DialogInterface.OnClickListener() {
	        public void onClick(DialogInterface dialog, int whichButton) {
	        	JNI.changeGameStates(0);
	        }
	    })
	    .setCancelable(false)
	    .show();
    }
	
	public static class THandler extends Handler

   	{
   		  public THandler()
   		  {
   		   
   		  }
   		  
   		  public THandler(Looper looper)
   		  {
   			  super(looper);
   		  }
   		  
   		  @Override
   		  public void handleMessage(Message msg) 
   		  {
   			  if (!Thread.currentThread().isInterrupted())
   			  {
   				  switch(msg.what)
   				  {
   				  case -1:
   					  Toast.makeText(theOnly, "网络不可用，请检查您的网络设置", Toast.LENGTH_SHORT).show();
   					  JNI.sendMessage(-1);
   					  break;
   				  case -2:
   					  Toast.makeText(theOnly, "下单失败！！", Toast.LENGTH_SHORT).show();
   					  JNI.sendMessage(-1);
   					  break;
   				  case -3:
   					  Toast.makeText(theOnly, "调取支付插件失败！！", Toast.LENGTH_SHORT).show();
   					  JNI.sendMessage(-1);
   					  break;
   				  case -4:
   			        ClipboardManager cm = (ClipboardManager) theOnly.getSystemService(Context.CLIPBOARD_SERVICE);
   			        // 将文本内容放到系统剪贴板里。
   			        ClipData myClip = ClipData.newPlainText("qqnum", (String)msg.obj);
   			        cm.setPrimaryClip(myClip);
   					  Toast.makeText(theOnly, "客服QQ已复制到剪切板！！", Toast.LENGTH_SHORT).show();
   					  break;
   				  case 0:
   					  JNI.sendMessage(0);
   					  break;
   				  default:
   					  JNI.sendMessage(-1);
   					  break;
   				  }
   			  } 
   		}
   	}

	@Override
	public void onUpdateProgress(Long... values) {
		// TODO Auto-generated method stub
        if (updateProgressDialog == null)
        {
        	double s = ((double) values[0] / 1024/1024);	
        	DecimalFormat df = new DecimalFormat("#.00");
            updateProgressDialog = new ProgressDialog(theOnly);
            updateProgressDialog.setMessage("正在下载	" + df.format(s) +"M");
            updateProgressDialog.setIndeterminate(false);
            updateProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
            updateProgressDialog.setMax(100);
            updateProgressDialog.setProgress(0);
            updateProgressDialog.setCanceledOnTouchOutside(false);
            updateProgressDialog.show();
        }

        if (updateProgressDialog != null && updateProgressDialog.isShowing())
        {
            updateProgressDialog.setProgress((int) (values[1] * 100.0 / values[0]));
        }
	}

	@Override
	public void onDownloadFinished(String destFilePath) {
		// TODO Auto-generated method stub
		JNI.changeGameStates(0);
        if (updateProgressDialog != null && updateProgressDialog.isShowing())
        {
            updateProgressDialog.dismiss();
            updateProgressDialog = null;
        }
        if (!Utils.IsExistFile(destFilePath))
        {
            return;
        }
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setAction(Intent.ACTION_VIEW);
        intent.setDataAndType(Uri.parse("file://" + destFilePath),
                "application/vnd.android.package-archive");
       theOnly.startActivity(intent);
	}

}
