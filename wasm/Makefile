
WASISDK=$(HOME)/wasi-sdk-12.0
CLANG=$(WASISDK)/bin/clang
#CLANG=emcc -g #--no-entry 
LIBA=../src/libatari800.a

all: atari8.wasm fopen_test.wasm
	/snap/bin/node testwasm.js
	convert -size 384x240 -depth 8 RGBA:testatari8.rgba testatari8.png

clean:
	rm -f atari8.wasm

# wasi-sdk
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
	wasmer inspect $@

# emcc (doesnt work with stdio?)
%.emcc.wasm: %.c $(LIBA)
	$(CLANG) -v \
	-D__wasi__ \
	-I. \
	-I../src \
	-I../src/libatari800 \
	-Iwasm \
	-O2 \
	-flto \
	-Wl,--export-all \
	-Wl,--lto-O2 \
	-o $@ \
	$(LIBA) \
	$<

