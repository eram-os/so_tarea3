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

int main()
{
	srand(time(NULL));//aleatoria cantidad de clientes
	int n_clientes=(rand()%10)+1//max 10 clientes minimo 1
	queue<cliente> clientes_caja1;
	int items_totales_caja1=0;//variable que va a contener cantidad total de items a pasar por caja
	for(int i=0;i<n_clientes;i++)
	{
		cliente c1(&buff1);
		items_totales_caja1+=c1.n_items;
		clientes_caja1.push(c1);
	}

	cout <<"N clientes: "<<n_clientes<<"\n";
	cout << "N items totales "<<items_totales_caja1<<"\n";
	int i=1;
	while(clientes_caja1.empty())
	{
		cliente aux=clientes_caja1.top();
		cout<<
	}
	//cliente c1(&buff1);
	//cajero caj1(&buff1,c1.n_items);

	//cout<<"N items cliente:"<<c1.n_items<<"\n";
	//for(int i=0;i<c1.n_items;i++)
	//{
	//	cout<<"item"<<i<<" canasta:"<<productos[c1.canasta[i]]<<"\n";
	//}
	//thread t1(&cliente::cliente_producto,&c1);
	//
	//thread t2(&cajero::cajero_productos,&caj1);
	//for(int j=0;j<5;j++)//imprime buffer cada 5 seg para ver cambios
	//{
	//	for(int i=0;i<5;i++)
	//	{
	//	cout<<"item"<<i<<" buffer:"<<productos[buff1.buffer[i]]<<"\n";
	//	}
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5000));//para salida
	//}
	//t1.join();
	//t2.join();

	return 0;
}
