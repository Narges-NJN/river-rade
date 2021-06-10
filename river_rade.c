#include <ncurses.h>                    //the program is based on ncurses library's text UI
#include "view.h"                       //local library including appierences
#include "compute.c"

int main (){
    initscr();                  //initiates the standard screen 
    keypad(stdscr, true);       //enables the keypad(arrow keys)   
    start_color();       
    start();                    //function in compute.c
    endwin();                   //finish program
    return 0;                                  
}