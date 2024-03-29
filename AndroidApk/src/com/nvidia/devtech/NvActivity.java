//----------------------------------------------------------------------------------
// File:            libs\src\com\nvidia\devtech\NvActivity.java
// Samples Version: Android NVIDIA samples 1.0 
// Email:           tegradev@nvidia.com
// Forum:           http://developer.nvidia.com/tegra/forums/tegra-forums/android-development
//
// Copyright 2010-2011 NVIDIA� Corporation 
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//----------------------------------------------------------------------------------
package com.nvidia.devtech;

import java.io.IOException;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;





/**
 *  The base class used by the NVIDIA samples.
 *  The intention of this code is to make it as simple as possible to
 *  port an existing C/C++ application using as little JAVA code as possible on Android.
 * 
 *  To accomplish this, a simple interface is defined for common operations that can be
 *  implemented immediately in C/C++ only using a very thin JAVA class to load the JNI
 *  library and define the function as being implemented in native code.
 *
 */
public abstract class NvActivity
    extends Activity 
    implements SensorEventListener
{
	protected String initArg = new String();
	
    protected Handler handler = null;

    protected boolean wantsMultitouch = true;
    protected float multiData[];
    protected static final int MULTI_DATA_STRIDE = 5;
    protected static final int MULTI_MAX_INPUTS = 10;


    //accelerometer related
    protected boolean wantsAccelerometer = false;
    protected SensorManager mSensorManager;
    // protected SensorManagerSimulator mSensorManager;
    protected int mSensorDelay = SensorManager.SENSOR_DELAY_GAME; //other options: SensorManager.SENSOR_DELAY_FASTEST, SensorManager.SENSOR_DELAY_NORMAL and SensorManager.SENSOR_DELAY_UI
    
    protected String apkFilePath = "";
    
    /**
     * Function called when the application wants to initialize the application.
     * 
     * @return True if initialization was successful.
     */
    public abstract boolean init(String apkPath);
    
    public abstract void setOffsets(int x, int y);
    
    public abstract void adsPanel();

    /**
     * Function called on input (motion) events
     * 
     * @param action The action of this event.
     * @param x The x coordinate of the event.
     * @param y The y coordinate of the event.
     * @param event The event object itself, in case further details are needed.
     * @return True if the event was handled.
     */
    public abstract boolean inputEvent(int action, float x, float y, MotionEvent event);
    public abstract boolean multiInputEvent(int action, int pointer,float x, float y, MotionEvent event);

    /**
     * Function called when app requested multitouch input (motion) events
     * 
     * Note this is not an abstract, so that users of standard inputEvent don't need to implement
     * a stub function -- we do so here already.
     * 
     * @param action The action of this event.
     * @param numInputs The number of active inputs.
     * @param data Array of touch fields for each of numInputs (e.g., x, y, size, pressure, id).
     * @param dataStride The number of sub-elements per input, for forward-compatibility.
     * @param event The event object itself, in case further details are needed.
     * @return True if the event was handled.
     */
    public boolean multitouchEvent(int action, int numInputs, float data[], int dataStride, MotionEvent event)
    {
    	return true;
    }


    /**
     * Function called on key events
     * 
     * @param action The action of this event.
     * @param unicodeChar The unicode character represented by the entered key char.
     * If this is 0, further info will have to be extracted from the event object. 
     * @param event The event object if further details are needed.
     * @return True if the event was handled.
     */
    public abstract boolean keyEvent(int action, int unicodeChar, int keyCode, KeyEvent event);

    /**
     * Function called when app requested sensor input events like accelerometer. Right now it is only called for accelerometer event.
     * 
     * Note this is not an abstract, so that people implementing subclasses of NvActivity that
     * don't use Sensors don't need to implement a native stub -- we do so here in java.
     * 
     * @param sensorType: The sensor type this event.
     * @param values0: values[0] passed to onSensorChanged(). For accelerometer: Acceleration minus Gx on the x-axis.
     * @param values1: values[1] passed to onSensorChanged(). For accelerometer: Acceleration minus Gy on the y-axis.
     * @param values2: values[2] passed to onSensorChanged(). For accelerometer: Acceleration minus Gz on the z-axis.
     * @return True if the event was handled.
     */
    public abstract boolean sensorEvent(int sensorType, float values0, float values1, float values2);
    
    /**
     * Called when the application is exiting
     */
    public abstract void cleanup();

    /**
     * System initialization code. Kept separate from the {@link #init()} function so that subclasses
     * in their simplest form do not need to call any of the parent class' functions. This to make
     * it easier for pure C/C++ application so that these do not need to call java functions from C/C++
     * code.
     * 
     * @see #init()
     * @return True if initialization was successful.
     */
    protected boolean systemInit()
    {
    	
        return init(apkFilePath);
    }

    /**
     * Called when the Activity is exiting and it is time to cleanup.
     * Kept separate from the {@link #cleanup()} function so that subclasses
     * in their simplest form do not need to call any of the parent class' functions. This to make
     * it easier for pure C/C++ application so that these do not need to call java functions from C/C++
     * code.
     * 
     * @see #cleanup()
     */
    protected void systemCleanup()
    {
        cleanup();
    }

    /**
     * Declaration for function defined in nv_time/nv_time.cpp
     * It initializes and returns time through Nvidia's egl extension for time.
     * It is useful while debugging the demo using PerfHUD.
     * 
     * @see: nv_time/nv_time.cpp for implementation details.
     */
    //public native void nvAcquireTimeExtension();
    //public native long nvGetSystemTime();
   
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        handler = new Handler();
        multiData = new float[MULTI_DATA_STRIDE*MULTI_MAX_INPUTS]; 
        if(wantsAccelerometer && (mSensorManager == null)){
        	Log.v("crystalWave","creating SENSOR!");
            mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
//            mSensorManager = SensorManagerSimulator.getSystemService(this, SENSOR_SERVICE);
//            mSensorManager.connectSimulator();
        }

        NvUtil.getInstance().setActivity(this);
        systemInit();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        if(mSensorManager != null)
            mSensorManager.registerListener(this, mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), mSensorDelay);
    }
    
	@Override
	protected void onStop() {
        if(mSensorManager != null)
    	    mSensorManager.unregisterListener(this);
	    super.onStop();
	}

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        multiData = null;
        systemCleanup();
    }

	//	        		int j = i*MULTI_DATA_STRIDE;
	//	        		// put x and y FIRST, so if people just want that data, there's nothing else
	//	        		// to jump over...
	//	        		multiData[j + 0] = (float)event.getX(i);
	//	        		multiData[j + 1] = (float)event.getY(i);
	//	        		multiData[j + 2] = (float)event.getPointerId(i);
	//	        		multiData[j + 3] = (float)event.getSize(i);
	//	        		multiData[j + 4] = (float)event.getPressure(i);
    
    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
    	
    	boolean ret = super.onTouchEvent(event);
    	if (!ret)
    	{
    		//dumpEvent(event);
    		if (wantsMultitouch)
    		{
    			int action = event.getAction();
    	    	int actionCode = action & MotionEvent.ACTION_MASK;
    	    	int id=-1;
    			
    			// marshal up the data.
    	    	if (actionCode == MotionEvent.ACTION_POINTER_DOWN
		    			|| actionCode == MotionEvent.ACTION_POINTER_UP) {
					id=action >> MotionEvent.ACTION_POINTER_ID_SHIFT;
					ret=multiInputEvent(actionCode,id, event.getX(id), event.getY(id), event);
    	    	}else{    	    	
    	    		int numEvents = event.getPointerCount();
    	    		for (int i=0; i<numEvents; i++)
    	    		{
      					id=event.getPointerId(i);
    	    			ret=multiInputEvent(actionCode,id, event.getX(i), event.getY(i), event);
    				}
    			}
    		}
    		else // old style input.
    		{
    			ret=inputEvent(event.getAction(), event.getX(), event.getY(), event);
    		}
    	}
    	return ret;

    }
    
    private void dumpEvent(MotionEvent event) {
    	String names[] = { "DOWN" , "UP" , "MOVE" , "CANCEL" , "OUTSIDE" ,
    			"POINTER_DOWN" , "POINTER_UP" , "7?" , "8?" , "9?" };
    	StringBuilder sb = new StringBuilder();
    	int action = event.getAction();
    	int actionCode = action & MotionEvent.ACTION_MASK;
    	sb.append("event ACTION_" ).append(names[actionCode]);
    	if (actionCode == MotionEvent.ACTION_POINTER_DOWN
    			|| actionCode == MotionEvent.ACTION_POINTER_UP) {
    		sb.append("(pid " ).append(
    				action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
    		sb.append(")" );
    	}
    	sb.append("[" );
    	for (int i = 0; i < event.getPointerCount(); i++) {
    		sb.append("#" ).append(i);
    		sb.append("(pid " ).append(event.getPointerId(i));
    		sb.append(")=" ).append((int) event.getX(i));
    		sb.append("," ).append((int) event.getY(i));
    		if (i + 1 < event.getPointerCount())
    			sb.append(";" );
    	}
    	sb.append("]" );
    	Log.d("input", sb.toString());
    }


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        boolean ret = super.onKeyDown(keyCode, event);
        if (!ret)
            ret = keyEvent(event.getAction(), event.getUnicodeChar(), event.getKeyCode(), event);
        return ret;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
        boolean ret = super.onKeyUp(keyCode, event);
        if (!ret)
            ret = keyEvent(event.getAction(), event.getUnicodeChar(), event.getKeyCode(), event);
        return ret;
    }

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// Auto-generated method stub
	}

	public void onSensorChanged(SensorEvent event) {
		// Auto-generated method stub
		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER){
			//System.out.println("event.values[0], event.values[1], event.values[2]: "+event.values[0] +","+ event.values[1] +","+ event.values[2]);
			sensorEvent(event.sensor.getType(), event.values[0], event.values[1], event.values[2]);
		}
		
//		int sensor = event.type;
//        float[] values = event.values;
//        if(sensor== Sensor.TYPE_ACCELEROMETER)
//        	sensorEvent(sensor, values[0], values[1], values[2]);
	}
	

}
