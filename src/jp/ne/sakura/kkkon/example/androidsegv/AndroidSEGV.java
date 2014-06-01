/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package jp.ne.sakura.kkkon.example.androidsegv;

import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.os.Bundle;


public class AndroidSEGV extends Activity implements OnClickListener
{
    private Button btn1;
    private Button btn2;
    private Button btn3;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        LinearLayout layout = new LinearLayout( this );
        layout.setOrientation( LinearLayout.VERTICAL );

        LayoutParams layoutParams = new LayoutParams( LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT );

        TextView  tv = new TextView(this);
        tv.setText( stringFromJNI() );
        layout.addView( tv );

        this.btn1 = new Button(this);
        this.btn1.setText( "invoke SEGV anonymous" );
        this.btn1.setOnClickListener( new OnClickListener() {
            @Override
            public void onClick( View v )
            {
                invokeSEGV();
            }
        });
        layout.addView( this.btn1 );

        this.btn2 = new Button(this);
        this.btn2.setText( "invoke SEGV" );
        this.btn2.setOnClickListener( this );
        layout.addView( this.btn2 );

        this.btn3 = new Button(this);
        this.btn3.setText( "invoke SEGV pthread" );
        this.btn3.setOnClickListener( this );
        layout.addView( this.btn3 );

        setContentView( layout );
    }

    @Override
    public void onClick(View v) {
        // TODO Auto-generated method stub
        if ( this.btn2 == v )
        {
            invokeSEGV();
        }
        else if ( this.btn3 == v )
        {
            invokeSEGV2();
        }
    }

    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    public native String  stringFromJNI();

    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    public native String  unimplementedStringFromJNI();

    public native void invokeSEGV();
    public native void invokeSEGV2();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("android-segv");
    }
}
