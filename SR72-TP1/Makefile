SOURCE_DIRECTORY=src
OUT_DIRECTORY=dist
VERSION=1.0.0

COMPILER=gcc
COMPILATION_FLAGS=-W -I -Wall -pedantic
LINKING_FLAFS=
SOURCE_FILES=$(shell find . -name "*.c")
OBJECT_FILES=$(SOURCE_FILES:.c=.o)

all: program-$(VERSION) clean

program-$(VERSION): $(OBJECT_FILES)
	mkdir -p $(OUT_DIRECTORY)
	$(COMPILER) -o $(OUT_DIRECTORY)/$@ $^ $(LINKING_FLAFS)

%.o: %.c
	$(COMPILER) -o $@ -c $< $(COMPILATION_FLAGS)

.PHONY: clean

clean:
	rm $(OBJECT_FILES)