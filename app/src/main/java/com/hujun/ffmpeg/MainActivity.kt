package com.hujun.ffmpeg

import android.annotation.SuppressLint
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private var hjPlayer:HJPlayer?=null
    private val videoPath = "/sdcard/test.mp4"

    private val permissions = arrayOf(
        android.Manifest.permission.READ_EXTERNAL_STORAGE
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        checkPermissions()

        hjPlayer = HJPlayer(surfaceview)
        btn_open.setOnClickListener{hjPlayer?.open(videoPath)}
    }

    @SuppressLint("NewApi")
    private fun checkPermissions() {
        if(checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)!=PackageManager.PERMISSION_GRANTED){
            requestPermissions(permissions,1)
        }
    }

}
