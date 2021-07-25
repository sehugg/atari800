
#WASISDK=$(HOME)/wasi-sdk-12.0
#CLANG=$(WASISDK)/bin/clang #"$(EMSDK)/upstream/bin/clang"
CLANG=emcc --no-entry
LIBA=../src/libatari800.a

all: atari8.wasm
	/snap/bin/node testwasm.js
	convert -size 384x240 -depth 8 RGBA:testatari8.rgba testatari8.png

%.wasm: %.c $(LIBA)
	$(CLANG) -v \
	-D__wasi__ \
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

