/*! \file gpio.c
  \author Dave Huseby <dave at linuxprogrammer.org>
  \brief GPIO App, mostly used for manual I/O pin control.
*/


#include "platform.h"
#include "command.h"
#include "gpio.h"

#define GPIO_APP

//! Handles a monitor command.
void gpio_handle_fn(uint8_t const app,
        uint8_t const verb,
        uint32_t const len);

// define the gpio app's app_t
app_t const gpio_app = {

  /* app number */
  GPIO,

  /* handle fn */
  gpio_handle_fn,

  /* name */
  "GPIO",

  /* desc */
  "\tThe GPIO app handles manual I/O pin control\n"
};

//! set up the ports for general I/O
static void gpio_setup()
{
  /* enable all of the pins */
  P1SEL &= ~BIT0;
  P5SEL &= ~BIT2;
  P5SEL &= ~BIT1;
  P5SEL &= ~BIT0;
  P5SEL &= ~BIT3;
  P4SEL &= ~BIT0;
  P6SEL &= ~BIT6;
  P2SEL &= ~BIT6;
  P3SEL &= ~BIT7;
  P3SEL &= ~BIT6;
}

//! start the gpio app
static void gpio_start()
{
}

//! stop the gpio app
static void gpio_stop()
{
}

//! configure specified pin to be in
static void gpio_set_pin_in( int pin )
{
  switch( pin )
  {
    case 0: //LED pin
      P1DIR &= ~BIT0;
      break;
    case 1:
      P5DIR &= ~BIT2;
      break;
    case 3:
      P5DIR &= ~BIT1;
      break;
    case 5:
      P5DIR &= ~BIT0;
      break;
    case 7:
      P5DIR &= ~BIT3;
      break;
    case 8:
      P4DIR &= ~BIT0;
      break;
    case 10:
      P6DIR &= ~BIT6;
      break;
    case 11:
      P2DIR &= ~BIT6;
      break;
    case 12:
      P3DIR &= ~BIT7;
      break;
    case 14:
      P3DIR &= ~BIT6;
      break;

    default:
    case 2:
    case 4:
    case 6:
    case 9:
    case 13:
      break;
  }
}

//! configure specified pin to be out
static void gpio_set_pin_out( int pin )
{
  switch( pin )
  {
    case 0: //LED pin
      P1DIR |= BIT0;
      break;
    case 1:
      P5DIR |= BIT2;
      break;
    case 3:
      P5DIR |= BIT1;
      break;
    case 5:
      P5DIR |= BIT0;
      break;
    case 7:
      P5DIR |= BIT3;
      break;
    case 8:
      P4DIR |= BIT0;
      break;
    case 10:
      P6DIR |= BIT6;
      break;
    case 11:
      P2DIR |= BIT6;
      break;
    case 12:
      P3DIR |= BIT7;
      break;
    case 14:
      P3DIR |= BIT6;
      break;

    default:
    case 2:
    case 4:
    case 6:
    case 9:
    case 13:
      break;
  }
}

//! set specified pin to given value
static void gpio_write_pin( int pin, int val )
{
  switch( pin )
  {
    case 0: //LED pin
      if ( val )
        P1OUT |= BIT0;
      else
        P1OUT &= ~BIT0;
      break;
    case 1:
      if ( val )
        P5OUT |= BIT2;
      else
        P5OUT &= ~BIT2;
      break;
    case 3:
      if ( val )
        P5OUT |= BIT1;
      else
        P5OUT &= ~BIT1;
      break;
    case 5:
      if ( val )
        P5OUT |= BIT0;
      else
        P5OUT &= ~BIT0;
      break;
    case 7:
      if ( val )
        P5OUT |= BIT3;
      else
        P5OUT &= ~BIT3;
      break;
    case 8:
      if ( val )
        P4OUT |= BIT0;
      else
        P4OUT &= ~BIT0;
      break;
    case 10:
      if ( val )
        P6OUT |= BIT6;
      else
        P6OUT &= ~BIT6;
      break;
    case 11:
      if ( val )
        P2OUT |= BIT6;
      else
        P2OUT &= ~BIT6;
      break;
    case 12:
      if ( val )
        P3OUT |= BIT7;
      else
        P3OUT &= ~BIT7;
      break;
    case 14:
      if ( val )
        P3OUT |= BIT6;
      else
        P3OUT &= ~BIT6;
      break;

    default:
    case 2:
    case 4:
    case 6:
    case 9:
    case 13:
      break;
  }
}

//! get value of specified pin
static uint8_t gpio_read_pin( int pin )
{
  switch( pin )
  {
    case 0: //LED pin
      return (P1IN | BIT0) ? 1 : 0;
    case 1:
      return (P5IN | BIT2) ? 1 : 0;
    case 3:
      return (P5IN | BIT1) ? 1 : 0;
    case 5:
      return (P5IN | BIT0) ? 1 : 0;
    case 7:
      return (P5IN | BIT3) ? 1 : 0;
    case 8:
      return (P4IN | BIT0) ? 1 : 0;
    case 10:
      return (P6IN | BIT6) ? 1 : 0;
    case 11:
      return (P2IN | BIT6) ? 1 : 0;
    case 12:
      return (P3IN | BIT7) ? 1 : 0;
    case 14:
      return (P3IN | BIT6) ? 1 : 0;

    default:
    case 2:
    case 4:
    case 6:
    case 9:
    case 13:
      break;
  }
  return 0;
}

//! Handles a monitor command.
void gpio_handle_fn(uint8_t const app,
          uint8_t const verb,
          uint32_t const len)
{
  int pin, val, pin1, pin2;

  switch(verb)
  {
  case START:
    gpio_start();
    txdata(app,verb,0);
    break;

  case STOP:
    gpio_stop();
    txdata(app,verb,0);
    break;

  case SETUP:
    gpio_setup();
    txdata(app,verb,0);
    break;

  case GPIO_WRITE_PIN:
    if ( len != 2 )
    {
      txdata(app,NOK,0);
      return;
    }
    pin = cmddata[0];
    val = cmddata[1];
    gpio_set_pin_out( pin );
    gpio_write_pin( pin, val );
    cmddata[0] = gpio_read_pin( pin );
    txdata(app,verb,1);
    break;

  case GPIO_READ_PIN:
    if ( len != 1 )
    {
      txdata(app,NOK,0);
    }
    pin = cmddata[0];
    gpio_set_pin_in( pin );
    cmddata[0] = gpio_read_pin( pin );
    txdata(app,verb,1);
    break;

  case GPIO_ROUND_TRIP:
    if ( len != 3 )
    {
      txdata(app,NOK,0);
      return;
    }
    pin1 = cmddata[0];
    val = cmddata[1];
    pin2 = cmddata[2];
    if ( pin1 == pin2 )
    {
      txdata(app,NOK,0);
      return;
    }
    gpio_set_pin_out( pin1 );
    gpio_set_pin_in( pin2 );
    gpio_write_pin( pin1, val );
    cmddata[0] = gpio_read_pin( pin2 );
    txdata(app,verb,1);
    break;

  default:
    txdata(app,NOK,0);
  }
}


