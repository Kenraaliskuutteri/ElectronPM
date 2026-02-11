CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -Iinclude

TARGET = electronpm

SRC = \
src/frontend/main.c \
src/frontend/cli.c \
src/frontend/ui.c \
src/backend/backend.c \
src/backend/install.c \
src/backend/remove.c \
src/backend/list.c \
src/backend/db.c \
src/backend/atom.c \
src/common/fs.c \
src/common/util.c \
src/common/logging.c \

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
