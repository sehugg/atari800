Atari800 emulator version 4.2.0
-------------------------------

This is a free and portable Atari 8-bit computer and 5200 console emulator,
originally written by David Firth and now developed by the Atari800
Development Team (please see DOC/CREDITS). This program is copyrighted and
released under the GPL (see COPYING).

Please read DOC/README, DOC/INSTALL and DOC/USAGE and eventual platform
specific versions of the above mentioned files.

For news in this release please read DOC/NEWS.

If you have a problem running this emulator please read DOC/FAQ first.
If the problem persists then please ask in newsgroup comp.sys.atari.8bit
or in the atari800-users mailing list (see below).

Look at DOC/BUGS before reporting something you consider a bug.

If you want to help developing Atari800 emulator you can subscribe to our
mailing list (http://lists.sourceforge.net/lists/listinfo/atari800-users).
DOC/TODO might point you in the right direction.

This version, and all previous versions back to v0.8.2 have been released Petr
Stehlik and are available at internet (current project URL is below). Thanks
go to original author David Firth and all the people that contributed since then.

EnJoy!

Petr Stehlik
December 28, 2019

E-mail: pstehlik@sophics.cz

Project homepage: http://atari800.github.io/

# libatari800

## wasi-sdk build

install: https://github.com/WebAssembly/wasi-sdk

	cp $(WASISDK)/share/misc/config.* .
	./configure --target=libatari800 --disable-rserial --disable-rnetwork --disable-riodevice CC="$WASISDK/bin/clang --sysroot=$WASISDK/share/wasi-sysroot" --host=wasm32
	make
	$(WASISDK)/bin/ranlib src/*.a
	cd wasm
	make

## Emscripten Build (doesn't work!!!)

Install from https://github.com/emscripten-core/emsdk

. ~/emsdk/emsdk_env.sh
emconfigure ./configure --target=libatari800 --disable-rserial --disable-rnetwork --disable-riodevice
emmake make
cd wasm && make
cp *.wasm ~/8bitworkshop/res/

# requires node 14.x+ (?)
node --experimental-wasm-bigint testwasm.js
