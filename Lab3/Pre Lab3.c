#include <stdio.h>
#include <stdlib.h>


void analyze(const unsigned x, unsigned *bit_count, int *high, int *low) {
    *bit_count=0;

    *high = *low= 0;

    if(x == 0){
        (*high) = -1;
        (*low) = -1;
    }
    
    for(int i = 0; i<32; i++){
        if (((x>>i)&1) == 1){
            (*bit_count)++;
            (*high) = i;
        }
    }
    for(int j = 0; j<32; j++){
        if (((x>>j)&1) == 1){
            (*low) = j;
            break;
        }
    }

}


void bit_analyze(const int x) {
    unsigned bit_count = 0;
    int high, low;
    high = low = 0;
    
    analyze(x, &bit_count, &high, &low);
    
    printf("   NUMBER: %u\n",x);
    printf("     BITS: %u\n",bit_count);
    printf("HIGHEST 1: %d\n",high);
    printf(" LOWEST 1: %d\n",low);
    printf("\n");
}
int stoi(const char * str) {
    char * x = NULL;
    const long parsed = strtol(str, &x, 0);
    return x==str ? 0 : (int)parsed;
}
int main(int argc,
         const char * argv[]) {
    bit_analyze(argc>1 ? stoi(argv[1]) : 0);
    return 0;
}
