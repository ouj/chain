#ifndef DEBUG_H_
#define DEBUG_H_

#include <cassert>
#include <stdio.h>
#include <cstdarg>

inline void message(const char* msg) {
	fprintf(stdout, "%s\n", msg);
    fflush(stderr);
}
    
inline void message_va(const char* msg, ...) {
    va_list args; 
    va_start (args, msg);
    vfprintf(stdout, msg, args);
    va_end (args);
    fprintf(stdout, "\n");
    fflush(stdout);
}

inline void warning(const char* msg) {
    fprintf(stderr, "warning: %s\n", msg);
    fflush(stderr);
}
    
inline void warning_va(const char* msg, ...) {
    fprintf(stderr, "warning: ");
    va_list args;
    va_start (args, msg);
	vfprintf(stderr, msg, args);
    va_end (args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

    
inline bool warning_if_not(bool check, const char* msg) {
    if(check) return check;
    fprintf(stderr, "warning: %s\n", msg);
    fflush(stderr);
    return check;
}

    
inline bool warning_if_not_va(bool check, const char* msg, ...) {
	if(check) return check;
    fprintf(stderr, "warning: ");
    va_list args;
    va_start (args, msg);
	vfprintf(stderr, msg, args);
    va_end (args);
    fprintf(stderr, "\n");
    fflush(stderr);
    return check;
}

    
inline void error(const char* msg) {
    fprintf(stderr, "error: %s\n", msg);
    fflush(stderr);
    assert(false);
}
    
inline void error_va(const char* msg, ...) {
    fprintf(stderr, "error: ");
    va_list args;
    va_start (args, msg);
	vfprintf(stderr, msg, args);
    va_end (args);
    fprintf(stderr, "\n");
    fflush(stderr);
	assert(false);
}

inline bool error_if_not(bool check, const char* msg) {
    if(check) return check;
    fprintf(stderr, "error: %s\n", msg);
    fflush(stderr);
    assert(check);
    return check;
}
    
inline bool error_if_not_va(bool check, const char* msg, ...) {
	if(check) return check;
    fprintf(stderr, "error: ");
    va_list args;
    va_start (args, msg);
	vfprintf(stderr, msg, args);
    va_end (args);
    fprintf(stderr, "\n");
    fflush(stderr);
	assert(check);
    return check;
}
    
inline void debug_break() {
    message("break: press any key to continue.");
    getchar();
}

#endif

