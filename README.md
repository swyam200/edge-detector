# Real-Time Edge Detection Viewer (Full version skeleton)

This repository contains a **full-version skeleton** for the assessment:
Android app (Kotlin) + Native C++ OpenCV processing + OpenGL ES renderer + TypeScript web viewer.

## What's included
- `app/` - Android app skeleton with Kotlin MainActivity and native C++ processor.
- `app/src/main/cpp/processor.cpp` - Native code (OpenCV) that converts NV21 -> BGR, applies Canny, and returns RGBA bytes via JNI callback.
- `gl/` - OpenGL renderer helper skeleton and shaders.
- `web/` - TypeScript web viewer to display an exported sample frame.

## Important setup notes (please read)
1. **OpenCV Android SDK**: Download the OpenCV Android SDK and place it relative to the project root:
   ```
   /full_project/opencv/sdk/native/jni
   ```
   or update `CMakeLists.txt` to point to your OpenCV installation.
2. **NDK & CMake**: Install Android NDK (r21+ recommended) and CMake. Configure `local.properties` with `ndk.dir` if needed.
3. **Camera2**: The `MainActivity` contains a placeholder for camera initialization. Replace `startCameraPlaceholder()` with Camera2 + ImageReader pipeline that passes NV21 byte[] to `nativeProcessFrame`.
4. **Permissions**: Camera permission request is included; ensure runtime permission acceptance.
5. **Building**:
   - Open the project in Android Studio.
   - Make sure `app/src/main/cpp/CMakeLists.txt` is referenced from `app/build.gradle` (not included in this skeleton).
   - Build and run on a real device (camera access required).

## How to export a sample frame for the web viewer
In `onProcessedFrameNative(...)`, convert the byte[] (RGBA) to a PNG using `Bitmap.compress()` and encode to Base64. Paste that Base64 data into `web/src/index.ts` replacing `sampleBase64`.

## Commit strategy (recommended)
- Make small commits: `init`, `camera-setup`, `jni-scaffold`, `opencv-processing`, `gl-renderer`, `web-viewer`, `docs`.
- Include screenshots in README before final submission.

## Limitations of this skeleton
- Camera2 pipeline is intentionally left short to keep this skeleton concise. For evaluation, implement full Camera2 ImageReader handling.
- GL draw calls are simplified and shader compilation is omitted. Add a shader helper to compile and render the textured quad.
- `CMakeLists.txt` assumes OpenCV is available at `opencv/sdk/native/jni`.

## License
MIT
