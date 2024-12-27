#include <iostream>
#include <array>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

struct KeyValue {
    int key;
    std::array<unsigned char, 16> value;
};

void generateUUIDs(KeyValue* keyValueStore, int start, int end, std::random_device& rd) {
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = start; i < end; ++i) {
        keyValueStore[i].key = i; // Assign key
        for (int j = 0; j < 16; ++j) {
            keyValueStore[i].value[j] = dis(gen); // Assign random value
        }
    }
}

int main() {
    const int numThreads = 16;
    const int numUUIDs = 1000000;

    // Create an array of key-value pairs
    std::vector<KeyValue> keyValueStore(numUUIDs);

    // Create a random device to be used by each thread
    std::random_device rd;

    // Calculate the range of work for each thread
    auto start = std::chrono::high_resolution_clock::now();
    int blockSize = numUUIDs / numThreads;
    
    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t) {
        int startIdx = t * blockSize;
        int endIdx = (t == numThreads - 1) ? numUUIDs : (t + 1) * blockSize;
        threads.push_back(std::thread(generateUUIDs, keyValueStore.data(), startIdx, endIdx, std::ref(rd)));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken with 16 threads: " << duration.count() << " seconds" << std::endl;
    std::cout << "Size: " << keyValueStore.size() << std::endl;

    return 0;
}

