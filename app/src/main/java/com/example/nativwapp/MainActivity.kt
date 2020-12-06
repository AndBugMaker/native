package com.example.nativwapp

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        val params = LinkedHashMap<String, Int>()
        params["Physics"] = 80
        params["Biology"] = 85
        params["Math"] = 90
        params["Chinese"] = 70
        sample_text.text = signParams(params, "abc")
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun signParams(
        params: LinkedHashMap<String, Int>,
        secret: String
    ): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
