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
        
        remaining -= i.first + 1;
        ++low;
    }

    if (remaining > 0) return -1;

    return low;
}

bool is_still_cover(
    int N,
    vector<int> &selected,
    vector<pair<int, int>> &degs
) {
    for (int i = 0; i < N; ++i) {
        if (selected[i] + degs[i].first == 0) return false;
    }

    return true;
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
    vector<int> &vertex,
    pair<int, vector<int>> &ans
) {
    if (remaining <= 0) {
        if (ans.first > cnt) ans = { cnt, choose };
        return;
    }

    if (idx == N) return;
    if (!is_still_cover(N, selected, degs)) return;
    
    int branching = calculate_branching(remaining, choose, degs);
    int lower_bound = cnt + branching;
    
    if (branching == -1 || branching > N - idx || ans.first <= lower_bound) return;
    
    auto v = vertex[idx];

    // Selected

    bool need_walk = false;

    need_walk |= !selected[v];
    remaining -= !selected[v];
    ++selected[v];

    for (auto &i: edges[v]) {  
        remaining -= !selected[i];
        need_walk |= !selected[i];
        ++selected[i];
        --degs[i].first;
    }

    choose[v] = 1;

    // Recurrence

    if (need_walk) bruteforce_helper(idx + 1, N, cnt + 1, remaining, choose, selected, edges, degs, vertex, ans);

    // Recover state
    choose[v] = -1;

    --selected[v];
    remaining += !selected[v];
    
    for (auto &i: edges[v]) {
        --selected[i];
        remaining += !selected[i];
        ++degs[i].first;
    }

    // Not Selected
    choose[v] = 0;

    bruteforce_helper(idx + 1, N, cnt, remaining, choose, selected, edges, degs, vertex, ans);

    choose[v] = -1;
}

void bruteforce_solve(int N, vector<vector<int>> &edges, pair<int, vector<int>> &ans) {
    vector<int> selected(N, 0), choose(N, -1), vertex(N, 0);
    vector<pair<int, int>> degs(N);

    for (int i = 0; i < N; ++i) degs[i] = { edges[i].size(), i };
    
    auto tmp = degs;
    sort(tmp.begin(), tmp.end(), greater<pair<int, int>>());

    for (int i = 0; i < N; ++i) vertex[i] = tmp[i].second;    

    bruteforce_helper(0, N, 0, N, choose, selected, edges, degs, vertex, ans);
}

#endif