#include <stdio.h>
#include <string.h>
int main() {
    char str[100];
    char binary[1000];
    char stuffed[1200];
    char received[1200];
    char destuffed[1000];
    char text[100];

    int i, j, k = 0, count = 0;
    int choice;

    printf("Enter the String: ");
    scanf("%s", str);

    for(i = 0; str[i] != '\0'; i++) {
        int ascii = (int)str[i];
        for(j = 7; j >= 0; j--)
        {
            binary[k++] = ((ascii >> j) & 1) + '0';
        }
    }

    binary[k] = '\0';

    printf("\nBinary Data : %s\n", binary);
    strcpy(stuffed, "01111110");
    k = 8;
    count = 0;
    for(i = 0; binary[i] != '\0'; i++) {
       stuffed[k++] = binary[i];
       if(binary[i] == '0'){
          count = 0;
       }
       else {
        if(i > 0 && binary[i - 1] == '0')
            count = 1;
        else
            count++;

        if(count == 5)
        {
            stuffed[k++] = '0';   // Stuff 0
            count = 0;
        }
       }
    }
    strcpy(&stuffed[k], "01111110");
    printf("\n========== Sender Side ==========\n");
    printf("Original String : %s\n", str);
    printf("Binary Data     : %s\n", binary);
    printf("Stuffed Frame   : %s\n", stuffed);

    strcpy(received, stuffed);

    printf("\nDo you want to insert an error?\n");
    printf("1. No\n");
    printf("2. Single Bit Error\n");
    printf("3.Multiple Bit Error\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    int f=0;

    if(choice == 2)
    {
        f =1;
        int errorPos;
        printf("Enter The Position Error To Be Added :\n");
        scanf("%d",&errorPos);

        if(received[errorPos] == '0')
            received[errorPos] = '1';
        else
           received[errorPos] = '0';

        printf("Error inserted at bit position %d\n", errorPos);
    }
    if(choice == 3){
       f=1;
       int pos1,pos2;
       printf("Enter The 2 Posiotion Error To Be Inserted :\n");
       scanf("%d %d",&pos1,&pos2);
       if(received[pos1] == '0')
          received[pos1] = '1';
       else
          received[pos1] = '0';
       if(received[pos2] == '0')
          received[pos2] = '1';
       else
          received[pos2] = '0';
    }

    printf("\n========== Receiver Side ==========\n");
    printf("Received Frame : %s\n", received);

    /* De-stuffing */
    int flag = 8;
    int len = strlen(received);
    i = flag;
    j = 0;
    count = 0;
    while(i < len - flag) {
       destuffed[j++] = received[i];
       if(received[i] == '0') {
        count = 0;
       }
       else{
          if(i > flag && received[i - 1] == '0')
             count = 1;
          else
             count++;
          if(count == 5){
            i++;          // Skip stuffed 0
            count = 0;
          }
       }
       i++;
    }
    destuffed[j] = '\0';
    printf("De-stuffed Data : %s\n", destuffed);

    /* Binary to String */

    k = 0;

    for(i = 0; i < strlen(destuffed); i += 8)
    {
        int value = 0;

        for(j = 0; j < 8; j++)
        {
            value = value << 1;

            if(destuffed[i + j] == '1')
                value = value | 1;
        }

        text[k++] = value;
    }

    text[k] = '\0';

    printf("Recovered String : %s\n", text);
    if(f == 1){
       printf("Error Detected ...\n");
       printf("Message Discared ...\n");
    }
    return 0;
}

/*Enter the String: Hello

Binary Data : 0100100001100101011011000110110001101111

========== Sender Side ==========
Original String : Hello
Binary Data     : 0100100001100101011011000110110001101111
Stuffed Frame   : 01111110010010000110010101101100011011000110111101111110

Do you want to insert an error?
1. No
2. Single Bit Error
3.Multiple Bit Error
Enter choice: 1

========== Receiver Side ==========
Received Frame : 01111110010010000110010101101100011011000110111101111110
De-stuffed Data : 0100100001100101011011000110110001101111
Recovered String : Hello
[24bcs017@mepcolinux ex1]$./a.out
Enter the String: h~i

Binary Data : 011010000111111001101001

========== Sender Side ==========
Original String : h~i
Binary Data     : 011010000111111001101001
Stuffed Frame   : 01111110011010000111110100110100101111110

Do you want to insert an error?
1. No
2. Single Bit Error
3.Multiple Bit Error
Enter choice: 2
Enter The Position Error To Be Added :
2
Error inserted at bit position 2

========== Receiver Side ==========
Received Frame : 01011110011010000111110100110100101111110
De-stuffed Data : 011010000111111001101001
Recovered String : h~i
Error Detected ...
Message Discared ...
[24bcs017@mepcolinux ex1]$./a.out
Enter the String: n*s

Binary Data : 011011100010101001110011

========== Sender Side ==========
Original String : n*s
Binary Data     : 011011100010101001110011
Stuffed Frame   : 0111111001101110001010100111001101111110

Do you want to insert an error?
1. No
2. Single Bit Error
3.Multiple Bit Error
Enter choice: 3
Enter The 2 Posiotion Error To Be Inserted :
2
3

========== Receiver Side ==========
Received Frame : 0100111001101110001010100111001101111110
De-stuffed Data : 011011100010101001110011
Recovered String : n*s
Error Detected ...
Message Discared ...*/
