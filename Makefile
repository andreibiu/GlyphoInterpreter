CC     := c++
CFLAGS := -std=c++17 -masm=intel -Wno-attributes -O3
SRC    := ./src/Glypho.cpp
EXEC   := glypho-interpreter

.PHONY: clean test

all: build

build: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: build
	./$(EXEC) $(source) $(base)

clean:
	rm -f *.o $(EXEC)

test:
	./test/test.sh
