#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    int* dist = (int*)malloc(sizeof(int) * (n + 1));
    for (int i = 1; i <= n; i++) {
        dist[i] = INT_MAX;
    }
    dist[k] = 0;

    for (int i = 1; i < n; i++) {
        int updated = 0;
        for (int j = 0; j < timesSize; j++) {
            int u = times[j][0];
            int v = times[j][1];
            int w = times[j][2];

            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                updated = 1;
            }
        }
        if (!updated) break;
    }

    int max_time = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INT_MAX) {
            free(dist);
            return -1;
        }
        if (dist[i] > max_time) {
            max_time = dist[i];
        }
    }

    free(dist);
    return max_time;
}
/*Input: times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
Output: 2
Input: times = [[1,2,1]], n = 2, k = 1
Output: 1*/
