#ifndef STACK
#define STACK

typedef struct _stack{
    enum {
        CHAR,
        UNSIGN,
        INT,
        LONG,
        DOUBLE
    }  type;  
    void *stk;
    size_t __size;
    size_t *size();
} stack;

#endif
