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
