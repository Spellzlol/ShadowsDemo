 package com.gsom.simpleengine;

import android.opengl.GLSurfaceView;
import android.content.Context;
import android.view.MotionEvent;
import android.util.Log;
import com.gsom.simpleengine.SimpleNativeWrapper;
import com.gsom.simpleengine.SimpleRenderer;

class SimpleSurfaceView extends GLSurfaceView 
{

    private SimpleRenderer mSimpleRenderer = null;

    public SimpleSurfaceView(Context context) 
    {
        super(context);
    }

    public void setSimpleRenderer(final SimpleRenderer renderer) 
    {
        mSimpleRenderer = renderer;
        setRenderer(mSimpleRenderer);
    }

    public boolean onTouchEvent(final MotionEvent event)
    {
        final int pointerCount = event.getPointerCount();

        switch (event.getActionMasked()) 
        {
            case MotionEvent.ACTION_POINTER_DOWN:
                {
                    int index = event.getActionIndex();
                    int id = event.getPointerId(index);
                    float x = event.getX(index);
                    float y = event.getY(index);

                    SimpleNativeWrapper.nativeOnTouchBegin(id, x, y); 
                }
                break;

            case MotionEvent.ACTION_DOWN:
                {
                    int id = event.getPointerId(0);
                    float x = event.getX();
                    float y = event.getY();

                    SimpleNativeWrapper.nativeOnTouchBegin(id, x, y); 
                }
                break;

            case MotionEvent.ACTION_MOVE:
                {
                    for (int index = 0; index < event.getPointerCount(); ++index)
                    {
                        int id = event.getPointerId(index);
                        float x = event.getX(index);
                        float y = event.getY(index);

                        SimpleNativeWrapper.nativeOnTouchMove(id, x, y);
                    }
                }
                break;

            case MotionEvent.ACTION_POINTER_UP:
                {
                    int index = event.getActionIndex();
                    int id = event.getPointerId(index);
                    float x = event.getX(index);
                    float y = event.getY(index);

                    SimpleNativeWrapper.nativeOnTouchEnded(id, x, y); 
                }
                break;

            case MotionEvent.ACTION_UP:
                {
                    int id = event.getPointerId(0);
                    float x = event.getX();
                    float y = event.getY();

                    SimpleNativeWrapper.nativeOnTouchEnded(id, x, y);
                }
                break;

            case MotionEvent.ACTION_CANCEL:
                {
                    for (int index = 0; index < event.getPointerCount(); ++index)
                    {
                        int id = event.getPointerId(index);
                        float x = event.getX(index);
                        float y = event.getY(index);

                        SimpleNativeWrapper.nativeOnTouchEnded(id, x, y);
                    }
                }
                break;
        }

        return true;
    }

    @Override
    protected void onSizeChanged(final int newWidth, final int newHeight, final int olgWidth, final int oldHeight)
    {
        if(!this.isInEditMode())
        {
            mSimpleRenderer.onResize(newWidth, newHeight);
        }
    }

   @Override
    public void onPause() 
    {
        super.onPause();
        SimpleNativeWrapper.nativePause();
    }

   @Override
    public void onResume() 
    {
        super.onResume();
        SimpleNativeWrapper.nativeResume();
    }
}
