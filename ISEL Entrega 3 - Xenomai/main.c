#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "wiringPi.h"
#include "fsm.h"
#include <stdio.h>
#include "tasks.h"

#define GPIO_BUTTON	2
#define GPIO_LED	3
#define GPIO_CUP	4
#define GPIO_COFFEE	5
#define GPIO_MILK	6

#define GPIO_COIN 7

#define CUP_TIME	250
#define COFFEE_TIME	3000
#define MILK_TIME	3000

#define C_PRICE	    50

#define T_SECUNDARIO 350000000 // en ns
// El otro tiempo secundario lo estimamos a 0.7 ns -> MCD (Máximo común divisor 0.35)
// El hiperperiodo sería el mcm (mínimo común múltiplo 0.7 ns)

enum cofm_state {
  COFM_WAITING,
  COFM_CUP,
  COFM_COFFEE,
  COFM_MILK,
};

enum monedero_state {
  COFM_MONEDERO,
};

static int button = 0;
//static void button_isr (void) { button = 1; }

static int timer = 0;
static void timer_isr (union sigval arg) { timer = 1; }

static int money = 0;
static int coin = 0;
static int flagDevolver = 0;
static int vueltas = 0;

int t;

static struct timespec start, end;

fsm_t* cofm_fsm;
fsm_t* monedero_fsm;

static  pthread_mutex_t pth_money;
static  pthread_mutex_t pth_devolver;

static void timer_start (int ms)
{
  timer_t timerid;
  struct itimerspec value;
  struct sigevent se;
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_value.sival_ptr = &timerid;
  se.sigev_notify_function = timer_isr;
  se.sigev_notify_attributes = NULL;
  value.it_value.tv_sec = ms / 1000;
  value.it_value.tv_nsec = (ms % 1000) * 1000000;
  value.it_interval.tv_sec = 0;
  value.it_interval.tv_nsec = 0;
  timer_create (CLOCK_REALTIME, &se, &timerid);
  timer_settime (timerid, 0, &value, NULL);
}

static int button_pressed (fsm_t* this)
{
  clock_gettime(CLOCK_REALTIME, &start);
  pthread_mutex_lock(&pth_money);
  if(C_PRICE > money){
    //printf("Introduzca 50 céntimos por favor...\n");
    pthread_mutex_unlock(&pth_money);
    clock_gettime(CLOCK_REALTIME, &end);
    t = (end.tv_nsec - start.tv_nsec);
    printf("Bloqueo money: %d ns.\n", t);
    return 0;
  }
  else if (button == 1){
    pthread_mutex_unlock(&pth_money);
    clock_gettime(CLOCK_REALTIME, &end);
    t = (end.tv_nsec - start.tv_nsec);
    printf("Bloqueo money: %d ns.\n", t);
    return 1;
  }
  pthread_mutex_unlock(&pth_money);
  clock_gettime(CLOCK_REALTIME, &end);
  t = (end.tv_nsec - start.tv_nsec);
  printf("Bloqueo money: %d ns.\n", t);
  return 0;
}

static int timer_finished (fsm_t* this)
{
  int ret = timer;
  timer = 0;
  return ret;
}

static void cup (fsm_t* this)
{
  //digitalWrite (GPIO_LED, LOW);
  //digitalWrite (GPIO_CUP, HIGH);
  timer_start (CUP_TIME);
  //printf("1. Preparando el vaso... \n");
}

static void coffee (fsm_t* this)
{
  //digitalWrite (GPIO_CUP, LOW);
  //digitalWrite (GPIO_COFFEE, HIGH);
  timer_start (COFFEE_TIME);
  //printf("2. Sirviendo el café... \n");
}

static void milk (fsm_t* this)
{
  //digitalWrite (GPIO_COFFEE, LOW);
  //digitalWrite (GPIO_MILK, HIGH);
  timer_start (MILK_TIME);
  //printf("3. Sirviendo la leche... \n");
}

static void finish (fsm_t* this)
{
  //digitalWrite (GPIO_MILK, LOW);
  //digitalWrite (GPIO_LED, HIGH);
  clock_gettime(CLOCK_REALTIME, &start);
  pthread_mutex_lock(&pth_devolver);
  	flagDevolver = 1; 
  pthread_mutex_unlock(&pth_devolver);
  clock_gettime(CLOCK_REALTIME, &end);
  t = (end.tv_nsec - start.tv_nsec);
  printf("Bloqueo devolver: %d ns.\n", t);
  //printf("4. Su café está listo! \n");
}

static int moneyCount (fsm_t* this)
{
    clock_gettime(CLOCK_REALTIME, &start);
    pthread_mutex_lock(&pth_money);
    	money += coin;
    pthread_mutex_unlock(&pth_money);
    clock_gettime(CLOCK_REALTIME, &end);
    t = (end.tv_nsec - start.tv_nsec);
    printf("Bloqueo money: %d ns.\n", t);
    return flagDevolver;
    
}

static void devolver (fsm_t* this){
    clock_gettime(CLOCK_REALTIME, &start);
    pthread_mutex_lock(&pth_money);
    if(flagDevolver == 1) {
       vueltas = money - C_PRICE;
       //printf("Sus vueltas son: %d \n", vueltas);  
       money = 0;
       vueltas = 0;
       flagDevolver = 0;
    }
    pthread_mutex_unlock(&pth_money);
    clock_gettime(CLOCK_REALTIME, &end);
    t = (end.tv_nsec - start.tv_nsec);
    printf("Bloqueo money y devolver: %d ns.\n", t);
}

// Explicit FSM description
static fsm_trans_t cofm[] = {
  { COFM_WAITING, button_pressed, COFM_CUP,     cup    },
  { COFM_CUP,     timer_finished, COFM_COFFEE,  coffee },
  { COFM_COFFEE,  timer_finished, COFM_MILK,    milk   },
  { COFM_MILK,    timer_finished, COFM_WAITING, finish },
  {-1, NULL, -1, NULL },
};

static fsm_trans_t monedero[] = {
    { COFM_MONEDERO, moneyCount, COFM_MONEDERO, devolver},
    {-1, NULL, -1, NULL },
};

// Utility functions, should be elsewhere

// res = a - b
//void
//timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b)
//{
//  res->tv_sec = a->tv_sec - b->tv_sec;
//  res->tv_usec = a->tv_usec - b->tv_usec;
//  if (res->tv_usec < 0) {
//    --res->tv_sec;
//  res->tv_usec += 1000000;
//}
//}

// res = a + b
//void
//timeval_add (struct timeval *res, struct timeval *a, struct timeval *b)
//{
//  res->tv_sec = a->tv_sec + b->tv_sec
//    + a->tv_usec / 1000000 + b->tv_usec / 1000000; 
//  res->tv_usec = a->tv_usec % 1000000 + b->tv_usec % 1000000;
//}

// wait until next_activation (absolute time)
void delay_until (struct timeval* next_activation)
{
  struct timeval now, timeout;
  gettimeofday (&now, NULL);
  timeval_sub (&timeout, next_activation, &now);
  select (0, NULL, NULL, NULL, &timeout);
}

static void
task1 (void)
{ 
  clock_gettime(CLOCK_REALTIME, &start);
  fsm_fire (monedero_fsm);
  clock_gettime(CLOCK_REALTIME, &end);
  t = (end.tv_nsec - start.tv_nsec);
  printf("Ejecución de monedero: %d ns.\n", t);
}

static void
task2 (void)
{
  clock_gettime(CLOCK_REALTIME, &start);
  fsm_fire (cofm_fsm);
  clock_gettime(CLOCK_REALTIME, &end);
  t = (end.tv_nsec - start.tv_nsec);
  printf("Ejecución de monedero: %d ns.\n", t); 
}

int main (int argc, char *argv[])
{  
  pthread_t t_cofm;
  pthread_t t_monedero;

  cofm_fsm = fsm_new (cofm);
  monedero_fsm = fsm_new (monedero);
 
  //void* ret;

  //struct timeval clk_period = { 0, 250 * 1000 };
  //struct timeval next_activation;

  //wiringPiSetup();

  init_mutex (&pth_devolver, 2);
  init_mutex (&pth_money, 2);
 
  //pinMode (GPIO_CUP, OUTPUT);
  //pinMode (GPIO_COFFEE, OUTPUT);
  //pinMode (GPIO_MILK, OUTPUT);
  //pinMode (GPIO_LED, OUTPUT);
  //digitalWrite (GPIO_LED, HIGH);
   
  while (scanf("%d %d %d", &timer, &button, &coin)==3) {
	
	clock_gettime(CLOCK_REALTIME, &start);
	create_task (&t_cofm, task1, NULL, 400, 2, 1024);
  	create_task (&t_monedero, task2, NULL, 800, 1, 1024);
	clock_gettime(CLOCK_REALTIME, &end);
	t = (end.tv_nsec - start.tv_nsec);
	printf("Tiempo de una ejecución: %d ns.\n\n", t); 
    
//  pthread_join(t_cofm, &ret);
//  pthread_join(t_monedero, &ret);
}
  return 0;
}
