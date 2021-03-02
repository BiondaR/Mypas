# Bacharelado em Ciências da Computação
# UNESP Rio Claro
# Período Integral 

# Desenvolvido pelo grupo 3:
# Bionda Rozin
# Matheus Missio Francisco
# Nicholas Seiti Handa
# Nikolas Gomes de Sá
  
# Data de criação: 21/02/2021

# Set default C-Flags to allow debug and also to have include diretory as "./include"
# The -g directive used for debugging, associates each line of code or ';' with a node in a graph
CFLAGS = -g -I./include
#LDFLAGS = -lm 

# The variable CC defines which compiler should be used.  
# The CFLAGS variable defines the options that should be passed to the compiler.
# With CC and CFLAGS defined, make uses implicit rules to individually compile the object files from the C sources.

# $@ - full name of the target of this rule
# $^ - list of all dependencies, separated by space
mypas: mypas.o lexer.o parser.o pseudocode.o keywords.o symtab.o
	$(CC) -o $@ $^

# Delete all files that are normally created by running make.
clean:
	$(RM) *.o

# Like ‘clean’, but may refrain from deleting a few files that people normally don’t want to recompile. 
# For example, the ‘mostlyclean’ target for GCC does not delete libgcc.a, 
# because recompiling it is rarely necessary and takes a lot of time.
mostlyclean: clean
	$(RM) *~
