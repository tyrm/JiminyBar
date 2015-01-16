# JiminyBar
Displays the status of the last 15 Puppet Runs using Adafruit Neopixels.
## Node JS Script
Create a user called Jiminy for the script to run under. The server by default listens on port 1492.

## Arduino
The Jiminy Bar uses an Arduino with an Ethernet Shield to make an HTTP call to the node.js script. It outputs to [Adafruit's Neopixels](https://github.com/adafruit/Adafruit_NeoPixel) and requires their library. There is a frame available on Thingiverse at http://www.thingiverse.com/thing:638047.

## Function
The Jiminy Bar uses 4 Colors to display the statuses of Puppet Runs:
* U - Nochange (Green)
* C - Changed (Blue)
* F - Failed (Red)
* N - Noop (Orange) *(notimplimented in node.js)*
* 0 - Null (Off)

When the Jiminy Bar starts up it will display a blue dashed pattern while trying to obtain an IP address. If it is unable it will flash a three part red pattern for 20 seconds and then try again. After it obtains an IP address it will begin polling the Jiminy Agent on the server every 5 seconds. If it is unable it will pulse red for 5 seconds and try again.

If you're using a reverse proxy it's important to update the url in the lower http header. 
