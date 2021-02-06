#http://makefiletutorial.com/
objects = vm.o fx.o main_vm.o
eseguibile = VM
cflags = -g3 -fsanitize=address -fsanitize=undefined -std=gnu89 -pedantic-errors -Wall -Wextra 
libs = -lm

$(eseguibile) : $(objects)
	gcc $(cflags) -o $(eseguibile) $(objects) $(libs)

clean:
	rm $(eseguibile)  $(objects)

#regole implicite
.c.o :
	gcc -c $(cflags) $< 
.h.c :
	touch $<

