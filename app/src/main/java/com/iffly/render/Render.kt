package com.iffly.render

import android.graphics.Bitmap

class Render(val width: Int, val height: Int) {

    /**
     * A native method that is implemented by the 'render' native library,
     * which is packaged with this application.
     */
    val bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
    val nativeAddr: Long = init(bitmap)


    fun line(x0: Int, y0: Int, x1: Int, y1: Int, color: Long) {
        line(nativeAddr, x0, y0, x1, y1, color)
    }

    fun updateCamera(dir: Int) {
        updateCamera(nativeAddr, dir)
    }

    fun updateCameraYawPitch(
        dx: Float,
        dy: Float,
        constrainPitch: Boolean = true
    ) {
        updateCameraYawPitch(nativeAddr, dx, dy, constrainPitch)
    }

    fun triangle(
        x0: Int,
        y0: Int,
        x1: Int,
        y1: Int,
        x2: Int,
        y2: Int,
        color: Long
    ) {
        triangle(nativeAddr, x0, y0, x1, y1, x2, y2, color)
    }

    fun renderObject() {
        renderObject(nativeAddr)
    }

    fun loadModel(fileName: String) {
        loadModel(nativeAddr, fileName)
    }

    fun lock() {
        lock(nativeAddr)
    }

    fun unlock() {
        unlock(nativeAddr)
    }

    fun clear() {
        clear(nativeAddr)
    }

    fun destroy() {
        destroy(nativeAddr)
    }


    external fun stringFromJNI(): String
    external fun line(nativeAddr: Long, x0: Int, y0: Int, x1: Int, y1: Int, color: Long)
    external fun triangle(
        nativeAddr: Long,
        x0: Int,
        y0: Int,
        x1: Int,
        y1: Int,
        x2: Int,
        y2: Int,
        color: Long
    )

    external fun loadModel(
        nativeAddr: Long,
        fileName: String
    )

    external fun renderObject(
        nativeAddr: Long
    )

    external fun lock(nativeAddr: Long)
    external fun unlock(nativeAddr: Long)

    external fun init(bitmap: Bitmap): Long

    external fun destroy(nativeAddr: Long)

    external fun updateCamera(nativeAddr: Long, dir: Int)

    external fun updateCameraYawPitch(
        nativeAddr: Long,
        dx: Float,
        dy: Float,
        constrainPitch: Boolean
    )

    external fun clear(nativeAddr: Long)


    companion object {
        // Used to load the 'render' library on application startup.
        init {
            System.loadLibrary("render")
        }
    }
}