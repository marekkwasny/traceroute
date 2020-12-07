traceroute: main.c
	gcc -std=gnu99 -Wall -Wextra -c main.c utils.c send.c receive.c
	gcc -std=gnu99 -Wall -Wextra -o traceroute main.o utils.o send.o receive.o

clean:
	rm -rf *.o

distclean:
	rm -rf *.o traceroute