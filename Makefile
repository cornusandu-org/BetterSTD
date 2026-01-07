# Building is recommended to be done on Linux (or WSL)

CC = g++                         # Install if you don't have it; Replace with a Linunx version if youre on Windows
CC_WIN = x86_64-w64-mingw32-g++  # Install if you dont have it; Replace with g++ if youre on Windows
CAR = gcc-ar
CXXFLAGS = -Wall -O0 -Iinclude/bstd -std=c++14 -Iinclude

SOURCES := $(shell find ./src -name "*.cpp")

.PHONY: all

all: build
	@printf "\n===== Linking =====\n"
	@printf "\n"
	@printf "[/] Creating static archive\r"
	@{ $(CAR) rcs bstd.a $(shell find ./tmp -name "*.o") 2>rcs.log; } || { printf "[-] Failed to create static archive\n\n"; cat rcs.log; exit 1; } 
	@{ $(CAR) rcs bstd_win.a $(shell find ./tmp -name "*.o") 2>rcs.log; } || { printf "[-] Failed to create static archive\n\n"; cat rcs.log; exit 1; } 
	@printf "[+] Created static archive         \n"
	@mkdir -p dist
	@mv bstd.a dist/bstd_linux.a
	@mv bstd_win.a dist/bstd_win.a
	@$(MAKE) --no-print-directory clean

build:
	mkdir -p tmp
	@printf "\n===== Building =====\n\n"
	@for item in $(SOURCES); do \
		printf "[/] Building $$item\r"; \
		{ $(CC) $(CXXFLAGS) -c $$item 2>log.txt; } || { printf "[-] Failed to build $$item        \n\n"; cat log.txt; exit 1;}; \
		{ $(CC_WIN) $(CXXFLAGS) -c $${item} -o $${item}_win.o 2>log.txt; } || { printf "[-] Failed to build $$item        \n\n"; cat log.txt; exit 1;}; \
		printf "[+] Built $$item          \n"; \
	done
	@mv *.o tmp/

devtest:
	@$(CC) tests/test.cpp dist/bstd.a -Iinclude -o test -O0

clean:
	@rm -f *.o
	@rm -f $(shell find . -name "*.o")
	@rm -f ./*log*

clear: clean
	@rm -rf $(shell find ./dist -name '*.a') $(shell find ./tmp)
	@rm -f test
