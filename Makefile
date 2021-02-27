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
CFLAGS = -g -I./include
#LDFLAGS = -lm 

mypas: mypas.o lexer.o parser.o pseudocode.o keywords.o symtab.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~
