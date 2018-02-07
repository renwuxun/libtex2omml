//
// Created by renwuxun on 18-1-18.
//

#ifndef TEX2OMML_FUNCTIONS_H
#define TEX2OMML_FUNCTIONS_H


#include <stddef.h>

struct buf_s {
    char* base;
    size_t len;
};

//= \mathrm{\varepsilon} (omlmathrm), =  \varepsilonup  (kpfonts mathdesign), = \upepsilon (upgreek), rounded epsilon, greek
int parse_column7_ok(char** startInColumn7, size_t* len, char* column7, size_t column7_len);

void parse_column7(char* column7, size_t column7_len, void (*cb)(const char* base, size_t len, void* data), void* data);

void buf_print(struct buf_s* buf);

int found_between(
        char* subject,
        size_t subjectlen,
        char left,
        char right,
        void (*onfound)(const char* base, size_t len, void* onfound_data),
        void* onfound_data,
        void (*onfinish)(const char* subject, size_t subjectlen, const char* lastend, void* onfinish_data),
        void* onfinish_data
);

#endif //TEX2OMML_FUNCTIONS_H
