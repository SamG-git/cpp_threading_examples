#include <thread>
#include <mutex>
#include <iostream>

#define BUFFER_SIZE 1000000


void some_work(const uint32_t *input, uint32_t *output_ptr, std::mutex *mtx){
    uint32_t output = 0;
    for(size_t i = 0; i < BUFFER_SIZE; i++){
        mtx->lock();
        output = (output + input[i]) % 1234;
        mtx->unlock();
    }
    *output_ptr = output;
}

void some_other_work(const uint32_t *input, uint32_t *output_ptr, std::mutex *mtx){
    uint32_t output = 0;
    for(size_t i = 0; i < BUFFER_SIZE; i++){
        mtx->lock();
        output += (output + input[i] + rand()) % 1234;
        mtx->unlock();
    }
    *output_ptr = output;
}

int main(){
    uint32_t buffer1[BUFFER_SIZE];
    uint32_t buffer2[BUFFER_SIZE];

    std::mutex mtx1, mtx2;

    for(size_t i = 0; i < BUFFER_SIZE; i++){
        buffer1[i] = rand();
        buffer2[i] = rand();
    }

    for(size_t i = 0; i < 10; i++){
        uint32_t work1_out, work2_out, other_work1_out, other_work2_out;

        std::thread work1(some_work, buffer1, &work1_out, &mtx1);
        std::thread other_work1(some_other_work, buffer1, &other_work1_out, &mtx1);
        std::thread work2(some_work, buffer2, &work2_out, &mtx2);
        std::thread other_work2(some_other_work, buffer2, &other_work2_out, &mtx2);

        work1.join();
        work2.join();
        other_work1.join();
        other_work2.join();

        printf("%02x %02x %02x %02x\n", work1_out, work2_out, other_work1_out, other_work2_out);
    }

    return 0;
}
