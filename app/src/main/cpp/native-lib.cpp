#include <jni.h>
#include <string>



#define JNIFUNCTION(sig) Java_com_hujun_ffmpeg_NativeJNI_##sig

extern "C" {
#include "libavcodec/avcodec.h"
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}


extern "C" JNIEXPORT jstring JNICALL JNIFUNCTION(
        stringFromJNI(JNIEnv * env, jobject /* this */)
        {
            std::string hello = "Hello from C++";
            return env->NewStringUTF(av_version_info());
        }
)

extern "C" JNIEXPORT jint JNICALL JNIFUNCTION(
        startPlay(JNIEnv * env, jobject /* this */,jstring vpath,jobject surface)
        {
            const char* path = env->GetStringUTFChars(vpath,NULL);
            //FFmpeg 播放视频和音频，先写播放视频

            //初始化网络模块，因为FFmpeg不止可以播放本地文件，也可以传递url直接播放网络视频
            avformat_network_init();




            return 0;
        }
)

