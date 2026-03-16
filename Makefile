WASI_SDK_PATH ?= /opt/wasi-sdk

CC     = $(WASI_SDK_PATH)/bin/clang
CFLAGS = --target=wasm32-wasi -Wall -Wextra -Werror -O2 \
         -nostdlib -Wl,--no-entry -Wl,--export-dynamic \
         -Wl,--allow-undefined -I src

.PHONY: all clean

all: dist/chip.wasm dist/chip.json

src/wokwi-api.h:
	curl -sL -o $@ https://wokwi.com/api/chips/wokwi-api.h

dist/chip.wasm: src/main.c src/wokwi-api.h
	mkdir -p dist
	$(CC) $(CFLAGS) -o $@ $<

dist/chip.json: chip.json
	mkdir -p dist
	cp $< $@

clean:
	rm -rf dist
