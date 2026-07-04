#include "supermercado.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <queue>
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
int rellenar_fila(queue<cliente> &fila, cliente c,int n_clientes)//rellena la fila con n_clientes
{
	int n_items_totales_caja=0;
	for(int i=0;i<n_clientes;i++)
	{
		c.relleno_canasta();
		n_items_totales_caja+=c.n_items;
		c.miliseg=50;
		fila.push(c);
	}
	return n_items_totales_caja;//se retorna los items totales de la caja

}
int main()
{
	srand(time(NULL));
	while(1)
	{
	int n_clientes=(rand()%9)+1;//max 10 clientes minimo 1
	queue<cliente> clientes_caja1;//fila de clientes para caja1
	cliente c_caja1(&buff1);
	c_caja1.miliseg=100;
	int items_totales_caja1=rellenar_fila(clientes_caja1,c_caja1,n_clientes);
	cajero caja1(&buff1,items_totales_caja1);
	cout <<"N clientes: "<<n_clientes<<"\n";
	cout << "N items totales "<<caja1.items_a_recuperar<<"\n";

	//int i=1;
//	queue<cliente> clientes_caja1_copy=clientes_caja1;//fila de clientes para caja1
//	
//	while(!clientes_caja1_copy.empty())
//	{
//		cliente aux=clientes_caja1_copy.front();
//		cout<<"Cliente "<<i<<" tiene "<<aux.n_items<<" items"<<"\n";
//		for(int j=0;j<aux.n_items;j++)
//		{
//			cout<<"item"<<j<<" canasta:"<<productos[aux.canasta[j]]<<"\n";
//		}
//		clientes_caja1_copy.pop();
//		i++;
//	}
	caja1.miliseg=500;
	thread t2(&cajero::cajero_productos,&caja1);//parte cajero
	vector<chrono::duration<double, std::milli> > tiempos_finales;
	while(!clientes_caja1.empty()){
		thread t1(&cliente::cliente_producto,&clientes_caja1.front());
		t1.join();
		tiempos_finales.push_back(clientes_caja1.front().tiempoEjecucion);
		clientes_caja1.pop();
	}
	for(auto tiempos: tiempos_finales)
	{
		cout<<"tiempo cliente:"<<tiempos<<"\n";
	}

	t2.join();

	cout<<"tiempo caja:"<<caja1.tiempoEjecucion<<"\n";
	}
	//cliente c1(&buff1);
	//cajero caj1(&buff1,c1.n_items);

	//cout<<"N items cliente:"<<c1.n_items<<"\n";

	//for(int j=0;j<5;j++)//imprime buffer cada 5 seg para ver cambios
	//{
	//	for(int i=0;i<5;i++)
	//	{
	//	cout<<"item"<<i<<" buffer:"<<productos[buff1.buffer[i]]<<"\n";
	//	}
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5000));//para salida
	//}

	return 0;
}
