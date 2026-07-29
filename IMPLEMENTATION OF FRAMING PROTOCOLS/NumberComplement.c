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
