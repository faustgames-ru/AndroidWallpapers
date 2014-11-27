package com.faustgames.forest;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import android.app.WallpaperManager;
import android.content.ActivityNotFoundException;
import android.content.ComponentName;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.Log;
//import ;

public class forest extends Activity
{
	/*class CustomGLSurfaceView extends GLSurfaceView {

		public CustomGLSurfaceView(Context context){
			super(context);

			// Set the Renderer for drawing on the GLSurfaceView
			setEGLContextClientVersion(2);
			setRenderer(new CustomRenderer());			
			setRenderMode(RENDERMODE_CONTINUOUSLY);
		}
	}

	public class CustomRenderer implements GLSurfaceView.Renderer {

		public void onSurfaceCreated(javax.microedition.khronos.opengles.GL10 unused, javax.microedition.khronos.egl.EGLConfig config) {
			forest.this._ni.onSurfaceCreated();
		}

		public void onDrawFrame(javax.microedition.khronos.opengles.GL10 unused) {
			forest.this._ni.onDrawFrame();
		}

		public void onSurfaceChanged(javax.microedition.khronos.opengles.GL10 unused, int width, int height) {
			forest.this._ni.onSurfaceChanged(width, height);
		}
	}*/

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		Intent intent = new Intent("android.intent.action.ACTION_LIVE_WALLPAPER_CHOOSER");
		if(Build.VERSION.SDK_INT >= 16)
		{
			intent.setAction("android.service.wallpaper.CHANGE_LIVE_WALLPAPER");
			intent.putExtra("android.service.wallpaper.extra.LIVE_WALLPAPER_COMPONENT", new ComponentName(getPackageName(), getPackageName() + ".WPService"));
		}
		else
		{ 
			intent.setAction(WallpaperManager.ACTION_LIVE_WALLPAPER_CHOOSER);
		}

		try {
			startActivity(intent);
		} catch (ActivityNotFoundException e) {
			Log.wtf("Engine", "No activity found to handle " + intent.toString());
		}
		super.finish();


		/*_ni.initAssetManager(getResources().getAssets());
		mGLView = new CustomGLSurfaceView(this);
        setContentView(mGLView);*/
	}

	//private GLSurfaceView mGLView;
	//private NativeInterface _ni = new NativeInterface();
	/*
	<service android:name=".WPService" android:configChanges="orientation" android:icon="@drawable/icon" android:label="@string/wallpaper" android:permission="android.permission.BIND_WALLPAPER" android:screenOrientation="portrait">
        - <intent-filter>
          <action android:name="android.service.wallpaper.WallpaperService" />
        </intent-filter>
        <meta-data android:name="android.service.wallpaper" android:resource="@xml/wallpaper" />
      </service>
      - <activity android:name=".Settings" android:configChanges="orientation" android:exported="true" android:label="@string/settings" android:screenOrientation="portrait">
        - <intent-filter>
          <category android:name="android.intent.category.LAUNCHER" />
          <category android:name="android.intent.category.PREFERENCE" />
        </intent-filter>
      </activity>
	*/
}