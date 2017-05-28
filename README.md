# libreroarp
Una sencilla biblioteca para manipular el protocolo ARP en sistemas Linux.

## El protocolo ARP
Es el responsable de encontrar una dirección física que corresponde a una
dirección IP en una red de computadoras.

Una trama ARP consta de los siguientes campos:

* Tipo de hardware en la red.
* Tipo de protocolo.
* Longitud de la dirección física en bytes.
* Longitud de la dirección de protocolo en bytes.
* Operación.
* Dirección física del remitente.
* Dirección de protocolo del remitente.
* Dirección física del destinatario
* Dirección de protocolo del destinatario

## Dependencias
* CMake >= 3.5
* libBoost >= 1.58
* Soporte completo para C++11

## Compilación e Instalación

```
$ git clone https://github.com/reroman/libreroarp.git
$ cd libreroarp/
$ mkdir build
$ cd build/
$ cmake ..
$ make
# make install
```

Por defecto se construyen algunos ejemplos incluídos, para
desactivarlos ejecuta lo siguiente en lugar del quinto comando de arriba:
```
$ cmake -DBUILD_EXAMPLES=false ..
```

## Uso
La documentación generalmente se instala en la carpeta: 
/usr/local/share/libreroarp/doc/html/index.html

El enlazado de la biblioteca se hace con -lreroarp, por ejemplo:
```
$ g++ -std=c++11 example.cpp -o example -lreroarp
```

## Autor
Ricardo Román <[reroman4@gmail.com](mailto:reroman4@gmail.com)>

