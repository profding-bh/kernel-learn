#include <stdio.h>


typedef int SINT32;


typedef struct str_test{
   SINT32 a;
   SINT32 b;
}str_test_t;


int main()
{
    str_test_t var = {
        .a = 11,
        .b = -22
    };

    printf("sizeof(int) = %ld\n",sizeof(int));
    printf("var.a = %d\t var.b = %d\n",var.a,var.b);

    return 0;
}

