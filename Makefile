CFLAGS=-Wall -Werror

uniview: uniview.o getkey.o tos.o
	gcc $(CFLAGS) -o $@ $^

.c.o:
	gcc $(CFLAGS) -c $<

uniview.o: getkey.h tos.h
getkey.o: getkey.h
tos.o: tos.h

clean:
	rm -f *.o uniview
