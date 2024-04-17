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

int calculate_branching(
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

bool is_still_cover(
    int N,
    vector<int> &selected,
    vector<int> &degs)
{
    for (int i = 0; i < N; ++i)
    {
        if (selected[i] + degs[i] == 0)
            return false;
    }

    return true;
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

    if (idx == N || !is_still_cover(N, selected, out_degs)) return;

    int branching = calculate_branching(remaining, choose, degs);
    int lower_bound = cnt + branching;

    if (branching == -1 || ans.first <= lower_bound)
        return;

    if (branching == N - idx) {
        #pragma omp critical(assigned)
        {
            if (lower_bound <= ans.first) {
                for (int i = idx; i < N; ++i) choose[vertex[i]] = 1;
                
                ans = { lower_bound, choose };

                for (int i = idx; i < N; ++i) choose[vertex[i]] = -1;
            }
        }
        return;
    }

    auto v = vertex[idx];

    // Selected
    if (pre_choose[v] != 0)
    {
        bool need_walk = false;

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

        choose[v] = 1;

        // Recurrence

        if (need_walk)
            bruteforce_helper(idx + 1, N, cnt + 1, remaining, pre_choose, choose, selected, edges, degs, out_degs, vertex, ans);

        // Recover state
        choose[v] = -1;
        --selected[v];
        remaining += !selected[v];

        for (auto &i : edges[v])
        {
            --selected[i];
            remaining += !selected[i];
            ++degs[i].first;
        }
    }

    if (pre_choose[v] != 1)
    {
        // Not Selected
        choose[v] = 0;
        --out_degs[v];
        for (auto &i : edges[v])
        {
            --out_degs[i];
        }

        bruteforce_helper(idx + 1, N, cnt, remaining, pre_choose, choose, selected, edges, degs, out_degs, vertex, ans);

        choose[v] = -1;
        ++out_degs[v];
        for (auto &i : edges[v])
        {
            ++out_degs[i];
        }
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

    #pragma omp parallel for
    for (int i = 0; i < 4; i++)
    {
        vector<int> selected(N, 0), choose(N, -1), pre_choose(N, -1);

        auto _degs = degs;
        auto _out_degs = out_degs;

        pre_choose[vertex[0]] = i & 1;
        pre_choose[vertex[1]] = (i >> 1) & 1;

        bruteforce_helper(0, N, 0, N, pre_choose, choose, selected, edges, _degs, _out_degs, vertex, ans);
    }
}

#endif