#include <limits.h>

int divide(int dividend, int divisor) {
    if (dividend == INT_MIN && divisor == -1) {
        return INT_MAX;
    }
    int isNegative = (dividend < 0) ^ (divisor < 0);
    long long abs_dividend = llabs((long long)dividend);
    long long abs_divisor = llabs((long long)divisor);
    long long quotient = 0;
    while (abs_dividend >= abs_divisor) {
        long long temp_divisor = abs_divisor;
        long long multiple = 1;
        while ((temp_divisor << 1) <= abs_dividend) {
            temp_divisor <<= 1;
            multiple <<= 1;
        }
        abs_dividend -= temp_divisor;
        quotient += multiple;
    }
    long long final_result = isNegative ? -quotient : quotient;
    
    if (final_result > INT_MAX) return INT_MAX;
    if (final_result < INT_MIN) return INT_MIN;
    return (int)final_result;
}
/*
Input: dividend = 10, divisor = 3
Output: 3
Input: dividend = 7, divisor = -3
Output: -2*/
