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

//    char* tex = "$$sqrt{a^{2}b}=\\left |a\\right |\\sqrt{b}=a\\sqrt{b}\\left ( a\\geqslant %d\\right )$$";
    //char* tex = "$$v_{0}=3cm/s$$";
//    char* tex = "$$ce{Na_{%d}O}$$";
    char* tex = "$$2C_{4}H_{10}+13O_{2}\\buildrel{&#x70b9;&#x71c3;%d}\\over =8CO_{2}+10H_{2}O$$";
    char texx[1024];
    char omml[10<<10] = {0};
    int i=0, j=1000;
    for (i = 0; i < j; ++i) {
        sprintf(texx, tex, i);
        tex2omml_translate_texopt(texx, strlen(texx), omml, 10<<10, MTEX2MML_DELIMITER_DOUBLE);
        //printf(">>>%s\n\n", omml);
    }
    printf(">>>%s\n\n", omml);

    struct timeval tv1 = {0};
    gettimeofday(&tv1, 0);
    printf("time ms/translate:%lu\n", (tv1.tv_sec*1000000+tv1.tv_usec - tv0.tv_sec*1000000+tv0.tv_usec)/(1000*j));

    tex2omml_free();
#ifndef RELEASE
    ProfilerStop();
#endif
    return 0;
}
