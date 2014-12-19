NES_Project
===========
Networked Embedded Systems Course - TU/e Project files

The Folder transmit_receive contsains simple unitcast transmitter and receiver code. 

radio_logger contains source to generate interference patterns using packet storming. 

Folder interference contains source to generate interference patterns using modulated and unmodulated carriers

rssi_floor contains source to run the RSSI scanner //Originally written by Carlo Alberto Boano, Copyrights maintained. 
Results folder contains spreedsheets of all the RSSI and power readings while generating the patterns

Folder my_scripts contains some python scripts to read files and plot grpahs

The power_trace folder contains the interference patterns code with the power monitoring functionalities inserted. 

To check a patern. Flash the rssi_scanner code in one TelosB node 
make TARGET=sky rssi_scanner.upload
and run the java viewer to see the GUI dipicting the RSSI values. 
 
In another Telosb flash an interference pattern source from the folders mentioned above
make TARGET=sky example_interference.upload

Get the values from the RSSI scanner in a txt file 
make TARGET=sky rssi_scanner.upload login >eg.txt

Run the python script over the file to plot the graph
Python powersee.py

