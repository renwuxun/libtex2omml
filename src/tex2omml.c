//
// Created by renwuxun on 18-1-24.
//

#include "tex2omml.h"


static char* mathsymbol_buf = NULL;
static xsltStylesheetPtr xslt = NULL;
static xmlBufferPtr buffer = NULL;

int tex2omml_init(const char* mathsymbol_file, const char* mml2omml_xsl_file) {
    if (mathsymbol_buf) {
        log_warning("you need call tex2omml_free() before");
        return 1;
    }
    mathsymbol_buf = (char*)malloc(MATHSYMBOL_BUF_SIZE);
    if (!mathsymbol_buf) {
        log_error("malloc(MATHSYMBOL_BUF_SIZE) error, MATHSYMBOL_BUF_SIZE:%d", MATHSYMBOL_BUF_SIZE);
        return 1;
    }
    mathsymbols_int(mathsymbol_file, mathsymbol_buf, MATHSYMBOL_BUF_SIZE);

    xslt = xsltParseStylesheetFile((const xmlChar*)mml2omml_xsl_file);
    if (!xslt) {
        log_error("xslt init error");
        free(mathsymbol_buf);
        return 1;
    }

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;

    return 0;
}

void tex2omml_free(void) {
    if (mathsymbol_buf) {
        free(mathsymbol_buf);
        mathsymbol_buf = NULL;
        xmlBufferFree(buffer);
        buffer = NULL;
        xsltCleanupGlobals();
        xsltFreeStylesheet(xslt);
        xslt = NULL;
    }
}

int tex2omml_mathml2omml(char* mathml, size_t mathmlLen, char* outOmml, size_t outOmmlSize) {
    char xmlbuf[XML_BUF_SIZE] = {0};
    html2xml(mathml, mathmlLen, xmlbuf, XML_BUF_SIZE);

    xmlDocPtr doc = xmlReadMemory(xmlbuf, (int)strlen(xmlbuf), NULL, "utf-8", XML_PARSE_NONET|XML_PARSE_NOENT);
    if (!doc) {
        log_error("load xml(mml) error");
        return 1;
    }
    xmlDocPtr res = xsltApplyStylesheet(xslt, doc, NULL);
    if (!res) {
        log_error("translate xml(mml) to omml error");
        xmlFreeDoc(doc);
        return 1;
    }


    xmlNodePtr root = xmlDocGetRootElement(res);
    if (!buffer) {
        buffer = xmlBufferCreate();
    }
    xmlBufferEmpty(buffer);
    if (-1 == xmlNodeDump(buffer, res, root, 0, 0)) {
        log_error("xmlNodeDump error");
        xmlFreeDoc(res);
        xmlFreeDoc(doc);
        return 1;
    }
    size_t contentlen = strlen((char*)buffer->content);
    if (contentlen>outOmmlSize-1) {
        log_error("need more buffer for omml out, current:%lu", outOmmlSize);
        xmlFreeDoc(res);
        xmlFreeDoc(doc);
        return 1;
    }

    memcpy(outOmml, buffer->content, contentlen+1);

    xmlCleanupParser();
    xmlFreeDoc(doc);
    xmlFreeDoc(res);

    return 0;
}

int tex2omml_translate_texopt(const char* tex, size_t texlen, char* buf, size_t bufsize, int texopt) {
    char* html = mtex2MML_parse(tex, texlen, texopt);
    if (!html) {
        log_error("translate tex to html error");
        return 1;
    }

    int r = tex2omml_mathml2omml(html, strlen(html), buf, bufsize);

    mtex2MML_free_string(html);

    return r;
}

int tex2omml_translate(const char* tex, size_t texlen, char* buf, size_t bufsize) {
    return tex2omml_translate_texopt(tex, texlen, buf, bufsize, 0);
}
