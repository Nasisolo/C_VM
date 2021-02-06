#include <stdio.h>

/* PUNTATORI */
#define STACK_HEAD *(stack + (*sp))
#define INSTRUCTION *(fetch + (*ip))
#define P1 *(fetch+(*ip)+1)
#define P2 *(fetch+(*ip)+2)
#define R1 (*(registers+r1))
#define R2 (*(registers+r2))


/* DIMENSIONI */
#define DIM_STACK 16384
#define N_REGISTERS 32

/* parametri di return */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define STACK_UNDERFLOW 2
#define STACK_OVERFLOW 3
#define REGISTER_NOT_FOUND 4
#define FILE_NOT_FOUND 5
#define DIV_ZERO 6
#define OVERFLOW 7
#define INSTRUCTION_ERROR 8

/* COLORI */
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define WHITE "\x1b[0m"

/* FUNZIONI AUSILIARIE */
int numero_esiste(const char *line);
int trova_cifra(const char *line);
int *fetch_fun(FILE *f_in, int *size_fetch);
int check_overflow(int a, int b);
int check_fetch(int *fetch, int *size_fetch, int *ip);

/* INSTRUCTION SET */
int halt();
void display(int r1, int *registers, int *ip);
void printf_stack(int n, int *stack, int *sp, int *ip);
int push(int *registers, int r1, int *stack, int *sp, int *ip);
int pop(int *registers, int r1, int *stack, int *sp, int *ip);
void mov(int *registers, int r1, int n, int *ip);
int call(int *ip, int pos, int *stack, int *sp);
int ret(int *stack, int *sp, int *ip);
void jmp(int *ip, int pos);
void jz(int *stack, int *sp, int *ip, int pos);
void jpos(int *stack, int *sp, int *ip, int pos);
void jneg(int *stack, int *sp, int *ip, int pos);
int add(int *registers, int r1, int r2, int *stack, int *sp, int *ip);
int sub(int *registers, int r1, int r2, int *stack, int *sp, int *ip);
int mul(int *registers, int r1, int r2, int *stack, int *sp, int *ip);
int divs(int *registers, int r1, int r2, int *stack, int *sp, int *ip);
