===========================
Respuestas a los ejercicios
===========================


Ejercicio cubo
--------------

Pregunta 1
''''''''''

Notar también, que la transparencia y el z-buffer siguen trayendo problemas; pero, dado que hay una sola
cara con transparencia, el problema se resuelve fácilmente. (¿cómo?)

Hay que poner la reja a ultimo momento.

Pregunta 2:
'''''''''''

Hay unas molestas líneas blancas en algunos bordes. ¿Será por el polygon offset, por el clamping/repeat,
por nearest/linear o por la imagen de textura mal hecha; o bien por una mezcla de algunas cosas? ¿Puede
solucionarlo?

Yo creo que poligonoffset no tiene nada que ver.
Puede ser por el clamping o repeat. O también puede ser por una imagen de textura mal hecha.
