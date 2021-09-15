package com.iffly.render

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
        binding.sampleText.text = "123"

        val render = Render(200, 200)
        render.lock()
        render.line(10, 10, 100, 200, 0xffff0000)
        render.line(10, 10, 200, 100, 0xff0000ff)
        render.line(10, 10, 10, 100, 0xff00ffff)
        render.line(10, 10, 100, 10, 0xffff00ff)
        render.unlock()

        binding.imageView.setImageBitmap(render.bitmap)
        render.destroy()
    }


}