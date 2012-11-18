#ifndef JNI_UTIL_H
#define JNI_UTIL_H

#include <jni.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <math.h>

#include "external/zlib/zip.h"

#include <string>

#include <zlib.h>

#include <android/log.h>

extern zip* APKArchive;

struct zip_error {
    int zip_err;	/* libzip error code (ZIP_ER_*) */
    int sys_err;	/* copy of errno (E*) or zlib error code */
    char *str;		/* string representation or NULL */
};

struct zip {
    char *zn;			/* file name */
    FILE *zp;			/* file */
    struct zip_error error;	/* error information */

    unsigned int flags;		/* archive global flags */
    unsigned int ch_flags;	/* changed archive global flags */

    struct zip_cdir *cdir;	/* central directory */
    char *ch_comment;		/* changed archive comment */
    int ch_comment_len;		/* length of changed zip archive
				 * comment, -1 if unchanged */
    int nentry;			/* number of entries */
    int nentry_alloc;		/* number of entries allocated */
    struct zip_entry *entry;	/* entries */
    int nfile;			/* number of opened files within archive */
    int nfile_alloc;		/* number of files allocated */
    struct zip_file **file;	/* opened files within archive */
};

/* file in zip archive, part of API */

struct zip_file {
    struct zip *za;		/* zip archive containing this file */
    struct zip_error error;	/* error information */
    int flags;			/* -1: eof, >0: error */

    int method;			/* compression method */
    off_t fpos;			/* position within zip file (fread/fwrite) */
    unsigned long bytes_left;	/* number of bytes left to read */
    unsigned long cbytes_left;  /* number of bytes of compressed data left */
    
    unsigned long crc;		/* CRC so far */
    unsigned long crc_orig;	/* CRC recorded in archive */
    
    char *buffer;
    z_stream *zstr;
};

class JNIUtil{
public:
	static JNIEnv* _my_jnienv;

	static int jni_load_texture(const char *s) {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,
			"load_texture",
			"(Ljava/lang/String;)I");
		// there could be some exception handling happening here, but there isn't
		jint ret;
		jstring mystr = env->NewStringUTF(s);
		ret = env->CallStaticIntMethod(cls, mid, mystr);
		env->DeleteLocalRef(mystr);
		return ret;
	}

	static int jni_getWidth() {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,"getWidth","()I");
		// there could be some exception handling happening here, but there isn't
		jint ret;
		ret = env->CallStaticIntMethod(cls, mid);
		return ret;
	}

	static int jni_getHeight() {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,"getHeight","()I");
		// there could be some exception handling happening here, but there isn't
		jint ret;
		ret = env->CallStaticIntMethod(cls, mid);
		return ret;
	}
	static void jni_music_play(const char *s) {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,
			"music_play",
			"(Ljava/lang/String;)V");
		// there could be some exception handling happening here, but there isn't
		jstring mystr = env->NewStringUTF(s);
		env->CallStaticVoidMethod(cls, mid, mystr);
		env->DeleteLocalRef(mystr);
	}

	static int jni_sound_load(const char *s) {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,"sound_load","(Ljava/lang/String;)I");
		// there could be some exception handling happening here, but there isn't
		jstring mystr = env->NewStringUTF(s);
		return env->CallStaticIntMethod(cls, mid, mystr);
		env->DeleteLocalRef(mystr);
	}

	static void jni_readLog() {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,"readLog","()V");
		env->CallStaticVoidMethod(cls, mid);
	}

	static void jni_sound_play(jint id) {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,"sound_play","(I)V");
		env->CallStaticVoidMethod(cls, mid, id);
	}

	static void jni_show_score(jint id) {
		JNIEnv* env = _my_jnienv;
		jclass cls = env->FindClass("br/gjteam/jni/Main");
		jmethodID mid = env->GetStaticMethodID(cls,"show_score","(I)V");
		env->CallStaticVoidMethod(cls, mid, id);
	}

	/*static FILE* openFile(const char* szFilename){
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "opening  %s",szFilename);

		struct zip_file* file = zip_fopen(APKArchive, szFilename, 0);
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "opened %s",szFilename);
		return file->za->zp;		
	}*/

	//void openFile(const char* szFilename){
	//	std::string shader;
	//	char tmp[8092];
	//	size_t read = 0;

	//	struct zip_file* file = zip_fopen(APKArchive, szFilename, 0);

	//	read = zip_fread(file, tmp, sizeof(tmp));
	//	while (read > 0)
	//	{
	//		shader.append(tmp, read);
	//		read = zip_fread(file, tmp, sizeof(tmp));
	//	}
	//}

	static void openFile(const char* szFilename,void* buffer,int size){
		//std::string shader;
		size_t read = 0;

		struct zip_file* file = zip_fopen(APKArchive, szFilename, 0);

		read = zip_fread(file, buffer, size);
		while (read > 0)
		{
			//shader.append((char*)buffer, read);
			read = zip_fread(file, buffer, size);
		}

		//std::pair<ShaderMap::iterator, bool> result = _shaders.insert(std::pair<std::string, std::string>(szFilename, shader));
		//if (!result.second) {
		//	result.first->second = shader;
		//}

	}




};

#endif