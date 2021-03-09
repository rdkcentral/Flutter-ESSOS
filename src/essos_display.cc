/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Copyright 2018 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef ESSOS_EGL_PLATFORM
#define ESSOS_EGL_PLATFORM 1
#endif

#include "essos_display.h"
#include "EssosIntegration.h"

#include <stdlib.h>
#include <unistd.h>

#include <cstring>

namespace flutter {

EssosDisplay::EssosDisplay(size_t width, size_t height)
    : screen_width_(width), screen_height_(height) {
  if (screen_width_ == 0 || screen_height_ == 0) {
    FLWAY_ERROR << "Invalid screen dimensions." << std::endl;
    return;
  }
  if (!SetupEGL()) {
    FLWAY_ERROR << "Could not setup EGL." << std::endl;
    return;
  }

  valid_ = true;
}

EssosDisplay::~EssosDisplay() {
        EssCtx* essCtx = EssCtxHolder::instance()->getEssCtx();
        if ( !EssContextDestroyNativeWindow(essCtx, _nativeWindow) ) {
            const char *detail = EssContextGetLastErrorDetail(essCtx);
            FLWAY_ERROR << "Essos error: '" << detail << "'" << std::endl;
        }
        EssCtxHolder::instance()->stopDispatching();


  if (egl_surface_) {
    eglDestroySurface(egl_display_, egl_surface_);
    egl_surface_ = nullptr;
  }

  if (egl_display_) {
    eglTerminate(egl_display_);
    egl_display_ = nullptr;
  }
}

bool EssosDisplay::IsValid() const {
  return valid_;
}

bool EssosDisplay::Run() {
  if (!valid_) {
    FLWAY_ERROR << "Could not run an invalid display." << std::endl;
    return false;
  }

  while (valid_) {
    EssContextRunEventLoopOnce(EssCtxHolder::instance()->getEssCtx());
    //wl_display_dispatch(display_);
  }

  return true;
}

static void LogLastEGLError() {
  struct EGLNameErrorPair {
    const char* name;
    EGLint code;
  };

#define _EGL_ERROR_DESC(a) \
  { #a, a }

  const EGLNameErrorPair pairs[] = {
      _EGL_ERROR_DESC(EGL_SUCCESS),
      _EGL_ERROR_DESC(EGL_NOT_INITIALIZED),
      _EGL_ERROR_DESC(EGL_BAD_ACCESS),
      _EGL_ERROR_DESC(EGL_BAD_ALLOC),
      _EGL_ERROR_DESC(EGL_BAD_ATTRIBUTE),
      _EGL_ERROR_DESC(EGL_BAD_CONTEXT),
      _EGL_ERROR_DESC(EGL_BAD_CONFIG),
      _EGL_ERROR_DESC(EGL_BAD_CURRENT_SURFACE),
      _EGL_ERROR_DESC(EGL_BAD_DISPLAY),
      _EGL_ERROR_DESC(EGL_BAD_SURFACE),
      _EGL_ERROR_DESC(EGL_BAD_MATCH),
      _EGL_ERROR_DESC(EGL_BAD_PARAMETER),
      _EGL_ERROR_DESC(EGL_BAD_NATIVE_PIXMAP),
      _EGL_ERROR_DESC(EGL_BAD_NATIVE_WINDOW),
      _EGL_ERROR_DESC(EGL_CONTEXT_LOST),
  };

#undef _EGL_ERROR_DESC

  const auto count = sizeof(pairs) / sizeof(EGLNameErrorPair);

  EGLint last_error = eglGetError();

  for (size_t i = 0; i < count; i++) {
    if (last_error == pairs[i].code) {
      FLWAY_ERROR << "EGL Error: " << pairs[i].name << " (" << pairs[i].code
                  << ")" << std::endl;
      return;
    }
  }

  FLWAY_ERROR << "Unknown EGL Error" << std::endl;
}

bool EssosDisplay::SetupEGL() {
        int width = 0;
        int height = 0;
        bool error = false;

        NativeDisplayType display;
        NativeWindowType nativeWindow;

        EssCtx *essCtx = EssCtxHolder::instance()->getEssCtx();

        if ( !EssContextGetEGLDisplayType(essCtx, &display) ) {
            error = true;
        }
        else if ( !EssContextGetDisplaySize(essCtx, &width, &height) ) {
            error= true;
        }
        else if ( !EssContextCreateNativeWindow(essCtx, width, height, &nativeWindow) ) {
            error = true;
        }
        else if ( !EssCtxHolder::instance()->startDispatching() ) {
            error = true;
        }

        if ( error ) {
            const char *detail = EssContextGetLastErrorDetail(essCtx);
        }
        _nativeWindow = nativeWindow;

  egl_display_ = eglGetDisplay((NativeDisplayType)display);
  if (egl_display_ == EGL_NO_DISPLAY) {
    LogLastEGLError();
    FLWAY_ERROR << "Could not access EGL display." << std::endl;
    return false;
  }

  if (eglInitialize(egl_display_, nullptr, nullptr) != EGL_TRUE) {
    LogLastEGLError();
    FLWAY_ERROR << "Could not initialize EGL display." << std::endl;
    return false;
  }

  EGLConfig egl_config = nullptr;

  // Choose an EGL config to use for the surface and context.
  {
    EGLint attribs[] = {
        // clang-format off
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
      EGL_RED_SIZE,        8,
      EGL_GREEN_SIZE,      8,
      EGL_BLUE_SIZE,       8,
      EGL_ALPHA_SIZE,      8,
      EGL_DEPTH_SIZE,      0,
      EGL_STENCIL_SIZE,    0,
      EGL_NONE,            // termination sentinel
        // clang-format on
    };

    EGLint config_count = 0;

    if (eglChooseConfig(egl_display_, attribs, &egl_config, 1, &config_count) !=
        EGL_TRUE) {
      LogLastEGLError();
      FLWAY_ERROR << "Error when attempting to choose an EGL surface config."
                  << std::endl;
      return false;
    }

    if (config_count == 0 || egl_config == nullptr) {
      LogLastEGLError();
      FLWAY_ERROR << "No matching configs." << std::endl;
      return false;
    }
  }

  // Create an EGL window surface with the matched config.
  {
    const EGLint attribs[] = {EGL_NONE};

    egl_surface_ =
        eglCreateWindowSurface(egl_display_, egl_config, nativeWindow, attribs);

    if (egl_surface_ == EGL_NO_SURFACE) {
      LogLastEGLError();
      FLWAY_ERROR << "EGL surface was null during surface selection."
                  << std::endl;
      return false;
    }
  }

  // Create an EGL context with the match config.
  {
    const EGLint attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

    egl_context_ = eglCreateContext(egl_display_, egl_config,
                                    nullptr /* share group */, attribs);

    if (egl_context_ == EGL_NO_CONTEXT) {
      LogLastEGLError();
      FLWAY_ERROR << "Could not create an onscreen context." << std::endl;
      return false;
    }
  }
  // Create an EGL context with the match config.
  {
    const EGLint attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

    egl_resource_context_ = eglCreateContext(egl_display_, egl_config,
                                    egl_context_, attribs);

    if (egl_resource_context_ == EGL_NO_CONTEXT) {
      LogLastEGLError();
      FLWAY_ERROR << "Could not create an onscreen resource context." << std::endl;
      return false;
    }
  }


  return true;
}

// |flutter::FlutterApplication::RenderDelegate|
bool EssosDisplay::OnApplicationContextMakeCurrent() {
  if (!valid_) {
    FLWAY_ERROR << "Invalid display." << std::endl;
    return false;
  }

  if (eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_) !=
      EGL_TRUE) {
    LogLastEGLError();
    FLWAY_ERROR << "Could not make the onscreen context current" << std::endl;
    return false;
  }

  return true;
}

// |flutter::FlutterApplication::RenderDelegate|
bool EssosDisplay::OnApplicationMakeResourceCurrent() {
  if (!valid_) {
    FLWAY_ERROR << "Invalid display." << std::endl;
    return false;
  }


  if (eglMakeCurrent(egl_display_, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_resource_context_) !=
      EGL_TRUE) {
    LogLastEGLError();
    FLWAY_ERROR << "Could not make the onscreen context current" << std::endl;
    return false;
  }

  return true;
}
// |flutter::FlutterApplication::RenderDelegate|
bool EssosDisplay::OnApplicationContextClearCurrent() {
  if (!valid_) {
    FLWAY_ERROR << "Invalid display." << std::endl;
    return false;
  }

  if (eglMakeCurrent(egl_display_, EGL_NO_SURFACE, EGL_NO_SURFACE,
                     EGL_NO_CONTEXT) != EGL_TRUE) {
    LogLastEGLError();
    FLWAY_ERROR << "Could not clear the context." << std::endl;
    return false;
  }

  return true;
}

// |flutter::FlutterApplication::RenderDelegate|
bool EssosDisplay::OnApplicationPresent() {
  if (!valid_) {
    FLWAY_ERROR << "Invalid display." << std::endl;
    return false;
  }

  if (eglSwapBuffers(egl_display_, egl_surface_) != EGL_TRUE) {
    LogLastEGLError();
    FLWAY_ERROR << "Could not swap the EGL buffer." << std::endl;
    return false;
  }

  return true;
}

// |flutter::FlutterApplication::RenderDelegate|
uint32_t EssosDisplay::OnApplicationGetOnscreenFBO() {
  if (!valid_) {
    FLWAY_ERROR << "Invalid display." << std::endl;
    return 999;
  }

  return 0;  // FBO0
}

}  // namespace flutter
