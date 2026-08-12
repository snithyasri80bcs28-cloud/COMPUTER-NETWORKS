#include <stdio.h>
#include <string.h>

#define MAX_TEXT_LEN 100
#define MAX_BITS_LEN 1000

// Helper function to convert a text string to a binary string
void text_to_binary(const char *text, char *binary_out) {
    binary_out[0] = '\0';
    for (int i = 0; text[i] != '\0'; i++) {
        char val = text[i];
        // Convert 1 byte (8 bits) of a character into binary characters '0' and '1'
        for (int j = 7; j >= 0; j--) {
            if ((val >> j) & 1) {
                strcat(binary_out, "1");
            } else {
                strcat(binary_out, "0");
            }
        }
    }
}

// Function to calculate 1s complement checksum for a series of 8-bit blocks
void calculate_checksum(const char *binary_data, int block_size, char *checksum_out) {
    int len = strlen(binary_data);
    int sum[8] = {0}; // Store the column-wise sum of bits for an 8-bit block

    // Step 1: Add all blocks together column by column
    for (int i = 0; i < len; i += block_size) {
        for (int j = 0; j < block_size; j++) {
            sum[j] += (binary_data[i + j] - '0');
        }
    }

    // Step 2: Handle carry overs from right to left (1s complement addition)
    for (int j = block_size - 1; j > 0; j--) {
        if (sum[j] > 1) {
            sum[j - 1] += sum[j] / 2;
            sum[j] = sum[j] % 2;
        }
    }

    // Step 3: Prototypical carry from MSB wraps around to LSB
    while (sum[0] > 1) {
        int carry = sum[0] / 2;
        sum[0] = sum[0] % 2;

        // Add carry to the least significant bit (LSB)
        sum[block_size - 1] += carry;

        // Re-propagate carries from right to left
        for (int j = block_size - 1; j > 0; j--) {
            if (sum[j] > 1) {
                sum[j - 1] += sum[j] / 2;
                sum[j] = sum[j] % 2;
            }
        }
    }

    // Step 4: Complement the sum bits to get the final checksum (0 becomes 1, 1 becomes 0)
    for (int j = 0; j < block_size; j++) {
        checksum_out[j] = (sum[j] == 0) ? '1' : '0';
    }
    checksum_out[block_size] = '\0';
}

int main() {
    char input_text[MAX_TEXT_LEN];
    char binary_data[MAX_BITS_LEN], sender_checksum[MAX_TEXT_LEN];
    char transmitted_bits[MAX_BITS_LEN], received_bits[MAX_BITS_LEN], receiver_checksum[MAX_TEXT_LEN];
    int block_size = 8;
    char choice;
    int bit_position;

    printf("--- CHECKSUM SENDER SIDE ---\n");
    printf("Enter input string text (e.g., ABC): ");
    scanf(" %[^\n]", input_text);

    // Convert string text to binary bits
    text_to_binary(input_text, binary_data);
    printf("Converted text to Binary Data: %s\n", binary_data);

    // Step 1: Calculate Checksum on Sender side
    calculate_checksum(binary_data, block_size, sender_checksum);
    printf("Calculated Sender Checksum: %s\n", sender_checksum);

    // Step 2: Construct Transmitted Codeword
    sprintf(transmitted_bits, "%s%s", binary_data, sender_checksum);
    printf("Transmitted Bitstream (Data + Checksum): %s\n\n", transmitted_bits);

    // --- ERROR INDUCTION SEGMENT ---
    printf("--- ERROR INDUCTION OPTION ---\n");
    // Clone transmitted data to simulate the transit channel
    strcpy(received_bits, transmitted_bits);

    printf("Do you want to induce an error? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        int total_len = strlen(transmitted_bits);
        printf("Enter the bit position to flip (0 to %d): ", total_len - 1);
        scanf("%d", &bit_position);

        if (bit_position >= 0 && bit_position < total_len) {
            // Flip the selected bit ('0' becomes '1' and vice-versa)
            received_bits[bit_position] = (received_bits[bit_position] == '0') ? '1' : '0';
            printf("� Bit flipped at position %d successfully!\n", bit_position);
        } else {
            printf(" Invalid bit position! Sending data without modifications.\n");
        }
    } else {
        printf("No error induced. Transmission running cleanly.\n");
    }
    printf("Final Bitstream reaching Receiver:       %s\n\n", received_bits);

    // --- RECEIVER SIDE ---
    printf("--- CHECKSUM RECEIVER SIDE ---\n");
    // Step 3: Calculate Checksum on Receiver side (includes the sender's checksum block)
    calculate_checksum(received_bits, block_size, receiver_checksum);
    printf("Receiver Checksum Result: %s\n", receiver_checksum);

    // Step 4: Check for errors (If result contains any '1', an error occurred)
    int error_detected = 0;
    for (int i = 0; i < block_size; i++) {
        if (receiver_checksum[i] != '0') {
            error_detected = 1;
            break;
        }
    }

    // Step 5: Output verdict
    if (error_detected) {
        printf("Result:  Error detected in data transmission!\n");
    } else {
        printf("Result:  No error detected. Data received successfully.\n");
    }

    return 0;
}
/*
[24bcs017@mepcolinux ex2]$./checksum
--- CHECKSUM SENDER SIDE ---
Enter input string text (e.g., ABC): sri
Converted text to Binary Data: 011100110111001001101001
Calculated Sender Checksum: 10110000
Transmitted Bitstream (Data + Checksum): 01110011011100100110100110110000

--- ERROR INDUCTION OPTION ---
Do you want to induce an error? (y/n): y
Enter the bit position to flip (0 to 31): 03
� Bit flipped at position 3 successfully!
Final Bitstream reaching Receiver:       01100011011100100110100110110000

--- CHECKSUM RECEIVER SIDE ---
Receiver Checksum Result: 00010000
Result:  Error detected in data transmission!
[24bcs017@mepcolinux ex2]$./checksum
--- CHECKSUM SENDER SIDE ---
Enter input string text (e.g., ABC): n^i
Converted text to Binary Data: 011011100101111001101001
Calculated Sender Checksum: 11001001
Transmitted Bitstream (Data + Checksum): 01101110010111100110100111001001

--- ERROR INDUCTION OPTION ---
Do you want to induce an error? (y/n): n
No error induced. Transmission running cleanly.
Final Bitstream reaching Receiver:       01101110010111100110100111001001

--- CHECKSUM RECEIVER SIDE ---
Receiver Checksum Result: 00000000
Result:  No error detected. Data received successfully.
*/
