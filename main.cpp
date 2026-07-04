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

int tiempo_ms = 1000;

void config_tiempos();
void config_tiempos_valores();

void config_tiempos(){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *menuwin = newwin(10, 50, 7, (xMax - 50) / 2);
    keypad(menuwin, true);

    string opciones[5] =
    {
        "Cajero 1",
        "Cajero 2",
        "Clientes Cajero 1",
        "Clientes Cajero 2",
        "Volver"
    };

    int highlight = 0;
    int choice;

    while(1)
    {
        werase(menuwin);
        box(menuwin,0,0);

        int ancho = getmaxx(menuwin);

        for(int i=0;i<5;i++)
        {
            int x = (ancho - opciones[i].length()) / 2;

            if(i == highlight)
                wattron(menuwin, A_REVERSE);

            mvwprintw(menuwin, i + 1, x, "%s", opciones[i].c_str());

            wattroff(menuwin, A_REVERSE);
        }

        wrefresh(menuwin);
        choice = wgetch(menuwin);

        switch(choice)
        {
            case KEY_UP:
                highlight = (highlight - 1 + 5) % 5;
                break;

            case KEY_DOWN:
                highlight = (highlight + 1) % 5;
                break;

            case 10:

                switch(highlight)
                {
                    case 0:
                        config_tiempos_valores();   // Tiempo Cajero 1
                        break;

                    case 1:
                        config_tiempos_valores();   // Tiempo Cajero 2
                        break;

                    case 2:
                        config_tiempos_valores();   // Tiempo Clientes Cajero 1
                        break;

                    case 3:
                        config_tiempos_valores();   // Tiempo Clientes Cajero 2
                        break;

                    case 4:
                        delwin(menuwin);
                        return;
                }

                break;
        }
    }
}

void config_tiempos_valores(){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *menuwin = newwin(8, 50, 7, (xMax - 50) / 2);
    keypad(menuwin, true);

    int valor = tiempo_ms;
    int choice;

    while (1)
    {
        werase(menuwin);
        box(menuwin, 0, 0);

        string titulo = "Tiempo entre acciones";
		string tiempo = "<   " + to_string(valor) + " ms   >";
        string ayuda = "ENTER";

        mvwprintw(menuwin, 2, (50 - titulo.length()) / 2, "%s", titulo.c_str());
        mvwprintw(menuwin, 4, (50 - tiempo.length()) / 2, "%s", tiempo.c_str());
        mvwprintw(menuwin, 6, (50 - ayuda.length()) / 2, "%s", ayuda.c_str());

        wrefresh(menuwin);
        choice = wgetch(menuwin);

        switch(choice)
        {
            case KEY_LEFT:
                if(valor > 500)
                    valor -= 100;
                break;

            case KEY_RIGHT:
                if(valor < 5000)
                    valor += 100;
                break;

            case 10:
                tiempo_ms = valor;
                delwin(menuwin);
                return;
        }
    }
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

    string choices[3] =
    {
        "Iniciar",
		"Config",
        "Salir"
    };
	int choice;
    int highlight = 0;
    
    while(1)
    {
        box(menuwin,0,0);

        int ancho = getmaxx(menuwin);
        for(int i=0;i<3;i++)
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
                highlight = (highlight - 1 + 3) % 3;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 3;
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
				config_tiempos();
				break;

                case 2:
                endwin();
                return 0;
            }
        }
    }

    getch();
    endwin();

    return 0;
}