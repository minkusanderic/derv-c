clean:
	rm -rf ./bin

test:
	mkdir -p bin
	gcc -o bin/test test.c derv.c
	./bin/test
