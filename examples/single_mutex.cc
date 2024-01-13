#include <thread>
#include <mutex>
#include <iostream>

#define BUFFER_SIZE 1000000

std::mutex mtx;

void some_work(const uint32_t *input, uint32_t *output_ptr){
	uint32_t output = 0;
	for(size_t i = 0; i < BUFFER_SIZE; i++){
		mtx.lock();
		output = (output + input[i]) % 1234;
		mtx.unlock();
	}
	*output_ptr = output;	
}

void some_other_work(const uint32_t *input, uint32_t *output_ptr){
	uint32_t output = 0;
	for(size_t i = 0; i < BUFFER_SIZE; i++){
		mtx.lock();
		output += (output + input[i] + rand()) % 1234;
		mtx.unlock();
	}
	*output_ptr = output;
}

int main(){
	uint32_t buffer1[BUFFER_SIZE];
	uint32_t buffer2[BUFFER_SIZE];
	
	for(size_t i = 0; i < BUFFER_SIZE; i++){
		buffer1[i] = rand();
		buffer2[i] = rand();
	}
	
	for(size_t i = 0; i < 10; i++){
		uint32_t some_work1_out, some_work2_out, other_work1_out, rand2_out;
		
		std::thread work1(some_work, buffer1, &some_work1_out);
		std::thread other_work1(some_other_work, buffer1, &other_work1_out);
		std::thread work2(some_work, buffer2, &some_work2_out);
		std::thread rand2(some_other_work, buffer2, &rand2_out);
		
		work1.join();
		work2.join();
		other_work1.join();
		rand2.join();
		
		printf("%02x %02x %02x %02x\n", some_work1_out, some_work2_out, other_work1_out, rand2_out);
	}
	
	return 0;
}
