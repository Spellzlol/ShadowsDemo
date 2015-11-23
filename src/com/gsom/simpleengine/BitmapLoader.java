package com.gsom.simpleengine;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.InputStream;
import java.io.IOException;

public class BitmapLoader
{
	private AssetManager mAssetManager = null;

	public BitmapLoader(Context context)
	{
		mAssetManager = context.getAssets();
	}

	public Bitmap load(String filename)
	{
		Bitmap bmp = null;

		try
        {
			bmp = BitmapFactory.decodeStream(mAssetManager.open(filename));
        }
        catch (Exception e) 
        { 
        	e.printStackTrace();
        }

        return bmp;
	}

	public int getWidth(Bitmap bmp) 
	{
    	return bmp.getWidth(); 
    }
    
    public int getHeight(Bitmap bmp) 
    {
    	return bmp.getHeight(); 
    }

    public void getPixels(Bitmap bmp, int[] pixels)
    {
        int w = bmp.getWidth();
        int h = bmp.getHeight();
        bmp.getPixels(pixels, 0, w, 0, 0, w, h);
    }

    public void close(Bitmap bmp)
    {
        bmp.recycle();
    }

    public String readTextFile(String filename)
    {
        InputStream input;
        String text = "";
        try 
        {
            input = mAssetManager.open(filename);

            int size = input.available();
            byte[] buffer = new byte[size];
            input.read(buffer);
            input.close();

            // byte buffer into a string
            text = new String(buffer);

        } 
        catch (IOException e) 
        {
            e.printStackTrace();
        }

        text += '\n';

        return text;
    }
}