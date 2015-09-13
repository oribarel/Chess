C_FLAGS=  -std=c99 -pedantic-errors -Wall -g -lm `sdl-config --cflags` `sdl-config --libs`
O_FILES= Chess_Logic.o best.o minimax.o Control_Components.o Chess_Controls.o Chess_Windows.o GUI_Main.o Console_Main.o chessprog.o

all: chessprog

clean:
	rm chessprog $(O_FILES)

chessprog: $(O_FILES)
	gcc -o $@ $^ $(C_FLAGS)

Chess_Logic.o: Chess_Logic.c 
	gcc -c $(C_FLAGS) Chess_Logic.c
	
best.o: best.c 
	gcc -c $(C_FLAGS) best.c

minimax.o: minimax.c 
	gcc -c $(C_FLAGS) minimax.c

Control_Components.o: Control_Components.c
	gcc -c $(C_FLAGS) Control_Components.c

Chess_Controls.o: Chess_Controls.c
	gcc -c $(C_FLAGS) Chess_Controls.c
	
Chess_Windows.o: Chess_Windows.c
	gcc -c $(C_FLAGS) Chess_Windows.c

GUI_Main.o: GUI_Main.c
	gcc -c $(C_FLAGS) GUI_Main.c

Console_Main.o: Console_Main.c
	gcc -c $(C_FLAGS) Console_Main.c

chessprog.o: chessprog.c
	gcc -c $(C_FLAGS) chessprog.c

