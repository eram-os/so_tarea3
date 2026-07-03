#include <thread>
#include <semaphore>
#include <mutex>
#include <chrono>//para sleeps
#include <cstdlib>//Uso de Rand
#include <ctime>

#define B_size 5 //tamaño buffer
#define N_item 20 //Numero de items

int buffer[B_size]; //a lo mejor hacerlo vector

int in=0 ;
int out=0 ;

std::counting_semaphore<B_size> vacios(B_size); //vacio inicializado con tamaño max
std::counting_semaphore<B_size> lleno(0);
std::mutex mut;


class cliente{
	public:
	int n_items;
	int canasta[20];
	
	cliente(){
		srand(time(NULL));
		n_items=(rand()%20);//cantidad de items en canasta es aleatorio
		for(int i=0;i< n_items;i++)
		{
			canasta[i]=rand()%10;//item aleatorio
		}


	}
		void cliente_producto() {
			for (int i = 0; i < n_items; i++) {
				int item = canasta[i];               // dato a pasar a supermercado

				vacios.acquire();          // espera espacio libre
				mut.lock(); // entra a sección crítica

				//Seccion critica
				buffer[in] = item;
				in = (in + 1) % B_size;

				mut.unlock();//sale seccion critica
				lleno.release();// incrementa ítems disponibles

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));//para salida
			}
		}
}

void cajero() {
    for (int i = 0; i < N_item; i++) {

        lleno.acquire();          // espera ítems disponibles
        mut.lock(); // entra a sección crítica
		   
	//Seccion critica
        int item = buffer[out];
        out = (out + 1) % B_size;

        mut.unlock();//sale seccion critica
        vacios.release();          // incrementa espacios libres

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));//para salida
    }
}
