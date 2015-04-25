#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "fsm.h"
#include <stdio.h>
#include "wiringPi.h"
#include <stdio.h>
#include "reactor.h"

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

enum cofm_state {
  COFM_WAITING,
  COFM_CUP,
  COFM_COFFEE,
  COFM_MILK,
};

enum monedero_state {
  COFM_MONEDERO,
};

fsm_t* cofm_fsm;
fsm_t* monedero_fsm;
double t;

static int button = 0;
//static void button_isr (void) { button = 1; }

static int timer = 0;
static void timer_isr (union sigval arg) { timer = 1; }

static int money = 0;
static int coin = 0;
static int flagDevolver = 0;
static int vueltas = 0;

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
    printf("Introduzca 50 céntimos por favor...\n");
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
    printf("1. Preparando el vaso... \n");
}

static void coffee (fsm_t* this)
{
  digitalWrite (GPIO_CUP, LOW);
  digitalWrite (GPIO_COFFEE, HIGH);
  timer_start (COFFEE_TIME);
     printf("2. Sirviendo el café... \n");
}

static void milk (fsm_t* this)
{
  digitalWrite (GPIO_COFFEE, LOW);
  digitalWrite (GPIO_MILK, HIGH);
  timer_start (MILK_TIME);
     printf("3. Sirviendo la leche... \n");
}

static void finish (fsm_t* this)
{
  digitalWrite (GPIO_MILK, LOW);
  digitalWrite (GPIO_LED, HIGH);
    flagDevolver = 1;
     printf("4. Su café está listo! \n");
}

static int moneyCount (fsm_t* this)
{
    money += coin;
    
    return flagDevolver;
    
}

static void devolver (fsm_t* this){
    if(flagDevolver == 1) {
       vueltas = money - C_PRICE;
       printf("Sus vueltas son: %d \n", vueltas);  
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

static struct timespec start, end;

static
void
task1_func (struct event_handler_t* this)
{
  
  static const struct timeval period = { 0, 350000 };
  printf ("Función CAFE\n");
  //Hay que llamar al fsm_fire de monedero y medimos
  clock_gettime(CLOCK_REALTIME, &start);
  fsm_fire(monedero_fsm);
  clock_gettime(CLOCK_REALTIME, &end);
  t += (end.tv_nsec - start.tv_nsec);
  printf("Tiempo ejecutándose: %f ns.\n", t);
  t = 0;

  timeval_add (&this->next_activation, &this->next_activation, &period);
}

static
void
task2_func (struct event_handler_t* this)
{
  
  static const struct timeval period = { 0, 350000 };
  printf ("Función MONEDERO\n");
  clock_gettime(CLOCK_REALTIME, &start);
  //Hay que llamar al fsm_fire de café
  fsm_fire (cofm_fsm);
  clock_gettime(CLOCK_REALTIME, &end);
  t += (end.tv_nsec - start.tv_nsec);
  printf("Tiempo ejecutándose: %f ns.\n", t);
  t = 0;
 
  timeval_add (&this->next_activation, &this->next_activation, &period);
}

int
main ()
{
  
  EventHandler eh1;
  EventHandler eh2;

  cofm_fsm = fsm_new (cofm);
  monedero_fsm = fsm_new (monedero);
  reactor_init ();
  
  // Café con mayor prioridad que monedero
  event_handler_init (&eh2, 2, task2_func);
  reactor_add_handler (&eh2);
 
  event_handler_init (&eh1, 1, task1_func);
  reactor_add_handler (&eh1);
  
  while (scanf("%d %d %d",&timer, &button, &coin) == 3) {
    reactor_handle_events ();

  }
return 0;

}