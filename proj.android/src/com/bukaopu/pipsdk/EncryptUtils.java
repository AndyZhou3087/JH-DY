/**
 * 
 */
package com.bukaopu.pipsdk;

import java.security.MessageDigest;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * @author Zed
 *
 */
public class EncryptUtils {
	
	//十六进制下数字到字符的映射数组  
    private final static String[] hexDigits = {"0", "1", "2", "3", "4",  
        "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"}; 
    
    /**
     * 双重MD5加密
     * @param originalString
     * @return
     */
    public static String encodeByDoubleMD5(String originalString) {
    	return encodeByMD5(encodeByMD5(originalString));
    }
    
    /**
     * 双重MD5加密with salt
     * @param originalString
     * @return
     */
    public static String encodeByDoubleMD5(String originalString, String salt) {
    	return encodeByMD5(encodeByMD5(originalString + salt));
    }
	
    /**
     * 双重MD5加密with key mDiameter
     * @param originalString
     * @return
     */
    public static String encodeByDoubleMD5(String originalString, String firstKey , String secondKey) {
    	return encodeByMD5(encodeByMD5(originalString + firstKey) + secondKey);
    }
    
	/**  对字符串进行MD5加密     */  
    public static String encodeByMD5(String originString){  
        if (originString != null){  
            try {  
                //创建具有指定算法名称的信息摘要  
                MessageDigest md = MessageDigest.getInstance("MD5");  
                //使用指定的字节数组对摘要进行最后更新，然后完成摘要计算  
                byte[] results = md.digest(originString.getBytes("UTF-8"));  
                //将得到的字节数组变成字符串返回  
                String resultString = byteArrayToHexString(results);  
                return resultString;
            } 
            catch(Exception ex){  
            	throw new IllegalStateException(ex.toString());
            }  
        }  
        return originString;  
    } 	
    
	/**  对字符串进行MD5加密     */  
    public static byte[] encodeByMD5(byte[] originString){  
        try {  
            //创建具有指定算法名称的信息摘要  
            MessageDigest md = MessageDigest.getInstance("MD5");  
            //使用指定的字节数组对摘要进行最后更新，然后完成摘要计算  
            return md.digest(originString);   
        } 
        catch(Exception ex){  
        	throw new IllegalStateException(ex.toString());
        } 
    } 
    
    
    /**  
     * 转换字节数组为十六进制字符串 
     * @param     字节数组 
     * @return    十六进制字符串 
     */  
    public static String byteArrayToHexString(byte[] b){  
        StringBuilder resultSb = new StringBuilder();  
        for (int i = 0; i < b.length; i++){  
            resultSb.append(byteToHexString(b[i]));  
        }  
        return resultSb.toString();  
    } 
    
    /** 将一个字节转化成十六进制形式的字符串     */  
    public static String byteToHexString(byte b){  
        int n = b;  
        if (n < 0)  
            n = 256 + n;  
        int d1 = n / 16;  
        int d2 = n % 16;  
        return hexDigits[d1] + hexDigits[d2];  
    }
    
	
	public static byte[] encryptByAes(byte[] text, String key) throws Exception {
	    Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
	    //The password must be 16  bytes
	    byte[] keyBytes = new byte[16];
	    byte[] b = key.getBytes("UTF-8");
	    int len = b.length;
	    if (len > keyBytes.length) {
	        len = keyBytes.length;
	    }
	    System.arraycopy(b, 0, keyBytes, 0, len);
	    SecretKeySpec keySpec = new SecretKeySpec(keyBytes, "AES");
	    IvParameterSpec ivSpec = new IvParameterSpec(keyBytes);
	    cipher.init(Cipher.ENCRYPT_MODE, keySpec, ivSpec);
	    return cipher.doFinal(text);
	}
	
	public static byte[] decryptByAes(byte[] text, String key) throws Exception {
	    Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
	    byte[] keyBytes = new byte[16];
	    byte[] b = key.getBytes("UTF-8");
	    int len = b.length;
	    if (len > keyBytes.length) {
	        len = keyBytes.length;
	    }
	    System.arraycopy(b, 0, keyBytes, 0, len);
	    SecretKeySpec keySpec = new SecretKeySpec(keyBytes, "AES");
	    IvParameterSpec ivSpec = new IvParameterSpec(keyBytes);
	    cipher.init(Cipher.DECRYPT_MODE, keySpec,ivSpec);
	    return cipher.doFinal(text);
	}
    
    public static void main(String args[]) throws Exception {
    	System.out.println(encodeByDoubleMD5("e32aafc00af92f49", "MZHD"));
    	
   
    }
    


}
