package com.gsom.simpleengine;

import com.gsom.simpleengine.BitmapLoader;

public class SimpleNativeWrapper {
    static {
        System.loadLibrary("SimpleEngine");
    }
    
    public static native void nativeCreate(BitmapLoader bitmapLoader);
    public static native void nativeInit(int w, int h);
    public static native void nativeResize(int w, int h);
    public static native void nativeRender();
    public static native void nativePause();
    public static native void nativeResume();
    public static native void nativeOnTouchBegin(int id, float x, float y);
    public static native void nativeOnTouchMove(int id, float x, float y);
    public static native void nativeOnTouchEnded(int id, float x, float y);
}
