#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "DiskScheduler.h"
#include "FileSystem.h"

void runDiskDemo() {
    int numTracks = 200;
    DiskScheduler scheduler(numTracks);

    // Example request sequence
    std::vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int start = 53;

    std::cout << "\n=== Disk Scheduling Demo ===\n";
    std::cout << "Requests: ";
    for (int r : requests) std::cout << r << " ";
    std::cout << "\nStart head position: " << start << "\n\n";

    auto runAlgo = [&](DiskAlgorithm algo, const std::string& name) {
        ScheduleResult res = scheduler.run(algo, requests, start);
        std::cout << name << " -> total head movement = "
                  << res.totalMovement << " tracks, order: ";
        for (int r : res.serviceOrder) std::cout << r << " ";
        std::cout << "\n";
    };

    runAlgo(DiskAlgorithm::SSTF,  "SSTF");
    runAlgo(DiskAlgorithm::SCAN,  "SCAN");
    runAlgo(DiskAlgorithm::CSCAN, "C-SCAN");
    runAlgo(DiskAlgorithm::LOOK,  "LOOK");
}

void benchmarkDiskAlgorithms() {
    std::cout << "\n=== Disk Scheduling Benchmark ===\n";

    const int numTracks = 500;
    const int numRequests = 1000;
    DiskScheduler scheduler(numTracks);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::vector<int> requests(numRequests);
    for (int i = 0; i < numRequests; ++i) {
        requests[i] = std::rand() % numTracks;
    }
    int start = numTracks / 2;

    auto measure = [&](DiskAlgorithm algo, const std::string& name) {
        clock_t t0 = std::clock();
        ScheduleResult res = scheduler.run(algo, requests, start);
        clock_t t1 = std::clock();
        double ms = 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
        std::cout << name << ": movement = " << res.totalMovement
                  << ", time = " << ms << " ms\n";
    };

    measure(DiskAlgorithm::SSTF,  "SSTF");
    measure(DiskAlgorithm::SCAN,  "SCAN");
    measure(DiskAlgorithm::CSCAN, "C-SCAN");
    measure(DiskAlgorithm::LOOK,  "LOOK");
}

void runFileSystemDemo() {
    std::cout << "\n=== Simple File System Demo ===\n";

    // 100 blocks, 512 bytes each
    SimpleFileSystem fs(100, 512);

    fs.createFile("report.txt", 1500, AllocationType::CONTIGUOUS);
    fs.createFile("data.bin",   4096, AllocationType::INDEXED);

    std::cout << "\nDirectory after create:\n";
    fs.listFiles();

    fs.writeFile("report.txt", "Hello OS project!");
    fs.writeFile("data.bin",   "Binary stuff");

    std::cout << "\nReading from report.txt -> " << fs.readFile("report.txt");
    std::cout << "Modifying report.txt...\n";
    fs.modifyFile("report.txt", "New contents");

    std::cout << "\nDeleting report.txt...\n";
    fs.deleteFile("report.txt");

    std::cout << "\nDirectory at end:\n";
    fs.listFiles();
}

int main() {
    while (true) {
        std::cout << "\n==== OS Project Menu ====\n";
        std::cout << "1. Run disk scheduling demo\n";
        std::cout << "2. Run file system demo\n";
        std::cout << "3. Benchmark disk algorithms\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) break;

        switch (choice) {
            case 1: runDiskDemo();          break;
            case 2: runFileSystemDemo();    break;
            case 3: benchmarkDiskAlgorithms(); break;
            case 0: return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}
