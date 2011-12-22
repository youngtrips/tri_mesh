all:
	gcc -fpic -c wrap_mesh.c ../src/mesh.c ../src/queue.c ../src/utils.c -I/usr/include/python2.6 -I../src/
	gcc -shared -o mesh.so wrap_mesh.o mesh.o queue.o utils.o

clean:
	rm *.o -f
	rm mesh.so -f

