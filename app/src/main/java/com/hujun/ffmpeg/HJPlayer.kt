package com.hujun.ffmpeg

import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

/**
 * Created by junhu on 2020-01-09
 */
class HJPlayer(var mSurfaceView: SurfaceView) : SurfaceHolder.Callback {
    private val nativeJNI = NativeJNI()
    companion object {
        private val TAG = this::class.java.name.replace("${'$'}Companion", "").split(".").last()
    }

    init {
        mSurfaceView.holder.addCallback(this)
    }

    fun open(videoPath: String) {
        nativeJNI.startPlay(videoPath,mSurfaceView.holder.surface)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        Log.d(TAG, "surfaceChanged: ")
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        Log.d(TAG, "surfaceDestroyed: ")
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        Log.d(TAG, "surfaceCreated: ")
    }

}