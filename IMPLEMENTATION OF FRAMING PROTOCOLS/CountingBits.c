#include <stdio.h>
#include <stdlib.h>

int* countBits(int n, int* returnSize) {
    *returnSize = n + 1;
    int* ans = (int*)malloc((*returnSize) * sizeof(int));
    for (int i = 0; i <= n; i++) {
        int count = 0;
        int num = i;
        while (num > 0) {
            count += (num % 2); 
            num /= 2;           
        }
        ans[i] = count;
    }
    return ans;
}
/*Input: n = 2
Output: [0,1,1]
Explanation:
0 --> 0
1 --> 1
2 --> 10
Input: n = 5
Output: [0,1,1,2,1,2]
Explanation:
0 --> 0
1 --> 1
2 --> 10
3 --> 11
4 --> 100
5 --> 101*/
