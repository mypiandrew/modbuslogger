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

int logValues(sqlite3 *dbhandle) 
{		
	sql = "INSERT INTO dataLog (deviceId,id,timeStamp,value) VALUES (?,?,?,?)";
	if ( sqlite3_prepare(dbhandle, sql, -1,  &stmt,  0) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not prepare statement - Log Data\n"); 
		return (-1);
	}
	
	if (sqlite3_bind_int(stmt, 1, dataPoint[i].deviceId) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not bind deviceId [%i]\n", dataPoint[i].deviceId);  
		return (-1);
	} 
	if (sqlite3_bind_int(stmt, 2, dataPoint[i].id) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not bind datapointId [%]\n", dataPoint[i].id); 
		return (-1);
	}
	
	if (sqlite3_bind_text(stmt,3, datetime, strlen(datetime)+1, SQLITE_STATIC) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not Bind datetime [%s]\n", datetime);  
		return (-1);
	}	
	if (sqlite3_bind_double(stmt, 4, dataLog[i].value) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not bind value [%f]\n", dataLog[i].value);  
		return (-1);
	}
	
	printf("SQL   : Statement Ready...");
	rc = sqlite3_step(stmt);
	printf("Statement Run\n");
	sqlite3_finalize(stmt);
}

int logEvent(sqlite3 *dbhandle) 
{		
	sql = "INSERT INTO eventLog (deviceId,datapointId,timeStamp,event) VALUES (?,?,?,?)";
	if ( sqlite3_prepare(dbhandle, sql, -1,  &stmt,  0) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not prepare statement\n"); 
		return (-1);
	}
	
	if (sqlite3_bind_int(stmt, 1, dataPoint[i].deviceId) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not bind deviceId [%i]\n", dataPoint[i].deviceId);  
		return (-1);
	} 

	if (sqlite3_bind_int(stmt, 2, dataPoint[i].id) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not bind deviceId [%i]\n", dataPoint[i].deviceId);  
		return (-1);
	} 

	if (sqlite3_bind_text(stmt,3, datetime, strlen(datetime)+1, SQLITE_STATIC) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not Bind datetime [%s]\n", datetime);  
		return (-1);
	}

	if (sqlite3_bind_text(stmt,4, eventMsg, strlen(eventMsg)+1, SQLITE_STATIC) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not Bind eventMsg [%s]\n", datetime);  
		return (-1);
	}
	
	printf("SQL   : Statement Ready...");
	rc = sqlite3_step(stmt);
	printf("Statement Run\n");
	sqlite3_finalize(stmt);
}

int logStatus(sqlite3 *dbhandle) 
{		
	sql = "INSERT OR REPLACE INTO deviceStatus (deviceId,status,timeStamp) VALUES (?,?,?)";
	if ( sqlite3_prepare(dbhandle, sql, -1,  &stmt,  0) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not prepare statement\n"); 
		return (-1);
	}
	
	if (sqlite3_bind_int(stmt, 1, dataPoint[i].deviceId) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not bind deviceId [%i]\n", dataPoint[i].deviceId);  
		return (-1);
	} 

	if (sqlite3_bind_int(stmt, 2, deviceStatus) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not bind deviceId [%i]\n", deviceStatus);  
		return (-1);
	} 

	if (sqlite3_bind_text(stmt,3, datetime, strlen(datetime)+1, SQLITE_STATIC) != SQLITE_OK) 
	{ 
		printf("SQL   : Could not Bind datetime [%s]\n", datetime);  
		return (-1);
	}
	
	printf("SQL   : Statement Ready...");
	rc = sqlite3_step(stmt);
	printf("Statement Run\n");
	sqlite3_finalize(stmt);
}
