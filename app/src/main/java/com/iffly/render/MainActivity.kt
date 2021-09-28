package com.iffly.render

import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Matrix
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.iffly.render.databinding.ActivityMainBinding
import java.io.File
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = "render"
        Thread {
            val file = File(externalCacheDir, "african_head.obj")
            if (!file.exists()) {
                copyAssetsToCache("african_head.obj", file)
            }
            val textureFile = File(externalCacheDir, "african_head_diffuse.tga")
            if (!textureFile.exists()) {
                copyAssetsToCache("african_head_diffuse.tga", textureFile)
            }
            val render: Render = Render(600, 600)
            render.lock()
//        render.line(10, 10, 100, 200, 0xffff0000)
//        render.line(10, 10, 200, 100, 0xff0000ff)
//        render.line(10, 10, 10, 100, 0xff00ffff)
//        render.line(10, 10, 100, 10, 0xffff00ff)
//        render.triangle(10, 10, 30, 10, 30, 50, 0xffff0000)
            render.loadModel(fileName = file.absolutePath)
            val startTime = System.currentTimeMillis()
            render.renderObject()
            val delta = System.currentTimeMillis() - startTime
            Log.i("myyf", "time:$delta")
            render.unlock()
            val bitmap = Bitmap.createBitmap(
                render.bitmap.width,
                render.bitmap.height,
                Bitmap.Config.ARGB_8888
            )
            val canvas = Canvas(bitmap)
            val matrix = Matrix()

            matrix.postRotate(180f, bitmap.width / 2f, bitmap.height / 2f)
            canvas.drawBitmap(render.bitmap, matrix, null)
            binding.imageView.post {
                binding.imageView.setImageBitmap(bitmap)
            }
            render.destroy()

        }.start()

    }

    override fun onDestroy() {

        super.onDestroy()
    }

    fun copyAssetsToCache(fileName: String, file: File) {

        val input = assets.open(fileName)
        val output = FileOutputStream(file)
        val buffer = ByteArray(1024)
        var byteCount = 0
        while ((input.read(buffer).also { byteCount = it }) != -1) {// 循环从输入流读取
            // buffer字节
            output.write(buffer, 0, byteCount)// 将读取的输入流写入到输出流
        }
        output.flush();// 刷新缓冲区
        input.close()
        output.close()

    }


}