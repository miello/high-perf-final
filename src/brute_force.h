#ifndef _POWERGRID_BRUTEFORCE
#define _POWERGRID_BRUTEFORCE

#include <vector>
#include <climits>
#include "utils.h"

using std::vector;
using std::min;
using std::pair;

void bruteforce_helper(
    int idx, 
    int N, 
    int cnt, 
    int remaining,
    vector<bool> &choose,
    vector<int> &selected, 
    vector<vector<int>> &edges,
    pair<int, vector<bool>> &ans
) {
    if (ans.first <= cnt) return;
    if (remaining == 0) {
        if (ans.first > cnt) ans = { cnt, choose };
        return;
    }
    if (idx == N) return;

    bruteforce_helper(idx + 1, N, cnt, remaining, choose, selected, edges, ans);

    if (!selected[idx]) --remaining;
    ++selected[idx];

    for (int i = 0; i < N; ++i) {
        if (edges[idx][i]) {
            if (!selected[i]) --remaining;
            ++selected[i];
        }
    }

    choose[idx] = true;
    bruteforce_helper(idx + 1, N, cnt + 1, remaining, choose, selected, edges, ans);
    choose[idx] = false;

    --selected[idx];
    for (int i = 0; i < N; ++i) {
        if (edges[idx][i]) --selected[i];
    }

}

vector<bool> bruteforce_solve(int N, vector<vector<int>> &edges) {
    vector<int> selected(N, 0);
    vector<bool> choose(N, 0);
    pair<int, vector<bool>> ans({ INT_MAX, vector<bool>() });
    
    bruteforce_helper(0, N, 0, N, choose, selected, edges, ans);

    return ans.second;
}

#endif