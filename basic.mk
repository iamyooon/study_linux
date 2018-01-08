CFLAGS		+= -Werror
LDFLAGS		+=
OBJ_LIST	= $(patsubst %.c, %.o, $(wildcard *.c))

all : $(OBJ_LIST)
	$(CC) -o example $^ $(LDFLAGS)
clean :
	rm -rf $(OBJ_LIST) example
