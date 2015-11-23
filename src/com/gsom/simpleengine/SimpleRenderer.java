package com.gsom.simpleengine;

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import com.gsom.simpleengine.SimpleNativeWrapper;

class SimpleRenderer implements GLSurfaceView.Renderer
{
    private int mWidth = 0;
    private int mHeight = 0;

    public void onResize(int newWidth, int newHeight)
    {
        mWidth = newWidth;
        mHeight = newHeight;
    }

    @Override
    public void onSurfaceCreated(final GL10 gl, final EGLConfig config)
    {
    	SimpleNativeWrapper.nativeInit(mWidth, mHeight);
    }

    @Override
    public void onSurfaceChanged(final GL10 gl, final int w, final int h) 
    {
    	SimpleNativeWrapper.nativeResize(w, h);
    }

    @Override
    public void onDrawFrame(final GL10 gl) 
    {
    	SimpleNativeWrapper.nativeRender();
    }
}