CC=gcc
CC_=arm-none-linux-gnueabi-gcc
P=-lpthread

all : t1 t2_1 t2_2 t2_3r t3 t4_tcp_C t4_tcp_S t4_udp_C t4_udp_S clear

t1: t1.o
	$(CC_) $^ -o $@
t2_1: t2_1.o
	$(CC_) $^ -o $@
t2_2: t2_2.o
	$(CC_) $^ -o $@
t2_3: t2_3.o
	$(CC_) $^ -o $@
t2_3r: t2_3r.o
	$(CC_) $^ -o $@
t3: t3.o
	$(CC_) $^ -o $@ $(P)
t4_tcp_C: t4_tcp_C.o
	$(CC_) $^ -o $@ $(P)
t4_tcp_S: t4_tcp_S.o
	$(CC) $^ -o $@ $(P)
t4_udp_C: t4_udp_C.o
	$(CC_) $^ -o $@ $(P)
t4_udp_S: t4_udp_S.o
	$(CC) $^ -o $@ $(P)

t1.o: test1.c
	$(CC_) -c $^ -o $*.o
t2_1.o: test2_1.c
	$(CC_) -c $^ -o $*.o
t2_2.o: test2_2.c
	$(CC_) -c $^ -o $*.o
t2_3.o: test2_3.c
	$(CC_) -c $^ -o $*.o
t2_3r.o: test2_3r.c
	$(CC_) -c $^ -o $*.o
t3.o: test3.c
	$(CC_) -c $^ -o $*.o $(P)
t4_tcp_C.o: test4_tcp_Client.c
	$(CC_) -c $^ -o $*.o $(P)
t4_tcp_S.o: test4_tcp_Server.c
	$(CC) -c $^ -o $*.o $(P)
t4_udp_C.o: test4_udp_Client.c
	$(CC_) -c $^ -o $*.o $(P)
t4_udp_S.o: test4_udp_Server.c
	$(CC) -c $^ -o $*.o $(P)

clear:
	-rm -f *.o
