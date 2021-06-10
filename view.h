#include <ncurses.h> 
#include <string.h>                     
#include <unistd.h>

void make_window_and_box(int start_y, int start_x, int end_y, int end_x){
    WINDOW *win;                                              //define and make a new window
    win = newwin(start_y, start_x , end_y , end_x);           //make the window starting from maxY/4 and maxX/4 points and with lenth of maxY/2 and width of maxX/2
    refresh(); 
    box(win, 0, 0);                                           //make a box on window
    wrefresh(win);                                            //refresh window
}

//------------This function prints the intro shape of plane------------
void welcome_plane (int y, int x)
{
    curs_set(0);  // set curser display off
    mvprintw(y   , x , "      -----|-----     ");
    mvprintw(y+1 , x , "    >=====[_]L)       ");
    mvprintw(y+2 , x , "         -'-`-        ");
    refresh();
}
//------------This function deletes the intro shape of plane------------
void delete_welcome_plane (int y, int x)
{
    mvprintw(y   , x , "                      ");
    mvprintw(y+1 , x , "                      ");
    mvprintw(y+2 , x , "                      ");
    refresh();
}

void display_welcome_page(int maxY,int maxX){
    mvprintw(maxY/4+8, maxX/2-10, "Welcome to River Rade");     
    int y_welcome = maxY/4 +12 , x_welcome = maxX/4 +1;         
    for( int go = maxX/4; go < maxX*3/4 - 24; go++)             //this loop makes the intro plane move
    {
        delete_welcome_plane (y_welcome, x_welcome);            //call the function that deletes the intro plane in previous location
        x_welcome ++;                                           //change x location 
        welcome_plane(y_welcome, x_welcome);                    //print the intro plane in the new location
        usleep(30*1000);                                        //delay for 30 mili seconds
    }
    delete_welcome_plane (y_welcome, x_welcome);                // call function to delete intro plane after the loop is done
}

void print_login_text(int maxY, int maxX){
    mvprintw(maxY/4+8, maxX/2-strlen("Enter your name please:")/2, "Enter your name please:");     
    mvprintw(maxY/4+14, maxX/2-strlen("Then press Enter to start the game!")/2, "Then press Enter to start the game!");
    refresh();
}

void print_pouse_text(int maxY, int maxX,char *name, int score){
    mvprintw(maxY/2-6, maxX/2-strlen(" name ")/2, "%s", name);
    mvprintw(maxY/2-4, maxX/2-strlen("Game is Poused")/2, "Game is Poused");
    mvprintw(maxY/2-2, maxX/2-strlen("To Resume -> Press Space")/2, "To Resume : Press Space");  
    mvprintw(maxY/2  , maxX/2-strlen("Your current score :   ")/2, "Your current score : %d", score); 
    refresh();
}

void print_game_over_text(int maxY,int maxX,char *name,int *high_score){
    mvprintw(maxY/2-2, maxX/2-strlen("Game Over")/2, "Game Over");                                              
    mvprintw(maxY/2+2, maxX/2-strlen(name)/2, "%s", name);                                                           
    mvprintw(maxY/2+4, maxX/2-strlen("Your high score is: ")/2, "Your high score is: %d", *high_score);             
    mvprintw(maxY/2+6, maxX/2-strlen("Press R or Space to play again")/2, "Press R or Space to play again");       
    mvprintw(maxY/2+8, maxX/2-strlen("Press E or Esc to play exit")/2, "Press E or Esc to play exit");
    refresh();
}

//------------This function prints the airplane------------ 
void print_airplane (int y_airplane, int x_airplane)               
{   
    //attribute on for color pair which is defined in start function
    attron(COLOR_PAIR(1)); 
    mvprintw( y_airplane   , x_airplane , "          *          "); 
    mvprintw( y_airplane+1 , x_airplane , " ________| |________ ");
    mvprintw( y_airplane+2 , x_airplane , "|========. .========|");
    mvprintw( y_airplane+3 , x_airplane , "        || ||        ");
    mvprintw( y_airplane+4 , x_airplane , "         | |         ");
    mvprintw( y_airplane+5 , x_airplane , "       __| |__       ");
    mvprintw( y_airplane+6 , x_airplane , "      |_______|      ");
    //attribute on for color pair which is defined in start function
    attroff(COLOR_PAIR(1)); 
    refresh(); //refresh the screen to make sure characters are printed
}
//------------This function erases the airplane by printing space------------ 
void delete_airplane (int y_airplane, int x_airplane)              
{   
    mvprintw( y_airplane   , x_airplane , "                     ");
    mvprintw( y_airplane+1 , x_airplane , "                     ");
    mvprintw( y_airplane+2 , x_airplane , "                     ");
    mvprintw( y_airplane+3 , x_airplane , "                     ");
    mvprintw( y_airplane+4 , x_airplane , "                     ");
    mvprintw( y_airplane+5 , x_airplane , "                     ");
    mvprintw( y_airplane+6 , x_airplane , "                     ");
    refresh();
}
//------------This function prints the obstacle------------ 
void show_obstacle(int y_obstacle, int x_obstacle)
{
  attron(COLOR_PAIR(2));
  mvprintw( y_obstacle   , x_obstacle , "     ___________     ");
  mvprintw( y_obstacle+1 , x_obstacle , "    |   ^   ^   |    ");
  mvprintw( y_obstacle+2 , x_obstacle , "    |   O   O   |    ");
  mvprintw( y_obstacle+3 , x_obstacle , "    |     *     |    ");
  mvprintw( y_obstacle+4 , x_obstacle , "    |     o     |    ");
  mvprintw( y_obstacle+5 , x_obstacle , "    |___________|    ");
  attroff(COLOR_PAIR(2)); 
  refresh();
}
//------------This function erases the airplane by printing space------------ 
void delete_obstacle(int y_obstacle, int x_obstacle)
{
  mvprintw( y_obstacle   , x_obstacle , "                     ");
  mvprintw( y_obstacle+1 , x_obstacle , "                     ");
  mvprintw( y_obstacle+2 , x_obstacle , "                     ");
  mvprintw( y_obstacle+3 , x_obstacle , "                     ");
  mvprintw( y_obstacle+4 , x_obstacle , "                     ");
  mvprintw( y_obstacle+5 , x_obstacle , "                     ");
  refresh();
}
//------------This function prints the boarders in two sides of the game------------
void print_boarder1 (int y_boarder, int maxX)
{
    attron(COLOR_PAIR(2));
    mvprintw( y_boarder     , maxX/2-40, "v"); 
    mvprintw( y_boarder     , maxX/2+40, "v");
    attroff(COLOR_PAIR(2)); 
    refresh();
}
//------------This function erases the boarders by printing space------------ 
void print_boarder2 (int y_boarder, int maxX)
{
    mvprintw( y_boarder     , maxX/2-40, "  ");
    mvprintw( y_boarder     , maxX/2+40, "  ");
    refresh();
}
//---------------This function prints user's information during the game--------------------------
void view_scoreboard (int maxY, int maxX, char*name, int score, int *high_score)
{
    mvprintw( 1 , maxX/2+45, "Player's Name: %s", name);                       
    mvprintw ( 3 , maxX/2+45, "Score: %d", score);                             
    mvprintw ( 5 , maxX/2+45, "High Score: %d", *high_score);                  
    mvprintw ( 7 , maxX/2+45, "To Pouse -> Press Space"); 
    refresh();
}