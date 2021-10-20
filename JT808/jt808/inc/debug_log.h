#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#define  DEBUG_ENABLE

#ifdef DEBUG_ENABLE
extern int debug_printf(int level, const char * file, const char * func, int line, const char * fmt, ...)__attribute__((format(printf, 5, 6)));

#define DBG(...) fprintf(stderr, " %s, %s, %d --- ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define DBG_LEVEL_6(fmt...) debug_printf(6, __FILE__, __FUNCTION__, __LINE__, fmt)

#else

#define DBG(...)
#define DBG_LEVEL_6(...)

#endif

#endif
