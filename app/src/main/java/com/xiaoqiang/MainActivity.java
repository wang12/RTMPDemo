package com.xiaoqiang;

import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import java.io.IOException;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private LibRtmp rtmp;
    private Camera mCamera;
    long time = 0;
    private byte[] data;
    SurfaceView surfaceView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        surfaceView = (SurfaceView) findViewById(R.id.surface);
        surfaceView.getHolder().addCallback(callback);
        findViewById(R.id.button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });
    }

//    private Handler mHandler = new Handler();
//    Runnable runnable = new Runnable() {
//        @Override
//        public void run() {
//            rtmp.SendVideoPage(new byte[640*360*3/2],640*360*3/2,i++);
//            Log.d("wqq_jni","当前时间："+i);
//            mHandler.postDelayed(runnable,40);
//        }
//    };
    @Override
    protected void onDestroy() {
        super.onDestroy();
//        mHandler.removeCallbacksAndMessages(null);

    }


    private void openCamera(){
        mCamera = Camera.open(Camera.CameraInfo.CAMERA_FACING_FRONT);
        Camera.Parameters parameters = mCamera.getParameters();
//        List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
        data = new byte[480 * 320*3/2];
        Log.d("wqq","thread name="+Thread.currentThread().getName());
        rtmp = new LibRtmp();
        rtmp.Init(480,320, Environment.getExternalStorageDirectory()+"/push_stream/demo.hls");
        String url = "rtmp://19345.mpush.live.lecloud.com/live/demo";
        rtmp.Connect(url);
        parameters.setPreviewSize(480, 320);
        parameters.setPictureFormat(ImageFormat.NV21);
        parameters.setPreviewFormat(ImageFormat.NV21);
        mCamera.setParameters(parameters);
        mCamera.setPreviewCallback(new Camera.PreviewCallback() {
            @Override
            public void onPreviewFrame(byte[] data, Camera camera) {
                if(time == 0){
                    time = System.nanoTime();
                }
                Log.d("wqq","thread name="+Thread.currentThread().getName());
                rtmp.SendVideoPage(data,data.length,System.nanoTime()-time);
            }
        });
        mCamera.addCallbackBuffer(data);
    }
    private void stopCamera(){
        time = 0;
        rtmp.DisConnect();
        rtmp.Release();
        if(mCamera != null){
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
    }
    private SurfaceHolder.Callback callback = new SurfaceHolder.Callback() {
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            try {
                openCamera();
                mCamera.setPreviewDisplay(holder);
                mCamera.startPreview();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            stopCamera();
        }
    };
}

