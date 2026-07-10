#include "supermercado.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <curses.h>

using namespace std;
//para imprimir
string productos[10]={
 "",
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

void config_tiempos(cliente &c_caja1,cliente &c_caja2,cajero &caja1,cajero &caja2);
void config_tiempos_valores(int &valor_tiempo);

void config_tiempos(cliente &c_caja1,cliente &c_caja2,cajero &caja1,cajero &caja2){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *menuwin = newwin(8, 50, 7, (xMax - 50) / 2);
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
                        config_tiempos_valores(caja1.miliseg);   // Tiempo Cajero 1
                        break;

                    case 1:
                        config_tiempos_valores(caja2.miliseg);   // Tiempo Cajero 2
                        break;

                    case 2:
                        config_tiempos_valores(c_caja1.miliseg);   // Tiempo Clientes Cajero 1
                        break;

                    case 3:
                        config_tiempos_valores(c_caja2.miliseg);   // Tiempo Clientes Cajero 2
                        break;

                    case 4:
                        delwin(menuwin);
                        return;
                }

                break;
        }
    }
}

void config_tiempos_valores(int &valor_tiempo){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *menuwin = newwin(8, 50, 7, (xMax - 50) / 2);
    keypad(menuwin, true);

    int valor = valor_tiempo;
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
                valor_tiempo = valor;
                delwin(menuwin);
                return;
        }
    }
};

void ventana_inicio(int coordenaday_caja1,queue<cliente> fila_clientes1,cajero caja1, int item_t_c1){



	int ANCHO_PRODUCTO = 14;//tamaño de string + espacios
	int ALTO_CAJERO = 5;//alto de cada caja
	int PRODUCTOS = B_size;//tamaño buffer
	int ancho_buffer_total= PRODUCTOS * ANCHO_PRODUCTO + (PRODUCTOS + 1);
	WINDOW *buffer_caja1[B_size];
	for (int i =0;i<PRODUCTOS;i++)
	{
		buffer_caja1[i]=newwin(ALTO_CAJERO,ANCHO_PRODUCTO,coordenaday_caja1,5+i*ANCHO_PRODUCTO);
		box(buffer_caja1[i],0,0);
		wrefresh(buffer_caja1[i]);
		refresh();
	}

	thread t2(&cajero::cajero_productos,&caja1,item_t_c1);//parte cajero
	WINDOW *cliente_actual1=newwin(ALTO_CAJERO,ANCHO_PRODUCTO,coordenaday_caja1,5+ANCHO_PRODUCTO*B_size);
	box(cliente_actual1,0,0);
	int n_cliente=1;
	while(!fila_clientes1.empty())
	{
		thread t1(&cliente::cliente_producto,&fila_clientes1.front());
		
		werase(cliente_actual1);
		mvwprintw(cliente_actual1,2,2,"Cliente: %d",n_cliente);
		mvwprintw(cliente_actual1,3,2,"N items: %d",fila_clientes1.front().n_items);
		box(cliente_actual1,0,0);
		wrefresh(cliente_actual1);
		while(t1.joinable())
		{
			//vuelve a refrescar buffer
			for(int i=0;i<PRODUCTOS;i++){
				werase(buffer_caja1[i]);
				box(buffer_caja1[i],0,0);
				mut.lock();
				mvwprintw(buffer_caja1[i],3,2,productos[buff1.buffer[i]].c_str());
				mut.unlock();

				wrefresh(buffer_caja1[i]);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			
		}
		n_cliente++;
		fila_clientes1.pop();
		
	}

	return;






}

int rellenar_fila(queue<cliente> &fila, cliente c,int n_clientes)//rellena la fila con n_clientes
{
	int n_items_totales_caja=0;
	for(int i=0;i<n_clientes;i++)
	{
		c.relleno_canasta();
		n_items_totales_caja+=c.n_items;
		c.miliseg=1000;
		fila.push(c);
	}
	return n_items_totales_caja;//se retorna los items totales de la caja

}
int main()
{
    srand(time(NULL));//inicializa random para el programa

    int n_clientes=0;//numero de clientes que van a haber por caja
		    
    //elementos caja1
    queue<cliente> clientes_caja1;//fila de clientes para caja1
    cliente c_caja1(&buff1);//clientes caja 1 usan buffer1
    cajero caja1(&buff1);//cajero 1 buffer 1
    int items_total_caja1=0;
    //elementos caja2
    queue<cliente> clientes_caja2;//fila de clientes para caja2
    cliente c_caja2(&buff2);//clientes caja 2 usan buffer2
    cajero caja2(&buff2);//cajero 2 buffer 2
    

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

    int y_caja1=(altura_titulo+2)+8+4;//coordenada y caja1 es altura y coordenadas de cosas anteriores + 4 para que tenga espacio

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
			//inicializacion fila caja1
			n_clientes=rand()%10+1;
			items_total_caja1=rellenar_fila(clientes_caja1, c_caja1,n_clientes);
			ventana_inicio(y_caja1,clientes_caja1,caja1,items_total_caja1);
			break;

		case 1:
			config_tiempos(c_caja1,c_caja2,caja1,caja2);
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
