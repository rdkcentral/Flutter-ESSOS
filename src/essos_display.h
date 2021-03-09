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
