#include <stdio.h>
#include <string.h>

// Converts an ASCII character into an 8-bit binary string
void char_to_binary(char ch, char *binary_str) {
    for (int i = 7; i >= 0; i--) {
        binary_str[7 - i] = ((ch >> i) & 1) ? '1' : '0';
    }
    binary_str[8] = '\0'; // Ensures a clean string termination
}

// Counts the total number of '1' bits inside a binary string
int count_ones(const char *str) {
    int count = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '1') count++;
    }
    return count;
}

int main() {
    char input_string[100];
    int parity_choice;

    // Securely capture the input phrase using fgets
    printf("Enter text string input (e.g., hi): ");
    fgets(input_string, sizeof(input_string), stdin);
    input_string[strcspn(input_string, "\n")] = '\0'; // Strip the trailing newline

    printf("Choose Parity Type (1 for Even, 2 for Odd): ");
    scanf("%d", &parity_choice);
    printf("\n");

    // Buffers to compile continuous stream tracking
    char raw_binary_output[1000] = "";
    char transmitted_output[1000] = "";

    // Loop through each text letter character by character
    for (int i = 0; i < strlen(input_string); i++) {
        char raw_binary[9];
        char transmitted_packet[10];
        char parity_bit;

        // 1. Transform raw text character to bit representations
        char_to_binary(input_string[i], raw_binary);

        // 2. Evaluate parity configurations
        int ones = count_ones(raw_binary);
        if (parity_choice == 1) {
            parity_bit = (ones % 2 == 0) ? '0' : '1';
        } else {
            parity_bit = (ones % 2 != 0) ? '0' : '1';
        }

        // 3. Assemble tight 9-bit package with no interior spacing
        sprintf(transmitted_packet, "%s%c", raw_binary, parity_bit);

        // 4. Concat to a single continuous stream sequence (WITHOUT spaces)
        strcat(raw_binary_output, raw_binary);
        strcat(transmitted_output, transmitted_packet);
    }

    // SENDER SIDE PRINT
    printf("-------------SENDER SIDE---------------\n");
    printf("Input Text         : %s\n", input_string);
    printf("Raw Binary Value   : %s\n", raw_binary_output);
    printf("Transmitted Stream : %s (Length: %d bits)\n\n", transmitted_output, (int)strlen(transmitted_output));

    // ==========================================
    // INTERACTIVE ERROR INJECTION PROMPT
    // ==========================================
    char error_choice;
    printf("Do you want to inject an error? (y/n): ");
    scanf(" %c", &error_choice);

    char receiver_stream[1000];
    strcpy(receiver_stream, transmitted_output);

    if (error_choice == 'y' || error_choice == 'Y') {
        int bit_pos;
        printf("Enter bit position to inject error (0 to %d): ", (int)strlen(transmitted_output) - 1);
        scanf("%d", &bit_pos);

        // Bounds checking to ensure a valid array position is selected
        if (bit_pos >= 0 && bit_pos < strlen(receiver_stream)) {
            // Flip the bit at the user's specific position index
            receiver_stream[bit_pos] = (receiver_stream[bit_pos] == '0') ? '1' : '0';
            printf("\nNoise injected successfully! Flipped bit at position %d.\n", bit_pos);
        } else {
            printf("\nInvalid bit position selected! No error injected.\n");
        }
    } else {
        printf("\nClear channel transmission. No noise applied.\n");
    }
    printf("\n");
    // RECEIVER SIDE PRINT
    printf("------------RECEIVER SIDE--------------\n");
    printf("Processing Received Continuous Binary Stream...\n\n");
    printf("Received Stream    : %s\n", receiver_stream);

    int stream_corrupted = 0;

    // Validate the stream block by block internally, without printing intermediate blocks
    int block_len = 9;
    for (int i = 0; i < strlen(receiver_stream); i += block_len) {
        char segment[10];
        strncpy(segment, &receiver_stream[i], block_len);
        segment[block_len] = '\0';

        int segment_ones = count_ones(segment);

        if (parity_choice == 1) {
            // Even parity validation: must have an even number of 1s
            if (segment_ones % 2 != 0) {
                stream_corrupted = 1;
                break;
            }
        } else {
            // Odd parity validation: must have an odd number of 1s
            if (segment_ones % 2 == 0) {
                stream_corrupted = 1;
                break;
            }
        }
    }
    // FINAL VERDICT
    if (stream_corrupted) {
        printf("Result: ERROR DETECTED! Frame has been DISCARDED.\n");
    } else {
        printf("Result: SUCCESS! Frame accepted cleanly.\n");
    }
    return 0;
}
/*
[24bcs017@mepcolinux ex2]$./parity
Enter text string input (e.g., hi): sri        
Choose Parity Type (1 for Even, 2 for Odd): 1

-------------SENDER SIDE---------------
Input Text         : sri
Raw Binary Value   : 011100110111001001101001
Transmitted Stream : 011100111011100100011010010 (Length: 27 bits)

Do you want to inject an error? (y/n): y
Enter bit position to inject error (0 to 26): 22

Noise injected successfully! Flipped bit at position 22.

------------RECEIVER SIDE--------------
Processing Received Continuous Binary Stream...

Received Stream    : 011100111011100100011000010
Result: ERROR DETECTED! Frame has been DISCARDED.
[24bcs017@mepcolinux ex2]$./parity
Enter text string input (e.g., hi): n&s
Choose Parity Type (1 for Even, 2 for Odd): 2

-------------SENDER SIDE---------------
Input Text         : n&s
Raw Binary Value   : 011011100010011001110011
Transmitted Stream : 011011100001001100011100110 (Length: 27 bits)

Do you want to inject an error? (y/n): n

Clear channel transmission. No noise applied.

------------RECEIVER SIDE--------------
Processing Received Continuous Binary Stream...

Received Stream    : 011011100001001100011100110
Result: SUCCESS! Frame accepted cleanly.
*/
