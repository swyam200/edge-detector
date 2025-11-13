package com.example.edgedetector

import android.Manifest
import android.app.Activity
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.graphics.ImageFormat
import android.media.ImageReader
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {
    companion object {
        init { System.loadLibrary("native-lib") }
        private const val TAG = "MainActivity"
    }

    private external fun nativeInit()
    private external fun nativeProcessFrame(data: ByteArray, width: Int, height: Int)
    private external fun nativeToggleMode()

    // Simple permission request and placeholders -- full Camera2 plumbing is lengthy;
    // For a real submission wire Camera2 with ImageReader to call nativeProcessFrame.
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        if (!allPermissionsGranted()) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.CAMERA), 1001)
        } else {
            startCameraPlaceholder()
        }

        nativeInit()
        findViewById<Button>(R.id.btn_toggle).setOnClickListener {
            nativeToggleMode()
        }
    }

    private fun allPermissionsGranted(): Boolean {
        return ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED
    }

    private fun startCameraPlaceholder() {
        // Placeholder: in real app, set up Camera2 + ImageReader and call onImageAvailable()
        Log.i(TAG, "startCameraPlaceholder called. Replace with Camera2 ImageReader pipeline.")
    }

    // Example callback invoked by native code to deliver processed frame bytes.
    // Signature matches JNI callback used in native code below.
    fun onProcessedFrameNative(bytes: ByteArray, width: Int, height: Int) {
        // Convert bytes (RGBA) -> Bitmap and display or forward to GL renderer.
        try {
            val bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
            val buffer = java.nio.ByteBuffer.wrap(bytes)
            bmp.copyPixelsFromBuffer(buffer)
            // TODO: queue bitmap to GL renderer. For this skeleton we just log.
            Log.i(TAG, "Received processed frame: ${width}x${height} size=${bytes.size}")
        } catch (e: Exception) {
            Log.e(TAG, "onProcessedFrameNative error: ${e.message}")
        }
    }
