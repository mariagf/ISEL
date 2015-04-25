# ISEL
Práctica 4: Reactor
En esta práctica manejamos eventos, en concreto 2 uno para el café y otro para el monedero. Cada evento deberá tener distinta prioridad para asegurar un correcto funcionamiento de forma que tenga mayor prioridad el café para que no se desborde si por ejemplo alguien introduce monedas etc… Además cada evento deberá llamar a una función en la que se ejecutará el fsm_fire correspondiente a cada máquina,  de forma que seamos capaces de medir los tiempos con clock_gettime antes y después de cada fsm_fire. Es importante para un correcto funcionamiento de la práctica, conocer la diferencia entre timeval (segundos y microsegundos) y timespec (segundos y nanosegundos).

Observaremos que se cumplen los tiempos que habíamos establecido para cada máquina de estados con margen (y eso que he incluido en este caso la interfaz con el usuario mediante printf etc), ya que obtenemos del fichero outputs.txt lo siguiente:

Función MONEDERO
Introduzca 50 céntimos por favor...
Tiempo ejecutándose: 5573.000000 ns.
Función CAFE
Tiempo ejecutándose: 1511.000000 ns.
Función MONEDERO
Introduzca 50 céntimos por favor...
Tiempo ejecutándose: 5729.000000 ns.
Función CAFE
Tiempo ejecutándose: 1979.000000 ns.
Función MONEDERO
Introduzca 50 céntimos por favor...
Tiempo ejecutándose: 6615.000000 ns.
Función CAFE
Tiempo ejecutándose: 1979.000000 ns.
Función MONEDERO
Introduzca 50 céntimos por favor...
Tiempo ejecutándose: 5417.000000 ns.
Función CAFE
Tiempo ejecutándose: 2240.000000 ns.
Función MONEDERO
1. Preparando el vaso... 
Tiempo ejecutándose: 367707.000000 ns.
Función CAFE
Tiempo ejecutándose: 2395.000000 ns.
Función MONEDERO
2. Sirviendo el café... 
Tiempo ejecutándose: 44062.000000 ns.
Función CAFE
Tiempo ejecutándose: 2083.000000 ns.
Función MONEDERO
3. Sirviendo la leche... 
Tiempo ejecutándose: 47187.000000 ns.
Función CAFE
Tiempo ejecutándose: 2135.000000 ns.
Función MONEDERO
4. Su café está listo! 
Tiempo ejecutándose: 6302.000000 ns.
Función CAFE
Sus vueltas son: 20 
Tiempo ejecutándose: 11093.000000 ns.
Función MONEDERO
Introduzca 50 céntimos por favor...
Tiempo ejecutándose: 6041.000000 ns.
Función CAFE
Tiempo ejecutándose: 2292.000000 ns.
Función MONEDERO
Introduzca 50 céntimos por favor...
Tiempo ejecutándose: 5782.000000 ns.
Función CAFE
Tiempo ejecutándose: 2083.000000 ns.
Función MONEDERO
Tiempo ejecutándose: 3333.000000 ns.
Función CAFE
Tiempo ejecutándose: 1875.000000 ns.
Función MONEDERO
Tiempo ejecutándose: 3385.000000 ns.
Función CAFE
Tiempo ejecutándose: 2135.000000 ns.
Función MONEDERO
Tiempo ejecutándose: 4271.000000 ns.
Función CAFE
Tiempo ejecutándose: 2084.000000 ns.
Función MONEDERO
Tiempo ejecutándose: 6198.000000 ns.
Función CAFE
Tiempo ejecutándose: 2239.000000 ns.
Función MONEDERO
Tiempo ejecutándose: 3125.000000 ns.
Función CAFE
Tiempo ejecutándose: 2344.000000 ns.








