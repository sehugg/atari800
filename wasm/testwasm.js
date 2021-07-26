


//const SegfaultHandler = require('segfault-handler');
//SegfaultHandler.registerHandler('crash.log');


const wasi = require('@wasmer/wasi')
const nodeBindings = require("@wasmer/wasi/lib/bindings/node");
const wasmfs = require('@wasmer/wasmfs')
const { spy } = require('spyfs')

var fs = new wasmfs.WasmFs()

var bindings = Object.create(nodeBindings.default);
bindings.fs = fs.fs;
fs.fs.mkdirSync('/tmp')
fs.fs.writeFileSync('.atari800.cfg', 'test');

const stdoutWrite = buffer => {
  console.log('>>>',buffer.toString());
  return buffer.length
}
fs.volume.fds[1].write = stdoutWrite.bind(this);
fs.volume.fds[2].write = stdoutWrite.bind(this);

var preopen = {};
preopen['/tmp'] = '/tmp' //process.cwd()
preopen['.'] = '.' //process.cwd()
console.log(preopen)

let myWASIInstance = new wasi.WASI({
  // OPTIONAL: The pre-opened dirctories
  preopenDirectories: preopen,

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

function stub(name) {
  return function() {
    console.log('stub',name);
  }
}

function allocString(str) {
  var m = atari8.malloc(str.length+1);
  console.log('alloc',str,m);
  for (var i=0; i<str.length; i++)
    m[i] = str.charCodeAt(i); //TODO: utf8
  m[i] = 0;
  return m;
}

var wasmbin = require('fs').readFileSync('atari8.wasm')
var wmod = new WebAssembly.Module(wasmbin)
var imports = myWASIInstance.getImports(wmod);
imports.env = {
  system: stub('system'),
  __sys_mkdir: stub('sys_mkdir'),
  __sys_chmod: stub('sys_chmod'),
  __sys_stat64: stub('sys_stat64'),
  __sys_unlink: stub('sys_unlink'),
  __sys_rename: stub('sys_rename'),
  __sys_getdents64: stub('sys_getdents64'),
  __sys_getcwd: function() { return allocString('/tmp') }, //stub('sys_getcwd'),
  __sys_rmdir: stub('sys_rmdir'),
  emscripten_thread_sleep: stub('thread_sleep'),
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

console.log(atari8.machine_std_display_width())
console.log(atari8.machine_std_display_height())
console.log(atari8.machine_max_display_size());

console.log(atari8.machine_display_width(sys));
console.log(atari8.machine_display_height(sys));

for (var i=0; i<100; i++) {
  atari8.machine_start_frame(sys);
}

console.log('pixels', atari8.machine_get_pixel_buffer(sys));

atari8.machine_tick(sys);
atari8.machine_exec(sys, 100000);

const pixels = new Uint8Array(
  atari8.memory.buffer,
  atari8.machine_get_pixel_buffer(sys),
  atari8.machine_max_display_size()
);

require('fs').writeFileSync('testatari8.rgba', pixels);

//fs.fs.writeFileSync('/tmp/atari8.img', pixels);
//fs.fs.readFileSync('/tmp/atari8.img');

var errno = atari8.machine_load_rom(sys, 0, 1000);
if (!errno) {
  console.log('rom loaded')
} else {
  console.log('could not load rom', errno)
}

//console.log('probe', atari8.machine_get_probe_buffer_size());

