package com.example.xunda.lab1_audio;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import android.view.View;



public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    Button pb;

    public native void doProcessing();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        pb = (Button) findViewById(R.id.process_button);
        pb.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                doProcessing();
                Intent finishIntent = new Intent(MainActivity.this, FinishActivity.class);
                startActivity(finishIntent);
            }
        });

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
