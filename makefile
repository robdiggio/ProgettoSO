CC=gcc
CCOPTS=--std=gnu99 -Wall
AR=ar

OBJS=top.o\
	top_info.o
	 
HEADERS=top_info.h

LIBS=libtop.a

BINS=top_test

all: $(LIBS) $(BINS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<
	
libtop.a: $(OBJS) 
	$(AR) -rcs $@ $^
	$(RM) $(OBJS)
	
top_test:	top.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^
	
clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
