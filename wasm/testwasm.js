


//const SegfaultHandler = require('segfault-handler');
//SegfaultHandler.registerHandler('crash.log');


const wasi = require('@wasmer/wasi')
const nodeBindings = require("@wasmer/wasi/lib/bindings/node");
const wasmfs = require('@wasmer/wasmfs')

var fs = new wasmfs.WasmFs()

var bindings = Object.create(nodeBindings.default);
bindings.fs = fs.fs;

let myWASIInstance = new wasi.WASI({
  // OPTIONAL: The pre-opened dirctories
  preopenDirectories: {},

  // OPTIONAL: The environment vars
  env: {},

  // OPTIONAL: The arguments provided
  args: [],

  // OPTIONAL: The environment bindings (fs, path),
  // useful for using WASI in diferent environments
  // such as Node.js, Browsers, ...
  bindings: bindings
    // hrtime: (time?: [number, number]) -> number
    // exit: (code?: number) -> void
    // kill: (pid: number, signal?: string | number) -> void
    // randomFillSync: (buffer: Buffer, offset?: number, size?: number) -> Buffer
    // isTTY: () -> bool
    // fs: Filesystem (with similar API interface as Node 'fs' module)
    // path: Path  (with similar API Interface as Node 'path' module)
});

function stub() {
  console.log('stub');
}

var wasmbin = require('fs').readFileSync('atari8.wasm')
var wmod = new WebAssembly.Module(wasmbin)
var imports = myWASIInstance.getImports(wmod);
imports.env = {
  system: stub,
  __sys_mkdir: stub,
  __sys_chmod: stub,
  __sys_stat64: stub,
  __sys_unlink: stub,
  __sys_rename: stub,
  __sys_getdents64: stub,
  __sys_getcwd: stub,
  __sys_rmdir: stub,
  emscripten_thread_sleep: stub,
}
var winst = new WebAssembly.Instance(wmod, imports);
console.log(winst)
myWASIInstance.start(winst);
var atari8 = winst.exports;
//console.log(atari8);
console.log('init')
const sys = atari8.machine_init();
console.log('sys',sys)

atari8.machine_reset(sys)

console.log(atari8.atari8_std_display_width())
console.log(atari8.atari8_std_display_height())
console.log(atari8.atari8_max_display_size());

console.log(atari8.atari8_display_width(sys));
console.log(atari8.atari8_display_height(sys));

console.log('pixels', atari8.machine_get_pixel_buffer(sys));

atari8.machine_tick(sys);
atari8.atari8_exec(sys, 100000);

const pixels = new Uint8Array(
  atari8.memory.buffer,
  atari8.machine_get_pixel_buffer(sys),
  atari8.atari8_max_display_size()
);

fs.writeFileSync('testatari8.rgba', pixels);

console.log('probe', atari8.machine_get_probe_buffer_size());

