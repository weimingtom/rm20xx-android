package br.gjteam.jni;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.opengl.GLU;
import android.opengl.GLUtils;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

import com.nvidia.devtech.NvGLESActivity;

//public class Main extends NvGLESActivity implements AdListener {
public class Main extends NvGLESActivity {

	private static Context context; 
	public MediaPlayer _music = null;
	public static Main app = null;
	public SoundPool _sounds;
	//private static ScoreNinjaAdapter scoreNinjaAdapter;
	//private AdView adView;
	private static boolean silentMode=false;
	private static BitmapFactory.Options sBitmapOptions  = new BitmapFactory.Options();
	private static HashMap<String, Integer> textures_=new HashMap<String, Integer>();
	
	private static int width=0;
	private static int height=0;
	
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		context=this;
		app=this;
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
		
		_sounds = new SoundPool(8,AudioManager.STREAM_MUSIC,0);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);  
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,   
				WindowManager.LayoutParams.FLAG_FULLSCREEN);  
		super.onCreate(savedInstanceState);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		//scoreNinjaAdapter = new ScoreNinjaAdapter(this, "crystalwave", "064DF3363A1B682862E1FA42C7B588C5");
		
		AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
		switch (am.getRingerMode()) {
		    case AudioManager.RINGER_MODE_SILENT:
		        Log.i("MyApp","Silent mode");
		        silentMode=true;
		        break;
		    case AudioManager.RINGER_MODE_VIBRATE:
		        Log.i("MyApp","Vibrate mode");
		        silentMode=true;
		        break;
		    case AudioManager.RINGER_MODE_NORMAL:
		        Log.i("MyApp","Normal mode");
		        silentMode=false;
		        break;
		}
		

	}

	public void adsPanel()
	{
		Log.v("adMOB","adsPanel");
		//adView = new AdView(this,AdSize.BANNER, "a14ea405e96996c");
		RelativeLayout.LayoutParams params= new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT);
		params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		params.addRule(RelativeLayout.CENTER_HORIZONTAL);
		//adView.setLayoutParams(params);

		RelativeLayout rl = new RelativeLayout(this);
		rl.addView(surfaceView);
		//rl.addView(adView);

		setContentView(rl);
	     
		//AdRequest request = new AdRequest();
		//request.setTesting(true); 
		//adView.loadAd(request);
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		int screenHeight = this.getWindowManager().getDefaultDisplay().getHeight();
		int viewHeight = surfaceView.getHeight();

		// Use the difference as the cursor offset
		//setOffsets(0, viewHeight - screenHeight);

		super.onWindowFocusChanged(hasFocus);
	}
	
	public static int getWidth() {
		return width;
	}
	
	public static int getHeight() {
		return height;
	}

	public static int load_texture(String fname) {
		Log.v("adMOB","loading: "+fname);
		
		AssetManager am = context.getAssets(); //new android.content.res.getAssets();
		if(textures_.containsKey(fname))
			return textures_.get(fname);
		
		try {
			InputStream stream = am.open(fname);
			//Bitmap bitmap = BitmapFactory.decodeStream(stream,null,sBitmapOptions);
			Bitmap bitmap = BitmapFactory.decodeStream(stream);
			width=bitmap.getWidth();
			height=bitmap.getHeight();
			int[] textures = new int[1];
			gl.glGenTextures(1, textures, 0);
			int error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
                Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }
			int textureID = textures[0];
			gl.glBindTexture(GL10.GL_TEXTURE_2D, textureID);
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }

			// no mipmaps
			//            gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
			//            gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);

			gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_S, GL10.GL_REPEAT );
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }
			gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_T, GL10.GL_REPEAT );
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }
			gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_NEAREST );
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }
			gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_NEAREST );
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }
			gl.glTexEnvf( GL10.GL_TEXTURE_ENV,GL10. GL_TEXTURE_ENV_MODE, GL10.GL_MODULATE );
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }

           	GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
            
            
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }
			stream.close();
			bitmap.recycle();
			error = gl.glGetError();
            if (error != GL10.GL_NO_ERROR) {
            	Log.d("glError", "GLError: " + error + " (" + GLU.gluErrorString(error) + ")");
            }
			textures_.put(fname, textureID);
			Log.v("adMOB","textureID: "+textureID);
			return textureID;
		} catch(IOException e) {
			//Log.v("adMOB","textureID: 0");
			e.printStackTrace();
			//          Log.v("load_texture:open() fail");
			return 0;
		}
		
	}

	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		textures_.clear();
		Log.v("onDestroy","onDestroy");
		music_stop();
		//cleanup();
		//adView.destroy();
		

	}

	// Native

	@Override
	public native boolean render(int drawWidth, int drawHeight, boolean forceRedraw);

	public native void shownScore();


	@Override
	public native void cleanup();

	@Override
	public native boolean init(String apkPath);

	@Override
	public native boolean inputEvent(int action, float x, float y, MotionEvent event);
	
	@Override
	public native boolean multiInputEvent(int action, int id, float x, float y, MotionEvent event);
	

	@Override
	public native boolean keyEvent(int action, int unicodeChar, int keyCode, KeyEvent event);

	@Override
	public native boolean sensorEvent(int sensorType, float values0, float values1, float values2);

	@Override
	public native void setOffsets(int x, int y);

	static
	{	
		System.loadLibrary("wcrm20xx");
	}

	public static void music_play(String fname) {
		if(silentMode)
			return;
		Log.v("music_play",fname);
		
		AudioManager audioManager = (AudioManager) context.getSystemService(AUDIO_SERVICE);
		float actualVolume = (float) audioManager
				.getStreamVolume(AudioManager.STREAM_MUSIC);
		float maxVolume = (float) audioManager
				.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		float volume = actualVolume / maxVolume;
		
		AssetManager am = app.getAssets();
		try {
			AssetFileDescriptor fd = am.openFd(fname);
			app._music = new MediaPlayer();
			app._music.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());
			fd.close();
			app._music.setLooping(true);
			app._music.prepare();
			app._music.setVolume(volume, volume);
			app._music.start();
		} catch(IOException e) { }
	}
	public static void music_stop() {
		if (app._music == null) { return; }
		app._music.stop();
	}
	public static void music_volume(float v) {
		if (app._music == null) { return; }
		app._music.setVolume(v,v);
	}

	public static int sound_load(String fname) {
		AssetManager am = app.getAssets();
		try {
			AssetFileDescriptor fd = am.openFd(fname);
			int sid = app._sounds.load(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength(),1);
			return sid;
		} catch(IOException e) { }
		return 0;
	}
	public static void sound_play(int sid) {
		if(silentMode)
			return;
		
		AudioManager audioManager = (AudioManager) context.getSystemService(AUDIO_SERVICE);
		float actualVolume = (float) audioManager
				.getStreamVolume(AudioManager.STREAM_MUSIC);
		float maxVolume = (float) audioManager
				.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		float volume = actualVolume / maxVolume;
		
		app._sounds.play(sid,volume,volume,0,0,(float)1.0);
	}
	
	public static void readLog() {
		/*Log.v("adMOB","readLog called");
		byte[] buffer=null;
		FileInputStream fos;
		try {
			fos = context.openFileInput("wcrpg.txt");
			fos.read(buffer);
			fos.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println(buffer);*/

	}

	public static void show_score(int score) {
		Log.v("adMOB",""+score);
		//scoreNinjaAdapter.show(score);
	}

	@Override
	protected void onActivityResult(
			int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Log.v("adMOB","onActivityResult "+requestCode+","+resultCode);
		shownScore();
		//scoreNinjaAdapter.onActivityResult(requestCode, resultCode, data);
	}

//	@Override
//	public void onDismissScreen(Ad arg0) {
//		Log.v("adMOB","onDismissScreen");
//		
//	}
//
//	@Override
//	public void onFailedToReceiveAd(Ad arg0, ErrorCode arg1) {
//		Log.v("adMOB","onFailedToReceiveAd");
//		
//	}
//
//	@Override
//	public void onLeaveApplication(Ad arg0) {
//		Log.v("adMOB","onLeaveApplication");
//		
//	}
//
//	@Override
//	public void onPresentScreen(Ad arg0) {
//		Log.v("adMOB","onPresentScreen");
//		
//	}
//
//	@Override
//	public void onReceiveAd(Ad arg0) {
//		Log.v("adMOB","onReceiveAd");
//		
//	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
	    super.onConfigurationChanged(newConfig);
	    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	}

}

