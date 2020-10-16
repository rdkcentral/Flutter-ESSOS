Flutter Essos
============

A Flutter Embedder that talks to Essos.

Running Flutter Applications
----------------------------

```
Flutter Essos Embedder
========================

Usage: `flutter_essos <path to assets> <flutter_flags>`

This utility runs an instance of a Flutter application and renders using
Essos.

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

```
