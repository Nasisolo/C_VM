#include "vm.h"
#include "fx.h"
#include <stdlib.h>
#include <stdio.h>

/* ------------------ SEZIONE COMANDI VM ------------------ */

/* comando STAMPA */
int stampa(const char *input){
  
  /* inizializzazione strutture dati per la stampa */
  /* non sono necessari stack o registri in quanto rimarrebbero inutilizzati */
  FILE *f_in;
  int *fetch = NULL;
  int *size_fetch = (int *)malloc(sizeof(int));
  int *ip = (int *)malloc(sizeof(int));

  *size_fetch = 0;
  *ip = 0;

  f_in = fopen(input, "r");


  /* in caso di file inesistente terminare la vm */
  if(f_in == NULL){
    free(size_fetch);
    size_fetch = NULL;
    free(ip);
    ip = NULL;
    return FILE_NOT_FOUND;
  }

  fetch = fetch_fun(f_in, size_fetch);

  /* switch per la stampa in base all'array ottenuto dopo il parsing (fecth) */ 
  while(*ip < *size_fetch){
    switch (INSTRUCTION)
    {
    case 0:
      printf("[%3d]  HALT\n", *ip);
      (*ip) += 1;
      break;
    
    case 1:
      printf("[%3d]  DISPLAY R%d\n", *ip, P1);
      (*ip) += 2;
      break;

    case 2:
      printf("[%3d]  PRINT_STACK %d\n", *ip, P1);
      (*ip) += 2;
      break;
    
    case 10:
      printf("[%3d]  PUSH R%d\n", *ip, P1);
      (*ip) += 2;
      break;

    case 11:
      printf("[%3d]  POP R%d\n", *ip, P1);
      (*ip) += 2;
      break;
    
    case 12:
      printf("[%3d]  MOV R%d %d\n", *ip, P1, P2);
      (*ip) += 3;
      break;

    case 20:
      printf("[%3d]  CALL %d\n", *ip, P1);
      *(ip) += 2;
      break;

    case 21:
      printf("[%3d]  RET\n", *ip);
      *(ip) += 1;
      break;
    
    case 22:
      printf("[%3d]  JMP %d\n", *ip, P1);
      *(ip) += 2;
      break;
    
    case 23:
      printf("[%3d]  JZ %d\n", *ip, P1);
      *(ip) += 2;
      break;
    
    case 24:
      printf("[%3d]  JPOS %d\n", *ip, P1);
      *(ip) += 2;
      break;
    
    case 25:
      printf("[%3d]  JNEG %d\n", *ip, P1);
      *(ip) += 2;
      break;
    
    case 30:
      printf("[%3d]  ADD R%d R%d\n", *ip, P1, P2);
      *(ip) += 3;
      break;

    case 31:
      printf("[%3d]  SUB R%d R%d\n", *ip, P1, P2);
      *(ip) += 3;
      break;

    case 32:
      printf("[%3d]  MUL R%d R%d\n", *ip, P1, P2);
      *(ip) += 3;
      break;
    
    case 33:
      printf("[%3d]  DIV R%d R%d\n", *ip, P1, P2);
      *(ip) += 3;
      break;

    default:
      break;
    }

  }
  
  /* chiusura del file e free dei puntatori */
  fclose(f_in);
  free(fetch);
  fetch = NULL;
  free(size_fetch);
  size_fetch = NULL;
  free(ip);
  ip = NULL;
  
  return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------------------------- */

/* comando ESEGUI */
int esegui(const char *input){

  /* dichiarazioni e inizializzazioni strutture dati */
  FILE *f_in;
  int *fetch = NULL;
  int *size_fetch = (int *)malloc(sizeof(int));
  int *ip = (int *)malloc(sizeof(int));
  int *stack = (int *)malloc(sizeof(int)*DIM_STACK);
  int *sp = (int *)malloc(sizeof(int));
  int *registers = (int *)malloc(sizeof(int)*N_REGISTERS);

  int i, errors, stop;

  *size_fetch = 0;
  *ip = 0;  /* puntatore attuale dell' instruction pointer */ 
  *sp = 0;  /* puntatore all'ultima posizione libera nello stack (testa) */

  /* inizializzo lo stack a 0 */
  for(i=0; i<DIM_STACK; i++)
    *(stack+i) = 0;
  /* inizializzo i registri a 0 */
  for(i=0; i<N_REGISTERS; i++)
    *(registers+i) = 0;

  f_in = fopen(input, "r");

  /* in caso di file inesistente terminare la vm */
  if(f_in == NULL){
    free(size_fetch);
    size_fetch = NULL;
    free(ip);
    ip = NULL;
    free(stack);
    stack =NULL;
    free(sp);
    sp = NULL;
    free(registers);
    return FILE_NOT_FOUND;
  }

  /* fetch: inserimento del codice macchina nell'array di fetch */
  fetch = fetch_fun(f_in, size_fetch);

  errors = 0; /* 0 equivale a EXIT_SUCCESS */
  /* cerca il primo errore che potrebbe compromettere il funzionamento della VM */
  errors = check_fetch(fetch, size_fetch, ip);
  /* DEBUG: mostra errore */
  /*
  printf("errore : %d\n", errors);
  */

  stop = 1; /* flag per terminare la VM in caso di HALT */
  /* esecuzione comandi dal fetch array*/
  /* continua l'esecuzione finchè non viene fatta una HALT er finchè non trova errori */
  /* errors = 0 rappresenta EXIT_SUCCESS */
  while(*ip < *size_fetch && stop == 1 && errors == 0){

    switch (INSTRUCTION)
    {
    case 0:
      stop = halt();
      break;

    case 1:
      display(P1, registers, ip);
      break;

    case 2:
      printf_stack(P1, stack, sp, ip);
      break;
    
    case 10:
      errors = push(registers, P1, stack, sp, ip);
      break;

    case 11:
      errors = pop(registers, P1, stack, sp, ip);
      break;
    
    case 12:
      mov(registers, P1, P2, ip);
      break;

    case 20:
      errors = call(ip, P1, stack, sp);
      break;

    case 21:
      errors = ret(stack, sp, ip);
      break;
    
    case 22:
      jmp(ip, P1);
      break;
    
    case 23:
      jz(stack, sp, ip, P1);
      break;
    
    case 24:
      jpos(stack, sp, ip, P1);
      break;
    
    case 25:
      jneg(stack, sp, ip, P1);
      break;
    
    case 30:
      errors = add(registers, P1, P2, stack, sp, ip);
      break;

    case 31:
      errors = sub(registers, P1, P2, stack, sp, ip);
      break;

    case 32:
      errors = mul(registers, P1, P2, stack, sp, ip);
      break;
    
    case 33:
      errors = divs(registers, P1, P2, stack, sp, ip);
      break;

    default:
      break;
    }

  }

  
  fclose(f_in);
  free(fetch);
  fetch = NULL;
  free(size_fetch);
  size_fetch = NULL;
  free(ip);
  ip = NULL;
  free(stack);
  stack = NULL;
  free(sp);
  sp = NULL;
  free(registers);
  return errors;
}