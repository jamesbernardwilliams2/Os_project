#include "DiskScheduler.h"
#include <algorithm>
#include <cmath>

DiskScheduler::DiskScheduler(int numTracks)
    : tracks(numTracks) {}

ScheduleResult DiskScheduler::run(DiskAlgorithm algo,
                                  const std::vector<int>& requests,
                                  int startTrack) {
    switch (algo) {
        case DiskAlgorithm::SSTF:
            return sstf(requests, startTrack);
        case DiskAlgorithm::SCAN:
            return scan(requests, startTrack, false);
        case DiskAlgorithm::CSCAN:
            return scan(requests, startTrack, true);
        case DiskAlgorithm::LOOK:
            return look(requests, startTrack);
        default:
            return {0, {}};
    }
}

ScheduleResult DiskScheduler::sstf(const std::vector<int>& requests, int start) const {
    std::vector<int> pending = requests;
    std::vector<int> order;
    int total = 0;
    int head = start;

    while (!pending.empty()) {
        auto closest = std::min_element(pending.begin(), pending.end(),
            [&](int a, int b) {
                return std::abs(a - head) < std::abs(b - head);
            });

        total += std::abs(*closest - head);
        head = *closest;
        order.push_back(head);
        pending.erase(closest);
    }

    return { total, order };
}

ScheduleResult DiskScheduler::scan(const std::vector<int>& requests,
                                   int start, bool circular) const {
    std::vector<int> left, right, order;

    for (int r : requests) {
        if (r < start) left.push_back(r);
        else right.push_back(r);
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    int total = 0;
    int head = start;

    // Move right first
    for (int r : right) {
        total += std::abs(r - head);
        head = r;
        order.push_back(r);
    }

    if (circular) {
        total += std::abs((tracks - 1) - head);
        head = 0;
    } else if (!left.empty()) {
        total += std::abs(head - left.back());
        head = left.back();
    }

    // Now go left
    for (auto it = left.rbegin(); it != left.rend(); ++it) {
        total += std::abs(*it - head);
        head = *it;
        order.push_back(head);
    }

    return { total, order };
}

ScheduleResult DiskScheduler::look(const std::vector<int>& requests, int start) const {
    std::vector<int> left, right, order;

    for (int r : requests) {
        if (r < start) left.push_back(r);
        else right.push_back(r);
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    int total = 0;
    int head = start;

    for (int r : right) {
        total += std::abs(r - head);
        head = r;
        order.push_back(r);
    }

    for (auto it = left.rbegin(); it != left.rend(); ++it) {
        total += std::abs(*it - head);
        head = *it;
        order.push_back(head);
    }

    return { total, order };
}
