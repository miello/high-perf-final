#ifndef _POWERGRID_BRUTEFORCE
#define _POWERGRID_BRUTEFORCE

#include <vector>
#include <climits>
#include <algorithm>
#include "utils.h"

using std::vector;
using std::min;
using std::pair;
using std::greater;
using std::sort;

int calculate_branching(
    int remaining, 
    vector<int> choose, 
    vector<pair<int, int>> degs
) {
    int low = 0;
    sort(degs.begin(), degs.end(), greater<pair<int, int>>());

    for (auto &i: degs) {
        if (remaining <= 0) break;
        if (choose[i.second] != -1) continue;
        
        remaining -= i.first;
        ++low;
    }

    if (remaining > 0) return -1;

    return low;
}

void bruteforce_helper(
    int idx, 
    int N, 
    int cnt,
    int remaining,
    vector<int> &choose,
    vector<int> &selected,
    vector<vector<int>> &edges,
    vector<pair<int, int>> &degs,
    pair<int, vector<int>> &ans
) {
    if (remaining <= 0) {
        if (ans.first > cnt) ans = { cnt, choose };
        return;
    }

    if (idx == N) return;
    
    int branching = calculate_branching(remaining, choose, degs);
    int lower_bound = cnt + branching;
    
    if (branching == -1 || ans.first <= lower_bound) return;
    
    // Selected

    remaining -= !selected[idx];
    ++selected[idx];
    --degs[idx].first;

    bool need_walk = false;

    for (auto &i: edges[idx]) {  
        remaining -= !selected[i];
        need_walk |= !selected[i];
        ++selected[i];
        --degs[i].first;
    }

    // Recurrence

    choose[idx] = 1;
    if (need_walk) bruteforce_helper(idx + 1, N, cnt + 1, remaining, choose, selected, edges, degs, ans);
    choose[idx] = -1;

    // Recover state

    --selected[idx];
    remaining += !selected[idx];
    ++degs[idx].first;
    
    for (auto &i: edges[idx]) {
        --selected[i];
        remaining += !selected[i];
        ++degs[i].first;
    }

    // Not Selected

    choose[idx] = 0;
    bruteforce_helper(idx + 1, N, cnt, remaining, choose, selected, edges, degs, ans);
}

void bruteforce_solve(int N, vector<vector<int>> &edges, pair<int, vector<int>> &ans) {
    vector<int> selected(N, 0), choose(N, -1);
    vector<pair<int, int>> degs(N);

    for (int i = 0; i < N; ++i) degs[i] = { edges[i].size(), i };
    
    bruteforce_helper(0, N, 0, N, choose, selected, edges, degs, ans);
}

#endif