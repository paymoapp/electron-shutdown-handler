export interface Addon {
	setMainWindowHandle: (handle: Buffer) => void;
	insertWndProcHook: (cb: () => void) => boolean;
	removeWndProcHook: () => boolean;
	acquireShutdownBlock: (reason: string) => boolean;
	releaseShutdownBlock: () => boolean;
}
