//
// Created by wangqiangqiang on 2017/7/4.
//

#include <myx264.h>
#include <string.h>
#include "lelog.h"

x264_param_t *pParam;
x264_t *pHandle;
x264_picture_t *pPic_in;
x264_picture_t *pPic_out;
x264_nal_t *pNals = NULL;
int iNal = 0;
unsigned char *u;
unsigned char *v;
long wh = 0;

int X264_Init(int width, int height) {
    pParam = new x264_param_t;
    pPic_in = new x264_picture_t;
    pPic_out = new x264_picture_t;
    wh = width * height;
    u = new unsigned char[wh / 4];
    v = new unsigned char[wh / 4];


    x264_param_default_preset(pParam,"veryfast", "zerolatency");
    pParam->i_width = width;
    pParam->i_height = height;
    pParam->i_csp = X264_CSP_NV21;

    x264_param_apply_profile(pParam, x264_profile_names[5]);
    return 0;
}

int X264_Start() {
    pHandle = x264_encoder_open(pParam);

    x264_picture_init(pPic_out);
    x264_picture_alloc(pPic_in, pParam->i_csp, pParam->i_width, pParam->i_height);
    return 0;
}

int X264_Stop() {
    x264_picture_clean(pPic_in);
    x264_encoder_close(pHandle);
    return 0;
}

int X264_CodeingX264(unsigned char *data, long length,long time) {
    strncpy((char *) pPic_in->img.plane[0], (const char *) data, wh); //y
    for (int i = 0; i < wh / 4; i++) {
        u[i] = data[wh + i];
        v[i] = data[wh + i + 1];
    }
    pPic_in->img.plane[1] = u; //u
    pPic_in->img.plane[2] = v;//v
    pPic_in->i_pts = time;
    pPic_in->i_dts = time;
    int ret = x264_encoder_encode(pHandle, &pNals, &iNal, pPic_in, pPic_out);
    if (ret< 0){
        LOGD("编码错误：%d",ret);
        return NULL;
    }
    for (int i = 0; i < iNal; ++i) {//将编码数据写入文件.
        LOGD("nal大小:%d,编码后数据长度:%d",iNal,pNals[i].i_payload);
    }
    return ret;
}

int X264_Realse() {
    if (pParam != NULL) {
        delete pParam;
    }
    if (pPic_in != NULL){
        delete pPic_in;
}
    if (pPic_out != NULL) {
        delete pPic_out;
    }
    if (u != NULL) {
        delete[] u;
    }
    if (v != NULL) {
        delete[] v;
    }
    pHandle = NULL;
    return 0;
}
