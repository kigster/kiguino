## Kiguino – Arduino Libs, Sketches and Useful Things

I've organized things as much as possible for complete clarity.  

  * sketches is where my sample, experimental, live/production, etc sketches live.
  * libraries is where my shareable libraries, that do lots of useful things are located. Most libs contain their own README.md, but if not read the comments of the .cpp or .h file for details. 
 
## Installation

Libraries are provided with a simple shell script that should work on Mac OS-X or Linux, or Windows with Cygwin. Simply run the installer, and pass your Arduino library path as the first argument.  The installer will create symlinks, so make sure to keep the github repo when yuo checked it out.  If you move it, just re-run installer again.

```bash
cd ~/Documents/Arduino
git clone https://github.com/kigster/kiguino.git
cd kiguino/arduino/libraries
../../install ~/Documents/Arduino/libraries
```

Or for Teensy3:

```bash
cd ~/Documents/Arduino/kiguino/teensy3/libraries
../../install ~/Documents/Arduino/libraries
```
  
## Author

&copy; 2014 Konstantin Gredeskoul, @kigster on github, @kig on twitter

Distributed under MIT License.




