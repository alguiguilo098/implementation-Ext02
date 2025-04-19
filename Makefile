
all:
	g++ main.cpp Shell.cpp UtilShell.cpp -o ext2	
clean:
	rm ext2
	rm *.o