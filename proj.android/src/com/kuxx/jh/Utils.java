package com.kuxx.jh;

import java.io.File;
import java.io.UnsupportedEncodingException;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo.State;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Environment;
import android.telephony.TelephonyManager;


public class Utils{
	
	static private Context sContext;
	
	public static void init(Context context)
	{
		sContext = context;
	}
	
	public static String getBaseDir()
	{
		 String SDPath = getExternalStorage();
		 String destFilePath = "";
		 if (SDPath != null)
		 {
			  destFilePath = SDPath + "/" + getPkgName();
	       	  File destDir = new File(destFilePath);
	       	  if (!destDir.exists()) 
	       	  {
	       		  destDir.mkdirs();
	       	  }
		 }
		 else
		 {
			 destFilePath = "";
		 }
		 return destFilePath;
		 
	}
	
    public static String getExternalStorage()
    {
    	boolean isExit = android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
    	if (isExit)
    		return Environment.getExternalStorageDirectory().toString();
    	else
    		return null;
    		
    }
    
    public static boolean IsExistFile(String path)
    {
        try
        {
            File file = new File(path);
            if (file.exists())
            {
                return true;
            }
        }
        catch (Exception e)
        {
        }
        return false;
    }
	
    public static String GetIMSI()
	{
    	TelephonyManager s_tm	= (TelephonyManager)sContext.getSystemService(Context.TELEPHONY_SERVICE);
		String imsi = "";
		try {
			imsi = s_tm.getSubscriberId(); 
		} catch (Exception e)
		{
			e.printStackTrace();
		}	
		
		if(imsi == null) imsi = "";
		return imsi;
	}
    
    public static String GetIMEI()
    {
        TelephonyManager tm = (TelephonyManager) sContext.getSystemService(Activity.TELEPHONY_SERVICE);
        return tm.getDeviceId();
    }
    
    public static String GetLocalMacAddress() {
		WifiManager wifi = (WifiManager) sContext.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();
		return info.getMacAddress();
	}
    
    public static String UUID()
    {
		String uuid = GetIMEI();
		
		if (uuid == null)
			uuid = "";

		String macAddr = GetLocalMacAddress();
		if (macAddr == null)
			macAddr = "";
		uuid += macAddr.replaceAll(":", "");
		return uuid;
    }
    
    /**
     * 获取版本号
     * @return 当前应用的版本号
     */
    public static String getVersion() {
        try {
            PackageManager manager = sContext.getPackageManager();
            PackageInfo info = manager.getPackageInfo(sContext.getPackageName(), 0);
            String vname = info.versionName;
            return vname;
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }
    
    public static int getVersionCode() {
        try {
            PackageManager manager = sContext.getPackageManager();
            PackageInfo info = manager.getPackageInfo(sContext.getPackageName(), 0);
            int vcode = info.versionCode;
            return vcode;
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
    }
    
    public static String getPkgName()
    {
        return sContext.getPackageName();
    }
    
	public static String getChannelID() {
		String channel = "";
		try {
			channel = sContext.getPackageManager().getApplicationInfo(getPkgName(), PackageManager.GET_META_DATA).metaData.getString("TH_PAYCHANNEL");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return channel;
	}
	
	public static String getMetaData(Context contex, String meta) {
		String data = "";
		try {
			data = contex.getPackageManager().getApplicationInfo(contex.getPackageName(), PackageManager.GET_META_DATA).metaData.getString(meta);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return data;
	}
	
    public static boolean getNetworkAvailable() 
    { 
    	boolean success = false;
    	//获得网络连接服务   
    	ConnectivityManager connManager = (ConnectivityManager) sContext.getSystemService(Context.CONNECTIVITY_SERVICE);   
    	State state = connManager.getNetworkInfo(   
    	ConnectivityManager.TYPE_WIFI).getState(); // 获取网络连接状态   
    	if (State.CONNECTED == state) 
    	{ // 判断是否正在使用WIFI网络   
    		success = true;
    		return success;
    	}
    	try{
	    	state = connManager.getNetworkInfo(   
	    	ConnectivityManager.TYPE_MOBILE).getState(); // 获取网络连接状态   
	    	if (State.CONNECTED == state) 
	    	{ // 判断是否正在使用GPRS网络   
	    		success = true;   
	    	}
    	}catch (Exception e)
    	{
    		e.printStackTrace();
    	}
    	return success;
    	  
    }
    
    public static String gbkToUTF8(String str)
    {
    	String retstr = "";
		try {
			retstr = new String(str.getBytes("UTF-8"),"ISO-8859-1");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

    	return retstr;
    }
}
