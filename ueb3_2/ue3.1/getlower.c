// getlower.c - ASCII table and case conversion
#include <stdio.h>
#include <string.h>

int main()
{
    int i;
    char s[256];
    
    // ASCII table
    printf("Dec\tHex\tChar\n");
    for(i = 32; i <= 126; i++)
    {
        printf("%d\t%X\t%c\n", i, i, i);
    }
    
    // String conversion
    printf("Enter string: ");
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = 0; // remove newline
    
    printf("Original: %s\n", s);
    
    for(i = 0; s[i]; i++)
    {
        if(s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] += 32;
        }
    }
    
    printf("Converted: %s\n", s);
    return 0;
}