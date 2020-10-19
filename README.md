### dmb player - beta

<img src="https://github.com/daltomi/dmb/raw/master/screenshot.png"/>


* ¿Qué es?

	Es un mini-reproductor de archivos de audio.
	Permite un mínimo control sobre el/los archivos
	de audio que se están escuchando, lo que lo hace
	ideal para escuchar rápidamente nuestra música desde
	un "file manager", si tener que recurrir a programas
	más complejos con funciones que nunca usaremos; además
	de mayor tiempo de inicio y consumo de recursos.


* ¿Cómo abro un archivo/s?

	Los archivos se abren por medio de parámetro
	del programa, puede ser entre 1 e infinitos(teórico) :)
	- Nota 1: Los nombres de archivo no pueden tener espacios.
			Si los tuviera de debe utilizar comillas dobles, ej:
			`dmb "mi archivo de audio.ogg" mi_otro_archivo.ogg`
	- Nota 2: Se puede utilizar comodines, ej: `dmb *.ogg`


* ¿Qué funciones tiene?

	Permite lo siguiente:
	- Pausar el audio.
	- Saltar al siguiente archivo de reproducción.
	- Saltar al anterior archivo de reproducción.

	Tiene un botón para salir o se pude presionar Esc.


* ¿Que formatos de archivos soporta?

	Todos aquellos formatos soportados por la librería `sndfile`.
	- Nota 1: sndfile no soporta el formato MPEG Layer 3 (mp3) por
			problemas con la licencia.
	- Nota 2: el soporte de algunos formatos dependerán de la librerías
			instaladas en su sistema.


* ¿Sistema operativo?

	Solamente GNU/Linux... pero también cualquier plataforma que soporte
	las siguientes librerías debería funcionar:
	FLTK, portaudio, sndfile, con sus respectivas dependencias.


