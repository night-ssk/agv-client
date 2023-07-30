# 声明变量
CC=arm-buildroot-linux-gnueabihf-gcc
#CC = gcc
CFLAGS=-I.

# 定义目标和依赖关系
can: can.o can_open.o
	$(CC) -g  -static -o can can.o can_open.o

can.o: can.c can_open.h
	$(CC) -g  -static -c can.c $(CFLAGS)

can_open.o: can_open.c can_open.h
	$(CC) -g  -static -c can_open.c $(CFLAGS)

# 清除生成的文件
clean:
	rm -f *.o can