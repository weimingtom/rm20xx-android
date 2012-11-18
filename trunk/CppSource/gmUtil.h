#ifndef GM_UTIL_H
#define GM_UTIL_H

#include <math.h>
#include <stdlib.h>
#include "Vector2D.h"
#include <android/log.h>


class GMUtil{
public:

	static const int SCREEN_WIDTH=800;
	static const int SCREEN_HEIGHT=480;

	static const float M11=0.0;
	static const float M12=1.0;
	static const float M13=0.0;
	static const float M14=0.0;
	static const float M21=-0.5;
	static const float M22=0.0;
	static const float M23=0.5;
	static const float M24=0.0;
	static const float M31=1.0;
	static const float M32=-2.5;
	static const float M33=2.0;
	static const float M34=-0.5;
	static const float M41=-0.5;
	static const float M42=1.5;
	static const float M43=-1.5;
	static const float M44=0.5;

	static void checkGlError(const char* op) {
		//for (GLint error = glGetError(); error; error= glGetError()) {
		//		//__android_log_print(ANDROID_LOG_INFO, "glError", "after %s() glError (0x%x)\n", op, error);
		//}
		//__android_log_print(ANDROID_LOG_INFO, "glError", "%s\n", op);
	}

	static Vector2D<float> motion_set(float angle,float speed){
		Vector2D<float> position;
		//__android_log_print(ANDROID_LOG_INFO, "*******", "before %f %f", position.x, position.y);
		position.x=cos(angle)*speed;
		position.y=sin(angle)*speed;

		//__android_log_print(ANDROID_LOG_INFO, "*******", "after %f %f", position.x, position.y);
		return position;
	}

	static float point_direction(float x1,float y1, float x2,float y2){
		return atan2(y2-y1,x2-x1);
	}

	static Vector2D<float> random_place(){
		Vector2D<float> position;
		int place=floor(rand()%4+1);

		//__android_log_print(ANDROID_LOG_INFO, "*******", "place %d", place);

		if(place==1){
			position.x=-32;
			position.y=rand()%SCREEN_HEIGHT;
		}

		if(place==2){
			position.y=-32;
			position.x=rand()%SCREEN_WIDTH;
		}

		if(place==3){
			position.x=SCREEN_WIDTH+32;
			position.y=rand()%SCREEN_HEIGHT;
		}

		if(place==4){
			position.x=rand()%SCREEN_WIDTH;
			position.y=SCREEN_HEIGHT+32;
		}

		//__android_log_print(ANDROID_LOG_INFO, "*******", "place %f %f", position.x, position.y);

		return position;
	}

	static Vector2D<float> catmullRomSpline(float x, Vector2D<float> v0,Vector2D<float> v1,
		Vector2D<float> v2,Vector2D<float> v3) {
			Vector2D<float> c1,c2,c3,c4;
			c1 = v1*M12;
			c2 = v0*M21 + v2*M23;
			c3 = v0*M31 + v1*M32 + v2*M33 + v3*M34;
			c4 = v0*M41 + v1*M42 + v2*M43 + v3*M44;
			return(((c4*x + c3)*x +c2)*x + c1);
	}

	static bool circlesColliding(int x1,int y1,int radius1, int x2,int y2, int radius2)
	{
		//compare the distance to combined radii
		int dx = x2 - x1;
		int dy = y2 - y1;
		int radii = radius1 + radius2;
		if ( ( dx * dx )  + ( dy * dy ) < radii * radii )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};

#endif