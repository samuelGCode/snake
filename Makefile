#Snake
#
#Descripcion: Compilacion de la reeproduccion del videojuego Snake
#Makefile
#para el video juego "Snake" con interfaz emulada desde la shell de linux
#
#Autor: SamuelGCode
#correo:samale456@gmail.com



#macros personalizadas
LINK=-lncurses
DIR_CORE=src/
DIR_BUILD=build/

snake: main
	$(CXX) $(CXXFLAGS) -o snake $(DIR_BUILD)main.o $(LINK)

main:
	$(CXX) $(CXXFLAGS) -c $(DIR_CORE)main.cpp -o $(DIR_BUILD)main.o

clean: 
	$(RM) $(DIR_BUILD)*.o snake