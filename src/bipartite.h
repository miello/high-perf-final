#ifndef _POWERGRID_BIPARTITE
#define _POWERGRID_BIPARTITE

#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <omp.h>
#include "utils.h"

using std::greater;
using std::min;
using std::pair;
using std::sort;
using std::vector;
using std::queue;

pair<bool, vector<int>> is_bipartite(int N, vector<vector<int>> &edges) {
    vector<int> color(N, -1);

    for (int i = 0; i < N; ++i) {
        if (color[i] == -1) {
            color[i] = 1;
            queue<int> q;
            q.emplace(i);

            while (q.size()) {
                int v = q.front();
                q.pop();

                for (int &i: edges[v]) {
                    if (color[i] == -1) {
                        color[i] = !color[v];
                        q.emplace(i);
                    } else if (color[i] == color[v]) {
                        return { false, color };
                    }
                }
            }
        }
    }

    return { true, color };
}

bool try_kuhn(int v, vector<vector<int>> &edges, vector<bool> &used, vector<int> &mt) {
    if (used[v])
        return false;
    used[v] = true;
    for (int to : edges[v]) {
        if (mt[to] == -1 || try_kuhn(mt[to], edges, used, mt)) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}

vector<vector<int>> construct_new_edges(int N, vector<int> &color, vector<vector<int>> &edges) {
    vector<vector<int>> new_edges(N, vector<int>());
    for (int i = 0; i < N; ++i) {
        for (int j: edges[i]) {
            if (color[i] == 0 && color[j] == 1) {
                new_edges[i].push_back(j);
            }
        }
    }

    return new_edges;
}

bool solve_bipartite(int N, vector<vector<int>> &edges, pair<int, vector<int>> &ans) {
    auto ans_bipartite = is_bipartite(N, edges);

    if (!ans_bipartite.first) return false;

    auto new_edges = construct_new_edges(N, ans_bipartite.second, edges);

    vector<int> mt(N, -1);
    vector<bool> used(N, false);
    int bi_ans = 0;

    for (int i = 0; i < N; ++i) {
        used.assign(N, false);
        if (try_kuhn(i, new_edges, used, mt)) {
            ++bi_ans;
        }
    }
    
    for (int i = 0; i < N; ++i) {
        if (mt[i] != -1) {
            cout << (mt[i]) << " " << i << std::endl;
        } else {
            cout << i << std::endl;
        }
    }

    cout << bi_ans << std::endl;

    return true;
}

#endif