SOURCE_DIRECTORY=src
OUT_DIRECTORY=dist
PROGRAM_NAME=program
VERSION=0.1.0

COMPILER=gcc
COMPILATION_FLAGS=-W -I -Wall -pedantic
LINKING_FLAGS=
SOURCE_FILES=$(shell find . -name "*.c")
OBJECT_FILES=$(SOURCE_FILES:.c=.o)

all: $(PROGRAM_NAME)-v$(VERSION) clean

$(PROGRAM_NAME)-v$(VERSION): $(OBJECT_FILES)
	@mkdir -p $(OUT_DIRECTORY)
	@echo "NOTICE: compile"
	@$(COMPILER) -o $(OUT_DIRECTORY)/$@ $^ $(LINKING_FLAGS)

%.o: %.c
	@$(COMPILER) -o $@ -c $< $(COMPILATION_FLAGS)

.PHONY: clean

clean:
	@echo "NOTICE: clean"
	@rm -f $(OBJECT_FILES)