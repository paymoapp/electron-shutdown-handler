# Electron Shutdown Handler

[![NPM](https://img.shields.io/npm/v/@paymoapp/electron-shutdown-handler)](https://www.npmjs.com/package/@paymoapp/electron-shutdown-handler)
[![Typescript](https://img.shields.io/npm/types/@paymoapp/electron-shutdown-handler)](https://www.npmjs.com/package/@paymoapp/electron-shutdown-handler)
[![N-API](https://raw.githubusercontent.com/nodejs/abi-stable-node/doc/assets/Node-API%20v6%20Badge.svg)](https://github.com/nodejs/node-addon-api)
[![License](https://img.shields.io/github/license/paymoapp/electron-shutdown-handler)](https://www.gnu.org/licenses/gpl-3.0.txt)

NodeJS library using native modules to capture the shutdown events on Windows in Electron applications.

### Table of Contents

<!-- toc -->

- [Getting started](#getting-started)
    - [Installation](#installation)
    - [Native addon](#native-addon)
    - [Example](#example)
- [API](#api)
    - [Functions](#functions)
    - [Events](#events)

<!-- tocstop -->

## Getting started

#### Installation

```bash
npm install --save @paymoapp/electron-shutdown-handler
```

#### Native addon

This project uses NodeJS Native Addons to function, so you can use this library in any NodeJS or Electron project, there won't be any problem with bundling and code signing.

The project uses [node-pre-gyp](https://www.npmjs.com/package/@mapbox/node-pre-gyp) to supply prebuilt libraries.

The project uses Node-API version 6, you can check [this table](https://nodejs.org/api/n-api.html#node-api-version-matrix) to see which node versions are supported.

If there's a compliant prebuilt binary, it will be downloaded during installation, or it will be built. You can also rebuild it anytime by running `npm run build:gyp`.

The library has native addons for Windows only, but it won't fail during install or during runtime on other platforms.

#### Example

You can run a demo application by calling `npm run demo` and use the `rmlogotext.exe` command to emit the shutdown event without actually shutting the system down.

```ts
import ElectronShutdownHandler from '@paymoapp/electron-shutdown-handler';
import { app, BrowserWindow } from 'electron';

app.whenReady().then(() => {
	const win = new BrowserWindow({
		width: 600,
		height: 600
	});

	win.loadFile('index.html');

	ElectronShutdownHandler.setWindowHandle(win.getNativeWindowHandle());
	ElectronShutdownHandler.blockShutdown('Please wait for some data to be saved');

	Electron.ShutdownHandler.on('shutdown', () => {
		console.log('Shutting down!');
		ElectronShutdownHandler.releaseShutdown();
		win.webContents.send('shutdown');
		app.quit();
	});
});
```

#### Usage

First of all you need to create an electron window, after which you can call the `setWindowHandle` method. Calling this method is required, otherwise the rest of the functions will throw. This function will store the HWND of the window in the addon and will set that the window will be the first process to shut down (otherwise the renderer process might exit first which results in the crash of electron).

To block the shutdown you need to call the `blockShutdown` function before a shutdown event occures, but you also need to set up an event listener, otherwise the hook won't be added to the window message callback.

You should also call app.quit() in the shutdown handler.

Please note that this addon is singleton, you can only use it for one window at the moment, so you should always set the main window's handle.

## API

#### Functions

###### 𝑓 &nbsp;&nbsp; setWindowHandle

```ts
type setWindowHandle = (handle: Buffer) => void
```

Set the window handle of the main window. You __MUST__ call this method before calling any other methods.

###### 𝑓 &nbsp;&nbsp; blockShutdown

```ts
type blockShutdown = (reason: string) => boolean
```

Block the system from shutting down. You need to set a reason which will be displayed to the user. The shutdown will only be blocked if you also have a shutdown event listener. The response indicates if the operation was successful.

###### 𝑓 &nbsp;&nbsp; releaseShutdown

```ts
type releaseShutdown = () => boolean
```

Allow the system to shut down. The response indicates if the operation was successful.

#### Events

The exported object extends the node [EventEmitter](https://nodejs.org/api/events.html) class.

###### ✨ &nbsp;&nbsp; shutdown

This event is emitted when the system is shutting down. You should avoid calling long running async code here, since as the function finishes, the process will exit.
