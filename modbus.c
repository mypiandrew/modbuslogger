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

int getModbusValues()
{
	
	if (dataSource[dataPoint[i].deviceId].deviceType == 2)
	{
		mb = modbus_new_tcp(dataSource[dataPoint[i].deviceId].interface, 502);
	}
	else
	{
		mb = modbus_new_rtu(dataSource[dataPoint[i].deviceId].interface, dataSource[dataPoint[i].deviceId].baudRate,dataSource[dataPoint[i].deviceId].parity[0],dataSource[dataPoint[i].deviceId].dataBits,dataSource[dataPoint[i].deviceId].stopBit);
		modbus_set_slave(mb, dataSource[dataPoint[i].deviceId].modbusId);
	}
	
	// Set per-byte and total timeouts
	struct timeval tv = {dataSource[dataPoint[i].deviceId].timeout, 0};
	modbus_set_byte_timeout(mb, &tv);
	modbus_set_response_timeout(mb, &tv);

	// Enable/Disable Modbus debug
	modbus_set_debug(mb, FALSE);
	
	if(modbus_connect(mb) == -1) 
	{
		printf("Connect Failed to Modbus ID [%i] on [%s]\n", dataSource[dataPoint[i].deviceId].modbusId, dataSource[dataPoint[i].deviceId].interface);		
		modbus_close(mb);
		modbus_free(mb);
		return -1;
	}	 
	
	// Modbus request type
	switch (dataPoint[i].modbusCode) 
	{
		case 3:
			/* Read Input Registers from 3x Range : modbushandle, startReg, NumberOfRegisters, dataRetrieved) */
			if (dataPoint[i].regType > 2)
			{
				rc = modbus_read_input_registers(mb, dataPoint[i].regAddress, 2, mbdata_UI16);
			}
			else
			{
				rc = modbus_read_input_registers(mb, dataPoint[i].regAddress, 1, mbdata_UI16);
			}
			break;
	
		case 4:
		default:	
			/* Read Holding Registers from 4x Range : modbushandle, startReg, NumberOfRegisters, dataRetrieved) */	
			if (dataPoint[i].regType >2)
			{
				// 2 registers required
				rc = modbus_read_registers(mb, dataPoint[i].regAddress, 2, mbdata_UI16);
			}
			else
			{
				rc = modbus_read_registers(mb, dataPoint[i].regAddress, 1, mbdata_UI16);
			}
			break;
			
		case 6:		
			/*  Write Single Register to 4x Range : modbushandle, startReg, writeValue) */	
			/*  Useful for resetting energy counters, *always* done at the end of a read sequence */
			printf("Write : Device Id [%i] Value [%i] to Reg [%i] \n",dataPoint[i].deviceId,dataPoint[i].writeValue, dataPoint[i].regAddress);
			rc = modbus_write_register(mb, dataPoint[i].regAddress, dataPoint[i].writeValue);			
			if (rc == -1) 
			{
				printf("Write Failed\n");
				// add to eventlog
				strcpy(eventMsg, "Modbus write failed");
				logEvent(dbhandle);
				modbus_close(mb);
				modbus_free(mb);				
				return -1;
			}	
			break;						
	}	
						
	if (rc == -1) 
	{
		printf("Request Failed\n");
		// Add to eventlog
		strcpy(eventMsg, "Modbus Read failed");
		logEvent(dbhandle); 	
		modbus_close(mb);
		modbus_free(mb);
		return -1;
	}	
	
	modbus_close(mb);
	modbus_free(mb);	
	return 0; 
}

void decodeRegisters()
{
	// decode modbus data 
	switch (dataPoint[i].regType)
	{							
		default:			
		case REG_UI16:
			dataLog[i].value  =  mbdata_UI16[0];
			break;
		case REG_SI16:
			 temp1 = mbdata_UI16[0];
			 temp2 = (int16_t) temp1; 			 
			 dataLog[i].value = temp2;			 
			break;
		case REG_UI32B:
			temp1 = (mbdata_UI16[1] + (mbdata_UI16[0] * 65536));											
			dataLog[i].value  = temp1;
			break;
		case REG_UI32L:
			temp1 = (mbdata_UI16[0] + (mbdata_UI16[1] * 65536));
			dataLog[i].value  = temp1;			
			break;				
		case REG_SI32B:
			dataLog[i].value  = (mbdata_UI16[1] + (mbdata_UI16[0] * 65536));								
			break;				
		case REG_SI32L:
			dataLog[i].value  = (mbdata_UI16[0] + (mbdata_UI16[1] * 65536));								
			break;				
		case REG_F32B:
			mbShift = mbdata_UI16[1] << 16;
			mbTotal = mbShift + mbdata_UI16[0];
			dataLog[i].value = *((float *)&mbTotal);
			break;								
		case REG_F32L:
			mbShift = mbdata_UI16[0] << 16;
			mbTotal = mbShift + mbdata_UI16[1];
			dataLog[i].value = *((float *)&mbTotal);						
			break;
	}								

}
