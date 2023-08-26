package com.example.kosaf_farm;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        <!-- Auto layout, variables, and unit scale are not yet supported -->
<RelativeLayout
        xmlns:android="http://schemas.android.com/apk/res/android"
        android:id="@+id/main_page"
        android:layout_width="390dp"
        android:layout_height="844dp"
        android:clipToOutline="true"
        android:background="#FFFFFF" />
    }
}

