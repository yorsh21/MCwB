Para ejecutar el algormo se ejecuta: make instance=nombre_archivo.txt

Donde el archivo ya se encuentra en el directorio inputs/
El archivo de salida de ubicará en el directorio outputs/ con el nombre: nombre_archivo.out

Ejemplos:
	make instance=a36
	make instance=eil22


Para ejecutar las pruebas en todas las instancias que se encuentran en el directorio inputs/ se debe ejecutar: make

Con esto se generará un archivo con las calidades y tiempos de cada una de las instancias. El archivo se ubica en el directorio outputs/ y se llama results.out.


Dependencias:
	sudo apt-get update
	sudo apt install make
	sudo apt install make-guile
	sudo apt-get install build-essential g++

