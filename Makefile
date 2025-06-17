CC         = clang-20
CFLAGS     = -Wall -Wextra -Werror -Os -flto
FFLAGS     = -flto -march=native

SRC  = ./src
BIN_DIR = ./bin

FILES  = $(wildcard $(SRC)/*)
SRCS   = $(filter %.c, $(FILES))

.PHONY: main

all: main

main: $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(FFLAGS)

.PHONY:
clean:
	rm -rf $(BIN_DIR)/*
