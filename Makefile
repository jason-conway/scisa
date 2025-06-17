CC         = clang-20
CFLAGS     = -Wall -Wextra -Werror -Os
CFLAGS_DBG = $(CFLAGS) -g -O0
FFLAGS     = -march=native
FFLAGS_DBG   = -fno-omit-frame-pointer -fno-optimize-sibling-calls

SRC  = ./src
BIN_DIR = ./bin

FILES  = $(wildcard $(SRC)/*)
SRCS   = $(filter %.c, $(FILES))

.PHONY: main

all: main main-dbg

main: $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(FFLAGS)

main-dbg: $(SRCS)
	$(CC) $(CFLAGS_DBG) -o $(BIN_DIR)/$@ $^ $(FFLAGS_DBG)

.PHONY:
clean:
	rm -rf $(BIN_DIR)/*
