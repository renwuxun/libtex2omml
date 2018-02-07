//
// Created by renwuxun on 18-1-17.
//

#include "mathsymbol.h"
#include "defs.h"
#include "functions.h"


struct mathsymbol_s* mathsymbols = NULL;


void mathsymbols_add(struct mathsymbol_s* mathsymbol) {
    HASH_ADD_STR(mathsymbols, symbol, mathsymbol);
}

struct mathsymbol_s* mathsymbols_get(char* symbol) {
    struct mathsymbol_s* mathsymbol = NULL;
    HASH_FIND_STR(mathsymbols, symbol, mathsymbol);
    return mathsymbol;
}

void mathsymbols_del(struct mathsymbol_s* mathsymbol) {
    HASH_DEL(mathsymbols, mathsymbol);
}

struct mathsymbol_s* mathsymbols_new(char** _buf, size_t* _buflen) {
    char* nextptr = align_ptr(*_buf+sizeof(struct mathsymbol_s), ALIGNMENT);
    if (nextptr > *_buf + *_buflen) {
        log_crit("mathsymbols_new: no more buf\n");
        exit(EXIT_FAILURE);
    }

    struct mathsymbol_s* mathsymbol = (struct mathsymbol_s*)*_buf;
    memset(mathsymbol, 0, sizeof(struct mathsymbol_s));

    *_buflen = *_buflen - (nextptr - *_buf);
    *_buf = nextptr;

    return mathsymbol;
}

struct bufptr_buflenptr_s {
    char** buf;
    size_t* len;
    char unicode[SYMBOL_UNICODE_CHAR_SIZE];
};

void mathsymbols_add_callback(const char* base, size_t len, void* data) {
    if (len +1 > SYMBOL_STR_MAX_LEN) {
        log_crit("mathsymbols_add_callback: column7 lenght:[%d] > SYMBOL_STR_MAX_LEN:[%d]", (int)len +1, SYMBOL_STR_MAX_LEN);
        exit(EXIT_FAILURE);
    }

    struct bufptr_buflenptr_s* b = (struct bufptr_buflenptr_s*)data;

    struct mathsymbol_s* new_mathsymbol = mathsymbols_new(b->buf, b->len);

    memcpy(new_mathsymbol->symbol, base, len);
    memcpy(new_mathsymbol->unicode, b->unicode, SYMBOL_UNICODE_CHAR_SIZE);

    mathsymbols_add(new_mathsymbol);
}

void mathsymbols_int(const char* mathsymbol_file, char* buf, size_t buflen) {
    FILE* f = fopen(mathsymbol_file, "r");
    if (!f) {
        log_crit("fopen(%s, \"r\")", mathsymbol_file);
        exit(EXIT_FAILURE);
    }

    struct mathsymbol_s* new_mathsymbol = NULL;

    char line[4096]={0};
    char* columns[8] = {line,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    int i=0, j;
    size_t col0len, linelen;
    struct bufptr_buflenptr_s arg = {0};
    for (;fgets(line, 4096, f);) {
        j=0;
        linelen = strlen(line);
        for (i = 0; i < linelen; ++i) {
            if (line[i] == '^') {
                line[i] = '\0';
                j++;
                columns[j] = &line[i+1];
            }
        }
        //printf("%s\n", columns[0]);
        // columns[0]:unicode columns[2]:latex columns[3]:unicode_math columns[7]:may be math
        col0len = strlen(columns[0]);
        if (col0len == 0) {
            continue;
        }
        if (columns[2][0]!=0 && !mathsymbols_get(&columns[2][0])) {
            new_mathsymbol = mathsymbols_new(&buf, &buflen);
            if (strlen(columns[2])+1 > SYMBOL_STR_MAX_LEN) {
                log_crit("strlen(columns[2])+1:[%d] > SYMBOL_STR_MAX_LEN:[%d]", (int)strlen(columns[2])+1, SYMBOL_STR_MAX_LEN);
                exit(EXIT_FAILURE);
            }
            memcpy(new_mathsymbol->symbol, columns[2], strlen(columns[2]));
            memcpy(new_mathsymbol->unicode, columns[0], col0len);
            mathsymbols_add(new_mathsymbol);
        }
        if (columns[3][0]!='\0' && !mathsymbols_get(columns[3])) {
            new_mathsymbol = mathsymbols_new(&buf, &buflen);
            if (strlen(columns[3])+1 > SYMBOL_STR_MAX_LEN) {
                log_crit("strlen(columns[3])+1:[%d] > SYMBOL_STR_MAX_LEN:[%d]", (int)strlen(columns[3])+1, SYMBOL_STR_MAX_LEN);
                exit(EXIT_FAILURE);
            }
            memcpy(new_mathsymbol->symbol, columns[3], strlen(columns[3]));
            memcpy(new_mathsymbol->unicode, columns[0], col0len);
            mathsymbols_add(new_mathsymbol);
        }
        if (columns[7][0]=='=') {
            memcpy(arg.unicode, columns[0], col0len);
            arg.len = &buflen;
            arg.buf = &buf;
            parse_column7(columns[7], strlen(columns[7]), mathsymbols_add_callback, &arg);
        }
    }

    fclose(f);
}

void mathsymbols_reset(void) {
    mathsymbols = NULL;
}
