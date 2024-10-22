#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>
void *memcpy(void *dest, const void *src, size_t len);
int strcmp(const char *p1, const char *p2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *destination, const char *source);
char *strncpy(char *destination, const char *source, size_t len);
size_t strlen(const char *str);
#endif
