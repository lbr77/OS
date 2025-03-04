#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef long int32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;


int64_t syscall(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2);
int64_t sys_write(int64_t fd, const char *buf, int64_t len);
int64_t sys_exit(int64_t exit_code);
void puts(const char *s) ;
int64_t printf(const char *format, ...);
void panic(const char *s, ...);
void write(int64_t fd, const char *buf, int64_t len);
void exit(int64_t code);
int64_t get_time();
int64_t yield();
#endif // _SYS_H_