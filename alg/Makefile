linear:linear_programming.c
	cc -g -Wall -o linear_programming.e linear_programming.c
rbtree:rbtree.o
	cc -g -Wall -o rbtree.e rbtree.o
rbtree.o:rbtree.c
	cc -g -c -Wall -o rbtree.o rbtree.c

clean:
	@echo "cleaning project"
	-rm *.e *.o
	@echo "clean project finished"
