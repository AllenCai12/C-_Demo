#ifndef C_ATOM_H
#define C_ATOM_H


int Atom_length(const char *str);
const char *Atom_new(const char *str, int length);
const char *Atom_int(long n);
const char *Atom_string(const char *str, int length);

#endif