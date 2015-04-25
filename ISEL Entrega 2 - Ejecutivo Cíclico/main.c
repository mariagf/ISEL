#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "fsm.h"
#include <stdio.h>
#include <wiringPi.h>

#define GPIO_BUTTON	2
#define GPIO_LED	3
#define GPIO_CUP	4
#define GPIO_COFFEE	5
#define GPIO_MILK	6

#define GPIO_COIN   7

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

static struct timespec tv, start, end;

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
  if(C_PRICE > money){
    //printf("Introduzca 50 céntimos por favor...\n");
    return 0;
  }
  else if (button == 1){
    return 1;
  }
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
  digitalWrite (GPIO_LED, LOW);
  digitalWrite (GPIO_CUP, HIGH);
  timer_start (CUP_TIME);
    //printf("1. Preparando el vaso... \n");
}

static void coffee (fsm_t* this)
{
  digitalWrite (GPIO_CUP, LOW);
  digitalWrite (GPIO_COFFEE, HIGH);
  timer_start (COFFEE_TIME);
     //printf("2. Sirviendo el café... \n");
}

static void milk (fsm_t* this)
{
  digitalWrite (GPIO_COFFEE, LOW);
  digitalWrite (GPIO_MILK, HIGH);
  timer_start (MILK_TIME);
     //printf("3. Sirviendo la leche... \n");
}

static void finish (fsm_t* this)
{
  digitalWrite (GPIO_MILK, LOW);
  digitalWrite (GPIO_LED, HIGH);
  flagDevolver = 1;
     //printf("4. Su café está listo! \n");
}

static int moneyCount (fsm_t* this)
{
    money += coin;
    
    return flagDevolver;
    
}

static void devolver (fsm_t* this){
    if(flagDevolver == 1) {
       vueltas = money - C_PRICE;
       //printf("Sus vueltas son: %d \n", vueltas);  
       money = 0;
       vueltas = 0;
       flagDevolver = 0;
    }
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
void
timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b)
{
  res->tv_sec = a->tv_sec - b->tv_sec;
  res->tv_usec = a->tv_usec - b->tv_usec;
  if (res->tv_usec < 0) {
    --res->tv_sec;
    res->tv_usec += 1000000;
  }
}

// res = a + b
void
timeval_add (struct timeval *res, struct timeval *a, struct timeval *b)
{
  res->tv_sec = a->tv_sec + b->tv_sec
    + a->tv_usec / 1000000 + b->tv_usec / 1000000; 
  res->tv_usec = a->tv_usec % 1000000 + b->tv_usec % 1000000;
}

// wait until next_activation (absolute time)
void delay_until (struct timeval* next_activation)
{
  struct timeval now, timeout;
  gettimeofday (&now, NULL);
  timeval_sub (&timeout, next_activation, &now);
  select (0, NULL, NULL, NULL, &timeout);
}

int main (int argc, char *argv[])
{
  struct timeval clk_period = { 0, 1 * 1e6 };
  struct timeval next_activation;
  fsm_t* cofm_fsm = fsm_new (cofm);
  fsm_t* monedero_fsm = fsm_new (monedero);

  gettimeofday (&next_activation, NULL);

  int trama = 1;
  double t = 0;
  
  while (scanf("%d %d %d", &timer, &button, &coin) == 3) {
    nanosleep(&tv, &tv);
    
    switch(trama){
       case 1:
        printf("Cafe + Monedero\n");
       	clock_gettime(CLOCK_REALTIME, &start);
        fsm_fire (cofm_fsm);
        fsm_fire(monedero_fsm);
        clock_gettime(CLOCK_REALTIME, &end);
        
	t = (end.tv_nsec - start.tv_nsec);
        
       printf("Tiempo ejecutándose: %f ns.\n", t);
	trama = 2;
       break;

       case 2:
        printf("Cafe\n");
        clock_gettime(CLOCK_REALTIME, &start);
        fsm_fire (cofm_fsm);
        clock_gettime(CLOCK_REALTIME, &end);
        //timeval_sub (&working, &end, &start);
        //t = working.tv_nsec;
	t = (end.tv_nsec - start.tv_nsec);
        printf("Tiempo ejecutándose: %f ns.\n", t);
	trama = 1;
       break;

       default:
        printf("-\n");
       break;

       tv.tv_nsec = (time_t) (T_SECUNDARIO - t);

       //trama++;
        
        timeval_add (&next_activation, &next_activation, &clk_period);
        delay_until (&next_activation);
    }
  }
  return 0;
}
