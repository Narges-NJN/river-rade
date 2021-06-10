#include <ncurses.h>                    //the program is based on ncurses library's text UI
#include <stdlib.h>                     //the program uses srand() and rand() functions from stdlib.h library
#include <string.h>                     //the program uses strlen() function from string.h library
#include <time.h>                       //the program uses time() function from time.h library
#include <unistd.h>                     //the program uses sleep() and usleep() functions from unistd.h library

//----defining colors----
#define OBSTACLE_PAIR  2
#define PLANE_PAIR  1

//---declaring functiond used in the program---
void start();
void login_page (char *name);
int game_core(char *name, int score, int *last_score, int *high_score, int maxY, int maxX);
int random_number (int maxX);
int speed(int score);
int end_game(char *name, int *last_score, int *high_score, int maxY, int maxX);
int pouse (char *name, int score, int maxX, int maxY);

void start(){                           
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); 
    init_pair(2, COLOR_GREEN, COLOR_BLACK); 
    srand(time(NULL));                          //seeds rand function by using the computer's internal clock.Since time, the seed is forever changing
    int maxX,maxY;
    getmaxyx(stdscr, maxY, maxX); 
    int score = 0;                              
    int last_score = 0;                         
    int high_score = 0;                                 
    char user_name [50];                        
    login_page(user_name);                      //call the fuction for creating log-in page 
    noecho();                                   //dont display input key value 
    game_core(user_name, score, &last_score, &high_score, maxY, maxX);  //call the fuction that starts and runs the game
    nodelay(stdscr, 0);                         //dont stop and wait for user input when getch() is called

    while(1)                                    
    {
        int play_again = end_game(user_name, &last_score, &high_score, maxY, maxX);
        if (play_again == 0)
            break;
        if (play_again == 1)
            game_core(user_name, score, &last_score, &high_score, maxY, maxX);
    }
}

//------------This function makes login page when program runs, and take player's name------------
void login_page (char *name)
{
        int maxX , maxY;                                            
        getmaxyx (stdscr, maxY, maxX);                              //get maximum x and y of standard screen and assing the values to maxY and maxX;
        make_window_and_box(maxY/2, maxX/2 , maxY/4 , maxX/4);      //function in view.h
        display_welcome_page(maxY,maxX);                            //function in view.h            
        print_login_text(maxY,maxX);                                //function in view.h
        curs_set(1);                                                //set the curses on(so that user car see the curses in input field)
        mvscanw(maxY/4+10, maxX/2-strlen(name)/2, "%s" , name);     //gets username from the user
        curs_set(0);                                                //set the curser display off again
}

//-------------This function caculates the games speed depending on the player's score--------------
int speed (int score)
{
    if(score <= 5)                                                  //set delay to 35 mili seconds when score is under 5
        usleep (30*1000);

    if(score > 5 && score <=10)                                     //set delay to 25 mili seconds when score is between 5 and 10
        usleep (25*1000);

    if(score > 10 && score <=20)                                    //set delay to 20 mili seconds when score is between 10 and 20
        usleep (20*1000);

    if(score > 20 && score<=50)                                     //set delay to 15 mili seconds when score is more than 20
        usleep (15*1000);

    if(score > 50)                                                  //set delay to 15 mili seconds when score is more than 20
        usleep (12*1000);

}
//---------------This function runs the game--------------------------
int game_core(char *name, int score, int *last_score, int *high_score, int maxY, int maxX)    
{
    nodelay(stdscr, 1);                                             //dont stop for user input
    clear();                                                        //clear the whole window
    int y_airplane = maxY-10, x_airplane = maxX/2-10, y_obstacle = maxY, x_obstacle, y_boarder = 0; 
    int ch;                                                         
    
    while(1)                                                        //this true loop runs the game untill the player lose
    {
        print_boarder1 (y_boarder, maxX);                           //function in view.h
        y_boarder += 1;                                             //increese y coordinate of obstacle to change location
        print_boarder2 (y_boarder, maxX);                           //function in view.h
        if (y_boarder >= maxY)                                      
            y_boarder = 0 ;                                         
        ch = getch();                                               //get a key from keybourd and assign it to int ch
        switch (ch)                                                 //function in view.h
        {
        case 'd':                                                   //in casw user presses d or right arrow key 
        case 'D':
        case KEY_RIGHT:
            delete_airplane(y_airplane,x_airplane);                 //function in view.h
            if (x_airplane <= maxX/2+ 10)                           //if x coordinate of airplane was not more than the boargers of the game
                x_airplane += 21;                                   //increase x coordinate to move airplane to the righ 
            break;
        case 'a':                                                   //in casw user presses a or left arrow key
        case 'A':
        case KEY_LEFT:
            delete_airplane(y_airplane,x_airplane);
            if (x_airplane >= maxX/2- 10)
                x_airplane -= 21;                                   //decrease x coordinate to move airplane to left
            break;
        default:
            break;
        case 32:
            pouse (name, score,  maxX,  maxY);
            *last_score = score; 
            game_core(name, score, last_score, high_score, maxY, maxX);
            return 0;
        }

        print_airplane(y_airplane,x_airplane);                      //function in view.h
        
        if (y_obstacle == maxY)                                     //check if y coordinate of obstacle equals maximam y
        {
            y_obstacle = 0;                                         
            x_obstacle = random_number(maxX);                       
        }

        delete_obstacle( y_obstacle, x_obstacle);                   //function in view.h
        y_obstacle++;                                               
        show_obstacle (y_obstacle, x_obstacle);                     //function in view.h

        if ( (y_airplane == y_obstacle+6) && (x_airplane == x_obstacle) )       //if this conditions are true means that player lose
        { 
            *last_score = score;                                                //assign current score to last score
            break;
        }
        if ( (y_airplane + 6 == y_obstacle + 5) && (x_airplane != x_obstacle)) // if this conditions are true means that an obstacle passes without hitting the airplane
            score ++;                                                          
                             
        view_scoreboard (maxY, maxX, name, score, high_score);                      
        speed (score);                                                         
    }
}

//---------------This function generates random numbers using rand() function--------------------------
int random_number (int maxX)
{
    /* 
    A random number between 0 and 3 is generated, then minus one, gives one of the numbers : -1 or 0 or 1 . 
    then multiply by 21(the width of airplane) and plus the x coordinate of the middle of the screen.
    finally the function retunrs one number from three numbers to make the obstacle apper in 3 random different collums 
    */
    return ((rand()%3)-1) * 21 + maxX/2 - 10; 
}
//---------------This function pouses the game--------------------------
int pouse (char *name, int score, int maxX, int maxY)
{
    clear();
    int ch;
    print_pouse_text(maxY,maxX,name,score);             //function in view.h
    while (1)
    {   
        usleep(10000);
        ch = getch();
        switch (ch)                                                 
        {
            case 32:
                return 0;
        }
    } 
}
//---------------This function runs when the player lose and give him a choice to play again or exit--------------------------
int end_game(char *name, int *last_score, int *high_score, int maxY, int maxX)
{             
    usleep(10000);                                                       
    clear();    
    if (*last_score >= *high_score) //compare last score with previous high score 
        *high_score = *last_score;  //if last score was more than high score, update high score by assigning last score's value to it

    print_game_over_text(maxY,maxX,name,high_score);                       
    
    int ch;                                                                                               
    if(ch=getch()){                   
        switch(ch){                
            case 'e':              
            case 'E':
            case 27:               //27 is ascii code for Esc key on the key board
                return 0;
            break;   
            case 'r':              //case user press r, means that he wants to play again so return 1
            case 'R': 
            case 32:               //32 is ascii code for space key
                return 1;
            break;
        }
    }
}