linker: linker.cpp
	module load gcc-9.2;\
	g++ -g linker.cpp -o linker

clean:
	rm -f linker *~