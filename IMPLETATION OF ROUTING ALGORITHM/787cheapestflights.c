#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int findCheapestPrice(int n, int** flights, int flightsSize, int* flightsColSize, int src, int dst, int k) {
    int* dist = (int*)malloc(sizeof(int) * n);
    int* temp = (int*)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        dist[i] = 1e9;
    }
    dist[src] = 0;

    for (int i = 0; i <= k; i++) {
        for (int j = 0; j < n; j++) {
            temp[j] = dist[j];
        }

        for (int j = 0; j < flightsSize; j++) {
            int u = flights[j][0];
            int v = flights[j][1];
            int w = flights[j][2];

            if (dist[u] != 1e9 && dist[u] + w < temp[v]) {
                temp[v] = dist[u] + w;
            }
        }

        for (int j = 0; j < n; j++) {
            dist[j] = temp[j];
        }
    }

    int result = dist[dst];
    free(dist);
    free(temp);

    return result == 1e9 ? -1 : result;
}
/*
Input: n = 4, flights = [[0,1,100],[1,2,100],[2,0,100],[1,3,600],[2,3,200]], src = 0, dst = 3, k = 1
Output: 700
Input: n = 3, flights = [[0,1,100],[1,2,100],[0,2,500]], src = 0, dst = 2, k = 1
Output: 200*/
