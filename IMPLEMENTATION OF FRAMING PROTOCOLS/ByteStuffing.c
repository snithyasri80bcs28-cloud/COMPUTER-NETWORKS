#include <stdio.h>
#include <string.h>

int main()
{
    char str[100];
    char flag[] = "01111110";
    char esc[] = "01000101";
    char byte[9];

    char frame[500][9];
    char received[500][9];
    char destuffed[500][9];

    int i, j;
    int count = 0, rcount = 0, dcount = 0;
    int choice;

    printf("Enter the String: ");
    scanf("%s", str);

    /* Starting Flag */
    strcpy(frame[count++], flag);

    /* Byte Stuffing */
    for(i = 0; str[i] != '\0'; i++)
    {
        int ascii = (int)str[i];

        for(j = 7; j >= 0; j--)
            byte[7-j] = ((ascii >> j) & 1) + '0';

        byte[8] = '\0';

        if(strcmp(byte, flag) == 0 || strcmp(byte, esc) == 0)
            strcpy(frame[count++], esc);

        strcpy(frame[count++], byte);
    }

    /* Ending Flag */
    strcpy(frame[count++], flag);

    printf("\n========== Sender Side ==========\n");

    printf("Stuffed Frame : ");
    for(i = 0; i < count; i++)
        printf("%s ", frame[i]);
    printf("\n");

    /* Copy to Receiver */
    for(i = 0; i < count; i++)
        strcpy(received[i], frame[i]);

    rcount = count;

    /*printf("\nDo you want to simulate an error? (1 for Yes, 0 for No): ");
    scanf("%d",&choice);

    if(choice == 1)
    {
        int errorPos = 2;

        if(errorPos < rcount)
        {
            if(received[errorPos][0]=='0')
                received[errorPos][0]='1';
            else
                received[errorPos][0]='0';

            printf("Error introduced at byte position %d\n",errorPos);
        }
    }*/

    printf("\n========== Receiver Side ==========\n");

    printf("Received Frame : ");
    for(i=0;i<rcount;i++)
        printf("%s ",received[i]);
    printf("\n");

    /* De-stuffing */

    for(i=1;i<rcount-1;i++)
    {
        if(strcmp(received[i],esc)==0)
        {
            i++;
        }

        strcpy(destuffed[dcount++],received[i]);
    }

    printf("De-stuffed Data : ");

    for(i=0;i<dcount;i++)
        printf("%s ",destuffed[i]);

    printf("\n");

    char recovered[100];
int index = 0;

for(i = 0; i < dcount; i++)
{
    int value = 0;

    for(j = 0; j < 8; j++)
    {
        value = value * 2 + (destuffed[i][j] - '0');
    }

    recovered[index++] = (char)value;
}

recovered[index] = '\0';

printf("Recovered Text : %s\n", recovered);

    return 0;
}
/*Enter the String: sri

========== Sender Side ==========
Stuffed Frame : 01111110 01110011 01110010 01101001 01111110

========== Receiver Side ==========
Received Frame : 01111110 01110011 01110010 01101001 01111110
De-stuffed Data : 01110011 01110010 01101001
Recovered Text : sri
[24bcs017@mepcolinux ex1]$./a.out
Enter the String: N@Sri

========== Sender Side ==========
Stuffed Frame : 01111110 01001110 01000000 01010011 01110010 01101001 01111110

========== Receiver Side ==========
Received Frame : 01111110 01001110 01000000 01010011 01110010 01101001 01111110
De-stuffed Data : 01001110 01000000 01010011 01110010 01101001
Recovered Text : N@Sri*/
