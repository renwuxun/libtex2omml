//
// Created by renwuxun on 18-1-15.
//

#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <string.h>


int main(int argc, char** argv) {

    char* mml = "<math xmlns='http://www.w3.org/1998/Math/MathML' display='inline'>"
            "<semantics><mrow><msqrt><mrow><msup><mi>a</mi>"
            "<mn>2</mn></msup><mi>b</mi></mrow></msqrt><mo>=</mo><mrow><mo>&#x0007C;</mo>"""
            "<mi>a</mi><mo>&#x0007C;</mo></mrow><msqrt><mi>b</mi></msqrt><mo>=</mo><mi>a</mi>"
            "<msqrt><mi>b</mi></msqrt><mrow><mo>(</mo><mi>a</mi><mo>&#x02A7E;</mo><mn>0</mn>"
            "<mo>)</mo></mrow></mrow></semantics></math>";


    xsltStylesheetPtr xslt = NULL;
    xmlDocPtr doc = NULL;
    xmlDocPtr res = NULL;


    xslt = xsltParseStylesheetFile((const xmlChar*)"mml2omml.xsl");
    if (!xslt) {
        perror("xsltParseStylesheetFile error\n");
        return 1;
    }

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;

    doc = xmlReadMemory(mml, (int)strlen(mml), NULL, "utf-8", 0);
    if (!doc) {
        perror("xmlReadMemory error\n");
        perror(mml);
        return 1;
    }

    res = xsltApplyStylesheet(xslt, doc, NULL);
    if (!res) {
        perror("xsltApplyStylesheet error\n");
        return 1;
    }

    xmlNodePtr root = xmlDocGetRootElement(res);

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlNodeDump(buffer, res, root, 0, 0);
    puts((char*)buffer->content);
    xmlBufferFree(buffer);


    xsltFreeStylesheet(xslt);
    xmlFreeDoc(doc);
    xmlFreeDoc(res);


    return 0;
}
