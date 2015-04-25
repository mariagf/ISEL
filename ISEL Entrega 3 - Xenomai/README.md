# ISEL
Práctica 3: Xenomai

En este caso trataremos threads (hebras) una para cada máquina de estados. Cada hebra llamará a una función donde se ejecutará el fsm_fire cada vez que se cree una tarea. Mediremos los tiempos que tarda la ejecución de cada máquina de estados así como el bloqueo que se produce en las variables compartidas entre las dos máquinas, que son los que aparecen a continuación:

FICHERO OUTPUTS.TXT

Como podemos observar se cumplen los tiempos especificados por nuestro cliente de forma que el diseño es válido.