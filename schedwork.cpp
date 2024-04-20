#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include <functional>
#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Prototype declaration for findSchedule to resolve 'not declared in scope' issue
bool findSchedule(int day, const AvailabilityMatrix& avail, const size_t dailyNeed,
                  const size_t maxShifts, DailySchedule& sched, vector<int>& shiftsCount);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
) {
    if (avail.empty() || avail[0].empty()) {
        cout << "Error: Availability matrix is empty or improperly initialized." << endl;
        return false;
    }

    vector<int> shiftsCount(avail[0].size(), 0);
    sched.clear();
    return findSchedule(0, avail, dailyNeed, maxShifts, sched, shiftsCount);
}

bool findSchedule(int day, const AvailabilityMatrix& avail, const size_t dailyNeed,
                  const size_t maxShifts, DailySchedule& sched, vector<int>& shiftsCount) {
    // cout << "Entering day " << day << " with dailyNeed " << dailyNeed << " and maxShifts " << maxShifts << endl;
    if (day == avail.size()) {
        // cout << "All days scheduled successfully." << endl;
        return true;
    }

    vector<Worker_T> availableWorkers;
    for (size_t i = 0; i < avail[day].size(); ++i) {
        if (avail[day][i] && shiftsCount[i] < maxShifts) {
            availableWorkers.push_back(i);
        }
    }

    // cout << "Available workers for day " << day << ": ";
    // for (auto w : availableWorkers) cout << w << " ";
    // cout << endl;

    if (availableWorkers.size() < dailyNeed) {
        // cout << "Not enough workers available for day " << day << ". Needed: " << dailyNeed << ", available: " << availableWorkers.size() << ". Backtracking..." << endl;
        return false;
    }

    vector<Worker_T> comb;
    function<bool(size_t, size_t)> backtrack = [&](size_t start, size_t k) {
        if (k == 0) {
            if (comb.size() == dailyNeed) {
                // cout << "Trying combination for day " << day << ": ";
                // for (auto w : comb) cout << w << " ";
                // cout << endl;

                sched.push_back(comb);
                for (Worker_T worker : comb) shiftsCount[worker]++;
                if (findSchedule(day + 1, avail, dailyNeed, maxShifts, sched, shiftsCount)) {
                    return true;
                }
                for (Worker_T worker : comb) shiftsCount[worker]--;
                sched.pop_back();
            }
            return false;
        }
        for (size_t i = start; i <= availableWorkers.size() - k; ++i) {
            comb.push_back(availableWorkers[i]);
            if (backtrack(i + 1, k - 1)) {
                return true;
            }
            comb.pop_back();
        }
        return false;
    };

    return backtrack(0, dailyNeed);
}
