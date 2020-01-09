package com.hujun.ffmpeg

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private var hjPlayer:HJPlayer?=null
    private val videoPath = "/sdcard/test.mp4"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        hjPlayer = HJPlayer(surfaceview)
        btn_open.setOnClickListener{hjPlayer?.open(videoPath)}
    }

}
