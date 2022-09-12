const { spawnSync } = require('child_process');

if (process.platform == 'win32') {
	console.log('Building addon');
	spawnSync('npm', ['run', 'install:win32'], {
		stdio: 'inherit',
		cwd: __dirname
	});
}
