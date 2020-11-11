package com.MyCompany.TowerDefense;
import java.util.Arrays;
import java.util.UUID;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.telephony.TelephonyManager;
import android.util.Log;

public class Test  {
    protected static final String PREFS_FILE = "gank_device_id.xml";
    protected static final String PREFS_DEVICE_ID = "gank_device_id";
    protected static String uuid;
    public static String getUUID(){
    	final TelephonyManager tm = (TelephonyManager) AppActivity.getContext().getSystemService(Context.TELEPHONY_SERVICE);  
        final String tmDevice, tmSerial, androidId;
        tmDevice = "" + tm.getDeviceId();
        tmSerial = "" + tm.getSimSerialNumber();
        androidId = "" + android.provider.Settings.Secure.getString(AppActivity.getContext().getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
        UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
        return  deviceUuid.toString();
    }
    
    public static String encrypt(byte[] plaintext) {
    	AES handle = AES.getInstance();
    	return  handle.encrypt(plaintext);
    }
}
