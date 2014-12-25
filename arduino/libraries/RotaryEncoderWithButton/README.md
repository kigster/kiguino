## Rotary Encoder with a Push Button

Easily read rotary encoder buttons that also incorporate a push button, like Adafruit Rotary Encoder.

https://www.adafruit.com/product/377

This is a simple wrapper library for the wonderful [Encoder Library](http://www.pjrc.com/teensy/td_libs_Encoder.html) that
provides a couple of small additional convenient methods. 

We also use another fantastic library [OneButton](https://github.com/mathertel/OneButton) that provides rich interface to capture single, double clicks and long presses of the button using callbacks. 

The main convenience of this library on top of Encoder, is that It returns a negative or positive delta value instead of absolute values, which makes it easier to use Rotary in applications. 

## Wiring

For the rotary, it should have 3 pins on the side.  Side pins are A and B, middle one is ground.

For the button, right one is ground (if facing knob with 2 pins upfront), the other one is the input pin. Note that library
enables pull up resistor on the input pins: all of the pins will be HIGH.

## Dependencies

 * [Encoder](http://www.pjrc.com/teensy/td_libs_Encoder.html)
 * [OneButton](https://github.com/mathertel/OneButton)
 * 
To run the example, also need

 * [Simple Timer](https://github.com/infomaniac50/SimpleTimer)


## Example

```c++
#include <RotaryEncoderWithButton.h>

// pinA, pinB, pinButton
RotaryEncoderWithButton rotary(2,3,4);
OneButton *button;

void buttonClick() {
    Serial.println("Button was clicked!");
}

void setup() {
    Serial.begin(9600);

    // required to initialize the button pin, and the library
    // assumes built-in pull up resistor is used
    rotary.begin();

    // get the button and setup callbacks
    button = rotary.getButton();
    button.attachClick(buttonClick);
}

void loop() {
    long delta = rotary.rotaryDelta();
    if (delta > 0) {
        // turned right
    } else if (delta < 0) {
        // turned left
    }
}

```

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Added some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Author

Konstantin Gredeskoul, @kig, http://github.com/kigster

## License

MIT.  See LICENSE file for more details.

