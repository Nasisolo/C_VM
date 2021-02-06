/*
 * GRUPPO2 8703741 8693862
 * 870374: Nasir Monan
 * 869386: Piazza Alessandro
*/

#include <stdio.h>
#include <string.h>
#include "vm.h"

int main(int argc, char const *argv[]) {

  int  i;
  
  /*
   *  argv[1] = comando 
   *  argv[2] = fileINPUT
  */

  printf("ACCENSIONE VIRTUAL MACHINE\nComando inserito: ");
  /* mostro il comando a video */
  for (i=0; i<argc; i++) {
    printf("%s ", argv[i]);
  }
  printf("\n");

  /* gestione errore troppi argomenti */
  if(argc > 3){
    printf(RED "ERRORE: troppi argomenti\n");
    printf("SYSTEM: per favore usare la sintassi: ./VM esegui/stampa programs/input_file.cvm\n" WHITE);
  }
  /* gestione pochi argomenti */
  else if(argc < 3){
    printf(RED "ERRORE: pochi argomenti\n");
    printf("SYSTEM: per favore usare la sintassi: ./VM esegui/stampa programs/input_file.cvm\n" WHITE);
  }

  /* gestione del ritorno degli errori della ESEGUI */
  else if(!strcmp(argv[1], "esegui"))
    switch(esegui(argv[2]))
    {
      case EXIT_SUCCESS:
        printf(GREEN "ESECUZIONE COMPLETATA CON SUCCESSO\n" WHITE);
        break;

      case INSTRUCTION_ERROR:
        printf(RED "ERRORE: ISTRUZIONE NON TROVATA\n" WHITE);
        break;

      case EXIT_FAILURE:
        printf(RED "ERRORE: FALLIMENTO ESECUZIONE\n" WHITE);
        break;

      case STACK_UNDERFLOW:
        printf(RED "ERRORE: STACK UNDEFLOW\n" WHITE);
        break;

      case STACK_OVERFLOW:
        printf(RED "ERRORE: STACK OVERFLOW\n" WHITE);
        break;
      
      case REGISTER_NOT_FOUND:
        printf(RED "ERRORE: REGISTRO INVALIDO NEL PROGRAMMA\n" WHITE);
        break;

      case FILE_NOT_FOUND:
        printf(RED "ERRORE: FILE NON TROVATO\n" WHITE);
        break;

      case DIV_ZERO:
        printf(RED "ERRORE: FILE NON TROVATO\n" WHITE);
        break;
      
      case OVERFLOW:
        printf(RED "ERRORE: OVERFLOW NUMERICO\n" WHITE);
        break;
    }
  else if(!strcmp(argv[1], "stampa")) 
    stampa(argv[2]);
  else
  {
    printf(RED "ERRORE: COMANDO NON TROVATO\n");
    printf("SYSTEM: per favore usare la sintassi: ./VM esegui/stampa input_file.cvm\n" WHITE);
  }
  
  printf("SPEGNIMENTO VIRTUAL MACHINE\n");
  return 0;
}
