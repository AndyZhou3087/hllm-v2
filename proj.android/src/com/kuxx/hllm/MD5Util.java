package com.kuxx.hllm;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.util.Log;

public class MD5Util
{
    protected   static   char  hexDigits[] = {  '0' ,  '1' ,  '2' ,  '3' ,  '4' ,  '5' ,  '6' ,   
        '7' ,  '8' ,  '9' ,  'a' ,  'b' ,  'c' ,  'd' ,  'e' ,  'f'  };   

    public static String getFileMD5String(String fileName) throws IOException
	{
    	MessageDigest messagedigest;
    	try
    	{
    		messagedigest = MessageDigest.getInstance("MD5");
    	}
    	catch  (NoSuchAlgorithmException nsaex)
    	{
    		Log.e("yytest", "NoSuchAlgorithmException nsaex");
    		return null;
    	}
    	
    	File file = new File(fileName);
		InputStream fis; 
        fis = new  FileInputStream(file);   
        byte [] buffer = new byte[1024];
        int numRead = 0;
        while ((numRead = fis.read(buffer)) > 0)
        {   
           messagedigest.update(buffer, 0 , numRead);   
        }   
        fis.close();
        return bufferToHex(messagedigest.digest());
    }   

    public static String getMD5String(String str) throws IOException
	{
    	MessageDigest messagedigest;
    	try
    	{
    		messagedigest = MessageDigest.getInstance("MD5");
    	}
    	catch  (NoSuchAlgorithmException nsaex)
    	{
    		Log.e("yytest", "NoSuchAlgorithmException nsaex");
    		return null;
    	}
    	
        messagedigest.update(str.getBytes());   
        return bufferToHex(messagedigest.digest());
    }   

    private static String bufferToHex(byte bytes[])
    {
    	return bufferToHex(bytes, 0, bytes.length);
    }
    
    private static String bufferToHex(byte bytes[], int start, int len)
    {   
        StringBuffer stringbuffer = new StringBuffer(2 * len);
        int k = start + len;
        for (int l = start; l < k; l++)
        {   
            appendHexPair(bytes[l], stringbuffer);
        }   
        return stringbuffer.toString();
    }
    
    private static void appendHexPair(byte bt, StringBuffer stringbuffer)
    {
    	char c0 = hexDigits[(bt & 0xf0) >> 4];
    	char c1 = hexDigits[bt & 0xf];
        stringbuffer.append(c0);
        stringbuffer.append(c1);
    }   
}
