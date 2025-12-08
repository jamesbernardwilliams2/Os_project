#define CATCH_CONFIG_MAIN
#include "third_party/catch.hpp"
#include "DiskScheduler.h"
#include "FileSystem.h"

TEST_CASE("SSTF services all requests", "[disk]") {
    DiskScheduler scheduler(200);
    std::vector<int> req = {98, 183, 37, 122, 14, 124, 65, 67};
    int start = 53;

    ScheduleResult res = scheduler.run(DiskAlgorithm::SSTF, req, start);

    REQUIRE(res.serviceOrder.size() == req.size());
    REQUIRE(res.totalMovement > 0);
}

TEST_CASE("SimpleFileSystem create and delete", "[fs]") {
    SimpleFileSystem fs(50, 512);

    REQUIRE(fs.createFile("a.txt", 1000, AllocationType::CONTIGUOUS));
    REQUIRE(fs.createFile("b.bin", 2000, AllocationType::INDEXED));

    REQUIRE_FALSE(fs.createFile("a.txt", 500, AllocationType::CONTIGUOUS)); // duplicate

    REQUIRE(fs.deleteFile("a.txt"));
    REQUIRE_FALSE(fs.deleteFile("a.txt")); // already deleted
}
