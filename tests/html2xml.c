//
// Created by renwuxun on 18-1-15.
//

#include <mtex2MML/mtex2MML.h>
#include <libxslt/xsltInternals.h>
#include "../src/mathsymbol.h"
#include "../src/html2xml.h"


int main(int argc, char** argv) {
    char buf0[512<<10] = {0};
    mathsymbols_int("unimathsymbols.txt", buf0, 512<<10);

    char* mml = "<math xmlns='http://www.w3.org/1998/Math/MathML' display='inline'>"
            "<semantics><mrow><msqrt><mrow><msup><mi>a</mi>"
            "<mn>2</mn></msup><mi>b</mi></mrow></msqrt><mo>=</mo><mrow><mo>&vert1;</mo>"""
            "<mi>a</mi><mo>&vert;</mo></mrow><msqrt><mi>b</mi></msqrt><mo>=</mo><mi>a</mi>"
            "<msqrt><mi>b</mi></msqrt><mrow><mo>(</mo><mi>a</mi><mo>&geqslant;</mo><mn>0</mn>"
            "<mo>)</mo></mrow></mrow></semantics></math>";

    printf("while buf is enough:\n");
    char _buf[1<<10] = {0};
    html2xml(mml, strlen(mml), _buf, sizeof(_buf));
    printf("%s\n", _buf);
    html2xml(mml, strlen(mml), _buf, sizeof(_buf));
    printf("%s\n", _buf);

    printf("\nwhile buf is not enough:\n");
    char __buf[200] = {0};
    html2xml(mml, strlen(mml), __buf, sizeof(__buf));
    printf("%s\n", __buf);
    html2xml(mml, strlen(mml), __buf, sizeof(__buf));
    printf("%s\n", __buf);

    return 0;
}
