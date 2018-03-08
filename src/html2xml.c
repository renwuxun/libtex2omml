//
// Created by renwuxun on 18-1-17.
//

#include <string.h>
#include "html2xml.h"
#include "mathsymbol.h"
#include "functions.h"
#include "defs.h"


int is_xml_specialchars(const char* base, size_t len) { // '&lt;', '&gt;', '&amp;', '&quot;', '&apos;'
    switch (len) {
        case 2:
            if (0 == memcmp(base, "lt", 2) || 0 == memcmp(base, "gt", 2)) {
                return 1;
            }
            break;
        case 3:
            if (0 == memcmp(base, "amp", 3)) {
                return 1;
            }
            break;
        case 4:
            if (0 == memcmp(base, "quot", 4) || 0 == memcmp(base, "apos", 4)) {
                return 1;
            }
            break;
        default:;
    }
    return 0;
}

void replace_by_unicode_found_cb(const char* base, size_t len, void* onfound_data) {
    log_debug("replace_by_unicode_found_cb");

    struct onfound_data_s* arg = (struct onfound_data_s*)onfound_data;
    size_t last_scan_len = base - arg->laststart;

    if (base[0] == '#' || is_xml_specialchars(base, len)) { // is unicode like &#x1bcd or &#123456;
        arg->xmlbuflen += last_scan_len + len;
        if (arg->xmlbuflen > arg->xmlbufsize) {
            log_warning("replace_by_unicode_found_cb: need more xmlbuf [xmlbuflen:%d, xmlbufsize:%d]", (int)arg->xmlbuflen, (int)arg->xmlbufsize);
            arg->xmlbuflen -= last_scan_len + len;
            return;
        }

        memcpy(arg->xmlbuf, arg->laststart, last_scan_len);
        arg->xmlbuf += last_scan_len;
        memcpy(arg->xmlbuf, base, len);
        arg->xmlbuf += len;

        arg->laststart = (char*)(base+len);
        return;
    }

    char w[len+2]; // !!!
    memset(w, 0, len+2); // !!!
    w[0] = '\\';
    memcpy(w+1, base, len);
    struct mathsymbol_s* symbol = mathsymbols_get(w);
    if (!symbol) {
        log_warning("unicode of math symbol[%s] not found", w);

        arg->xmlbuflen += last_scan_len + len;
        if (arg->xmlbuflen > arg->xmlbufsize) {
            log_warning("replace_by_unicode_found_cb: need more xmlbuf [xmlbuflen:%d, xmlbufsize:%d]", (int)arg->xmlbuflen, (int)arg->xmlbufsize);
            arg->xmlbuflen -= last_scan_len + len;
            return;
        }

        memcpy(arg->xmlbuf, arg->laststart, last_scan_len);
        arg->xmlbuf += last_scan_len;
        memcpy(arg->xmlbuf, base, len);
        arg->xmlbuf += len;

        arg->laststart = (char*)(base+len);
        return;
    }


    size_t unicode_len = strlen(symbol->unicode);

    arg->xmlbuflen += last_scan_len+2+unicode_len;
    log_debug("replace_by_unicode_found_cb:%d, %d", (int)arg->xmlbuflen, (int)arg->xmlbufsize);
    if (arg->xmlbuflen > arg->xmlbufsize) {
        log_warning("replace_by_unicode_found_cb: need more xmlbuf [xmlbuflen:%d, xmlbufsize:%d]", (int)arg->xmlbuflen, (int)arg->xmlbufsize);
        arg->xmlbuflen -= last_scan_len+2+unicode_len;
        return;
    }

    memcpy(arg->xmlbuf, arg->laststart, last_scan_len);
    arg->xmlbuf += last_scan_len;
    memcpy(arg->xmlbuf, "#x", 2);
    arg->xmlbuf += 2;
    memcpy(arg->xmlbuf, symbol->unicode, unicode_len);
    arg->xmlbuf += unicode_len;

    arg->laststart = (char*)(base+len);
}

void replace_by_unicode_finish_cb(const char* subject, size_t subjectlen, const char* lastend, void* onfinish_data) {
    struct onfound_data_s* arg = (struct onfound_data_s*)onfinish_data;

    arg->xmlbuflen += (subject+subjectlen)-lastend+1;
    log_debug("replace_by_unicode_finish_cb:%d,%d", (int)arg->xmlbuflen, (int)arg->xmlbufsize);
    if (arg->xmlbuflen > arg->xmlbufsize) {
        log_warning("replace_by_unicode_finish_cb: need more xmlbuf [xmlbuflen:%d, xmlbufsize:%d]", (int)arg->xmlbuflen, (int)arg->xmlbufsize);
        arg->xmlbuflen -= (subject+subjectlen)-lastend+1;
        return;
    }

    memcpy(arg->xmlbuf, lastend, (subject+subjectlen)-lastend);
    arg->xmlbuf += (subject+subjectlen)-lastend;
    *arg->xmlbuf = '\0';
    arg->xmlbuf += 1;
}

void html2xml(char* htmlstr, size_t htmlstrlen, char* xmlbuf, size_t xmlbufsize) {
    struct onfound_data_s onfound_data;
    onfound_data.xmlbuf = xmlbuf;
    onfound_data.xmlbufsize = xmlbufsize;
    onfound_data.xmlbuflen = 0;
    onfound_data.laststart = htmlstr;

    size_t min = htmlstrlen<xmlbufsize-1 ? htmlstrlen : xmlbufsize-1;
    if (!found_between(
            htmlstr,
            htmlstrlen,
            '&',
            ';',
            replace_by_unicode_found_cb,
            &onfound_data,
            replace_by_unicode_finish_cb,
            &onfound_data
    )) {
        memcpy(xmlbuf, htmlstr, min);
        xmlbuf[min] = '\0';
    }
}
