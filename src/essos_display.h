// Copyright 2018 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once
#include <essos-app.h>

#include <memory>
#include <string>

#include "flutter_application.h"
#include "macros.h"

namespace flutter {

class EssosDisplay : public FlutterApplication::RenderDelegate {
 public:
  EssosDisplay(size_t width, size_t height);

  ~EssosDisplay();

  bool IsValid() const;

  bool Run();

 private:
  bool valid_ = false;
  const int screen_width_;
  const int screen_height_;
  NativeWindowType _nativeWindow;

  EGLDisplay egl_display_ = EGL_NO_DISPLAY;
  EGLSurface egl_surface_ = nullptr;
  EGLContext egl_context_ = EGL_NO_CONTEXT;
  EGLContext egl_resource_context_ = EGL_NO_CONTEXT;
  bool SetupEGL();
  bool StopRunning();

  // |flutter::FlutterApplication::RenderDelegate|
  bool OnApplicationContextMakeCurrent() override;
  // |flutter::FlutterApplication::RenderDelegate|
  bool OnApplicationMakeResourceCurrent() override;

  // |flutter::FlutterApplication::RenderDelegate|
  bool OnApplicationContextClearCurrent() override;

  // |flutter::FlutterApplication::RenderDelegate|
  bool OnApplicationPresent() override;

  // |flutter::FlutterApplication::RenderDelegate|
  uint32_t OnApplicationGetOnscreenFBO() override;

  FLWAY_DISALLOW_COPY_AND_ASSIGN(EssosDisplay);
};

}  // namespace flutter
