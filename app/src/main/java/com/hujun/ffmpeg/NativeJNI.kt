package com.hujun.ffmpeg

import android.view.Surface

/**
 * Created by junhu on 2020-01-09
 */
class NativeJNI {
    // Used to load the 'native-lib' library on application startup.
    init {
        System.loadLibrary("hujunplayer")
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun startPlay(path:String,surface: Surface):Int
}