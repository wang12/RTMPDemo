package com.xiaoqiang;

import android.util.Log;

/**
 * Created by xiaoqiang on 2017/7/3.
 */

public class LibRtmp {
    static {
        System.loadLibrary("native-lib");
    }
    public   native int Init(int width,int height);
    public  native int Connect(String url);
    public  native int DisConnect();
    public native void Release();
    public native int SendVideoPage(byte[] data,long length,long time);
    public native int SendAudioPage(byte[] data,long length,int time);
    public void nativeCallback(int type, int state)
    {
        Log.e("wqq_jni","notify_id="+type+";param="+type);
    }
}
