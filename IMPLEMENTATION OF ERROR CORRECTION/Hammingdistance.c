#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
int main() {
    char inputString[100];
    int data[800];
    int hamming[1000], received[1000];
    int m = 0, r = 0, totalBits;
    int i, j, parity, errorPos = 0;
    printf("HAMMING CODE - STRING TO BINARY & SINGLE BIT ERROR CORRECTION\n");
    printf("\nEnter a sentence: ");
    if (scanf("%99[^\n]", inputString) != 1) {
        printf("Invalid input string.\n");
        return 1;
    }
    int stringLength = strlen(inputString);
    int bitIndex = 1;
    for (i = 0; i < stringLength; i++) {
        char ch = inputString[i];
        for (j = 7; j >= 0; j--) {
            data[bitIndex++] = (ch >> j) & 1;
        }
    }
    m = bitIndex - 1;
    printf("Converted Binary Data (%d bits total):\n", m);
    for (i = 1; i <= m; i++) {
        printf("%d", data[i]);
        if (i % 8 == 0) printf(" "); 
    }
    printf("\n");
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
    totalBits = m + r;
    printf("\nNumber of parity bits = %d", r);
    printf("\nTotal bits (Data + Parity) = %d\n", totalBits);
    j = 1;
    for (i = 1; i <= totalBits; i++) {
        if ((i & (i - 1)) == 0) {
            hamming[i] = 0;
        } else {
            hamming[i] = data[j++];
        }
    }
    for (i = 0; i < r; i++) {
        int pos = (int)pow(2, i);
        parity = 0;
        for (j = 1; j <= totalBits; j++) {
            if (j & pos) {
                parity ^= hamming[j];
            }
        }
        hamming[pos] = parity;
    }
    printf("\nGenerated Hamming Code:\n");
    for (i = 1; i <= totalBits; i++) {
        printf("%d", hamming[i]);
    }
    printf("\n");
    for (i = 1; i <= totalBits; i++) {
        received[i] = hamming[i];
    }
    srand((unsigned int)time(NULL));
    int randomBit = rand() % totalBits + 1;
    received[randomBit] ^= 1;
    printf("\n***************** TRANSMISSION****************");
    printf("\n[ALERT] Bit at position %d was flipped dynamically during transmission.\n", randomBit);
    printf("Received Code Packet:\n");
    for (i = 1; i <= totalBits; i++) {
        printf("%d", received[i]);
    }
    printf("\n");
    errorPos = 0;
    for (i = 0; i < r; i++) {
        int pos = (int)pow(2, i);
        parity = 0;
        for (j = 1; j <= totalBits; j++) {
            if (j & pos) {
                parity ^= received[j];
            }
        }
        if (parity) {
            errorPos += pos;
        }
    }
    printf("\nCalculated Syndrome Value = %d", errorPos);
    if (errorPos == 0) {
        printf("\nNo Error Detected.\n");
    } else {
        printf("\nError detected at bit position %d. Autofixing stream...", errorPos);
        received[errorPos] ^= 1;
        printf("\nCorrected Hamming Code Matrix:\n");
        for (i = 1; i <= totalBits; i++) {
            printf("%d", received[i]);
        }
        printf("\n");
    }
    int recoveredData[800];
    int recIdx = 1;
    for (i = 1; i <= totalBits; i++) {
        if ((i & (i - 1)) != 0) {
            recoveredData[recIdx++] = received[i];
        }
    }
    printf("\nRecovered String Output: ");
    for (i = 0; i < stringLength; i++) {
        char ch = 0;
        for (j = 0; j < 8; j++) {
          ch = (ch << 1) | recoveredData[i * 8 + j + 1];
        }
        printf("%c", ch);
    }
    printf("\n");
    return 0;
}
/*output:
[24bcs017@mepcolinux ex3]$cc hamdis.c -lm
[24bcs017@mepcolinux ex3]$./a.out
HAMMING CODE - STRING TO BINARY & SINGLE BIT ERROR CORRECTION

Enter a sentence: ni*
Converted Binary Data (24 bits total):
01101110 01101001 00101010

Number of parity bits = 5
Total bits (Data + Parity) = 29

Generated Hamming Code:
11001100111001110100100101010

***************** TRANSMISSION****************
[ALERT] Bit at position 5 was flipped dynamically during transmission.
Received Code Packet:
11000100111001110100100101010

Calculated Syndrome Value = 5
Error detected at bit position 5. Autofixing stream...
Corrected Hamming Code Matrix:
11001100111001110100100101010

Recovered String Output: ni*
[24bcs017@mepcolinux ex3]$h~i
bash: h~i: command not found...
[24bcs017@mepcolinux ex3]$./a.out
HAMMING CODE - STRING TO BINARY & SINGLE BIT ERROR CORRECTION

Enter a sentence: h~i
Converted Binary Data (24 bits total):
01101000 01111110 01101001

Number of parity bits = 5
Total bits (Data + Parity) = 29

Generated Hamming Code:
11011100100001101111001101001

***************** TRANSMISSION****************
[ALERT] Bit at position 4 was flipped dynamically during transmission.
Received Code Packet:
11001100100001101111001101001

Calculated Syndrome Value = 4
Error detected at bit position 4. Autofixing stream...
Corrected Hamming Code Matrix:
11011100100001101111001101001

Recovered String Output: h~i
*/
