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
*/
int readConfig()
{	
	int result=sqlite3_open(db_config,&dbhandle);
	
	if (result != SQLITE_OK) 
	{
		printf("Failed to open database \n\r");
		sqlite3_close(dbhandle);
		return 1;
	}
	printf("Opened db %s OK\n\r",db_config);

	
	//***************************************************************
	//********************** CONFIG *********************************
	//***************************************************************
	
	sql = "SELECT logInterval, transferInterval, dsTotal, dpTotal,alignedData FROM config";
	result = sqlite3_prepare_v2(dbhandle, sql,-1, &stmt, NULL);	
	if (result != SQLITE_OK) 
	{
		printf("Failed to prepare statement main.config- %i\n\r",result);
		sqlite3_close(dbhandle);
		return 2;
	}

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) 
		{ 			
			config.logInterval      = sqlite3_column_int(stmt,0);
			config.transferInterval = sqlite3_column_int(stmt,1);
			config.dsTotal          = sqlite3_column_int(stmt,2);
			config.dpTotal          = sqlite3_column_int(stmt,3);
			config.alignedData      = sqlite3_column_int(stmt,4);
		}
	} while (result == SQLITE_ROW);
	
	sqlite3_finalize(stmt);
	
	// Ensure we're not about to try and import more data than we've allowed for
	if (config.dsTotal > (MAX_DATASOURCES -1))
	{
		printf("Max DataSources [%i] Exceeded...Exiting\n",MAX_DATASOURCES);
	}
	
	if (config.dpTotal > (MAX_DATAPOINTS -1))
	{
		printf("Max DataPoints [%i] Exceeded...Exiting\n",MAX_DATAPOINTS);
	}
	
	// Warning if more datapoints configured than we're intending to use
	sql = "SELECT COUNT (*) FROM dataPoint";
	result = sqlite3_prepare_v2(dbhandle, sql,-1, &stmt, NULL);	
	if (result != SQLITE_OK) 
	{
		printf("Failed to prepare statement Count - %i\n\r",result);
		sqlite3_close(dbhandle);
		return 2;
	}
	
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) 
		{ 			
			if (sqlite3_column_int(stmt,0) > (MAX_DATAPOINTS - 1))
			{
				printf("\n**********************************************************************\n");
				printf("Warning : Datapoints table longer than amount of datapoints configured\n",MAX_DATAPOINTS);
				printf("**********************************************************************\n\n");
			}
			
		}
	} while (result == SQLITE_ROW);
	
	sqlite3_finalize(stmt);

	//***************************************************************
	//********************** DATA SOURCES ***************************
	//***************************************************************	

	sql = "select deviceId, deviceType, modbusId, baudRate, dataBits, stopBit, interface, parity, deviceTimeout  from dataSource";
	result = sqlite3_prepare_v2(dbhandle, sql,-1, &stmt, NULL);
	
	if (result != SQLITE_OK) 
	{
		printf("Failed to prepare statement datasources - %i\n\r",result);
		sqlite3_close(dbhandle);
		return 2;
	}

	i=1;
	do {		
		result = sqlite3_step(stmt);
		
		if (result == SQLITE_ROW) 
		{ 
			
			dataSource[i].deviceId   = sqlite3_column_int(stmt,0);
			dataSource[i].deviceType = sqlite3_column_int(stmt,1);
			dataSource[i].modbusId   = sqlite3_column_int(stmt,2);
			dataSource[i].baudRate   = sqlite3_column_int(stmt,3);
			dataSource[i].dataBits   = sqlite3_column_int(stmt,4);		
			dataSource[i].stopBit    = sqlite3_column_int(stmt,5);
			dataSource[i].timeout    = sqlite3_column_int(stmt,8);						
			strcpy(dataSource[i].interface, (char *)sqlite3_column_text(stmt,6));
			strcpy(dataSource[i].parity,    (char *)sqlite3_column_text(stmt,7));
	
			i++;
		}
	} while (result == SQLITE_ROW);

   sqlite3_finalize(stmt);
   
	//***************************************************************
	//********************** DATA POINTS ****************************
	//***************************************************************
	
	// Make sure any write actions are processed at the end of each DAQ run
	sql = "SELECT id, deviceId, regAddress, regType, modbusCode, writeValue, scale, scaleValue, offset FROM dataPoint ORDER BY modbusCode, id, deviceId";
	result = sqlite3_prepare_v2(dbhandle, sql,-1, &stmt, NULL);
	
	if (result != SQLITE_OK) 
	{
		printf("Failed to prepare statement datapoint - %i\n\r",result);
		sqlite3_close(dbhandle);
		return 2;
	}
	i=1;

	do {
		
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) 
		{ 
			dataPoint[i].id         = sqlite3_column_int(stmt,0);
			dataPoint[i].deviceId   = sqlite3_column_int(stmt,1);
			dataPoint[i].regAddress = sqlite3_column_int(stmt,2);
			dataPoint[i].regType    = sqlite3_column_int(stmt,3);
			dataPoint[i].modbusCode = sqlite3_column_int(stmt,4);
			dataPoint[i].writeValue = sqlite3_column_int(stmt,5);		
			dataPoint[i].scale      = sqlite3_column_int(stmt,6);
			dataPoint[i].scaleValue = sqlite3_column_double(stmt,7);
			dataPoint[i].offset     = sqlite3_column_double(stmt,8);
			

			i++;
		}
	} while (result == SQLITE_ROW);

	
    /* Finish off + Free handle*/
	sqlite3_finalize(stmt);
	sqlite3_close(dbhandle);
	
	return 0;
}

void printConfig()
{
	printf("--------- Config Imported ----------\n\n");
	printf("Config logInterval       = [%i]\n", config.logInterval);
	printf("Config transferInterval  = [%i]\n", config.transferInterval);
	printf("Config dsTotal           = [%i]\n", config.dsTotal);
	printf("Config dpTotal           = [%i]\n", config.dpTotal);
	printf("Config alignedData       = [%i]\n", config.alignedData);
	printf("\n\n");


   for(i=1 ; i<(config.dsTotal+1);i++)
   {	
	
		printf("dataSource [%i] deviceId    = [%i]\n", i, dataSource[i].deviceId);
		printf("dataSource [%i] deviceType  = [%i]\n", i, dataSource[i].deviceType);
		printf("dataSource [%i] modbusId    = [%i]\n", i, dataSource[i].modbusId);
		printf("dataSource [%i] interface   = [%s]\n", i, dataSource[i].interface);
		printf("dataSource [%i] baudRate    = [%i]\n", i, dataSource[i].baudRate);
		printf("dataSource [%i] dataBits    = [%i]\n", i, dataSource[i].dataBits);
		printf("dataSource [%i] parity      = [%s]\n", i, dataSource[i].parity);	
		printf("dataSource [%i] stopBit     = [%i]\n", i, dataSource[i].stopBit);
		printf("dataSource [%i] timeout     = [%i]\n", i, dataSource[i].timeout);
		printf("\n");		
		
	}
	printf("\n");

   for(i=1 ; i<(config.dpTotal+1);i++)
   {	
		printf("dataPoint [%i] datapointId = [%i]\n", i, dataPoint[i].id);
		printf("dataPoint [%i] deviceId    = [%i]\n", i, dataPoint[i].deviceId);
		printf("dataPoint [%i] regAddress  = [%i]\n", i, dataPoint[i].regAddress);
		printf("dataPoint [%i] regType     = [%i]\n", i, dataPoint[i].regType);
		printf("dataPoint [%i] modbusCode  = [%i]\n", i, dataPoint[i].modbusCode);
		printf("dataPoint [%i] writeValue  = [%i]\n", i, dataPoint[i].writeValue);
		
		printf("\n");
	}
	printf("------------------------------------\n\n\n");
	 
	printf("\n");
	
}
