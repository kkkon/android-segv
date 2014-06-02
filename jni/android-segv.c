/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>

#if defined(ANDROID)
#include <android/log.h>
#endif // defined(ANDROID)

#if defined(LOG_TAG)
#undef LOG_TAG
#endif // defined(LOG_TAG)

#define LOG_TAG         "android-segv"

#if defined(ANDROID)
#if defined(NDEBUG)
#define LOGD(...)       
#define LOGI(...)       __android_log_print(ANDROID_LOG_INFO ,LOG_TAG,__VA_ARGS__)
#define LOGE(...)       __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else // defined(NDEBUG)
#define LOGD(...)       __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...)       __android_log_print(ANDROID_LOG_INFO ,LOG_TAG,__VA_ARGS__)
#define LOGE(...)       __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif // defined(NDEBUG)
#else // defined(ANDROID)
#define LOGD(...)       
#define LOGI(...)       
#define LOGE(...)       
#endif // defined(ANDROID)


static
void invokeSEGV(void)
{
    char * p = NULL;
    *p = 'a';

    return;
}

/* This is a trivial JNI example where we use a native method
 * to return a new VM String.
 */
jstring
Java_jp_ne_sakura_kkkon_example_androidsegv_AndroidSEGV_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #define ABI "armeabi-v7a/NEON"
    #else
      #define ABI "armeabi-v7a"
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__mips__)
   #define ABI "mips"
#else
   #define ABI "unknown"
#endif

    return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
}

void
Java_jp_ne_sakura_kkkon_example_androidsegv_AndroidSEGV_invokeSEGV
(
    JNIEnv* env,
    jobject thiz
)
{
    invokeSEGV();
}


#include <pthread.h>

static
void * thread_invoke( void * pArg )
{
    invokeSEGV();

    return NULL;
}

void
Java_jp_ne_sakura_kkkon_example_androidsegv_AndroidSEGV_invokeSEGV2
(
    JNIEnv* env,
    jobject thiz
)
{
    pthread_t thread = -1;
    pthread_create( &thread, NULL, thread_invoke, NULL );
    {
        void * result = NULL;
        pthread_join( thread, &result );
    }
}


static
void
sigaction_handler_nothing( int signum, siginfo_t *siginfo, void *param )
{

    signal( signum, SIG_DFL );
}

void
Java_jp_ne_sakura_kkkon_example_androidsegv_AndroidSEGV_invokeSEGV3
(
    JNIEnv* env,
    jobject thiz
)
{
    struct sigaction act;
    struct sigaction old;
    memset( &act, 0, sizeof(act) );
    memset( &old, 0, sizeof(old) );

    act.sa_sigaction = sigaction_handler_nothing;
    act.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset( &act.sa_mask );

    sigaction( SIGSEGV, &act, &old );
    LOGI( "old.sa_sigaction=%p", old.sa_sigaction );

    invokeSEGV();

    sigaction( SIGSEGV, &old, NULL );
}
