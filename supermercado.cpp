#include <thread>
#include <semaphore>//semaforo cpp
#include <mutex>
#include <chrono>//para sleeps
#include <cstdlib>//Uso de Rand
#include <ctime>

#define B_size 5 //tamaño buffer
#define N_item_max 20 //Numero de items maximo por cliente

struct buffers{

int buffer[B_size]; 
int in;
int out;
};
//buffers 
buffers buff1={{0,0,0,0,0},0,0};//se inicializan valores de los buffer en 0
buffers buff2={{0,0,0,0,0},0,0};

//sintaxis es conting_semaphore<TAMAÑO MAXIMO> semaforo (CANTIDA CON LA QUE EMPIEZA)
std::counting_semaphore<B_size> vacios(B_size); //semaforo vacio empieza lleno, todos los espacios vacios
std::counting_semaphore<B_size> lleno(0);//semaforo lleno empieza vacio, no hay elementos en el buffer
std::mutex mut;
std::mutex mutex_rand;//para que las funciones no llamen rand al mismo tiempo para evitar errores

class cliente{
	public:
	int n_items;
	int canasta[N_item_max];
	buffers *dir_buffer; //puntero a la variable global para que se sepa a cual de los dos buffer debe acceder el cliente

	cliente(buffers *buffer){
		dir_buffer=buffer;
		mutex_rand.lock();
		srand(time(NULL));
		n_items=(rand()%N_item_max);//cantidad de items en canasta es aleatorio
		for(int i=0;i< n_items;i++)
		{
			canasta[i]=rand()%10;//item aleatorio, valor para printear despues esta entre 0-9
		}
		mutex_rand.unlock();
	}
	void cliente_producto() {
		for (int i = 0; i < n_items; i++) {
			int item = canasta[i];// dato a pasar a caja

			vacios.acquire();// espera espacio libre
			mut.lock(); // entra a sección crítica

			//Seccion critica
			dir_buffer->buffer[dir_buffer->in] = item;
			dir_buffer->in = (dir_buffer->in + 1) % B_size;

			mut.unlock();//sale seccion critica
			lleno.release();// incrementa ítems disponibles

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));//para salida
		}
	}
};

class cajero{
	public:
		buffers *dir_buffer;//igual que cliente,se usa un puntero para asignar el buffer del cajero
		int items_a_recuperar;
		cajero(buffers *buffer,int items)
		{
			dir_buffer=buffer;
			items_a_recuperar=items;
		}

		void cajero_productos() {

			for (int i = 0; i < items_a_recuperar; i++) {
				lleno.acquire();// espera ítems disponibles
				mut.lock(); // entra a sección crítica

				//Seccion critica
				int item = dir_buffer->buffer[dir_buffer->out];
				dir_buffer->out = (dir_buffer->out + 1) % B_size;

				mut.unlock();//sale seccion critica
				vacios.release();// incrementa espacios libres

				std::this_thread::sleep_for(std::chrono::milliseconds(2000));//para salida
			}

	}
};
