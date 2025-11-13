#include <jni.h>
#include <android/log.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "native-lib", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "native-lib", __VA_ARGS__)

using namespace cv;
using namespace std;

static JavaVM* gJvm = nullptr;
static jobject gActivityObj = nullptr; // global ref to MainActivity instance
static bool useEdges = true;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    gJvm = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_edgedetector_MainActivity_nativeInit(JNIEnv* env, jobject thiz) {
    LOGI("nativeInit");
    // store global ref to call back later
    if (gActivityObj == nullptr) {
        gActivityObj = env->NewGlobalRef(thiz);
    }
}

// Helper: convert NV21 byte array to BGR Mat
static void nv21ToBgrMat(const unsigned char* nv21, int width, int height, Mat& outBgr) {
    Mat yuv(height + height/2, width, CV_8UC1, (void*)nv21);
    cvtColor(yuv, outBgr, COLOR_YUV2BGR_NV21);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_edgedetector_MainActivity_nativeToggleMode(JNIEnv* env, jobject thiz) {
    useEdges = !useEdges;
    LOGI("nativeToggleMode -> %d", useEdges);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_edgedetector_MainActivity_nativeProcessFrame(JNIEnv* env, jobject thiz,
                                                              jbyteArray data, jint width, jint height) {
    jbyte* nv21 = env->GetByteArrayElements(data, NULL);
    if (!nv21) return;
    auto t0 = chrono::high_resolution_clock::now();

    Mat bgr;
    nv21ToBgrMat((unsigned char*)nv21, width, height, bgr);

    Mat gray, edges, outRGBA;
    cvtColor(bgr, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, Size(5,5), 1.5);
    if (useEdges) {
        Canny(gray, edges, 50, 150);
        cvtColor(edges, outRGBA, COLOR_GRAY2RGBA); // edges 0/255 -> RGBA
    } else {
        cvtColor(gray, outRGBA, COLOR_GRAY2RGBA);
    }

    int outSize = outRGBA.total() * outRGBA.elemSize();
    jbyteArray outArr = env->NewByteArray(outSize);
    env->SetByteArrayRegion(outArr, 0, outSize, (jbyte*)outRGBA.data);

    // Call back into Java: onProcessedFrameNative([BII)V
    jclass cls = env->GetObjectClass(gActivityObj);
    if (cls) {
        jmethodID mid = env->GetMethodID(cls, "onProcessedFrameNative", "([BII)V");
        if (mid) {
            env->CallVoidMethod(gActivityObj, mid, outArr, width, height);
        } else {
            LOGE("Method onProcessedFrameNative not found");
        }
    }

    env->ReleaseByteArrayElements(data, nv21, JNI_ABORT);
    env->DeleteLocalRef(outArr);

    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();
    LOGI("Frame processed in %.2f ms", ms);
}
