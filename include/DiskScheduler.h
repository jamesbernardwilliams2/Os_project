#pragma once

#include <vector>

enum class DiskAlgorithm {
    SSTF,
    SCAN,
    CSCAN,
    LOOK
};

struct ScheduleResult {
    int totalMovement;             // total head movement
    std::vector<int> serviceOrder; // order in which requests were serviced
};

class DiskScheduler {
public:
    explicit DiskScheduler(int numTracks);

    ScheduleResult run(DiskAlgorithm algo,
                       const std::vector<int>& requests,
                       int startTrack);

private:
    int tracks;

    ScheduleResult sstf(const std::vector<int>& requests, int start) const;
    ScheduleResult scan(const std::vector<int>& requests, int start, bool circular) const;
    ScheduleResult look(const std::vector<int>& requests, int start) const;
};
