package com.gsom.simpleengine;

import android.app.Activity;
import android.os.Bundle;

import com.gsom.simpleengine.SimpleSurfaceView;
import com.gsom.simpleengine.SimpleRenderer;
import com.gsom.simpleengine.SimpleNativeWrapper;
import com.gsom.simpleengine.BitmapLoader;

public class SimpleEngineActivity extends Activity
{
	private SimpleSurfaceView mView = null;
    private BitmapLoader mBitmapLoader = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);

        mView = new SimpleSurfaceView(this);
        mView.setEGLContextClientVersion(2);
        mView.setSimpleRenderer(new SimpleRenderer());
        setContentView(mView);

        mBitmapLoader = new BitmapLoader(getApplicationContext());

        SimpleNativeWrapper.nativeCreate(mBitmapLoader);
	}
	
    @Override
    protected void onPause() 
    {
        super.onPause();
        mView.onPause();
    }

    @Override
    protected void onResume() 
    {
        super.onResume();
        mView.onResume();
    }
}


