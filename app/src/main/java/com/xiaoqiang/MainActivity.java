package com.xiaoqiang;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

public class MainActivity extends AppCompatActivity {
     LibRtmp rtmp;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        rtmp = new LibRtmp();
        rtmp.Init(640,360);
        findViewById(R.id.button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String url = "rtmp://19345.mpush.live.lecloud.com/live/demo";
                rtmp.Connect(url);
                rtmp.SendVideoPage(new byte[640*360*3/2],640*360*3/2,1);
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        rtmp.DisConnect();
        rtmp.Release();
    }
}

