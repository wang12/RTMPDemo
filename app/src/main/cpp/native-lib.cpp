#include <jni.h>
#include <string>
#include "rtmp.h"
extern "C"
JNIEXPORT jstring JNICALL
Java_com_xiaoqiang_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    RTMP *rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    int i = RTMP_SetupURL(rtmp,"rtmp://19345.mpush.live.lecloud.com/live/1234");
    RTMPPacket *packet;
    RTMPPacket_Alloc(packet, sizeof(RTMPPacket));
    i = RTMP_Connect(rtmp,packet);
    hello = "hello rtmp_demo  ";
    return env->NewStringUTF(hello.c_str());
}
