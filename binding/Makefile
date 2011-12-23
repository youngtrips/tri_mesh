all:
	gcc -fpic -c mesh_object.c ../src/mesh.c ../src/queue.c ../src/utils.c -I/usr/include/python2.6 -I../src/
	gcc -shared -o mesh.so mesh_object.o mesh.o queue.o utils.o

clean:
	rm *.o -f
	rm mesh.so -f

