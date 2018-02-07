//
// Created by renwuxun on 18-1-17.
//

#ifndef TEX2OMML_DEFS_H
#define TEX2OMML_DEFS_H

#include <unistd.h>
#include <stdio.h>


#ifndef container_of
# define container_of(ptr, type, field) ((type*)((char*)(ptr) - ((char*)&((type*)0)->field)))
#endif


/*致命错误*/
#define log_crit(fmt,...) fprintf(stderr, "[pid:%d] : "fmt"\n", getpid(), ##__VA_ARGS__)
/*错误*/
#define log_error(fmt,...) fprintf(stderr, "[pid:%d] : "fmt"\n", getpid(), ##__VA_ARGS__)
/*警告*/
#ifdef NOWARNING
# define log_warning(fmt,...)
#else
# define log_warning(fmt,...) fprintf(stderr, "[pid:%d] : "fmt"\n", getpid(), ##__VA_ARGS__)
#endif
/*注意，统计类日志*/
#define log_notice(fmt,...) fprintf(stderr, "[pid:%d] : "fmt"\n", getpid(), ##__VA_ARGS__)
/*调试信息*/
#define log_info(fmt,...) fprintf(stdout, "[pid:%d] : "fmt"\n", getpid(), ##__VA_ARGS__)

#ifdef DEBUG
# define log_debug(fmt,...) fprintf(stdout, "[pid:%d] : "fmt"\n", getpid(), ##__VA_ARGS__)
#else
# define log_debug(fmt,...)
#endif




#ifndef ALIGNMENT
#define ALIGNMENT sizeof(unsigned long)
#endif

#ifndef align
# define align(size, alignment) (((size) + (alignment - 1)) & ~(alignment - 1))
#endif

#ifndef align_ptr
# define align_ptr(ptr, alignment) (char*)(((uintptr_t) (ptr) + ((uintptr_t) alignment - 1)) & ~((uintptr_t) alignment - 1))
#endif

#endif //TEX2OMML_DEFS_H
