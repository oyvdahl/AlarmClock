#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



// Port B - Outputs
#define COM2 PB4
#define COM3 PB5
#define SEGMENT_B PB6

// Port D - Outputs
#define COM1 PD4
#define SEGMENT_A PD6
#define SEGMENT_F PD7

// Port F - Outputs
#define  COM4 PF7
#define  SEGMENT_C PF5
#define  SEGMENT_D PF1
#define  SEGMENT_E PF0
#define  SEGMENT_G PF6
#define  SEGMENT_DP PF4

// Port E - Outputs
#define BUZZER PE6

// Port C - Inputs
#define BTN_INPUT1 PC6
#define BTN_INPUT2 PC7

volatile int countdown_from_s = 15;
volatile int count = 0;
volatile int seconds = 0;
volatile int disp1_value = 9;
volatile int disp2_value = 8;
volatile int disp3_value = 7;
volatile int disp4_value = 6;
volatile int stop;





void setup() {
  // Setup outputs
  DDRB = ((1 << SEGMENT_B) | (1 << COM2) | (1 << COM3));
  DDRD = ((1 << SEGMENT_A) | (1 << SEGMENT_F) | (1 << COM1));
  DDRF = ((1 << SEGMENT_C) | (1 << SEGMENT_D) | (1 << SEGMENT_E) | (1 << SEGMENT_G) | (1 << SEGMENT_DP) | (1 << COM4));
  DDRE = (1 << BUZZER);
  

  // Disable buzzer
  enable_buzzer(false);

  //Enable the timer
  //enable_timer();

  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  //TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCNT1 = 49911;
  TCCR1B |= (1 << CS12) | (1 << CS10);    // 1024 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

void loop() {


  ////////////////////
  // UPDATE DISPLAY //
  ////////////////////

  set_digit(disp1_value);
  set_segDP(false);
  enable_digit_x(1);
  _delay_ms(5);

  set_digit(disp2_value);
  set_segDP(true);
  enable_digit_x(2);
  _delay_ms(5);

  set_digit(disp3_value);
  set_segDP(false);
  enable_digit_x(3);
  _delay_ms(5);

  set_digit(disp4_value);
  set_segDP(false);
  enable_digit_x(4);
  _delay_ms(5);

}




void enable_timer() {

  TCCR1B |= 1 << WGM12;
  TCCR1B |= 1 << CS12;
  OCR1A = 300;
  TIMSK1 |= 1 << OCIE1A;


  sei();                               // enable all interrupts

}


void enable_buzzer(int enable) {
  if (enable) {
    PORTE |= (1 << PE6);
  } else {
    PORTE &= ~(1 << PE6);
  }
}

//Pull COM1 low to enable
void enable_digit_1(int enable)
{
  if (!enable) {
    PORTD |= (1 << PD4);
  } else {
    PORTD &= ~(1 << PD4);
  }
}

//Pull COM2 low to enable
void enable_digit_2(int enable)
{
  if (!enable) {
    PORTB |= (1 << PB4);
  } else {
    PORTB &= ~(1 << PB4);
  }
}

//Pull COM3 low to enable
void enable_digit_3(int enable)
{
  if (!enable) {
    PORTB |= (1 << PB5);
  } else {
    PORTB &= ~(1 << PB5);
  }
}

//Pull COM4 low to enable
void enable_digit_4(int enable)
{
  if (!enable) {
    PORTF |= (1 << PF7);
  } else {
    PORTF &= ~(1 << PF7);
  }
}

void set_segA(int enable) {
  if (enable) {
    PORTD |= (1 << PD6);
  } else {
    PORTD &= ~(1 << PD6);
  }
}

void set_segB(int enable) {
  if (enable) {
    PORTB |= (1 << PB6);
  } else {
    PORTB &= ~(1 << PB6);
  }
}

void set_segC(int enable) {
  if (enable) {
    PORTF |= (1 << PF5);
  } else {
    PORTF &= ~(1 << PF5);
  }
}

void set_segD(int enable) {
  if (enable) {
    PORTF |= (1 << PF1);
  } else {
    PORTF &= ~(1 << PF1);
  }
}

void set_segE(int enable) {
  if (enable) {
    PORTF |= (1 << PF0);
  } else {
    PORTF &= ~(1 << PF0);
  }
}

void set_segF(int enable) {
  if (enable) {
    PORTD |= (1 << PD7);
  } else {
    PORTD &= ~(1 << PD7);
  }
}

void set_segG(int enable) {
  if (enable) {
    PORTF |= (1 << PF6);
  } else {
    PORTF &= ~(1 << PF6);
  }
}

void set_segDP(int enable) {
  if (enable) {
    PORTF |= (1 << PF4);
  } else {
    PORTF &= ~(1 << PF4);
  }
}

void enable_digit_x(int digit)
{
  switch (digit)
  {
    case 1:
      enable_digit_1(true);
      enable_digit_2(false);
      enable_digit_3(false);
      enable_digit_4(false);
      break;
    case 2:
      enable_digit_1(false);
      enable_digit_2(true);
      enable_digit_3(false);
      enable_digit_4(false);
      break;
    case 3:
      enable_digit_1(false);
      enable_digit_2(false);
      enable_digit_3(true);
      enable_digit_4(false);
      break;
    case 4:
      enable_digit_1(false);
      enable_digit_2(false);
      enable_digit_3(false);
      enable_digit_4(true);
      break;

  }
}

void set_digit(int number)
{

  // The following switch statement turns on the correct segments for the number to show
  switch (number) {

    case 0 :
      set_segA(true);
      set_segB(true);
      set_segC(true);
      set_segD(true);
      set_segE(true);
      set_segF(true);
      set_segG(false);
      break;

    case 1  :
      set_segA(false);
      set_segB(true);
      set_segC(true);
      set_segD(false);
      set_segE(false);
      set_segF(false);
      set_segG(false);
      break;

    case 2  :
      set_segA(true);
      set_segB(true);
      set_segC(false);
      set_segD(true);
      set_segE(true);
      set_segF(false);
      set_segG(true);
      break;

    case 3  :
      set_segA(true);
      set_segB(true);
      set_segC(true);
      set_segD(true);
      set_segE(false);
      set_segF(false);
      set_segG(true);
      break;

    case 4  :
      set_segA(false);
      set_segB(true);
      set_segC(true);
      set_segD(false);
      set_segE(false);
      set_segF(true);
      set_segG(true);
      break;

    case 5 :
      set_segA(true);
      set_segB(false);
      set_segC(true);
      set_segD(true);
      set_segE(false);
      set_segF(true);
      set_segG(true);
      break;

    case 6 :
      set_segA(true);
      set_segB(false);
      set_segC(true);
      set_segD(true);
      set_segE(true);
      set_segF(true);
      set_segG(true);
      break;

    case 7 :
      set_segA(true);
      set_segB(true);
      set_segC(true);
      set_segD(false);
      set_segE(false);
      set_segF(false);
      set_segG(false);
      break;

    case 8 :
      set_segA(true);
      set_segB(true);
      set_segC(true);
      set_segD(true);
      set_segE(true);
      set_segF(true);
      set_segG(true);
      break;

    case 9 :
      set_segA(true);
      set_segB(true);
      set_segC(true);
      set_segD(true);
      set_segE(false);
      set_segF(true);
      set_segG(true);
      break;

    // sets E for Error
    default :
      set_segA(true);
      set_segB(false);
      set_segC(false);
      set_segD(true);
      set_segE(true);
      set_segF(true);
      set_segG(true);
  }


}




ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  int seconds_without_minutes, minutes_without_seconds, disp_s, stop_cd;
  //TCNT1 = 34286;            // preload timer
  TCNT1 = 49911;
  seconds++;

  // Display time left from countdown:
  disp_s = countdown_from_s - seconds;

  if (disp_s == 0) {
    stop_cd = true;

    while (1) {
      enable_buzzer(true);
      _delay_ms(200);
      enable_buzzer(false);
      _delay_ms(100);
    }
  }

  // Separate the digits:
  seconds_without_minutes = disp_s % 60;
  disp4_value =  seconds_without_minutes % 10;
  disp3_value =  seconds_without_minutes / 10;

  minutes_without_seconds = disp_s / 60;
  disp2_value = minutes_without_seconds % 10;
  disp1_value = minutes_without_seconds / 10;
}






