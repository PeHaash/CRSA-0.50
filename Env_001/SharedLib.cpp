#include "algorithm"
#include "SharedLib.h"

double MappedScore(double min_num, double goal_num, double max_num, double status) {
    // the idea is status is in a [min,max] range, and
    // MappedScore(min) = 0
    // MappedScore(max) = 0
    // MappedScore(goal) = 1
    // (because it is peaked in that point)
    // for now, the chart you can see the graph is here: https://www.desmos.com/calculator/xb4xtnndqf
    return std::min(
        (status - goal_num) / (goal_num - min_num),
        (goal_num - status) / (max_num - goal_num)) + 1;
}
