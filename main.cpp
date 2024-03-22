#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::vector;

typedef std::pair<int, int> Edge;

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    int N, M;
    std::cin >> N >> M;

    std::vector<int> degree(N);
    std::vector<vector<int>> edges(M);

    for (int i = 0; i < M; ++i)
    {
        int u, v;

        cin >> u >> v;
        
        edges[u][v] = 1;
        edges[v][u] = 1;
    }

}