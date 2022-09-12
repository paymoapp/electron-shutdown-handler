const { spawnSync } = require('child_process');

if (process.platform == 'win32') {
	console.log('Building addon');
	const status = spawnSync('npm', ['run', 'install:win32'], {
		stdio: 'inherit',
		cwd: __dirname,
		shell: true
	});

	process.exit(status.status || 0);
}
