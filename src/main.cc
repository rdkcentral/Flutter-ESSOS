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

#include <stdlib.h>

#include <string>
#include <vector>

#include "flutter_application.h"
#include "utils.h"
#include "essos_display.h"

namespace flutter {

static void PrintUsage() {
  std::cerr << "Flutter Essos Embedder" << std::endl << std::endl;
  std::cerr << "========================" << std::endl;
  std::cerr << "Usage: `" << GetExecutableName()
            << " <asset_bundle_path> <flutter_flags>`" << std::endl
            << std::endl;
  std::cerr << R"~(
This utility runs an instance of a Flutter application and renders using
Essos core protocols.

The Flutter tools can be obtained at https://flutter.io/

asset_bundle_path: The Flutter application code needs to be snapshotted using
                   the Flutter tools and the assets packaged in the appropriate
                   location. This can be done for any Flutter application by
                   running `flutter build bundle` while in the directory of a
                   valid Flutter project. This should package all the code and
                   assets in the "build/flutter_assets" directory. Specify this
                   directory as the first argument to this utility.

    flutter_flags: Typically empty. These extra flags are passed directly to the
                   Flutter engine. To see all supported flags, run
                   `flutter_tester --help` using the test binary included in the
                   Flutter tools.
)~" << std::endl;
}

static bool Main(std::vector<std::string> args) {
  if (args.size() == 0) {
    std::cerr << "   <Invalid Arguments>   " << std::endl;
    PrintUsage();
    return false;
  }

  const auto asset_bundle_path = args[0];

  if (!FlutterAssetBundleIsValid(asset_bundle_path)) {
    std::cerr << "   <Invalid Flutter Asset Bundle>   " << std::endl;
    PrintUsage();
    return false;
  }

  const size_t kWidth = 1920;
  const size_t kHeight = 1080;

  for (const auto& arg : args) {
    FLWAY_ERROR << "Arg: " << arg << std::endl;
  }

  EssosDisplay display(kWidth, kHeight);

  if (!display.IsValid()) {
    FLWAY_ERROR << "Essos display was not valid." << std::endl;
    return false;
  }

  FlutterApplication application(asset_bundle_path, args, display);
  if (!application.IsValid()) {
    FLWAY_ERROR << "Flutter application was not valid." << std::endl;
    return false;
  }

  if (!application.SetWindowSize(kWidth, kHeight)) {
    FLWAY_ERROR << "Could not update Flutter application size." << std::endl;
    return false;
  }

  display.Run();

  return true;
}

}  // namespace flutter

int main(int argc, char* argv[]) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  return flutter::Main(std::move(args)) ? EXIT_SUCCESS : EXIT_FAILURE;
}
