#include "chessprog.h"

int main(int argc, char *argv[]) {
	if (argc == 1){
		Console_Main();
	}
	else if (argc == 2 && strncmp(argv[1], "console", 7) == 0){
		Console_Main();
	}
	else if (argc == 2 && strncmp(argv[1], "gui", 3) == 0){
		GUI_Main();
	}
	return 0;
}
