
#WASISDK=$(HOME)/wasi-sdk-12.0
#CLANG=$(WASISDK)/bin/clang #"$(EMSDK)/upstream/bin/clang"
CLANG=emcc --no-entry
LIBA=../src/libatari800.a

all: atari8.wasm

test: wasm/c64.wasm wasm/zx.wasm wasm/vic20.wasm
	node --experimental-modules --experimental-wasm-modules wasm/testc64.mjs
	#convert -size 392x272 -depth 8 RGBA:testc64.rgba testc64.png
	node --experimental-modules --experimental-wasm-modules wasm/testzx.mjs
	node --experimental-modules --experimental-wasm-modules wasm/testvic20.mjs

%.wasm: %.c $(LIBA)
	$(CLANG) -v \
	--sysroot=$(WASISDK)/share/wasi-sysroot \
	-I. \
	-I../src \
	-I../src/libatari800 \
	-I$(WASISDK)/include \
	-Iwasm \
	-O2 \
	-flto \
	-Wl,--export-all \
	-Wl,--lto-O2 \
	-o $@ \
	$(LIBA) \
	$<

%-sdk.wasm: %.c $(LIBA)
	$(CLANG) -v \
	--sysroot=$(WASISDK)/share/wasi-sysroot \
	-I. \
	-I../src \
	-I../src/libatari800 \
	-I$(WASISDK)/include \
	-Iwasm \
	--target=wasm32-wasi \
	-O2 \
	-flto \
	-Wl,--export-all \
	-Wl,--lto-O2 \
	-o $@ \
	$(LIBA) \
	$<


# from https://dassur.ma/things/c-to-webassembly/
%-bad.wasm: %.c $(LIBA)
	$(CLANG) -v \
	--sysroot=$(WASISDK)/share/wasi-sysroot \
	-I. \
	-I../src \
	-I../src/libatari800 \
	-I$(WASISDK)/include \
	-Iwasm \
	--target=wasm32-wasi \
	-O2 \
	-flto \
	-nostdlib \
	-Wl,--no-entry \
	-Wl,--export-all \
	-Wl,--lto-O2 \
	-o $@ \
	$(LIBA) \
	$<

