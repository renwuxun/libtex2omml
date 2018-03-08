//
// Created by renwuxun on 18-1-24.
//

#ifndef TEX2OMML_TEX2OMML_H
#define TEX2OMML_TEX2OMML_H


#include <stddef.h>
#include <mtex2MML/mtex2MML.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include "defs.h"
#include "mathsymbol.h"
#include "html2xml.h"


#ifndef MATHSYMBOL_BUF_SIZE
# define MATHSYMBOL_BUF_SIZE 512<<10
#endif

#ifndef XML_BUF_SIZE
#define XML_BUF_SIZE 10<<10
#endif


int tex2omml_init(const char* mathsymbol_file, const char* mml2omml_xsl_file);

void tex2omml_free(void);

int tex2omml_translate_texopt(const char* tex, size_t texlen, char* ommlbuf, size_t ommlbufsize, int texopt);
int tex2omml_translate(const char* tex, size_t texlen, char* buf, size_t bufsize);



#endif //TEX2OMML_TEX2OMML_H
