//
// Created by renwuxun on 18-1-17.
//

#ifndef TEX2OMML_MATHSYMBOL_H
#define TEX2OMML_MATHSYMBOL_H


#include <uthash/uthash.h>

#ifndef SYMBOL_STR_MAX_LEN
# define SYMBOL_STR_MAX_LEN 34
#endif

#ifndef SYMBOL_UNICODE_CHAR_SIZE
# define SYMBOL_UNICODE_CHAR_SIZE 6
#endif

struct mathsymbol_s {
    char symbol[SYMBOL_STR_MAX_LEN];
    char unicode[SYMBOL_UNICODE_CHAR_SIZE];//00041\0
    UT_hash_handle hh;
};

// https://github.com/Code-ReaQtor/latex2mathml/blob/master/latex2mathml/unimathsymbols.txt
void mathsymbols_int(const char* mathsymbol_file, char* buf, size_t buflen);

struct mathsymbol_s* mathsymbols_get(char* symbol);

void mathsymbols_reset(void);



#endif //TEX2OMML_MATHSYMBOL_H
