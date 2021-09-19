package com.iffly.render

import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Matrix
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.iffly.render.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = "render"
        Thread {
            val render: Render = Render(600, 600)
            render.lock()
//        render.line(10, 10, 100, 200, 0xffff0000)
//        render.line(10, 10, 200, 100, 0xff0000ff)
//        render.line(10, 10, 10, 100, 0xff00ffff)
//        render.line(10, 10, 100, 10, 0xffff00ff)
//        render.triangle(10, 10, 30, 10, 30, 50, 0xffff0000)
            render.renderObject(assetManager = assets, fileName = "african_head.obj")
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


}