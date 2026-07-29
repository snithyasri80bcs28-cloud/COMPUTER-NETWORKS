#include <stdlib.h>

// Helper comparison function for qsort to remove duplicate prefixes quickly
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int findMaximumXOR(int* nums, int numsSize) {
    int maxXOR = 0;
    int mask = 0;
    
    // Allocate space to store unique prefixes for the current bit level
    int* prefixes = (int*)malloc(numsSize * sizeof(int));

    // Iterate from the most significant bit (30) down to the least significant bit (0)
    for (int i = 30; i >= 0; i--) {
        // Set the i-th bit in the mask
        mask |= (1 << i);
        
        // Extract the prefixes for all numbers up to the i-th bit
        for (int j = 0; j < numsSize; j++) {
            prefixes[j] = nums[j] & mask;
        }
        
        // Sort prefixes to easily filter duplicates and perform fast binary search
        qsort(prefixes, numsSize, sizeof(int), compare);
        
        // Compress array to unique elements
        int uniqueSize = 0;
        for (int j = 0; j < numsSize; j++) {
            if (uniqueSize == 0 || prefixes[j] != prefixes[uniqueSize - 1]) {
                prefixes[uniqueSize++] = prefixes[j];
            }
        }

        // Greedily assume the i-th bit can be set to 1 in our max XOR result
        int candidateXOR = maxXOR | (1 << i);
        
        // Check if this candidate is achievable using the property: If A ^ B = C, then A ^ C = B
        for (int j = 0; j < uniqueSize; j++) {
            int target = prefixes[j] ^ candidateXOR;
            
            // Perform an optimized Binary Search to find 'target' in our unique prefixes
            int low = 0, high = uniqueSize - 1;
            int found = 0;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (prefixes[mid] == target) {
                    found = 1;
                    break;
                } else if (prefixes[mid] < target) {
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            
            if (found) {
                maxXOR = candidateXOR;
                break; // Found a valid pair, move to the next bit
            }
        }
    }

    free(prefixes);
    return maxXOR;
}
/*Input: nums = [3,10,5,25,2,8]
Output: 28
Explanation: The maximum result is 5 XOR 25 = 28.
Input: nums = [14,70,53,83,49,91,36,80,92,51,66,70]
Output: 127 */
