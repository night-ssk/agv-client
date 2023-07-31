# 声明变量
CC=arm-buildroot-linux-gnueabihf-gcc
#CC = gcc
CFLAGS=-I.

# 定义目标和依赖关系
can: can.o can_open.o can_init.o can_send.o can_recv.o tcp_send.o
	$(CC) -g  -pthread -static -o can can.o can_open.o can_init.o can_send.o can_recv.o  tcp_send.o
	rm -f *.o
can.o: can.c can.h 
	$(CC) -g  -c can.c $(CFLAGS)
can_open.o: can_open.c can_open.h 
	$(CC) -g  -c can_open.c $(CFLAGS)
can_init.o: can_init.c can.h 
	$(CC) -g  -c can_init.c $(CFLAGS)
can_send.o: can_send.c can.h
	$(CC) -g  -c can_send.c $(CFLAGS)
can_recv.o: can_recv.c can.h
	$(CC) -g  -c can_recv.c $(CFLAGS)
tcp_send.o: tcp_send.c tcp.h
	$(CC) -g  -c tcp_send.c $(CFLAGS)


# 清除生成的文件
clean:
	rm -f *.o can