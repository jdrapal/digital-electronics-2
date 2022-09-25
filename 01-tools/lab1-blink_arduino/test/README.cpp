# Lab 1: Jakub Drápal

### Morse code

1. Listing of C code which repeats one "dot" and one "comma" (BTW, in Morse code it is letter `A`) on a LED. Always use syntax highlighting, meaningful comments, and follow C guidelines:

```c
#define SHORT_DELAY 250
#define LONG_DELAY 750
#define Pause 1250
#define LED_RED PB0
#define PB0 8

int main(void)
{
    // Set pin where on-board LED is connected as output
    pinMode(LED_RED, OUTPUT);

    // Infinite loop
    while (1)
    {
        // Generate a lettre `A` Morse code
        // .
        digitalWrite(LED_RED, HIGH);
        _delay_ms(SHORT_DELAY);
        digitalWrite(LED_RED, LOW);
        _delay_ms(SHORT_DELAY);

        // -
        digitalWrite(LED_RED, HIGH);
        _delay_ms(LONG_DELAY);
        digitalWrite(LED_RED, LOW);

        // Pause
        _delay_ms(Pause);

    }

    // Will never reach this
    return 0;
}
```

2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure](https://github.com/jdrapal/digital-electronics-2/blob/main/01-tools/images/schema.png?raw=true)
