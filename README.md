# Multi tool serial plotter/logger/console
goal : 
* replace with one program the the text console, the serial plotter and a serial logger.
* be able to visualize raw datas and plotted datas at the same time.
* add tool to change gain, sacle and oofset on each data channel.

# Multi platform
The program is writen with QT to alow all platform version.

# Basic use 
## Logging
1. select your serial port : *Settings -> Port*
2. if you want to log datas, select an output file : *File -> select output file*
	1. You can creat a new file or choose an already existing one.
	2. If you choose an existing file you will be asked to select a write mode : append / overwrite
	3. Append will add news datas after the end of the file
	4. Overwrite will erase the file and write new datas in 
3. Start and stop logging : *File -> record / stop record*
	
# Plotting
1. select your serial port : *Settings -> Port*
2. Select separators : *Settings -> separators*
	1. Datas on the serial port are supposed to be numeric in text format, separated by, commas, semicolons or white char
	2. Each line of datas is supposed to have the same number of fields
	3. You can preview the effect off separator in the console with : fields visu
3. Once you have selected separators you can see on the top of the plotter area one checkbox for each fields 
	1. the plotter area remain blank until you select wich channel you want to plot
	2. You can acces settings of each channel to tune gain and offset as you want.
 