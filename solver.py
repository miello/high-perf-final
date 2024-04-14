#!/usr/bin/python3
from pulp import *

import io, os, sys

if len(sys.argv) != 3:
    print("Usage: python3 base_solver.py <input_file> <output_file>")
    sys.exit(1)

sys.stdin = open(sys.argv[1], 'r')
sys.stdout = open(sys.argv[2], 'w')

# input = io.BytesIO(os.read(0,os.fstat(0).st_size)).readline

N = int(input())
M = int(input())

graph = [[0.0 for _ in range(N)] for _ in range(N)]
vertex = [LpVariable(f"x{i}", 0, 1, LpInteger) for i in range(N)]

for i in range(M):
    [u, v] = list(map(int,input().strip().split()))
    graph[u][v] = 1.0
    graph[v][u] = 1.0

for i in range(N):
    graph[i][i] = 1.0

vertex_cover = LpProblem("vertex_cover_model", LpMinimize)
vertex_cover += lpSum(vertex)

for i in range(N):
    vertex_cover += lpSum([graph[i][j] * vertex[j] for j in range(N)]) >= 1

vertex_cover.solve(PULP_CBC_CMD(msg=0))

for i in range(N):
    print(int(value(vertex[i])), end="")
sys.stdout.close()