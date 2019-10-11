bminor: main.o scanner.o parser.o
	gcc $^ -o $@ -lm

# This pattern indicates that any .o file depends
# upon the .c file of the same name, and all of the .h files.
# So, if a .o file is needed, it is built automatically.

%.o: %.c *.h
	gcc -c $< -o $@

# Only the files generated by flex and bison need explicit rules.

scanner.c: scanner.flex parser.h
	flex -oscanner.c $<

parser.c parser.h: parser.bison
	bison --defines=parser.h --output=parser.c -v $<

# clean causes all intermediate files to be deleted.

clean:
	rm -f parser.c parser.h parser.output scanner.c *.o bminor

.PHONY: clean
