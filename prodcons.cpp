#include <iostream>
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
void productor() {
    for (int i = 0; i < N_item; i++) {
        int item = i;               // dato a producir

        vacios.acquire();          // espera espacio libre
        mut.lock(); // entra a sección crítica

        buffer[in] = item;
	std::cout<<"P -> produjo "<<item<<" en "<<in<<"\n";
        in = (in + 1) % B_size;

        mut.unlock();
        lleno.release();          // incrementa ítems disponibles

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));//para salida
    }
}

void consumidor() {
    for (int i = 0; i < N_item; i++) {
        lleno.acquire();          // espera ítems disponibles
        mut.lock(); // entra a sección crítica

        int item = buffer[out];
	std::cout<<"C <- consumio "<<item<<"desde "<<out<<"\n";
        out = (out + 1) % B_size;

        mut.unlock();
        vacios.release();          // incrementa espacios libres

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));//para salida
    }
}

int main(void) {

    std::thread t1(productor),t2(consumidor);
    
    t1.join();
    t2.join();

    return 0;
}


