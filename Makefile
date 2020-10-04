CC = gcc
CFLAGS = -Werror -Wall -pedantic -ansi

EXEC = TurtleGraphics
OBJ = turtleGraphics.o fileIO.o utils.o linkedList.o effects.o command.o settings.o

EXECs = TurtleGraphicsSimple
OBJs = turtleGraphicsSimple.o fileIO.o utils.o linkedList.o effects.o commandSimple.o settings.o

EXECd = TurtleGraphicsDebug
OBJd = turtleGraphics.o fileIO.o utils.o linkedList.o effects.o commandDebug.o settings.o

#All
all : $(EXEC) $(EXECd) $(EXECs)


#Normal
$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lm

turtleGraphics.o : turtleGraphics.c turtleGraphics.h boolean.h fileIO.h settings.h command.h linkedList.h
	$(CC) -c turtleGraphics.c $(CFLAGS)

fileIO.o : fileIO.c fileIO.h boolean.h command.h linkedList.h utils.h
	$(CC) -c fileIO.c $(CFLAGS)

utils.o : utils.c utils.h boolean.h
	$(CC) -c utils.c $(CFLAGS)

linkedList.o : linkedList.c linkedList.h
	$(CC) -c linkedList.c $(CFLAGS)

effects.o : effects.c effects.h
	$(CC) -c effects.c $(CFLAGS)

command.o : command.c command.h settings.h effects.h linkedList.h utils.h
	$(CC) -c command.c $(CFLAGS)

settings.o : settings.c settings.h effects.h
	$(CC) -c settings.c $(CFLAGS)


#Simple
$(EXECs) : $(OBJs)
	$(CC) $(OBJs) -o $(EXECs) -lm

turtleGraphicsSimple.o : turtleGraphics.c turtleGraphics.h boolean.h fileIO.h settings.h command.h linkedList.h
	$(CC) -c turtleGraphics.c -DNO_COLOURS=1 -o turtleGraphicsSimple.o $(CFLAGS)

commandSimple.o : command.c command.h settings.h effects.h linkedList.h utils.h
	$(CC) -c command.c -DNO_COLOURS=1 -o commandSimple.o $(CFLAGS)


#Debug
$(EXECd) : $(OBJd)
	$(CC) $(OBJd) -o $(EXECd) -lm

commandDebug.o : command.c command.h settings.h effects.h linkedList.h utils.h
	$(CC) -c command.c -DPRINT_LOG=1 -o commandDebug.o $(CFLAGS)


clean:
	$(RM) $(EXEC) $(OBJ) $(EXECs) $(OBJs) $(EXECd) $(OBJd) graphics.log
