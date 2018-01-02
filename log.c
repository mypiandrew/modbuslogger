/* 
	The MIT License (MIT)

	Copyright (c) 2015 Andrew O'Connell

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Pre-requisits :

	apt-get install sqlite3 libsqlite3-dev libmodbus5 libmodbus-dev

	Compile as below or use 'make all' to allow program to use external system (.so) libraries 
	gcc log.c -o log -lsqlite3 -lmodbus
	

*/

// System includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

// External Libs
#include <sqlite3.h>       //Public domain
#include <modbus/modbus.h> //LGPL 2.1

// Types & Variables
#include "logTypes.h"

// Functions
#include "config.c"
#include "modbus.c"
#include "sql.c"

int waitMinute(void);
void getLogTime();

int main(int argc, char* argv[])
{
	printf("\nModLog - v1.0\n\n");
	
	printf("Max Log Rate    = [%i]\n",   MAX_LOG_RATE);
	printf("Max DataSources = [%i]\n",   MAX_DATASOURCES);
	printf("Max DataPoints  = [%i]\n",   MAX_DATAPOINTS);
	printf("DataLog Slots   = [%i]\n\n", DATALOG_SPACE);

	
	if (readConfig() == 0)
	{
		printf("Config Setup OK\n\n\n");
	}
	else
	{
		printf("Can't process config\n");
		exit(1);
	}
	
	printConfig();
	
	getLogTime();
	
    printf("Start time is [%s]\n\n", datetime);
 
    while( waitMinute() == 1 )
    {
		getLogTime();
		currentMin=(time(NULL)%3600)/60;
		
		printf( "Time now is [%s]\n", datetime);
		
		// Log interval
        if (currentMin % config.logInterval == 0)
		{
			getLogTime(); 
			printf(">>DAQ Interval [%i] Reached\n",config.logInterval);	
			
			int result=sqlite3_open(db_data,&dbhandle);
	
			if (result != SQLITE_OK) 
			{
				printf("Failed to open db_data database \n\r");
				sqlite3_close(dbhandle);
				exit(1);
			}
	
			printf("Opened db %s OK\n\r",db_data);
	
			for(i=1 ; i<(config.dpTotal+1);i++)
			{
				deviceStatus=DEVICE_OK; // Innocent until proven guilty
				
				// Have 3 attempts to get the data then give up
				for ( mbReadAttempt = 0 ; mbReadAttempt <3 ; mbReadAttempt++)
				{
					dataLog[i].value = 0;
					
					rc = getModbusValues();
					if (rc == 0)
					{
						decodeRegisters();
						// Add scaling
						if (dataPoint[i].scale == 1)
						{
							dataLog[i].value = (dataLog[i].value * dataPoint[i].scaleValue); 
						}
						
						// Add/Subtract offset
						dataLog[i].value = (dataLog[i].value + dataPoint[i].offset);

						// Update device status
						deviceStatus=DEVICE_OK;			
						logStatus(dbhandle); 						
						
						break;
					}
					else
					{						
						printf("** Error reading DataSource **\n");
						
						// Add to eventlog
						strcpy(eventMsg, "Error reading DataSource");
						logEvent(dbhandle);						
						// Update device status
						deviceStatus=DEVICE_FAIL;			
						logStatus(dbhandle); 						
					}
				}
								
				dataLog[i].id = i;
				dataLog[i].deviceId = dataPoint[i].deviceId;

				// If aligned data is not enabled get a fresh time stamp for the datapoint
				if (config.alignedData == 0)
				{
					getLogTime();
				}
				
				strcpy(dataLog[i].timeStamp, datetime);									
				
				if (dataPoint[i].modbusCode <6)
				{
					printf("Read  : DataPoint Id [%i] Device Id [%i] Modbus Code [%i] Register [%i] RegType [%i] Value [%f]\n",dataPoint[i].id,dataPoint[i].deviceId,dataPoint[i].modbusCode, dataPoint[i].regAddress,dataPoint[i].regType, dataLog[i].value);							
				}
			}
			
			// Save values to database
			for(i=1 ; i<(config.dpTotal+1);i++)
			{
				// Only log read in values, not anything written 
				if (dataPoint[i].modbusCode <6)
					logValues(dbhandle); 
			}
			

			
			sqlite3_close(dbhandle);	
						
		}	
		
		// Transfer Interval
		if ( currentMin % config.transferInterval == 0)
		{
			printf(">>Transfer Interval [%i] Reached\n",config.transferInterval);	
			
			// Move db_data database, construct new one, initiate data transfer
			system(INVOKE_TRANSFER);
		}
        
    }
		
	return 0;
}

void getLogTime(void)
{	
    timeNow = time(NULL); 
    ts = localtime(&timeNow);
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M", ts);		
}

int waitMinute(void)
{  
	time_t startMin = time(NULL) / 60;  
   
	while(((time(NULL)/60) == startMin))
	{     
		sleep(1);

		if( access(FILE_RELOAD_CONFIG, F_OK) != -1) 
		{
			printf("\nReloading Config...\n\n");
			readConfig();
			printConfig();
			unlink (FILE_RELOAD_CONFIG);
		} 
	}

   return (1);
}
