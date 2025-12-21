CC = g++
CAR = gcc-ar
CFLAGS = -Wall -Ofast -Iinclude/bstd

SOURCES := $(shell find ./src -name "*.cpp")
LOCSOURCES := $(shell find ./local -name "*.cpp")

.PHONY: all

all: build
	@printf "\n===== Linking =====\n"
	@printf "\n"
	@printf "[/] Creating static archive\r"
	@{ $(CAR) rcs bstd.a $(shell find ./tmp/loc -name "*.o") 2>rcs.log; } || { printf "[-] Failed to create static archive\n\n"; cat rcs.log; exit 1; } 
	@printf "[+] Created static archive         \n"
	@mkdir -p dist
	@printf "[/] Creating shared/linked binary\r"
	@{ $(CC) $(CLFAGS) --shared -o bstd.so $(shell find ./tmp/dyn -name "*.o") 2>shared.log; } || { printf "[-] Failed to create shared/linked binary\n\n"; cat shared.log; exit 1; }
	@printf "[+] Created shared/linked binary     \n"
	@mv bstd.a dist/
	@mv bstd.so dist/
	@$(MAKE) --no-print-directory clean

build:
	@mkdir -p tmp/dyn
	@mkdir -p tmp/loc
	@printf "===== Building Dynamic =====\n\n"
	@for item in $(SOURCES); do \
		printf "[/] Building $$item\r"; \
		{ $(CC) $(CFLAGS) -c $$item 2>log.txt; } || { printf "[-] Failed to build $$item        \n\n"; cat log.txt; exit 1;}; \
		printf "[+] Built $$item          \n"; \
	done
	@mv *.o tmp/dyn

	@printf "\n===== Building Local =====\n\n"
	@for item in $(LOCSOURCES); do \
		printf "[/] Building $$item\r"; \
		{ $(CC) $(CFLAGS) -c $$item 2>log.txt; } || { printf "[-] Failed to build $$item        \n\n"; cat log.txt; exit 1;}; \
		printf "[+] Built $$item          \n"; \
	done
	@mv *.o tmp/loc

devtest:
	@$(CC) tests/test.cpp dist/bstd.a -Iinclude -o test -O0

clean:
	@rm -f *.o
	@rm -f ./*log*

clear: clean
	@rm -rf $(shell find . -name '.a') $(shell find . -name '.so') $(shell find ./tmp)
	@rm test
