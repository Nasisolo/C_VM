<<<<<<< HEAD
# C Virtual Machine

Implementazione una macchina virtuale con registri e stack che esegua dei programmi specificati in codice macchina in un’array di interi nel quale ciascuna istruzione occupa un numero variabile di posizioni.

### Stack

La macchina virtuale utilizza un’area di memoria di 64KB come stack (accesso LIFO).
Il registro speciale SP indica la posizione della prossima posizione dello stack da scrivere (0 stack vuoto).
L’istruzione PUSH inserisce un intero nello stack (ed incrementa SP), l’istruzione POP preleva un elemento dallo stack (dopo aver decrementato SP).
In caso di stack overflow (PUSH su stack pieno) o stack underflow (POP da stack vuoto) il programma deve terminare immediatamente segnalando un errore.

### Instruction pointer, jump, chiamate a subroutine

La vm dopo il bootstrap durante il quale vengono inizializzate le strutture dati ed i registri, ripetutamente legge le istruzioni del programma in codice macchina (fetch), ne prepara gli operandi e le esegue (execute).

Il registro speciale instruction pointer (IP) contiene l’indice della prossima istruzione da eseguire. Dopo il fetch di un’istruzione viene incrementato in modo tale da indicare l’istruzione successiva (dipende dal numero di argomenti dell’istruzione, v. sezione instruction set della documentazione).

L’istruzione jump (JMP) sostituisce il valore corrente di IP con il valore indicato mentre le istruzioni jump condizionate eseguono il salto solo se le rispettive condizione sono vere verificate.

La chiamata a subroutine (CALL) inserisce nello stack il valore corrente di IP (già aggiornato alla posizione successiva a CALL) e lo sostituisce con il valore indicato. 
RET permette di rientrare da una chiamata a subroutine sostituendo ad IP il valore prelevato dallo stack

### Registri

La macchina virtuale può utilizzare 32 registri interi R0-R31. Ciascun registro può essere utilizzato come operando o contenere il risultato di operazioni. Vedere la sezione instruction set per i dettagli.

## Utilizzo

Compilare il sorgente utilizzando `make` dal terminale nella directory, verranno prodotti dei file oggetto e l'eseguibile di nome **VM**. Questa virtual machine permette di leggere il codice sorgente di un programma oppure di eseguirlo tramite gli appositi comandi da linea di comando. (alcuni programmi di test sono stati inseriti nella directory '/programs')

Per leggere il codice sorgente di un programma, digitare il comando `./VM leggi programs/input_file.cvm` sostituendo opportunamente `input_file` con il nome di un programma.

Per eseguire il codice, la procedura rimane la stessa, sostituendo `leggi` con `esegui` del comando precedentemente citato.
