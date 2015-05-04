# ISEL
Práctica 3: Xenomai.

En esta práctica haremos uso de threads para comprobar los tiempos de ejecución. Utilizamos los mismos tiempos que para el ejecutivo cíclico 0.45 segundos y 0.9 segundos para el café y monedero respectivamente. Cada máquina de estados tiene una hebra que accede a una serie de variables. Para que no se superpongan debemos bloquear las variables globales mediante mutex a la hora de leer o escribir estas variables comunes.
 
Podemos afirmar que estos tiempos se cumplen puesto que al ejecutar el main.c obtenemos como salida en el fichero outputs lo siguiente:

Bloqueo money: 8698 ns.
Bloqueo money: 1875 ns.
Ejecución de monedero: 7135 ns.
Ejecución de monedero: 13594 ns.
Tiempo de una ejecución: 41197 ns.

Bloqueo money: 1406 ns.
Ejecución de monedero: 10312 ns.
Tiempo de una ejecución: 64843 ns.

Bloqueo money: 1407 ns.
Ejecución de monedero: 625 ns.
Bloqueo money: 1198 ns.
Ejecución de monedero: 6302 ns.
Tiempo de una ejecución: 93385 ns.

Bloqueo money: 1198 ns.
Ejecución de monedero: 0 ns.
Bloqueo money: 1250 ns.
Ejecución de monedero: 6041 ns.
Tiempo de una ejecución: 89322 ns.

Bloqueo money: 1458 ns.
Ejecución de monedero: 2084 ns.
Bloqueo money: 1146 ns.
Ejecución de monedero: 5521 ns.
Tiempo de una ejecución: 107499 ns.

Bloqueo money: 1459 ns.
Bloqueo money: 1250 ns.
Ejecución de monedero: 6094 ns.
Ejecución de monedero: 89583 ns.
Tiempo de una ejecución: 101562 ns.

Ejecución de monedero: 13229 ns.
Bloqueo money: 1198 ns.
Ejecución de monedero: 5677 ns.
Tiempo de una ejecución: 89895 ns.

Ejecución de monedero: 15313 ns.
Bloqueo money: 1198 ns.
Ejecución de monedero: 5417 ns.
Tiempo de una ejecución: 89947 ns.

Bloqueo devolver: 105 ns.
Ejecución de monedero: 3907 ns.
Bloqueo money: 1250 ns.
Bloqueo money y devolver: 1250 ns.
Ejecución de monedero: 4739 ns.
Tiempo de una ejecución: 84895 ns.

Bloqueo money: 1198 ns.
Ejecución de monedero: 573 ns.
Bloqueo money: 1250 ns.
Ejecución de monedero: 5156 ns.
Tiempo de una ejecución: 88958 ns.

Bloqueo money: 1458 ns.
Ejecución de monedero: 2761 ns.
Bloqueo money: 1302 ns.
Ejecución de monedero: 7240 ns.
Tiempo de una ejecución: 25937 ns.

Bloqueo money: 1406 ns.
Ejecución de monedero: 50520 ns.
Bloqueo money: 1198 ns.
Ejecución de monedero: 5990 ns.
Tiempo de una ejecución: 92291 ns.

Ejecución de monedero: 11198 ns.
Bloqueo money: 1198 ns.
Ejecución de monedero: 5989 ns.
Tiempo de una ejecución: 89374 ns.

Ejecución de monedero: 1510 ns.
Bloqueo money: 1198 ns.
Ejecución de monedero: 5260 ns.
Tiempo de una ejecución: 88645 ns.

Ejecución de monedero: 11770 ns.
Bloqueo money: 1198 ns.
Ejecución de monedero: 5469 ns.
Tiempo de una ejecución: 87291 ns.

Bloqueo devolver: 1354 ns.
Ejecución de monedero: 5260 ns.

En el que el tiempo máximo (el caso peor) sería de 107499 ns. << 0.45 segundos. Es decir el sistema cumpliría las especificaciones establecidas.
