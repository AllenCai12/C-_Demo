#ifndef C_BASE_H
#define C_BASE_H

struct base {
    void (* fun1)(struct base*b );  
    void (* fun2)(struct base*b );  
    void (* fun3)(struct base*b, char *str);  
};

#endif
