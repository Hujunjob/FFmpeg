#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <zconf.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
}
extern "C" JNIEXPORT jstring JNICALL
Java_com_hujun_ffmpeg_NativeJNI_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(av_version_info());
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_hujun_ffmpeg_NativeJNI_startPlay(JNIEnv *env, jobject instance,
                                          jstring path_, jobject surface) {
    const char *path = env->GetStringUTFChars(path_, 0);
    avformat_network_init();
    AVFormatContext *formatContext = avformat_alloc_context();
    //1、打开URL
    AVDictionary *opts = NULL;
    //设置超时3秒
    av_dict_set(&opts, "timeout", "3000000", 0);
    //强制指定AVFormatContext中AVInputFormat的。这个参数一般情况下可以设置为NULL，这样FFmpeg可以自动检测AVInputFormat。
    //输入文件的封装格式
//    av_find_input_format("avi")
//    ret为零 表示成功
    int ret = avformat_open_input(&formatContext, path, NULL, &opts);

    //通知ffmepg解析视频流
    avformat_find_stream_info(formatContext, NULL);
    //一次解析所有流，包括视频流、音频流和字幕流等
    int videoIndex = -1;
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        AVStream *stream = formatContext->streams[i];
        AVCodecParameters *parameters = stream->codecpar;
        //先解析视频流
        if (parameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }

    env->ReleaseStringUTFChars(path_, path);

    if (videoIndex == -1) {
        return -1;
    }

    //视频流
    AVStream *stream = formatContext->streams[videoIndex];
    AVCodecParameters *parameters = stream->codecpar;
    int width = parameters->width;
    int height = parameters->height;

    //解码器 h264
    AVCodec *codec = avcodec_find_decoder(parameters->codec_id);
    //获取解码器上下文
    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    //将解码器参数copy到解码器上下文
    avcodec_parameters_to_context(codecContext, parameters);

    avcodec_open2(codecContext, codec, nullptr);

    //解码，可以得到avpacket
    AVPacket *avPacket = av_packet_alloc();

//    struct SwsContext *sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
//                                      int dstW, int dstH, enum AVPixelFormat dstFormat,
//                                      int flags, SwsFilter *srcFilter,
//                                      SwsFilter *dstFilter, const double *param);
//

    //视频格式转换方式，有很多很多转换方式
    //重视转换速度
//    int flags = SWS_FAST_BILINEAR;
    //重视转换质量
    int flags = SWS_BILINEAR;
    //将视频从原始格式，转换为AV_PIX_FMT_RGBA格式
    SwsContext *swsContext = sws_getContext(parameters->width, parameters->height,
                                            codecContext->pix_fmt,
                                            parameters->width, parameters->height, AV_PIX_FMT_RGBA,
                                            flags, nullptr, nullptr, nullptr);

    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    //设置窗口缓冲区大小
    ANativeWindow_setBuffersGeometry(nativeWindow, codecContext->width, codecContext->height,
                                     WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer outBuffer;

    //从视频流中读取数据包，是被压缩的数据，还没有解包
    //不断循环取数据
    while (av_read_frame(formatContext, avPacket) >= 0) {
        avcodec_send_packet(codecContext, avPacket);
        AVFrame *frame = av_frame_alloc();
        ret = avcodec_receive_frame(codecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            break;
        }
        //绘制
        //首先进行格式转换，将yuv转换为rgba

        //搞一个接收rgba数据的容器
//        int av_image_alloc(uint8_t *pointers[4], int linesizes[4],
//                           int w, int h, enum AVPixelFormat pix_fmt, int align);
        uint8_t *pointers[4];
        int linesizes[4];
        //最后一个参数代表对其方式，1是左对齐
        av_image_alloc(pointers, linesizes, parameters->width, parameters->height, AV_PIX_FMT_RGBA,
                       1);


//        int sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
//                      const int srcStride[], int srcSliceY, int srcSliceH,
//                      uint8_t *const dst[], const int dstStride[]);

//        const uint8_t *const srcSlice[] = frame->data;
        sws_scale(swsContext, frame->data, frame->linesize, 0, frame->height,
                  pointers, linesizes);

        //交换缓冲区，将数据拷贝到窗口缓冲区
        //先把窗口锁注，防止其他人写入
        ANativeWindow_lock(nativeWindow, &outBuffer, nullptr);

        uint8_t *dst= (uint8_t *) outBuffer.bits;
//            拿到一行有多少个字节 RGBA
        int destStride=outBuffer.stride*4;
        uint8_t *src_data = pointers[0];
        int src_linesize = linesizes[0];
        uint8_t *firstWindown = static_cast<uint8_t *>(outBuffer.bits);

        //进行渲染
        //数据循环拷贝
        //一排一排的数据进行拷贝
        for (int i = 0; i < codecContext->height; ++i) {
            //内存拷贝
//            memcpy()
            memcpy(firstWindown + i * destStride, src_data + i * src_linesize, destStride);
        }



        //解锁窗口
        ANativeWindow_unlockAndPost(nativeWindow);
        usleep(1000 * 16);
        av_frame_free(&frame);

    }

    ANativeWindow_release(nativeWindow);
    avcodec_close(codecContext);
    avformat_free_context(formatContext);
    env->ReleaseStringUTFChars(path_, path);



//    if(parameters!=nullptr){
//        printf("width=%d,height=%d",parameters->width,parameters->height);
//    }

    return 0;
}
