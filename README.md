# Tarea 3 Sistemas Operativos
- Programa en C++ que simula el comportamiento de clientes que tienen productos y cajeros que los procesan.
- Se requiere terminal con tamaño de pantalla completo para que funcione correctmente.

## Integrantes
- Cristian Sanchez
- Edinson Ramos
- Esteban Cadiz

## Compilacion 
- Se hace uso de librerias estandar y ncurses de C++.
- El compilado se realizo con gcc en linux, prinicpalmente en ubuntu.
- Se puede usar Make para compilar el programa. Como requisito debe tener Make instalado. 
- De no tener Make para correr el archivo make es necesarios usar el siguiente comando para compilar: 
```
g++ -std=c++20 -pthread prueba.cpp  -o programa -lncurses
```
- El uso de -stdc++20 es necesario pues la version de gcc instalada (v 13.3.0) en la maquina no soporta semaphore de la libreria semaphore.

## Uso 
- El programa se controla con las flechas del teclado para seleccionar las distintas opciones y con enter para acceder a la opcion seleccionada. 
- La opcion Iniciar corre la simulacion de los clientes-cajers.
- La opcion config permite modificar el tiempo entre acciones de los clientes/cajeros. 
- Para modificar el tiempo de un cliente/cajero se usan las felchas izquierda y derecha, una ves seleccionado el tiempo deseado se debe confirmar con la tecla enter.
- El tiempo minimo entre acciones es de 500ms (0,5 segundos) y el tiempo maximo entre acicones se de 5000ms (5 segundos).
- La opcioon salir termina el programa.
```
./main
```
