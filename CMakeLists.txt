cmake_minimum_required(VERSION 2.8.7)
project(libtex2omml)


set(CMAKE_C_FLAGS_DEBUG, "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG, "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall")


IF(CMAKE_BUILD_TYPE STREQUAL Release)
    add_definitions(-DNOWARNING)
    add_definitions(-DRELEASE)
ENDIF()


FIND_PATH(XSLT_INCLUDE_DIR libxslt/transform.h)
FIND_LIBRARY(XSLT_LIBRARY xslt)
IF (XSLT_INCLUDE_DIR AND XSLT_LIBRARY)
    include_directories(${XML2_INCLUDE_DIR})
ELSE()
    message(FATAL_ERROR "libxslt not found!")
ENDIF()

FIND_PATH(XML2_INCLUDE_DIR libxml2/libxml/tree.h)
FIND_LIBRARY(XML2_LIBRARY xml2)
IF (XML2_INCLUDE_DIR AND XML2_LIBRARY)
    include_directories(${XML2_INCLUDE_DIR}/libxml2)
ELSE()
    message(FATAL_ERROR "libxml2 not found!")
ENDIF()

FIND_PATH(MTEX2MML_INCLUDE_DIR NAMES mtex2MML/mtex2MML.h)
FIND_LIBRARY(MTEX2MML_LIBRARY NAMES mtex2MML)
IF(MTEX2MML_INCLUDE_DIR AND MTEX2MML_LIBRARY)
    include_directories(${MTEX2MML_INCLUDE_DIR})
ELSE()
    message(FATAL_ERROR "libmtex2MML not found!")
ENDIF()



include_directories(deps)


add_subdirectory(src tests)


add_executable(tex2mml tests/tex2mml.c)
add_executable(mml2omml tests/mml2omml.c)
add_executable(mathsymbol tests/mathsymbol.c)
add_executable(functions tests/functions.c)
add_executable(html2xml tests/html2xml.c)
add_executable(tex2omml_test tests/tex2omml.c)


target_link_libraries(tex2mml tex2omml)
target_link_libraries(mml2omml tex2omml)
target_link_libraries(mathsymbol tex2omml)
target_link_libraries(functions tex2omml)
target_link_libraries(html2xml tex2omml)
IF(CMAKE_BUILD_TYPE STREQUAL Release)
        target_link_libraries(tex2omml_test tex2omml)
ELSE()
        target_link_libraries(tex2omml_test tex2omml profiler tcmalloc)
ENDIF()


INSTALL(DIRECTORY src/ deps/uthash DESTINATION include/libtex2omml
        FILES_MATCHING PATTERN "*.h")

INSTALL(DIRECTORY ${PROJECT_BINARY_DIR}/tests/ DESTINATION lib/libtex2omml
        FILES_MATCHING PATTERN "libtex2omml.*"
        PATTERN "CMakeFiles" EXCLUDE)

