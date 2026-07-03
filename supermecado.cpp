#include <thread>
#include <semaphore>
#include <mutex>
#include <chrono>

#define B_size 5 //tamaño buffer
#define N_item 20 //Numero de items

int buffer[B_size]; //a lo mejor hacerlo vector

int in=0 ;
int out=0 ;

std::counting_semaphore<B_size> vacios(B_size); //vacio inicializado con tamaño max
std::counting_semaphore<B_size> lleno(0);
std::mutex mut;
void cliente() {
    for (int i = 0; i < N_item; i++) {
        int item = i;               // dato a producir

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
