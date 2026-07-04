#include "supermercado.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <curses.h>

using namespace std;
//para imprimir
string productos[10]={
 "Arvejas",
 "Banana",
 "Tomate",
 "Ketchup",
 "Arroz",
 "Tallarines",
 "Mayonesa",
 "Manzana",
 "Te",
 "Cafe"
};

int main()
{
    initscr();               // Iniciar ncurses
    noecho();                // No mostrar teclas
    cbreak();                // Entrada inmediata

	int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    box(stdscr, 0, 0);

    // TITULO
    string titulo[] ={
"  ____  _   _ ____  _____ ____  __  __ _____ ____   ____    _    ____   ___ ",
" / ___|| | | |  _ \\| ____|  _ \\|  \\/  | ____|  _ \\ / ___|  / \\  |  _ \\ / _ \\",
" \\___ \\| | | | |_) |  _| | |_) | |\\/| |  _| | |_) | |     / _ \\ | | | | | | |",
"  ___) | |_| |  __/| |___|  _ <| |  | | |___|  _ <| |___ / ___ \\| |_| | |_| |",
" |____/ \\___/|_|   |_____|_| \\_\\_|  |_|_____|_| \\_\\\\____/_/   \\_\\____/ \\___/ "
    };

    int altura_titulo = 5;

    attron(A_BOLD);

    for(int i = 0; i < altura_titulo; i++) {
        mvprintw(i + 1,(xMax - titulo[i].length()) / 2, titulo[i].c_str());
    }

    attroff(A_BOLD);

    //crear ventana
    WINDOW *menuwin = newwin(8, xMax - 10, altura_titulo + 2, 5);
    box(menuwin,0,0);
    refresh();
    wrefresh(menuwin);

    keypad(menuwin,true); //habilitar flechas

    string choices[2] =
    {
        "Iniciar",
        "Salir"
    };
	int choice;
    int highlight = 0;
    
    while(1)
    {
        box(menuwin,0,0);

        int ancho = getmaxx(menuwin);
        for(int i=0;i<2;i++)
        {
            int x = (ancho - choices[i].length())/2;
            if(i==highlight)
            wattron(menuwin,A_REVERSE);
            mvwprintw(menuwin,i+2,x,choices[i].c_str());
            wattroff(menuwin,A_REVERSE);
        }
        wrefresh(menuwin);
        choice = wgetch(menuwin);

        switch(choice)
        {
            case KEY_UP:
                highlight = (highlight - 1 + 2) % 2;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 2;
                break;
            default:
                break;
        }

        if(choice == 10)
        {
            switch(highlight){
                case 0:
                {
					//FUNCION PLACEHOLDER
                    break;
                }

                case 1:
                endwin();
                return 0;
            }
        }
    }

    getch();
    endwin();

    return 0;
}