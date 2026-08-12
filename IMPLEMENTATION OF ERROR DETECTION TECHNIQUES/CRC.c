#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Simple function to do CRC Binary XOR division
void crc_division(char *dividend, const char *divisor, char *remainder) {
    int data_len = strlen(dividend);
    int div_len = strlen(divisor);
    char temp[2000];

    strcpy(temp, dividend);

    for (int i = 0; i <= data_len - div_len; i++) {
        if (temp[i] == '1') {
            for (int j = 0; j < div_len; j++) {
                temp[i + j] = (temp[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }
    // Extract the final remainder bits
    strcpy(remainder, &temp[data_len - (div_len - 1)]);
}

// Converts a text character into an 8-bit binary string
void char_to_binary(char ch, char *binary_str) {
    for (int i = 7; i >= 0; i--) {
        binary_str[7 - i] = ((ch >> i) & 1) ? '1' : '0';
    }
    binary_str[8] = '\0';
}

// NEW FUNCTION: Converts verified raw binary stream chunks back into characters
void binary_stream_to_text(const char *binary_stream, int data_len, char *text_out) {
    int text_idx = 0;

    // Process the stream chunk by chunk in 8-bit increments
    for (int i = 0; i < data_len; i += 8) {
        char ch = 0;
        for (int j = 0; j < 8; j++) {
            ch = (ch << 1) | (binary_stream[i + j] - '0'); // Bitwise left-shift and accumulate bits
        }
        text_out[text_idx++] = ch;
    }
    text_out[text_idx] = '\0'; // Null-terminate string output
}

// Parses an algebraic polynomial string (e.g., "x^4+1") into a binary string
void parse_polynomial_to_binary(const char *poly_str, char *binary_out) {
    int max_degree = 0;
    int degrees[100] = {0}; // Array to keep track of present terms
    int len = strlen(poly_str);
    int i = 0;

    while (i < len) {
        if (poly_str[i] == ' ' || poly_str[i] == '+') {
            i++;
            continue;
        }

        if (poly_str[i] == 'x' || poly_str[i] == 'X') {
            i++;
            if (poly_str[i] == '^') {
                i++;
                int deg = 0;
                while (i < len && isdigit(poly_str[i])) {
                    deg = deg * 10 + (poly_str[i] - '0');
                    i++;
                }
                degrees[deg] = 1;
                if (deg > max_degree) max_degree = deg;
            } else {
                degrees[1] = 1;
                if (1 > max_degree) max_degree = 1;
            }
        } else if (isdigit(poly_str[i])) {
            int val = 0;
            while (i < len && isdigit(poly_str[i])) {
                val = val * 10 + (poly_str[i] - '0');
                i++;
            }
            if (val == 1) {
                degrees[0] = 1;
            }
        } else {
            i++;
        }
    }

    int idx = 0;
    for (int d = max_degree; d >= 0; d--) {
        binary_out[idx++] = degrees[d] ? '1' : '0';
    }
    binary_out[idx] = '\0';
}

int main() {
    char input_string[100];
    char poly_expr[100];
    char generator_poly[100] = "";
    char raw_binary_stream[1000] = "";
    char dividend[2000] = "", remainder[100] = "", transmitted[2000] = "";
    char reconstructed_text[100] = "";
    char error_choice;
    int bit_pos;

    // 1. Capture user inputs
    printf("Enter text string input (e.g., hi): ");
    fgets(input_string, sizeof(input_string), stdin);
    input_string[strcspn(input_string, "\n")] = '\0';

    printf("Enter polynomial divisor expression (e.g., x^4+x+1): ");
    fgets(poly_expr, sizeof(poly_expr), stdin);
    poly_expr[strcspn(poly_expr, "\n")] = '\0';
    printf("\n");

    // Convert polynomial expression into its binary equivalent string
    parse_polynomial_to_binary(poly_expr, generator_poly);
    int poly_len = strlen(generator_poly);
    int original_data_len = strlen(input_string) * 8; // Each character is exactly 8 bits

    // Convert the data string into its continuous binary representation
    for (size_t i = 0; i < strlen(input_string); i++) {
        char temp_bin[9];
        char_to_binary(input_string[i], temp_bin);
        strcat(raw_binary_stream, temp_bin);
    }

    // Count the number of ones in the generator polynomial
    int ones_count = 0;
    for (int i = 0; i < poly_len; i++) {
        if (generator_poly[i] == '1') {
            ones_count++;
        }
    }

    // ==========================================
    // 2. DISPLAY PARSED BINARY VALUES & RULES CHECK
    // ==========================================
    printf("=================================================================================\n");
    printf("                               INPUT BINARY VALUES                               \n");
    printf("=================================================================================\n");
    printf("String Text Binary    : %s\n", raw_binary_stream);
    printf("Polynomial Expression : %s\n", poly_expr);
    printf("Generator Poly Binary : %s (Length: %d bits, Ones: %d)\n", generator_poly, poly_len, ones_count);
    printf("=================================================================================\n\n");

    // STRICT RULES CHECK
    if (poly_len <= 2) {
        printf("Verdict: BAD DIVISOR! The divisor must be above 2 bits long.\n");
        return 0;
    }
    if (generator_poly[0] != '1' || generator_poly[poly_len - 1] != '1') {
        printf("Verdict: BAD DIVISOR! Binary stream must start and end with 1.\n");
        return 0;
    }
    if (ones_count % 2 != 0) {
        printf("Verdict: BAD DIVISOR! Must contain an EVEN number of ones.\n");
        return 0;
    }

    printf("Verdict: GOOD DIVISOR! All criteria passed. Proceeding to sender...\n\n");

    // ==========================================
    // 3. SENDER SIDE
    // ==========================================
    strcpy(dividend, raw_binary_stream);
    for (int i = 0; i < poly_len - 1; i++) {
        strcat(dividend, "0"); // Pad with zeros equal to (divisor length - 1)
    }

    crc_division(dividend, generator_poly, remainder);
    sprintf(transmitted, "%s%s", raw_binary_stream, remainder); // Concat data + remainder

    printf("--- SENDER ---\n");
    printf("Transmitted Stream : %s (Length: %d bits)\n\n", transmitted, (int)strlen(transmitted));

    // ==========================================
    // 4. INTERACTIVE ERROR PROMPT
    // ==========================================
    printf("Do you want to inject an error? (y/n): ");
    scanf(" %c", &error_choice);

    if (error_choice == 'y' || error_choice == 'Y') {
        printf("Enter bit position to flip (0 to %d): ", (int)strlen(transmitted) - 1);
        scanf("%d", &bit_pos);

        if (bit_pos >= 0 && (size_t)bit_pos < strlen(transmitted)) {
            transmitted[bit_pos] = (transmitted[bit_pos] == '0') ? '1' : '0'; // Flip single bit
            printf("Noise injected successfully!\n");
        } else {
            printf("Invalid position. No noise injected.\n");
        }
    }
    printf("\n");

    // ==========================================
    // 5. RECEIVER SIDE
    // ==========================================
    char check_remainder[100] = "";
    crc_division(transmitted, generator_poly, check_remainder);

    printf("--- RECEIVER ---\n");
    printf("Received Stream    : %s\n", transmitted);
    printf("Remainder Found    : %s\n\n", check_remainder);

    // Check if remainder is all zeros
    int corrupted = 0;
    for (size_t i = 0; i < strlen(check_remainder); i++) {
        if (check_remainder[i] != '0') {
            corrupted = 1;
            break;
        }
    }

    // ==========================================
    // 6. FINAL VERDICT & TEXT RECONSTRUCTION
    // ==========================================
    if (corrupted) {
        printf("Result: ERROR DETECTED! Frame DISCARDED.\n");
        printf("Action: Cannot reconstruct text from corrupted binary stream.\n");
    } else {
        printf("Result: SUCCESS! Frame accepted cleanly.\n");

        // Convert the prefix message bits back into text, ignoring trailing CRC checksum bits
        binary_stream_to_text(transmitted, original_data_len, reconstructed_text);
        printf("Reconstructed String Text from Receiver: \"%s\"\n", reconstructed_text);
    }

    return 0;
}
/*
[24bcs017@mepcolinux ex2]$./crc
Enter text string input (e.g., hi): h*i
Enter polynomial divisor expression (e.g., x^4+x+1): x^2+1

=================================================================================
                               INPUT BINARY VALUES                               
=================================================================================
String Text Binary    : 011010000010101001101001
Polynomial Expression : x^2+1
Generator Poly Binary : 101 (Length: 3 bits, Ones: 2)
=================================================================================

Verdict: GOOD DIVISOR! All criteria passed. Proceeding to sender...

--- SENDER ---
Transmitted Stream : 01101000001010100110100111 (Length: 26 bits)

Do you want to inject an error? (y/n): y
Enter bit position to flip (0 to 25): 4
Noise injected successfully!

--- RECEIVER ---
Received Stream    : 01100000001010100110100111
Remainder Found    : 10

Result: ERROR DETECTED! Frame DISCARDED.
Action: Cannot reconstruct text from corrupted binary stream.
[24bcs017@mepcolinux ex2]$./crc
Enter text string input (e.g., hi): n~a
Enter polynomial divisor expression (e.g., x^4+x+1): x^3+1

=================================================================================
                               INPUT BINARY VALUES                               
=================================================================================
String Text Binary    : 011011100111111001100001
Polynomial Expression : x^3+1
Generator Poly Binary : 1001 (Length: 4 bits, Ones: 2)
=================================================================================

Verdict: GOOD DIVISOR! All criteria passed. Proceeding to sender...

--- SENDER ---
Transmitted Stream : 011011100111111001100001000 (Length: 27 bits)

Do you want to inject an error? (y/n): n

--- RECEIVER ---
Received Stream    : 011011100111111001100001000
Remainder Found    : 000

Result: SUCCESS! Frame accepted cleanly.
Reconstructed String Text from Receiver: "n~a"
*/
