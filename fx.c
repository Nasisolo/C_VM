#include "fx.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/* controlla la riga contiene almeno un numero prima del ';' */
int numero_esiste(const char *line){
  int risposta = 0;
  int i = 0;
  while(*(line+i)!=';' && *(line+i)!= '\t' && *(line+i)!='\n' && *(line+i)!= '\0'){
    if(*(line+i)>='0' && *(line+i)<='9')
      risposta = 1;
    i++;
  }
  return risposta;
}

/* trova la cifra nella riga antecendente al carattere ';' */
int trova_cifra(const char *line){
  int i = 0;
  int i_numero = 0;
  char *numero= (char*)malloc(sizeof(int)*1000);
  int risultato = 0;

  /* scorrimento della riga finchè non trovo un ; */
  while(*(line+i)!=';'){
    if(*(line+i)>='0' && *(line+i)<='9'){
      /* costuzione del buffer per la atoi */
      numero[i_numero] = *(line+i);
      i_numero++;
    }
    i++;   
  }
  risultato = atoi(numero);
  free(numero);
  numero = NULL;
  return risultato;
}

/* FETCH: inserimento del codice macchina nell'array fetch */
int *fetch_fun(FILE *f_in, int *size_fetch){
  int *fetch = NULL;
  int pos_fetch = 0; 
  
  /* parametri della getline */
  char *line;
  size_t len = 0;
  ssize_t nread;
  
  while((nread = getline(&line, &len, f_in)) != -1){
    /* controllo se nella riga è presente un numero */
    if(numero_esiste(line)){

      if(*(size_fetch)==0){
        /*
        cerco il numero, se è la prima riga con numero
        la prima riga con un numero rappresenta la dimensione
        dell'array fecth contente codice macchina
        */
        *size_fetch = trova_cifra(line);
        fetch = (int*)malloc(sizeof(int)*(*size_fetch));
      }else{
        /*
        in caso la prima riga contenente la dimensione
        fosse stata trovata, popolo l'array di fetch
        con il codice macchina
        */
        *(fetch+pos_fetch) = trova_cifra(line);
        /* DEBUG : mostra il valore preso */
        /*
        printf("FETCH ARRAY: %d -> %d\n", pos_fetch, *(fetch+pos_fetch));
        */
        pos_fetch++;
      }
    }

    /* pulizia riga appena utilizzata */
    free(line);
    line=NULL;
  }

  /* DEBUG: STAMPA ARRAY FECTH  */
  /*
  for(pos_fetch=0; pos_fetch<(*size_fetch); pos_fetch++){
      printf("%d\n", *(fetch+pos_fetch));
    }
  */
  /* FINE FETCH */
  free(line);
  line = NULL;

  return fetch;
}

/* funzione per controllare se un risultato non è rappresentabile (overflow) */
int check_overflow(int a, int b){
  int risp = EXIT_SUCCESS; /* assumo che non ci sia overflow */
  if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b))
    risp = EXIT_FAILURE;

  return risp; 
}


int check_mul_overflow(int a, int b){
  int risp = EXIT_SUCCESS;
  
  if(a != 0 && b != 0 && (INT_MAX / b) < a)
      risp = EXIT_FAILURE;
  
  return risp;
}


/*  funzione per controllare che non ci siano istruzioni inesistenti o
    registri (come parametri delle istruzioni) non validi 
*/
int check_fetch(int *fetch, int *size_fetch, int *ip){
  int errors = EXIT_SUCCESS; /* assumo che non ci siano errori */

  while(*ip < *size_fetch && errors == 0){
    switch (INSTRUCTION)
    {
    case 0:   /* HALT */
    case 21:  /* RET */
      (*ip) += 1;
      break;
    
    case 2:   /* PRINT_STACK */
      (*ip) += 2;
      break;

    case 1:   /* DISPLAY */
    case 10:  /* PUSH */
    case 11:  /* POP */
      /* se il registro passato alla funzione non esiste, ritorna il valore di errore */
      errors = (P1 < 0 || P1 > N_REGISTERS) ? REGISTER_NOT_FOUND : EXIT_SUCCESS;
      (*ip) += 2;
      break;
    
    case 12:  /* MOV */
      /* se il registro passato alla funzione non esiste, ritorna il valore di errore */
      errors = (P1 < 0 || P1 > N_REGISTERS) ? REGISTER_NOT_FOUND : EXIT_SUCCESS;
      (*ip) += 3;
      break;

    case 20:  /* CALL */
    case 22:  /* JMP */
    case 23:  /* JZ */
    case 24:  /* JPOS */
    case 25:  /* JNEG */
      (*ip) += 2;
      break;

    case 30:  /* ADD */
    case 31:  /* SUB */
    case 32:  /* MUL */
    case 33:  /* DIV */
      errors = (P1 < 0 || P1 > N_REGISTERS || P2 < 0 || P2 > N_REGISTERS) ? REGISTER_NOT_FOUND : EXIT_SUCCESS;
      (*ip) += 3;
      break;
    
    default:  /* qualsiasi altro numero al di fuori di questo instruction set è errore */
      errors = INSTRUCTION_ERROR;
      break;
    }
  }
  /* reset del valore di IP */
  (*ip) = 0;
  return errors;
}

/* --------------------ISTRUCTION SET-------------------- */

/* ISTRUZIONE 0: termina il programma */
/* (esce dallo switch) */
int halt(){
  return EXIT_SUCCESS;
} /* gestione errore troppi argomenti */

/* ISTRUZIONE 1: mostra su console il contenuto del registers[reg] */
void display(int r1, int *registers, int *ip){
  /* stampa del registro P1 con relativo contenuto */
  printf("OUTPUT: %d\n", R1);
  /* incremento IP della lunghezza dell'istruzione */
  *(ip) += 2;
}

/* ISTRUZIONE 2: stampa da SP-1 a SP-N valori dello stack */
void printf_stack(int n, int *stack, int *sp, int *ip){
  int i;
  for(i = *(sp)-1; i >= *(sp)-n; i--){
    if(i >= 0)  /* stampa possibile solo fino a SP = 0 */
      printf("STACK pos: %2d, value: %d\n", *(sp)-i, *(stack-i));
  }
  /* incremento IP della lunghezza dell'istruzione */
  (*ip) += 2;
}

/* ISTRUZIONE 10: immette il valore di registers[reg] nello stack */
int push(int *registers, int r1, int *stack, int *sp, int *ip){
  if(*sp < DIM_STACK){  /* controllo il possibile overflow */
    /* POP in posizione stack[sp] il valore del registro[r1] */
    STACK_HEAD = R1;
    /* incremento SP per indicarne la prossima cella libera */
    (*sp) += 1;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 2;
    return EXIT_SUCCESS;
  }else{
    return STACK_OVERFLOW;
  }
}

/* ISTRUZIONE 11: estrae l'ultimo elemento dello stack (SP-1) */
int pop(int *registers, int r1, int *stack, int *sp, int *ip){
  if(*sp > 0){  /* controllo del possibile underflow */
    /* decremento lo SP in quanto estrarrò il suo valore */
    (*sp) -= 1;
    /* inserisce nel registers[r1] il valore poppato */
    R1 = STACK_HEAD;
    /* 'pulisce' la cella dello stack */
    STACK_HEAD = 0;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 2;
    return EXIT_SUCCESS;
  }else{
    return STACK_UNDERFLOW;
  }
}

/* ISTRUZIONE 12: inserisce in registers[reg] il P2 */
void mov(int *registers, int r1, int n, int *ip){
  R1 = n;
  /* incremento IP della lunghezza dell'istruzione */
  (*ip) += 3;
}

/* ISTRUZIONE 20: chiama a subroutine */
int call(int *ip, int pos, int *stack, int *sp){
  if(*sp < DIM_STACK){  /* controllo del possibile overflow */
    /* push dell'IP nello stack */
    STACK_HEAD = *(ip) + 2;
    /* incremento del SP di 1 */
    *sp += 1;
    /* aggiorna IP a posizione */
    (*ip) = pos;
    return EXIT_SUCCESS;
  }else{
    return STACK_OVERFLOW;
  }
  
}
/* ISTRUZIONE 21 : ritorno dalla subrutine */
int ret(int *stack, int *sp, int *ip){
  if(*sp > 0){  /* controllo del possibile  underflow*/
    /* decremento  del SP di 1 */
    (*sp) -= 1;
    /* assegna alla poizione ip del vettore istruction register il valore poppato  */
    (*ip) = *(stack + (*sp));
    /* resetta l'ultimo cella disponibile nello stack*/
    STACK_HEAD = 0;
    return EXIT_SUCCESS;
  }else{
    return STACK_UNDERFLOW;
  }
}
/*ISTRUZIONE 22 : salto alla posizione indicata passata come parametro */
void jmp(int *ip, int pos){
  /*aggiorna il valore dell' istruction pointer con la posizione */
  (*ip) = pos; 
}

/* ISTRUZIONE 22 : salto condizionato in posizione pos (P1) se l'ultimo elem dello stack e' = 0 */
/* poppa l'elemento dallo stack a prescindere, fa il jump in ip se lo stack non e' vuoto  */
void jz(int *stack, int *sp, int *ip, int pos){
  if((*sp > 0) && (*(stack + (*sp)-1) == 0)){
    /* salta all'IP definito come parametro pos */
    jmp(ip, pos);
    /* pop dell'ultimo elemento inserito nello stack */
    (*sp) -= 1;
    /* resetta la testa dello stack (diventa una cella libera) */
    STACK_HEAD = 0;
  }else{
    /* pop dell'ultimo elemento inserito nello stack */
    (*sp) -= 1;
    STACK_HEAD = 0;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 2;
  }
}

/* ISTRUZIONE 24: salto condizionato in posizione pos (P1 ) se l'ultimo elem dello stack e' > 0  */
/* poppa l'elemento dallo stack a prescindere, fa il jump in ip se lo stack non e' vuoto  */
void jpos(int *stack, int *sp, int *ip, int pos){
  if((*sp > 0) && (*(stack + (*sp)-1) > 0)){
    jmp(ip, pos);
    /* rimozione dell'ultimo elemento dello stack con rrelativo decremento */
    (*sp) -= 1;
    STACK_HEAD = 0;
  }else{
    (*sp) -= 1;
    STACK_HEAD = 0;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 2;
    }
}

/* ISTRUZIONE 25: salto condizionato in posizione pos (P1 ) se l'ultimo elem dello stack e' < 0 */
/* poppa l'elemento dallo stack a prescindere, fa il jump in ip se lo stack non e' vuoto  */
void jneg(int *stack, int *sp, int *ip, int pos){
  if((*sp >0) && (*(stack + (*sp)-1) < 0)){
    jmp(ip, pos);
    /* rimozione dell'ultimo elemento dello stack con rrelativo decremento */
    (*sp) -= 1;
    *(stack + (*sp)) = 0;
  }else{
    /* rimozione dell'ultimo elemento dello stack con rrelativo decremento */
    (*sp) -= 1;
    *(stack + (*sp)) = 0;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 2;
  }
}

/* ISTRUZIONE 30 : somma tra P1  e P2 che sono 2 registri */
int add(int *registers, int r1, int r2, int *stack, int *sp, int *ip){
  if(*sp < DIM_STACK){  /*controlla lo stack overflow */
    /* controllo dell'overflow */
    if(check_overflow(R1, R2) == EXIT_FAILURE)
      return OVERFLOW;
    /* push nello stack del risultato della ADD con relativo incremento di SP */
    STACK_HEAD = R1 + R2;
    (*sp) += 1;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 3;
    return EXIT_SUCCESS;
  }
  else
    return STACK_OVERFLOW;
}

/*ISTRUZIONE 31 : e sottrazione tra i registri P1 e P2  */
int sub(int *registers, int r1, int r2, int *stack, int *sp, int *ip){
  if(*sp < DIM_STACK){  /*controlla lo stack overflow */
    /* controllo dell'overflow */
    if(check_overflow(R1, R2) == EXIT_FAILURE)
      return OVERFLOW;
    
    /* push nello stack del risultato della SUB con relativo incremento di SP */
    STACK_HEAD = R1 - R2;
    (*sp) += 1;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 3;
    return EXIT_SUCCESS;
  }
  else
    return STACK_OVERFLOW;
  
}

/*ISTRUZIONE 32 : moltiplicazione   tra i registri P1 e P2 */
int mul(int *registers, int r1, int r2, int *stack, int *sp, int *ip){
  if(*sp < DIM_STACK){  /*controlla lo stack overflow */
    if(check_mul_overflow(R1, R2) == EXIT_FAILURE)
      return OVERFLOW;
    /* push nello stack del risultato della MUL con relativo incremento di SP */
    STACK_HEAD = R1 * R2;
    (*sp) += 1;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 3;
    return EXIT_SUCCESS;
  }
  else
    return STACK_OVERFLOW;
  
}

/*ISTRUZIONE 33 : divisione tra i registri P1 e P2 */
int divs(int *registers, int r1, int r2, int *stack, int *sp, int *ip){
  if(*sp < DIM_STACK){  /* controllo stack overflow */
      if(r2 == 0) /* controlla la divisione per 0 che non e' ammessa */
        return DIV_ZERO;
    /* push nello stack del risultato della DIV con relativo incremento di SP */
    STACK_HEAD = R1 / R2;
    (*sp) += 1;
    /* incremento IP della lunghezza dell'istruzione */
    (*ip) += 3;
    return EXIT_SUCCESS;
  }
  else
    return STACK_OVERFLOW;
}

