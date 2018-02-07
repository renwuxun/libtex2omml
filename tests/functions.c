//
// Created by renwuxun on 18-1-18.
//


#include "../src/functions.h"
#include <string.h>
#include <stdio.h>

void cb(const char* base, size_t len, void* data) {
    struct buf_s b = {.base=(char*)base, .len=len};
    printf("cb[");
    buf_print(&b);
    printf("]\n");
}

void cb1(const char* base, size_t len, void* data) {
    struct buf_s b = {.base=(char*)base, .len=len};
    printf("cb1[");
    buf_print(&b);
    printf("]\n");
}

int main(int argc, char** argv) {
    char* str = "=  \\mathrm{\\varepsilon}  (omlmathrm), "
            "=  \\varepsilonup  (kpfonts mathdesign), "
            "= \\upepsilon (upgreek), rounded epsilon, greek";

    parse_column7(str, strlen(str), cb, NULL);
    parse_column7(str, strlen(str), cb, NULL);


    char* mml = "<math xmlns='http://www.w3.org/1998/Math/MathML' display='inline'>"
            "<semantics><mrow><msqrt><mrow><msup><mi>a</mi>"
            "<mn>2</mn></msup><mi>b</mi></mrow></msqrt><mo>=</mo><mrow><mo>&vert;</mo>"""
            "<mi>a</mi><mo>&vert;</mo></mrow><msqrt><mi>b</mi></msqrt><mo>=</mo><mi>a</mi>"
            "<msqrt><mi>b</mi></msqrt><mrow><mo>(</mo><mi>a</mi><mo>&geqslant;</mo><mn>0</mn>"
            "<mo>)</mo></mrow></mrow><annotation encoding='application/x-tex'>"
            "\\sqrt{a^{2}b}=\\left |a\\right |\\sqrt{b}=a\\sqrt{b}\\left ( a\\geqslant 0\\right )"
            "</annotation></semantics></math>";

    found_between(mml, strlen(mml), '&',';', cb1, NULL, NULL, NULL);
    found_between(mml, strlen(mml), '&',';', cb1, NULL, NULL, NULL);


    return 0;
}
