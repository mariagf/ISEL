# ISEL
Práctica 3: Xenomai

En este caso trataremos threads (hebras) una para cada máquina de estados. Cada hebra llamará a una función donde se ejecutará el fsm_fire cada vez que se cree una tarea. Mediremos los tiempos que tarda la ejecución de cada máquina de estados así como el bloqueo que se produce en las variables compartidas entre las dos máquinas, por problemas de instalación de Xenomai no se ha podido ejecutar el código, pero compila correctamente, por lo que no hemos podido comprobar dichos tiempos.