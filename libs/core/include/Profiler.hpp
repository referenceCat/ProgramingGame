#ifndef __PROJECTS_PROGRAMINGGAME_LIBS_CORE_INCLUDE_PROFILER_HPP_
#define __PROJECTS_PROGRAMINGGAME_LIBS_CORE_INCLUDE_PROFILER_HPP_

#include <string>
#include <chrono>
#include <vector>
#include <assert.h>
#include <map>

struct Stopwatch {
    enum {
        Running,
        Stopped
    } state;

    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::microseconds elapsed;
};

class Profiler {
    std::map<std::string, Stopwatch> measures;

public:
    static Profiler& instance() {
        static Profiler profiler;
        return profiler;
    }

    void startStopwatch(std::string name) {
        if (measures.contains(name)) {
            assert(measures[name].state == Stopwatch::Stopped);
            measures[name].start = std::chrono::system_clock::now();
            measures[name].state = Stopwatch::Running;
        } else {
            measures[name].elapsed = std::chrono::microseconds(0);
            measures[name].start = std::chrono::system_clock::now();
            measures[name].state = Stopwatch::Running;
        }
    }

    void stopStopwatch(std::string name) {
        assert(measures.contains(name));
        assert(measures[name].state == Stopwatch::Running);
        measures[name].state = Stopwatch::Stopped;
        measures[name].elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - measures[name].start);
    }

    std::chrono::microseconds getElapsed(std::string name) {
        assert(measures.contains(name));
        return measures[name].elapsed;
    }

    std::vector<std::string> getAllNames() {
        std::vector<std::string> keys;
        for (auto const& key : measures)
            keys.push_back(key.first);
        return keys;
    }

    void clearStopwatches() {
        measures.clear();
    }
};
#endif // __PROJECTS_PROGRAMINGGAME_LIBS_CORE_INCLUDE_PROFILER_HPP_