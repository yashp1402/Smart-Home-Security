#ifndef __PRINTF_H__
#define __PRINTF_H__

#if defined (ARDUINO) && !defined (__arm__) && !defined(__ARDUINO_X86__)

int serial_putc( char c, FILE * )
{
  Serial.write( c );

  return c;
}

void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

#elif defined (__arm__)

void printf_begin(void){}

#elif defined(__ARDUINO_X86__)
int serial_putc( char c, FILE * )
{
  Serial.write( c );

  return c;
}

void printf_begin(void)
{
  stdout = freopen("/dev/ttyGS0","w",stdout);
  delay(500);
  printf("redirecting to Serial...");
}
#else
#error This example is only for use on Arduino.
#endif // ARDUINO

#endif // __PRINTF_H__
