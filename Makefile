CC = gcc
CFLAGS = -Wall

SRCS = vsfsck.c superblock.c inode.c bitmap.c block_checker.c
OBJS = $(SRCS:.c=.o)

vsfsck: $(OBJS)
	$(CC) $(CFLAGS) -o vsfsck $(OBJS)

clean:
	rm -f vsfsck *.o
