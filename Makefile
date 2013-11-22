CC=cc
CFLAGS=-Wall -Werror

uniview: uniview.o getkey.o tos.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

uniview.o: getkey.h tos.h
getkey.o: getkey.h
tos.o: tos.h

clean:
	rm -f *.o uniview
