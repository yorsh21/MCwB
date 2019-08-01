Los archivos de instancias se encuentran en inputs/
Los archivos de salida se encuentran en outputs/

Para compilar el proyecto ejecutar: make
Para ejecutar una instancia específica con una semilla aleatoria: make instance=nombre_instancia

Ejemplos:
	make instance=a36
	make instance=eil22

Para ejecutar un pool de pruebas con las 30 semillas seteadas ejecutar el siguiente script de python:

- Todas las instancias: python3 scripts/threads.py
- Una instancia específica: python3 scripts/threads.py nombre_instancia
- Una instancia específica con una semilla específica: python3 scripts/threads.py nombre_instancia numero_semilla


Los resutlados de cada ejecución se almacenan en un archivo llamado results_threads.out ubicado dentro del directorio outputs/



Dependencias c++:
	sudo apt-get update
	sudo apt install make
	sudo apt install make-guile
	sudo apt-get install build-essential g++

Dependencias python:
	sudo apt-get update
	sudo apt-get install python3
	sudo apt-get install python3-pip
	pip install numpy
	pip install matplotlib
	pip install pylab

