# Building is recommended to be done on Linux (or WSL)

CC = g++                         # Install if you don't have it; Replace with a Linunx version if youre on Windows
CC_WIN = x86_64-w64-mingw32-g++  # Install if you dont have it; Replace with g++ if youre on Windows
CAR = gcc-ar
CXXFLAGS = -Wall -O0 -Iinclude/bstd -Iinclude -std=gnu++23

SOURCES := $(shell find ./src -name "*.cpp")

.PHONY: all
.PHONY: build
.PHONY: devtest
.PHONY: clean
.PHONY: clear

all: build
	@mkdir -p ./tmp_win
	@mv $(shell find ./ -name "*_win.o") ./tmp_win 
	@printf "\n===== Linking =====\n"
	@printf "\n"
	@printf "[/] Creating static archive\r"
	@{ $(CAR) rcs bstd_win.a $(shell find ./tmp -name "*_win.o") 2>rcs.log; } || { printf "[-] Failed to create static archive\n\n"; cat rcs.log; exit 1; }
	@{ $(CAR) rcs bstd.a $(shell find ./tmp -name "*.o") 2>rcs.log; } || { printf "[-] Failed to create static archive\n\n"; cat rcs.log; exit 1; } 
	@printf "[+] Created static archive         \n"
	@mkdir -p dist
	@mv bstd.a dist/bstd_linux.a
	@mv bstd_win.a dist/bstd_win.a

build:
	@mkdir -p tmp
	@mkdir -p tmp_win
	@printf "\n===== Building =====\n\n"
	@for item in $(SOURCES); do \
		printf "[/] Building $$item\r"; \
		{ $(CC) $(CXXFLAGS) -c $$item 2>log.txt; } || { printf "[-] Failed to build $$item        \n\n"; cat log.txt; exit 1;}; \
		{ $(CC_WIN) $(CXXFLAGS) -c $${item} -o $${item}_win.o 2>log.txt; } || { printf "[-] Failed to build $$item        \n\n"; cat log.txt; exit 1;}; \
		printf "[+] Built $$item          \n"; \
	done

devtest:
	@$(CC) tests/test.cpp dist/bstd_linux.a -Iinclude -o test -O0
	
	@if ./test; then \
	    printf "\n\n\nTests succeeded/passed\n"; \
	else \
	    printf "\n\n\nTests failed\n"; \
	    exit 1; \
	fi

clean:
	@rm -f *.o
	@rm -f $(shell find . -name "*.o")
	@rm -f ./*log*
	@mkdir -p tmp

clear: clean
	@rm -rf $(shell find ./dist -name '*.a') $(shell find ./tmp)
	@rm -f test
	@mkdir -p tmp
