This assignment covers the implementation of wireless flashing or famously known as OTA(Over The Air) flashing.<br>
The procedure to be followed is :
1. First, flash the skeleton code in the Wireless Flash directory using wire USB to B. This sets up the esp as a receiver or client.
2. Then, choose the actual code to be flashed and change the the upload port and upload protocol as done in the platform.ini in the flash_code and using $pio run -t upload$, you should be able to flash the code wirelessly. 
