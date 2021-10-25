build :
	gcc -Wall -Wextra -std=c99 main.c -o gigel

run: build
	./gigel <in >out

clean:
	rm -r gigel
