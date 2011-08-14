SRC=main.c function.c token.c parser.c syntaxtree.c
build: sexc

sexc: $(SRC)
	$(CC) $(CFLAGS) -o sexc $(SRC) 

sexc-debug: $(SRC)
	$(CC) $(CFLAGS) -DDEBUG -o sexc $(SRC)


.PHONY: clean
clean:
	rm sexc


