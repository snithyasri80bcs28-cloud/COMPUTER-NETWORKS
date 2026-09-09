#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct {
    int row;
    int col;
    int effort;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->data = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->data[left].effort < minHeap->data[smallest].effort)
        smallest = left;

    if (right < minHeap->size && minHeap->data[right].effort < minHeap->data[smallest].effort)
        smallest = right;

    if (smallest != idx) {
        swap(&minHeap->data[idx], &minHeap->data[smallest]);
        minHeapify(minHeap, smallest);
    }
}

void push(MinHeap* minHeap, HeapNode node) {
    if (minHeap->size == minHeap->capacity) return;

    int idx = minHeap->size;
    minHeap->data[idx] = node;
    minHeap->size++;

    while (idx != 0 && minHeap->data[(idx - 1) / 2].effort > minHeap->data[idx].effort) {
        swap(&minHeap->data[idx], &minHeap->data[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

HeapNode pop(MinHeap* minHeap) {
    if (minHeap->size <= 0) {
        HeapNode empty = {-1, -1, -1};
        return empty;
    }
    if (minHeap->size == 1) {
        minHeap->size--;
        return minHeap->data[0];
    }

    HeapNode root = minHeap->data[0];
    minHeap->data[0] = minHeap->data[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);

    return root;
}

int minimumEffortPath(int** heights, int heightsSize, int* heightsColSize) {
    int rows = heightsSize;
    int cols = heightsColSize[0];

    int** efforts = (int**)malloc(sizeof(int*) * rows);
    for (int i = 0; i < rows; i++) {
        efforts[i] = (int*)malloc(sizeof(int) * cols);
        for (int j = 0; j < cols; j++) {
            efforts[i][j] = INT_MAX;
        }
    }

    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    MinHeap* minHeap = createMinHeap(rows * cols);
    
    efforts[0][0] = 0;
    HeapNode start = {0, 0, 0};
    push(minHeap, start);

    while (minHeap->size > 0) {
        HeapNode curr = pop(minHeap);
        int r = curr.row;
        int c = curr.col;
        int currentEffort = curr.effort;

        if (r == rows - 1 && c == cols - 1) {
            for (int i = 0; i < rows; i++) free(efforts[i]);
            free(efforts);
            free(minHeap->data);
            free(minHeap);
            return currentEffort;
        }

        if (currentEffort > efforts[r][c]) continue;

        for (int i = 0; i < 4; i++) {
            int newR = r + dRow[i];
            int newC = c + dCol[i];

            if (newR >= 0 && newR < rows && newC >= 0 && newC < cols) {
                int nextEffort = fmax(currentEffort, abs(heights[r][c] - heights[newR][newC]));

                if (nextEffort < efforts[newR][newC]) {
                    efforts[newR][newC] = nextEffort;
                    HeapNode nextNode = {newR, newC, nextEffort};
                    push(minHeap, nextNode);
                }
            }
        }
    }

    return 0;
}
/*
Input: heights = [[1,2,2],[3,8,2],[5,3,5]]
Output: 2
  Input: heights = [[1,2,3],[3,8,4],[5,3,5]]
Output: 1*/
