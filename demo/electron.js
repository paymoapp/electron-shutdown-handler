const { app, BrowserWindow } = require('electron');
const ShutdownHandler = require('../dist').default;

app.whenReady().then(() => {
	const win = new BrowserWindow({
		width: 600,
		height: 300,
		title: 'Electron-shutdown-handler demo'
	});

	win.loadFile('index.html');

	console.log('--- Electron-shutdown-handler demo ---');

	console.log('PID =', process.pid);
	console.log('You can use rmlogotest.exe <PID> to "simulate" a shutdown');
	console.log(
		'You can close this app by executing Ctrl+C in the terminal or shutting down your system'
	);

	let allowQuit = false;

	app.on('before-quit', e => {
		if (!allowQuit) {
			e.preventDefault();
		}
	});

	win.on('close', e => {
		if (!allowQuit) {
			e.preventDefault();
		}
	});

	ShutdownHandler.setWindowHandle(win.getNativeWindowHandle());
	ShutdownHandler.blockShutdown('Yayy! It works! Shutdown in 10 seconds');

	ShutdownHandler.on('shutdown', () => {
		console.log('!!!!!!!!!!!!!!!!!!!!!!!!!!!');
		console.log('! System is shutting down !');
		console.log('!!!!!!!!!!!!!!!!!!!!!!!!!!!');

		console.log();
		console.log('Setting a 10 second timer');

		setTimeout(() => {
			console.log('Shutting down NOW');
			ShutdownHandler.releaseShutdown();
			allowQuit = true;
			app.quit();
		}, 10_000);
	});
});
