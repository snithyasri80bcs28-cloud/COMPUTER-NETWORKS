#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STOP 1000000

typedef struct Node {
    int val;
    struct Node* next;
} Node;

int numBusesToDestination(int** routes, int routesSize, int* routesColSize, int source, int target) {
    if (source == target) return 0;

    int max_stop = 0;
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            if (routes[i][j] > max_stop) {
                max_stop = routes[i][j];
            }
        }
    }
    if (source > max_stop || target > max_stop) return -1;

    Node** stopToBuses = (Node**)calloc(max_stop + 1, sizeof(Node*));
    int total_nodes = 0;
    for (int i = 0; i < routesSize; i++) {
        total_nodes += routesColSize[i];
    }
    Node* pool = (Node*)malloc(sizeof(Node) * total_nodes);
    int poolIdx = 0;

    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            int stop = routes[i][j];
            Node* newNode = &pool[poolIdx++];
            newNode->val = i;
            newNode->next = stopToBuses[stop];
            stopToBuses[stop] = newNode;
        }
    }

    int* busVisited = (int*)calloc(routesSize, sizeof(int));
    int* stopVisited = (int*)calloc(max_stop + 1, sizeof(int));

    int* queue = (int*)malloc(sizeof(int) * (max_stop + 1));
    int head = 0, tail = 0;

    queue[tail++] = source;
    stopVisited[source] = 1;

    int count = 0;

    while (head < tail) {
        int sz = tail - head;
        for (int i = 0; i < sz; i++) {
            int currStop = queue[head++];

            if (currStop == target) {
                free(stopToBuses);
                free(pool);
                free(busVisited);
                free(stopVisited);
                free(queue);
                return count;
            }

            Node* busNode = stopToBuses[currStop];
            while (busNode != NULL) {
                int bus = busNode->val;
                if (!busVisited[bus]) {
                    busVisited[bus] = 1;
                    for (int j = 0; j < routesColSize[bus]; j++) {
                        int nextStop = routes[bus][j];
                        if (!stopVisited[nextStop]) {
                            stopVisited[nextStop] = 1;
                            queue[tail++] = nextStop;
                        }
                    }
                }
                busNode = busNode->next;
            }
        }
        count++;
    }

    free(stopToBuses);
    free(pool);
    free(busVisited);
    free(stopVisited);
    free(queue);

    return -1;
}
/*
Input: routes = [[1,2,7],[3,6,7]], source = 1, target = 6
Output: 2
  Input: routes = [[7,12],[4,5,15],[6],[15,19],[9,12,13]], source = 15, target = 12
Output: -1*/
