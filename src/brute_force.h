#ifndef _POWERGRID_BRUTEFORCE
#define _POWERGRID_BRUTEFORCE

#include <vector>
#include <climits>
#include <algorithm>
#include <omp.h>
#include "utils.h"

using std::greater;
using std::min;
using std::pair;
using std::sort;
using std::vector;

inline int calculate_branching(
    int remaining,
    vector<int> &choose,
    vector<pair<int, int>> degs)
{
    int low = 0;
    sort(degs.begin(), degs.end(), greater<pair<int, int>>());

    for (auto &i : degs)
    {
        if (remaining <= 0)
            break;
        if (choose[i.second] != -1)
            continue;

        remaining -= i.first + 1;
        ++low;
    }

    if (remaining > 0)
        return -1;

    return low;
}

enum MutateState {
    FORWARD = 1,
    REVERT = 0
};

inline bool mutate_selected(
    int v,
    MutateState mode,
    int &remaining,
    vector<int> &selected,
    vector<vector<int>> &edges,
    vector<pair<int, int>> &degs
) {
    bool need_walk = false;

    if (mode == MutateState::FORWARD) {
        need_walk |= !selected[v];
        remaining -= !selected[v];
        ++selected[v];

        for (auto &i : edges[v])
        {
            need_walk |= !selected[i];
            remaining -= !selected[i];
            ++selected[i];
            --degs[i].first;
        }
    } else {
        --selected[v];
        remaining += !selected[v];

        for (auto &i : edges[v])
        {
            --selected[i];
            remaining += !selected[i];
            ++degs[i].first;
        }
    }

    return need_walk;
}

bool mutate_not_selected(int v, MutateState mode, vector<int> &out_degs, vector<vector<int>> &edges, vector<int> &selected) {
    bool is_cover = true;

    if (mode == MutateState::FORWARD) {
        --out_degs[v];
        is_cover &= (out_degs[v] + selected[v]) != 0;

        for (auto &i : edges[v]) {
            --out_degs[i];
            is_cover &= (out_degs[i] + selected[i]) != 0;
        }
    } else {
        ++out_degs[v];
        for (auto &i : edges[v]) {
            ++out_degs[i];
        }
    }

    return is_cover;
}

void bruteforce_helper(
    int idx,
    int N,
    int cnt,
    int remaining,
    vector<int> &pre_choose,
    vector<int> &choose,
    vector<int> &selected,
    vector<vector<int>> &edges,
    vector<pair<int, int>> &degs,
    vector<int> &out_degs,
    vector<int> &vertex,
    pair<int, vector<int>> &ans)
{
    if (remaining <= 0)
    {
        #pragma omp critical(assigned)
        {
            if (ans.first > cnt)
                ans = {cnt, choose};
        } 
        return;
    }

    if (idx == N) return;
    
    auto v = vertex[idx];

    if (choose[v] != -1) {
        bruteforce_helper(idx + 1, N, cnt, remaining, pre_choose, choose, selected, edges, degs, out_degs, vertex, ans);
        return;
    }

    int branching = calculate_branching(remaining, choose, degs);
    int lower_bound = cnt + branching;
    
    if (branching == -1 || ans.first <= lower_bound)
        return;

    if (branching == N - idx) {
        choose[vertex[idx]] = 2;
        #pragma omp critical(assigned)
        {
            if (lower_bound < ans.first) {        
                ans = { lower_bound, choose };
            }
        }
        choose[vertex[idx]] = -1;
        return;
    }

    if (pre_choose[v] != 1) {
        // Not Selected
        choose[v] = 0;
        bool is_cover = mutate_not_selected(v, MutateState::FORWARD, out_degs, edges, selected);

        if (is_cover) bruteforce_helper(idx + 1, N, cnt, remaining, pre_choose, choose, selected, edges, degs, out_degs, vertex, ans);

        choose[v] = -1;
        mutate_not_selected(v, MutateState::REVERT, out_degs, edges, selected);
    }

    if (pre_choose[v] != 0) {
        // Selected
        choose[v] = 1;
        bool need_walk = mutate_selected(v, MutateState::FORWARD, remaining, selected, edges, degs);

        // Recurrence

        if (need_walk)
            bruteforce_helper(idx + 1, N, cnt + 1, remaining, pre_choose, choose, selected, edges, degs, out_degs, vertex, ans);

        // Recover state

        choose[v] = -1;
        mutate_selected(v, MutateState::REVERT, remaining, selected, edges, degs);
    }
}

void bruteforce_solve(int N, vector<vector<int>> &edges, pair<int, vector<int>> &ans)
{
    vector<int> vertex(N, 0), out_degs(N, 0);
    vector<pair<int, int>> degs(N);
    vector<pair<int, vector<int>>> ans_vector(4);

    #pragma omp parallel for
    for (int i = 0; i < 4; i++)
        ans_vector[i] = ans;

    #pragma omp parallel for
    for (int i = 0; i < N; ++i)
        degs[i] = {edges[i].size(), i};

    auto tmp = degs;
    sort(tmp.begin(), tmp.end(), greater<pair<int, int>>());

    #pragma omp parallel for
    for (int i = 0; i < N; ++i)
    {
        vertex[i] = tmp[i].second;
        out_degs[i] = degs[i].first + 1;
    }

    vector<int> selected(N, 0), choose(N, -1);
    int cnt = 0, remaining = N;
    for (int i = 0; i < N; ++i) {
        int deg_i = edges[i].size();
        if (deg_i == 0) {
            ++cnt;
            choose[i] = 1;
            mutate_selected(i, MutateState::FORWARD, remaining, selected, edges, degs);
        }
    
        if (deg_i == 1) {
            int other_v = edges[i][0];

            if (choose[other_v] == -1) {
                ++cnt;
                choose[other_v] = 1;
                mutate_selected(other_v, MutateState::FORWARD, remaining, selected, edges, degs);
            }

            if (choose[i] == -1) {
                choose[i] = 0;
                mutate_not_selected(i, MutateState::FORWARD, out_degs, edges, selected);
            }
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < 4; ++i) {
        vector<int> pre_choose(N, -1);

        auto _degs = degs;
        auto _out_degs = out_degs;
        auto _choose = choose;
        auto _selected = selected;

        for (int j = 0, k = 1; j < N && k >= 0; ++j) {
            if (_choose[vertex[j]] == -1) {
                pre_choose[vertex[j]] = (i >> k) & 1;
                --k;
            }
        }
    
        bruteforce_helper(0, N, cnt, remaining, pre_choose, _choose, _selected, edges, _degs, _out_degs, vertex, ans);
    }
}

#endif