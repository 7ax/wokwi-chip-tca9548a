WASI_SDK_PATH ?= /opt/wasi-sdk
CC = $(WASI_SDK_PATH)/bin/clang
CFLAGS = --target=wasm32-wasi -Wall -Wextra -Werror -O2 -I src

.PHONY: all clean

all: dist/chip.wasm dist/chip.json

src/wokwi-api.h:
	curl -sL -o $@ https://raw.githubusercontent.com/wokwi/wokwi-chip-api/main/include/wokwi-api.h

dist/chip.wasm: src/main.c src/wokwi-api.h
	mkdir -p dist
	$(CC) $(CFLAGS) -o $@ $< -nostartfiles -Wl,--no-entry -Wl,--export-all

dist/chip.json: chip.json
	mkdir -p dist
	cp $< $@

clean:
	rm -rf dist
