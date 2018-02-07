//
// Created by renwuxun on 18-1-17.
//

#ifndef TEX2OMML_HTML2XML_H
#define TEX2OMML_HTML2XML_H


struct onfound_data_s {
    char* xmlbuf;
    size_t xmlbufsize;
    size_t xmlbuflen;
    char* laststart;
};

//void replace_by_unicode_found_cb(const char* base, size_t len, void* onfound_data);

//void replace_by_unicode_finish_cb(const char* subject, size_t subjectlen, const char* lastend, void* onfinish_data);

void html2xml(char* htmlstr, size_t htmlstrlen, char* xmlbuf, size_t xmlbuflen);


#endif //TEX2OMML_HTML2XML_H
