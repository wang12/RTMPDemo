package com.xiaoqiang;

import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity {
     LibRtmp rtmp;
    int i = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        rtmp = new LibRtmp();
        rtmp.Init(640,360, Environment.getExternalStorageDirectory()+"/push_stream/demo.hls");
        findViewById(R.id.button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String url = "rtmp://19345.mpush.live.lecloud.com/live/demo";
                i = 0;
                rtmp.Connect(url);
                mHandler.post(runnable);
            }
        });
    }

    private Handler mHandler = new Handler();
    Runnable runnable = new Runnable() {
        @Override
        public void run() {
            rtmp.SendVideoPage(new byte[640*360*3/2],640*360*3/2,i++);
            Log.d("wqq_jni","当前时间："+i);
            mHandler.postDelayed(runnable,40);
        }
    };
    @Override
    protected void onDestroy() {
        super.onDestroy();
        mHandler.removeCallbacksAndMessages(null);
        rtmp.DisConnect();
        rtmp.Release();
    }
}

