#pragma once

#include <string>

namespace HitOnly {

struct GoodCutTotals {
    long long score = 0;
    long long max = 0;
    int count = 0;

    void Add(int cutScore, int maxCutScore) {
        score += cutScore;
        max += maxCutScore;
        count++;
    }
    // Sum of good-cut scores over what those notes could have scored (115 each for normal notes).
    // Returns a negative value before the first good cut.
    float Percent() const { return max > 0 ? 100.0f * score / max : -1.0f; }
};

// Counts for the current song, or the last finished one once back in the menu
struct SongStats {
    GoodCutTotals all;
    // By note color: ColorA (red) is the left saber, ColorB (blue) the right
    GoodCutTotals left;
    GoodCutTotals right;
    int badCuts = 0;
    int misses = 0;
};

extern SongStats currentStats;

// "96.4%", or "--.-%" before any good cut
std::string FormatPercent(float percent);

}  // namespace HitOnly
