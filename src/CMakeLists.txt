set(SRC                                 defs.h mathsymbol.c html2xml.c functions.c tex2omml.c)
add_library(tex2omml                    SHARED ${SRC})
add_library(tex2omml_static             STATIC ${SRC})

set(SRC_LIB                             ${XSLT_LIBRARY} ${XML2_LIBRARY} ${MTEX2MML_LIBRARY})
target_link_libraries(tex2omml          ${SRC_LIB})
target_link_libraries(tex2omml_static   ${SRC_LIB})

set_target_properties(tex2omml_static   PROPERTIES OUTPUT_NAME "tex2omml")

set_target_properties(tex2omml          PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(tex2omml_static   PROPERTIES CLEAN_DIRECT_OUTPUT 1)

set_target_properties(tex2omml          PROPERTIES VERSION 0.1.0 SOVERSION 0.1)
