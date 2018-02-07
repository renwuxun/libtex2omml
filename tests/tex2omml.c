//
// Created by renwuxun on 18-1-24.
//



#include <sys/time.h>
#ifndef RELEASE
# include <gperftools/profiler.h>
#endif
#include "../src/tex2omml.h"

int main(int argc, char** argv) {
#ifndef RELEASE
    ProfilerStart("1.prof");
#endif
    tex2omml_init("unimathsymbols.txt", "mml2omml.xsl");

    struct timeval tv0 = {0};
    gettimeofday(&tv0, 0);

    char* tex = "$\\sqrt{a^{2}b}=\\left |a\\right |\\sqrt{b}=a\\sqrt{b}\\left ( a\\geqslant %d\\right )$";
    //char* tex = "$\\ce{Na_{%d}O}$";
    char texx[1024];
    char omml[10<<10] = {0};
    int i=0, j=1000;
    for (i = 0; i < j; ++i) {
        sprintf(texx, tex, i);
        tex2omml_translate(texx, strlen(texx), omml, 10<<10);
        printf(">>>%s\n\n", omml);
    }

    struct timeval tv1 = {0};
    gettimeofday(&tv1, 0);
    printf("time ms/translate:%lu\n", (tv1.tv_sec*1000000+tv1.tv_usec - tv0.tv_sec*1000000+tv0.tv_usec)/(1000*j));

    tex2omml_free();
#ifndef RELEASE
    ProfilerStop();
#endif
    return 0;
}
