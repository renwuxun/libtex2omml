//
// Created by renwuxun on 18-1-18.
//

#include <stdio.h>
#include <string.h>
#include "functions.h"
#include "defs.h"


//= \mathrm{\varepsilon} (omlmathrm), =  \varepsilonup  (kpfonts mathdesign), = \upepsilon (upgreek), rounded epsilon, greek
int parse_column7_ok(char** startInColumn7, size_t* len, char* column7, size_t column7_len) {
    char* start = NULL;
    char* alphaptr = NULL;
    char* end = NULL;
    while (column7_len--) {
        switch (*column7) {
            case '=':
                start = column7;
                break;
            case ' ':
            case ',':
            case '^':
            case '\n':
                end = column7;
                break;
            default:
                alphaptr = column7;
        }
        if (start && start < alphaptr && alphaptr < end && *(start+1)==' ') {
            start++;
            for (;start<alphaptr;start++) {
                if (*start != ' ') {
                    break;
                }
            }
            *startInColumn7 = start;
            *len = end-start;
            return 1;
        }
        column7++;
    }
    return 0;
}

void parse_column7(char* column7, size_t column7_len, void (*cb)(const char*, size_t, void*), void* data) {
    char* _column7 = column7;
    size_t _column7_len = column7_len;
    char* start = NULL;
    size_t len=0;
    for (;parse_column7_ok(&start, &len, _column7, _column7_len);) {
        log_debug("parse_column7: call cb, column7:%s", column7);
        cb(start, len, data);
        _column7 = start+len;
        _column7_len = column7_len - (column7-_column7);
        start=NULL;
        len=0;
    }
}

void buf_print(struct buf_s* buf) {
    struct buf_s b;
    b.base = buf->base;
    b.len = buf->len;
    for (;b.len--;) {
        putc(*b.base, stdout);
        b.base++;
    };
}

int found_between(
        char* subject,
        size_t subjectlen,
        char left,
        char right,
        void (*onfound)(const char* base, size_t len, void* onfound_data),
        void* onfound_data,
        void (*onfinish)(const char* subject, size_t subjectlen, const char* lastend, void* onfinish_data),
        void* onfinish_data
) {
    char* start = NULL;
    char* end = NULL;
    char* lastend = NULL;
    size_t _subjectlen = subjectlen;
    char* _subject = subject;
    char* lastptr = subject+subjectlen-1;
    do {
        start = memchr(_subject, left, _subjectlen);
        if (!start) {
            break;
        }
        _subjectlen -= start-_subject;
        start++;
        end = memchr(start, right, _subjectlen);
        if (!end) {
            break;
        }
        _subjectlen -= end-start;
        onfound(start, (size_t)(end-start), onfound_data);
        lastend = end;
        end++;
        if (end >= lastptr) {
            break;
        }
        _subject = end;
    } while (1);
    if (lastend) { // found
        if (onfinish) { // defined callback
            onfinish(subject, subjectlen, lastend, onfinish_data);
        }
        return 1;
    } else {
        return 0;
    }
}
