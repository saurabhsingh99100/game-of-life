/**
 * @brief 
 * @see https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 * 
 */
//      Any live cell with two or three live neighbours survives.
//      Any dead cell with three live neighbours becomes a live cell.
//      All other live cells die in the next generation. Similarly, all other dead cells stay dead.

#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>

const unsigned int mSec = 1000;
const unsigned int Sec = 1000000;
const char cellChar = '#';
const char noneChar = ' ';
const int frameLim = 10;

// StdScr
const int screenLim_x = 130;
const int screenLim_y = 70;

// World
const int worldLim_x = 120;
const int worldLim_y = 40;
WINDOW * gameWin;

// Print Game Window
void printGameWin(bool world[worldLim_x][worldLim_y])
{
    wclear(gameWin);
    box(gameWin, 0, 0);
    for(int row=0; row<worldLim_y; row++)
    {
        for(int col=0; col<worldLim_x; col++)
        {
            wmove(gameWin, row, col);
            wprintw(gameWin, "%c", world[col][row] ? cellChar : noneChar);
        }
    }
    wrefresh(stdscr);
    wrefresh(gameWin);
}


int main(int argc, char ** argv)
{
    ///////////// Init /////////////
    initscr();
    cbreak();                   // disable getch() buffering
    noecho();                   // disable getch() echo
    nodelay(stdscr, TRUE);      // disable getch() waiting for keypress
    curs_set(0);                // Hide cursor

    ///////////// Game window /////////////
    bool world[worldLim_x][worldLim_y];

    // clear world
    for(int row=0; row<worldLim_y; row++)
    {
        for(int col=0; col<worldLim_x; col++)
        {
            world[col][row] = false;
        }
    }

    // init from file
    if(argc == 2)
    {
        std::vector<std::string> fcontents;
        std::ifstream f (argv[1]);
        if (f.is_open())
        {
            std::string line;
            while ( getline (f,line) )
                fcontents.push_back(line);
            f.close();
        }
        else
        {
            std::cout << "Unable to open file : " << argv[1] << "\n"; 
            endwin();
            return 1;
        }

        for(unsigned int v=0; v<fcontents.size() && v<worldLim_y; v++)
        {
            for(unsigned int l=0; l<fcontents[v].length() && l<worldLim_x; l++)
            {
                if(fcontents[v][l] != ' ')
                    world[l][v] = true;
            }
        }
    }

    gameWin = newwin(worldLim_y, worldLim_x, 1, 1);
    wrefresh(stdscr);
    wrefresh(gameWin);

    ///////////// Game Params /////////////
    unsigned long tick = 0;
    unsigned long population = 0;
    bool paused = true;
    int frameCount = 0;

    int cursor_x = 0;
    int cursor_y = 0;
    
    int delay = 10*mSec;

    printGameWin(world);
    // Gameloop
    while(1)
    {
        // process input 
        char inp = getch();
        if(inp == 'p')
        {
            paused = !paused;
        }
        else if(inp == 'q')
        {
            break;
        }
        else if(inp == '1')
        {
            delay = 70 * mSec;   
        }
        else if(inp == '2')
        {
            delay = 25 * mSec;
        }
        else if(inp == '3')
        {
            delay = 10 * mSec;
        }
        else if(inp == '4')
        {
            delay = 1 * mSec;
        }
        else if(inp == 'c')
        {
            for(int row=0; row<worldLim_y; row++)
            {
                for(int col=0; col<worldLim_x; col++)
                {
                    world[col][row] = false;
                }
            }
            printGameWin(world);
        }
        else if(inp == 's')
        {
            wmove(stdscr, 0, 0);
            wprintw(stdscr, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
            wmove(stdscr, 0, 0);
            wprintw(stdscr, "Enter filename : ");

            std::string fname;
            char c = '\0';
            nodelay(stdscr, FALSE);      // enable getch() waiting for keypress
            while(1)
            {
                c = getch();
                if(c == '-' && fname.length()>0)   // backspace
                {
                    wmove(stdscr, 0, 18+fname.length()-1); 
                    wprintw(stdscr, " ");
                    fname.pop_back();
                    wmove(stdscr, 0, 18+fname.length());
                }
                else if(c == '\n')
                {
                    break;
                }
                else
                {
                    wprintw(stdscr, "%c", c);
                    fname.push_back(c);
                }
            }
            nodelay(stdscr, TRUE);      // disable getch() waiting for keypress


            std::ofstream f (fname);
            if (f.is_open())
            {
                for(int row=0; row<worldLim_y; row++)
                {
                    for(int col=0; col<worldLim_x; col++)
                    {
                        f << (world[col][row]? "#" : " ");
                    }
                    f << "\n";
                }
                f.close();
            }
        }

        if(paused && inp == 'j' && cursor_x!=0)
            cursor_x--;
        else if(paused && inp == 'l' && cursor_x!=worldLim_x-1)
            cursor_x++;
        else if(paused && inp == 'i' && cursor_y!=0)
            cursor_y--;
        else if(paused && inp == 'k' && cursor_y!=worldLim_y-1)
            cursor_y++;


        // Render screen
        if(frameCount == frameLim)
        {
            if(paused)  // Edit Mode
            {
                printGameWin(world);
                wmove(gameWin, cursor_y, cursor_x);
                wprintw(gameWin, "%c", 'O');

                if(inp == ' ')  // Invert cell state
                    world[cursor_x][cursor_y] = !world[cursor_x][cursor_y];
            }
            else        // Sim Mode
            {
                population = 0;
                
                // create new world
                bool worldnew[worldLim_x][worldLim_y];
                // clear new world
                for(int row=0; row<worldLim_y; row++)
                {
                    for(int col=0; col<worldLim_x; col++)
                    {
                        worldnew[col][row] = false;
                    }
                }

                // Calculate
                for(int row=0; row<worldLim_y; row++)
                {
                    for(int col=0; col<worldLim_x; col++)
                    {
                        // Calculate current generation population
                        population += (world[col][row] ? 1 : 0);

                        // neighbour calculation
                        int nalive = 0;
                        
                        if(!(row==0 || col==0))    // TL
                        {
                            nalive += world[col-1][row-1];
                        }
                        if(!(row==0 || col==worldLim_x-1))    // TR
                        {
                            nalive += world[col+1][row-1];
                        }
                        if(!(row==worldLim_y-1 || col==0))    // BL
                        {
                            nalive += world[col-1][row+1];
                        }
                        if(!(row==worldLim_y-1 || col==worldLim_x-1))    // BR
                        {
                            nalive += world[col+1][row+1];
                        }
                        if(!(row==0))    // T
                        {
                            nalive += world[col][row-1];
                        }
                        if(!(row==worldLim_y-1))    // B
                        {
                            nalive += world[col][row+1];
                        }
                        if(!(col==0))    // TL
                        {
                            nalive += world[col-1][row];
                        }
                        if(!(col==worldLim_x-1))    // TL
                        {
                            nalive += world[col+1][row];
                        }

                        // rules

                        // Any live cell with two or three live neighbours survives.
                        if(world[col][row] == true && (nalive == 2 || nalive == 3))
                            worldnew[col][row] = true;

                        // Any dead cell with three live neighbours becomes a live cell.
                        else if(world[col][row] == false && nalive == 3)
                            worldnew[col][row] = true;

                        // All other live cells die in the next generation. Similarly, all other dead cells stay dead.
                        else
                            worldnew[col][row] = false;
                    }
                }

                // copy
                for(int r=0; r<worldLim_y; r++)
                {
                    for(int c=0; c<worldLim_x; c++)
                    {
                        world[c][r] = worldnew[c][r];
                    }
                }

                // Render
                wclear(gameWin);
                box(gameWin, 0, 0);

                printGameWin(world);

                tick++;
                frameCount = 0;
            }
            wmove(stdscr, 0, 0);
            wprintw(stdscr, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
            wmove(stdscr, 0, 0);
            wprintw(stdscr, "Tick: %ld\t\tPopulation: %ld\t\tCursor: %d,%d\t\t[%s]", tick, population, cursor_x, cursor_y, (paused? "Editing Mode" : "Running" ));
            wrefresh(stdscr);
            wrefresh(gameWin);
        }
        else
        {
            frameCount++;
        }
        usleep(delay);
    }

    endwin();
}