# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

### [1.0.8](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.7...v1.0.8) (2022-09-06)

### [1.0.7](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.6...v1.0.7) (2022-09-06)


### Bug Fixes

* **addon:** Shut the main thread down first, so we will not get errors because the renderer process already exited ([0079193](https://github.com/paymoapp/electron-shutdown-handler/commit/007919342d41837ed09f492d81f348be03f0234c))


### Improvements

* **demo:** Better demo ([672f687](https://github.com/paymoapp/electron-shutdown-handler/commit/672f6874f48dd3d938ab028feb0c6287267bac77))


### Documentation

* Updated docs ([3cd964e](https://github.com/paymoapp/electron-shutdown-handler/commit/3cd964ebe458b962e4d604c8659699318eb12b7e))

### [1.0.6](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.5...v1.0.6) (2022-09-06)


### Bug Fixes

* **addon:** Handle WM_ENDSESSION where wParam is false ([b94dfb1](https://github.com/paymoapp/electron-shutdown-handler/commit/b94dfb1307b44f52201b7075fb3ebe969ea1b98b))

### [1.0.5](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.4...v1.0.5) (2022-09-06)

### [1.0.4](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.3...v1.0.4) (2022-09-06)


### Bug Fixes

* **addon:** Call javascript callback on QueryEndSession instead of EndSession ([92d435b](https://github.com/paymoapp/electron-shutdown-handler/commit/92d435b37b99b64f80bc29591c3eab9c44e0fe10))

### [1.0.3](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.2...v1.0.3) (2022-09-06)


### Build/CI

* Use MSVS version 2019 ([4824d58](https://github.com/paymoapp/electron-shutdown-handler/commit/4824d58ac798491862698067c9c793c0b0180c71))

### [1.0.2](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.1...v1.0.2) (2022-09-05)


### Bug Fixes

* **js:** Fixed import of addon ([adc9ad8](https://github.com/paymoapp/electron-shutdown-handler/commit/adc9ad8643e6ee9526dc558732c79bddbf58bdcf))

### [1.0.1](https://github.com/paymoapp/electron-shutdown-handler/compare/v1.0.0...v1.0.1) (2022-09-05)


### Build/CI

* Migrate to prebuild from node-pre-gyp ([c268174](https://github.com/paymoapp/electron-shutdown-handler/commit/c268174d20d3fd1db673d4144cc5556784683cbe))

## [1.0.0](https://github.com/paymoapp/electron-shutdown-handler/compare/v0.1.1...v1.0.0) (2022-09-05)


### Documentation

* Added documentation ([c814279](https://github.com/paymoapp/electron-shutdown-handler/commit/c814279841b863c9fc07f02796a451a1102b746b))

### 0.1.1 (2022-09-05)


### Features

* Implemented C++ code of addon ([88c0a54](https://github.com/paymoapp/electron-shutdown-handler/commit/88c0a54e7f896f9d3dd52f91994431a9c860f9e4))
* Implemented driver code ([ef3e54f](https://github.com/paymoapp/electron-shutdown-handler/commit/ef3e54f724e59927235eb890d942cdf3116b41b3))


### Build/CI

* Added pipeline ([6940946](https://github.com/paymoapp/electron-shutdown-handler/commit/6940946d9e2038124ef41b1326b3b1fab5c3dea3))
