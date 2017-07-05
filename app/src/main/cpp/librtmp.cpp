#include <jni.h>
#include "include/lelog.h"
#include "include/rtmp.h"
#include <string.h>
#include "include/myx264.h"

jmethodID callback = NULL;
jobject jobj;
JNIEnv *jnienv;
RTMP *rtmp;
const int RTMP_ERROR = -1;


const int RTMP_INIT = 0;

#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)


extern "C"
{
JNIEXPORT jint JNICALL
Java_com_xiaoqiang_LibRtmp_Init(JNIEnv *env, jobject obj,jint width,jint hegith,jstring path) {
    jclass cls = env->GetObjectClass(obj);
    if (!callback) {
        jnienv = env;
        jobj = obj;
        callback = env->GetMethodID(cls, "nativeCallback", "(II)V");
    }
    if (!callback) {
        LOGD("加载native_callback函数失败");
        return -1;
    }
    if (rtmp) {
        env->CallVoidMethod(jobj, callback, RTMP_ERROR, -1);
        return -2;
    }
//    rtmp = RTMP_Alloc();
//    RTMP_Init(rtmp);
//    env->CallVoidMethod(jobj, callback, RTMP_INIT, 0);
    char* localpath = (char *) env->GetStringUTFChars(path, 0);
    X264_Init(width,hegith,localpath);
    return 0;
}
JNIEXPORT jint JNICALL
Java_com_xiaoqiang_LibRtmp_Connect(JNIEnv *env, jobject,jstring url) {
//    jboolean b = JNI_TRUE;
//    char* path = (char *) env->GetStringUTFChars(url, &b);
//    if(RTMP_SetupURL(rtmp,path) == FALSE){
//        RTMP_Free(rtmp);
//        rtmp = NULL;
//        return -1;
//    }
//    env->ReleaseStringUTFChars(url,path);
//    RTMP_EnableWrite(rtmp);
//    if(RTMP_Connect(rtmp,NULL) == FALSE){
//        RTMP_Free(rtmp);
//        rtmp = NULL;
//        return -2;
//    }
//    if (RTMP_ConnectStream(rtmp,0) == FALSE)
//    {
//        RTMP_Close(rtmp);
//        RTMP_Free(rtmp);
//        return -3;
//    }

    X264_Start();
    return 0;
}
JNIEXPORT jint JNICALL
Java_com_xiaoqiang_LibRtmp_DisConnect(JNIEnv *env, jobject obj) {
    X264_Stop();
//    if (rtmp) {
//        RTMP_Close(rtmp);
//        RTMP_Free(rtmp);
//        rtmp = NULL;
//    }
    return 0;
}

int sendPack(unsigned int nPacketType, unsigned char *data, unsigned int size,
             unsigned int nTimestamp) {
    RTMPPacket *pack = new RTMPPacket;
    memset(pack, 0, RTMP_HEAD_SIZE);
    pack->m_body = (char *) pack + RTMP_HEAD_SIZE;
    pack->m_nBodySize = size;
    memcpy(pack->m_body, data, size);

    pack->m_hasAbsTimestamp = 0;
    pack->m_packetType = nPacketType;
    pack->m_nInfoField2 = rtmp->m_stream_id;
    pack->m_nChannel = 0x04;

    pack->m_headerType = RTMP_PACKET_SIZE_LARGE;
    if (RTMP_PACKET_TYPE_AUDIO == nPacketType && size != 4) {
        pack->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    }
    pack->m_nTimeStamp = nTimestamp;
    /*∑¢ÀÕ*/
    int nRet = 0;
    if (RTMP_IsConnected(rtmp)) {
        nRet = RTMP_SendPacket(rtmp, pack, TRUE);
    }
    delete pack;
    return nRet;

}
JNIEXPORT jint JNICALL
Java_com_xiaoqiang_LibRtmp_Release(JNIEnv *env, jobject ) {
    return X264_Realse();
}
JNIEXPORT jint JNICALL
Java_com_xiaoqiang_LibRtmp_SendVideoPage(JNIEnv *env, jobject obj, jbyteArray data, jlong length,
                                         jlong time) {
    if (data == NULL && length < 11) {
        return -1;
    }
    unsigned char* arrar = new unsigned char[length] ;
    env->GetByteArrayRegion(data, 0, length, (jbyte *) arrar);
    X264_CodeingX264(arrar, length,time);
//    env->ReleaseByteArrayElements(data, (jbyte *) arrar, 0);
    delete[] arrar;
//    unsigned char *body = new unsigned char[length + 9];
//    memset(body, 0, length + 9);
//
//    int i = 0;
////    if(bIsKeyFrame){
////        body[i++] = 0x17;// 1:Iframe  7:AVC
////        body[i++] = 0x01;// AVC NALU
////        body[i++] = 0x00;
////        body[i++] = 0x00;
////        body[i++] = 0x00;
////
////
////        // NALU size
////        body[i++] = length>>24 &0xff;
////        body[i++] = length>>16 &0xff;
////        body[i++] = length>>8 &0xff;
////        body[i++] = length&0xff;
////        // NALU data
////        memcpy(&body[i],data,size);
////        SendVideoSpsPps(metaData.Pps,metaData.nPpsLen,metaData.Sps,metaData.nSpsLen);
////    }else{
//    body[i++] = 0x27;// 2:Pframe  7:AVC
//    body[i++] = 0x01;// AVC NALU
//    body[i++] = 0x00;
//    body[i++] = 0x00;
//    body[i++] = 0x00;
//
//
//    // NALU size
//    body[i++] = length >> 24 & 0xff;
//    body[i++] = length >> 16 & 0xff;
//    body[i++] = length >> 8 & 0xff;
//    body[i++] = length & 0xff;
//    // NALU data
//    memcpy(&body[i], data, length);
////    }
//
//    int bRet = sendPack(RTMP_PACKET_TYPE_VIDEO, body, i + length, time);
//    delete[] body;
//    return bRet;
    return 0;
}
JNIEXPORT jint JNICALL
Java_com_xiaoqiang_LibRtmp_SendAudioPage(JNIEnv *env, jobject obj, jcharArray data, jlong length,
                                         jlong time) {

    return 0;
}
}

