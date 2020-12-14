package com.example.nativwapp

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.nativwapp.util.EncryptUtils
import kotlinx.android.synthetic.main.activity_main.*
import java.util.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        val params = TreeMap<String, String>()
        params["api_version"] = "1.0"
        params["api_id"] = "999999"
        params["oid"] = "352575071991660"
        params["source"] = "android"
        params["timestamp"] = "1477892707861"
        params["verifymethod"] = "md5"
        params["version"] = "v1.3.6"
        sample_text.text = signParams(params, "abc")
        sample_text_java.text = EncryptUtils.signByMd5("abc",params);
    }

    external fun signParams(
        params: Map<String, String>,
        secret: String
    ): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
