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

typedef struct 
{
	int logInterval;	
	int transferInterval; 
	int dsTotal; 
	int dpTotal; 
	int alignedData;
}CONFIG;	
		
typedef struct 
{
	int deviceId;
	int deviceType;
	int modbusId; 
	int dataBits;
	int baudRate;
	int stopBit;
	int timeout;
	char parity[6];
	char interface[20];  // /dev/ttyS0  or 111.222.333.444
}DATASOURCE;		
		
typedef struct 
{
	int id;
	int deviceId;
	int regAddress;               
	int regType;
	int modbusCode;
	int writeValue;	
	int scale;
	float scaleValue;
	float offset;
}DATAPOINT;	
	
typedef struct 
{
	int deviceId;
	int id;
	float value;
	char timeStamp[20];						
}DATALOG;	

//Globals

#define REG_UI16   1
#define REG_SI16   2
#define REG_UI32B  3
#define REG_UI32L  4
#define REG_SI32B  5
#define REG_SI32L  6
#define REG_F32B   7
#define REG_F32L   8

#define DEVICE_OK  0
#define DEVICE_FAIL   10

#define MAX_LOG_RATE    1 		// In Mins

#define MAX_DATASOURCES 50 		// Devices
#define MAX_DATAPOINTS  500		// LogPoints

#define DATALOG_SPACE   (MAX_DATAPOINTS * MAX_LOG_RATE) + MAX_DATAPOINTS // we start array at 1 not 0

#define FILE_RELOAD_CONFIG  "/tmp/reload-config"
#define INVOKE_TRANSFER     "./transfer.sh"

CONFIG config;
DATAPOINT dataPoint[MAX_DATAPOINTS];     // Max Data Points 
DATALOG dataLog[DATALOG_SPACE];          // Max recorded values per sweep
DATASOURCE dataSource[MAX_DATASOURCES];  // Max devices/data sources


// sql 
char * db_config="db_config.db";
char * db_data="db_data.db";
sqlite3 * dbhandle;
sqlite3_stmt *stmt;
char eventMsg[255];
char * sql;
int rc;

// time
time_t timeNow;
struct tm *ts;
char datetime[20];
int currentMin;

// modbus
modbus_t *mb;  
uint16_t mbdata_UI16[5]; // defines storage per modbus read
int mbShift;
int mbTotal;
int mbReadAttempt =0;
int deviceStatus;
unsigned int temp1;
int temp2;

int i;
int argc;
