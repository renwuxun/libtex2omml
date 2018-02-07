//
// Created by renwuxun on 18-1-15.
//

#include <mtex2MML/mtex2MML.h>
#include <stdio.h>


int main(int argc, char** argv) {

    char* tex = "$\\sqrt{a^{2}b}=\\left |a\\right |\\sqrt{b}=a\\sqrt{b}\\left ( a\\geqslant 0\\right )$";

    char* mml = mtex2MML_parse(tex, strlen(tex), 0);
    if (!mml) {
        printf("tex parse error\n");
        return 1;
    }

    printf("%s\n", mml);

    mtex2MML_free_string(mml);

    return 0;
}
