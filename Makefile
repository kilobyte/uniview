CFLAGS=-Wall -Werror

uni: uni.o getkey.o tos.o
	gcc $(CFLAGS) -o $@ $^

.c.o:
	gcc $(CFLAGS) -c $<

uni.o: getkey.h tos.h
getkey.o: getkey.h
tos.o: tos.h

clean:
	rm -f *.o uni
