The on-shaft sensor will send out a serial packet like this:

sprintf(packet, "%d_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%d_%.2f_#", loopMillis, orientX, orientY, orientZ, angvX, angvY, angvZ, 
                                                                      accelX, accelY, accelZ, magX, magY, magZ, quatX, quatY, quatZ, quatW, 
                                                                      piezoV_A, piezoV_B, mic, strain_HX711);

An example serial response look like this:
12_0.00_0.00_0.06_0.00_-0.06_0.12_0.00_-0.01_-0.27_-7.75_-4.06_-30.38_-0.00_0.00_0.00_1.00_0.08_0.00_1798_-0.00_#
*Sanity check: The looptime is normally 10-20usec.


Sensors used:
MCU - ESP32 Feather - https://www.adafruit.com/product/5400
IMU - BNO055 - https://www.adafruit.com/product/4646
MIC - MAX4466 - https://www.adafruit.com/product/1063?gclid=Cj0KCQjwwJuVBhCAARIsAOPwGAR-SRRdLXUoWyHFM6EZYVZswj-b4KkUSDjksN9TWcVxea-5rMYYTnMaAmpWEALw_wcB
Piezo A, B - MEAS - https://www.sparkfun.com/products/9198
Strain Gauge DAQ - HX711 - https://www.sparkfun.com/products/13879
Strain Gauge - Customized - https://ieeexplore.ieee.org/document/8794408


See below for connection diagram.
