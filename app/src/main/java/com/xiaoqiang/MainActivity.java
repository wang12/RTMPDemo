package com.xiaoqiang;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final LibRtmp rtmp = new LibRtmp();
        rtmp.Init();
        findViewById(R.id.button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String url = "rtmp://19345.mpush.live.lecloud.com/live/demo";
                rtmp.Connect(url);
                rtmp.SendVideoPage(new byte[640*360*3/2],640*360*3/2,1);
            }
        });
    }
}
