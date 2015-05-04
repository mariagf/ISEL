# ISEL
Práctica 2: Ejecutivo Cíclico.

Respecto a unas especificaciones que nos pide un cliente (fabricante de maquinas de cafe), es decir, que el tiempo que tarde desde que introduzca una moneda hasta que este listo el café sea de como máximo 0.45 (se dijo en clase la condición de que este tiempo debería estar entre 0.3 y 0.6). Y que el  monedero tarde en ejecutarse desde que se introduce el dinero suficiente hasta que devuelve las vueltas 0.9 segundos (he elegido este tiempo para simplificar el sistema, al ser múltiplo del periodo anterior).

El sistema quedaría por tanto:

t = 0		t = 0.45	t = 0.9		t = 1.35	t = 1.8
café		café		café		café		café		…
monedero			monedero			monedero

Es decir, el primer periodo secundario y el segundo tendrían la misma duración de 0.45 segundos (Máximo común divisor (MCD)) mientras que el hiperperiodo sería de 0.9 segundos (mínimo común múltiplo (mcm)).

Podemos afirmar que estos tiempos se cumplen puesto que al ejecutar el main.c obtenemos como salida en el fichero outputs lo siguiente:

Cafe + Monedero
Tiempo ejecutándose: 2917.000000 ns.
Cafe
Tiempo ejecutándose: 1563.000000 ns.
Cafe + Monedero
Tiempo ejecutándose: 1458.000000 ns.
Cafe
Tiempo ejecutándose: 1250.000000 ns.
Cafe + Monedero
Tiempo ejecutándose: 1511.000000 ns.
Cafe
Tiempo ejecutándose: 1302.000000 ns.
Cafe + Monedero
Tiempo ejecutándose: 1458.000000 ns.
Cafe
Tiempo ejecutándose: 1250.000000 ns.
Cafe + Monedero
Tiempo ejecutándose: 1407.000000 ns.
Cafe
Tiempo ejecutándose: 1198.000000 ns.
Cafe + Monedero
Tiempo ejecutándose: 1406.000000 ns.
Cafe
Tiempo ejecutándose: 1302.000000 ns.
Cafe + Monedero
Tiempo ejecutándose: 1458.000000 ns.
Cafe
Tiempo ejecutándose: 1198.000000 ns.
Cafe + Monedero
Tiempo ejecutándose: 1459.000000 ns.



En el que el tiempo máximo (el caso peor) sería de 2917 ns << 0.45 segundos. Es decir el sistema cumpliría las especificaciones establecidas.
