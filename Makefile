all: examples

examples: single_mutex dual_mutex thread_safe_check_clang

single_mutex:
	g++ -Wall -o single_mutex examples/single_mutex.cc -lpthread -O3
	
dual_mutex:
	g++ -Wall -o dual_mutex examples/dual_mutex.cc -lpthread -O3
	
thread_safe_check_clang: lib/mutex.o
	clang++ -Iinclude/ -Werror -Wthread-safety -O3 -o thread_safe_check_clang lib/mutex.o examples/thread_safe_check_clang.cc

lib/mutex.o: lib/
	clang++ -Iinclude/ -c -Werror -Wthread-safety -o lib/mutex.o src/mutex.cc

lib/:
	mkdir -p lib/
	
clean:
	rm -rf lib/ single_mutex dual_mutex thread_safe_check_clang
