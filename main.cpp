#include <iostream>
#include <unordered_map>
#include <array>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

void generateUUIDs(std::unordered_map<int, std::array<unsigned char, 16>>& keyValueStore, int start, int end) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = start; i < end; ++i) {
        std::array<unsigned char, 16> rawUUID;
        for (int j = 0; j < 16; ++j) {
            rawUUID[j] = dis(gen);
        }
        keyValueStore[i] = std::move(rawUUID); // Use move semantics to avoid copy
    }
}

int main() {
    const int numThreads = 16;
    const int numUUIDs = 1000000;

    // Create a vector of threads
    std::vector<std::thread> threads;
    std::unordered_map<int, std::array<unsigned char, 16>> keyValueStore;
    keyValueStore.reserve(numUUIDs);  // Pre-allocate space for 1 million elements

    // Divide the work among 16 threads
    auto start = std::chrono::high_resolution_clock::now();
    int blockSize = numUUIDs / numThreads;

    for (int t = 0; t < numThreads; ++t) {
        int startIdx = t * blockSize;
        int endIdx = (t == numThreads - 1) ? numUUIDs : (t + 1) * blockSize;
        threads.push_back(std::thread(generateUUIDs, std::ref(keyValueStore), startIdx, endIdx));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken with 16 threads: " << duration.count() << " seconds" << std::endl;

    return 0;
}
