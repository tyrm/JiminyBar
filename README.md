# JiminyBar
Displays the status of the last 15 Puppet Runs using Adafruit Neopixels.
## Node JS Script
Create a user called Jiminy for the script to run under. The server by default listens on port 1492.

## Arduino
The Jiminy Bar uses an Arduino with an Ethernet Shield to make an HTTP call to the node.js script. It outputs to [Adafruit's Neopixels](https://github.com/adafruit/Adafruit_NeoPixel) and requires their library. There is a frame available on Thingiverse at http://www.thingiverse.com/thing:638047.
