int findComplement(int num) {
    // Start our mask at 1
    unsigned int mask = 1;
    
    // Grow the mask until it is greater than or equal to num
    while (mask < num) {
        mask = (mask << 1) | 1;
    }
    
    // XOR flips the bits where mask has 1s
    return num ^ mask;
}
/*Input: num = 5
Output: 2
Explanation: The binary representation of 5 is 101 (no leading zero bits), and its complement is 010. So you need to output 2.
Input: num = 1
Output: 0
Explanation: The binary representation of 1 is 1 (no leading zero bits), and its complement is 0. So you need to output 0.*/
