Modbus Logger

Demo Program to log data from a Modbus TCP/RTU device to a SQL lite database


Install dependances first

apt-get install sqlite3 libsqlite3-dev libmodbus5 libmodbus-dev


Compile using :

make


Regenerate demo config :

./createdbs.sh


Use Modbus TCP Slave simulator :

https://sourceforge.net/projects/modrssim2




root@raspberrypi:~/modbuslogger-master# ./log

ModLog - v1.0

Max Log Rate    = [1]
Max DataSources = [50]
Max DataPoints  = [500]
DataLog Slots   = [1000]

Opened db db_config.db OK
Config Setup OK


--------- Config Imported ----------

Config logInterval       = [1]
Config transferInterval  = [15]
Config dsTotal           = [2]
Config dpTotal           = [4]
Config alignedData       = [1]


dataSource [1] deviceId    = [1]
dataSource [1] deviceType  = [1]
dataSource [1] modbusId    = [1]
dataSource [1] interface   = [/dev/ttyS0]
dataSource [1] baudRate    = [9600]
dataSource [1] dataBits    = [8]
dataSource [1] parity      = [N]
dataSource [1] stopBit     = [1]
dataSource [1] timeout     = [7]

dataSource [2] deviceId    = [2]
dataSource [2] deviceType  = [2]
dataSource [2] modbusId    = [1]
dataSource [2] interface   = [192.168.1.30]
dataSource [2] baudRate    = [0]
dataSource [2] dataBits    = [0]
dataSource [2] parity      = [0]
dataSource [2] stopBit     = [0]
dataSource [2] timeout     = [7]


dataPoint [1] datapointId = [1]
dataPoint [1] deviceId    = [2]
dataPoint [1] regAddress  = [2]
dataPoint [1] regType     = [1]
dataPoint [1] modbusCode  = [4]
dataPoint [1] writeValue  = [0]

dataPoint [2] datapointId = [3]
dataPoint [2] deviceId    = [2]
dataPoint [2] regAddress  = [4]
dataPoint [2] regType     = [2]
dataPoint [2] modbusCode  = [4]
dataPoint [2] writeValue  = [0]

dataPoint [3] datapointId = [4]
dataPoint [3] deviceId    = [2]
dataPoint [3] regAddress  = [6]
dataPoint [3] regType     = [2]
dataPoint [3] modbusCode  = [4]
dataPoint [3] writeValue  = [0]

dataPoint [4] datapointId = [2]
dataPoint [4] deviceId    = [2]
dataPoint [4] regAddress  = [9]
dataPoint [4] regType     = [1]
dataPoint [4] modbusCode  = [6]
dataPoint [4] writeValue  = [991]

------------------------------------



Start time is [2018-01-02 16:46]

Time now is [2018-01-02 16:47]
>>DAQ Interval [1] Reached
Opened db db_data.db OK
SQL   : Statement Ready...Statement Run
Read  : DataPoint Id [1] Device Id [2] Modbus Code [4] Register [2] RegType [1] Value [333.000000]
SQL   : Statement Ready...Statement Run
Read  : DataPoint Id [3] Device Id [2] Modbus Code [4] Register [4] RegType [2] Value [555.000000]
SQL   : Statement Ready...Statement Run
Read  : DataPoint Id [4] Device Id [2] Modbus Code [4] Register [6] RegType [2] Value [777.000000]
Write : Device Id [2] Value [991] to Reg [9]
SQL   : Statement Ready...Statement Run
SQL   : Statement Ready...Statement Run
SQL   : Statement Ready...Statement Run
SQL   : Statement Ready...Statement Run
^C
