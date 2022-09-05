const { app, BrowserWindow } = require('electron');
const ShutdownHandler = require('../dist').default;

app.whenReady().then(() => {
	const win = new BrowserWindow({
		width: 600,
		height: 300
	})

	win.loadFile('index.html');
	console.log('PID = ', process.pid);

	ShutdownHandler.setWindowHandle(win.getNativeWindowHandle());

	ShutdownHandler.on('shutdown', () => {
		console.log('!!!!!!!!!!!!!!!!!!!!!!!!!!!')
		console.log('! System is shutting down !')
		console.log('!!!!!!!!!!!!!!!!!!!!!!!!!!!')
	})
});