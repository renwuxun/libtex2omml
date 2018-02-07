//
// Created by renwuxun on 18-1-17.
//


#include "../src/mathsymbol.h"
#include "../src/defs.h"



int main(int argc, char** argv) {

    char buf[512<<10] = {0};


    mathsymbols_int("unimathsymbols.txt", buf, 512<<10);

    char* unicode = "025C0";
    char* symbol0 = "\\LHD";
    char* symbol1 = "\\blacktriangleleft";
    char* symbol2 = "\\blacktriangleleft_not_exist";
    struct mathsymbol_s* mathsymbol0 = mathsymbols_get(symbol0);
    struct mathsymbol_s* mathsymbol1 = mathsymbols_get(symbol1);
    struct mathsymbol_s* mathsymbol2 = mathsymbols_get(symbol2);

    printf("target unicode : ");
    puts(unicode);
    printf("unicode of \\LHD is : ");
    puts(mathsymbol0->unicode);
    printf("unicode of \\blacktriangleleft is : ");
    puts(mathsymbol1->unicode);
    printf("unicode of \\blacktriangleleft_not_exist is : %p\n",mathsymbol2);

    mathsymbols_reset();

    return 0;
}
