CC = g++
CAR = gcc-ar
CXXFLAGS = -Wall -O0 -Iinclude/bstd -std=c++14

SOURCES := $(shell find ./src -name "*.cpp")

.PHONY: all

all: build
	@printf "\n===== Linking =====\n"
	@printf "\n"
	@printf "[/] Creating static archive\r"
	@{ $(CAR) rcs bstd.a $(shell find ./tmp/loc -name "*.o") 2>rcs.log; } || { printf "[-] Failed to create static archive\n\n"; cat rcs.log; exit 1; } 
	@printf "[+] Created static archive         \n"
	@mkdir -p dist
	@mv bstd.a dist/
	@$(MAKE) --no-print-directory clean

build:
	mkdir -p tmp
	@printf "\n===== Building =====\n\n"
	@for item in $(SOURCES); do \
		printf "[/] Building $$item\r"; \
		{ $(CC) $(CXXFLAGS) -c $$item 2>log.txt; } || { printf "[-] Failed to build $$item        \n\n"; cat log.txt; exit 1;}; \
		printf "[+] Built $$item          \n"; \
	done
	@mv *.o tmp/

devtest:
	@$(CC) tests/test.cpp dist/bstd.a -Iinclude -o test -O0

clean:
	@rm -f *.o
	@rm -f ./*log*

clear: clean
	@rm -rf $(shell find . -name '.a') $(shell find . -name '.so') $(shell find ./tmp)
	@rm test
