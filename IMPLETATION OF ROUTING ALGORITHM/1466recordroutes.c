#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int to;
    int dir;
    struct Edge* next;
} Edge;

void dfs(int node, int parent, Edge** graph, int* ans) {
    Edge* curr = graph[node];
    while (curr != NULL) {
        if (curr->to != parent) {
            *ans += curr->dir;
            dfs(curr->to, node, graph, ans);
        }
        curr = curr->next;
    }
}

int minReorder(int n, int** connections, int connectionsSize, int* connectionsColSize) {
    Edge** graph = (Edge**)calloc(n, sizeof(Edge*));
    Edge* pool = (Edge*)malloc(sizeof(Edge) * 2 * connectionsSize);
    int poolIdx = 0;

    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];

        Edge* e1 = &pool[poolIdx++];
        e1->to = v;
        e1->dir = 1;
        e1->next = graph[u];
        graph[u] = e1;

        Edge* e2 = &pool[poolIdx++];
        e2->to = u;
        e2->dir = 0;
        e2->next = graph[v];
        graph[v] = e2;
    }

    int ans = 0;
    dfs(0, -1, graph, &ans);

    free(graph);
    free(pool);

    return ans;
}
/*
Input: n = 6, connections = [[0,1],[1,3],[2,3],[4,0],[4,5]]
Output: 3
  Input: n = 5, connections = [[1,0],[1,2],[3,2],[3,4]]
Output: 2*/
