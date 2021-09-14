package com.iffly.render

import android.graphics.Bitmap

class Render {

    /**
     * A native method that is implemented by the 'render' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun line(x0: Int, y0: Int, x1: Int, y1: Int, color: Long, bitmap: Bitmap)

    companion object {
        // Used to load the 'render' library on application startup.
        init {
            System.loadLibrary("render")
        }
    }
}