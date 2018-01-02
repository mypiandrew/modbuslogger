/*
	Database	: db_data.db

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

PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
DROP TABLE "dataLog";
CREATE TABLE "dataLog" (
"deviceId"  INTEGER,
"id"  INTEGER,
"timeStamp"  TEXT,
"value"  REAL
);
INSERT INTO "dataLog" VALUES(1,1,'2014-04-26 14:05',0.0);
INSERT INTO "dataLog" VALUES(2,3,'2014-04-26 14:05',9.16540649414062436051e+02);
INSERT INTO "dataLog" VALUES(2,4,'2014-04-26 14:05',268304384.0);
INSERT INTO "dataLog" VALUES(1,1,'2014-04-26 14:10',0.0);
INSERT INTO "dataLog" VALUES(2,3,'2014-04-26 14:10',9.16540649414062436051e+02);
INSERT INTO "dataLog" VALUES(2,4,'2014-04-26 14:10',268304384.0);
INSERT INTO "dataLog" VALUES(1,1,'2014-04-26 15:10',0.0);
INSERT INTO "dataLog" VALUES(2,3,'2014-04-26 15:10',0.0);
INSERT INTO "dataLog" VALUES(2,4,'2014-04-26 15:10',0.0);
INSERT INTO "dataLog" VALUES(1,1,'2014-04-26 15:17',0.0);
INSERT INTO "dataLog" VALUES(2,3,'2014-04-26 15:17',9.16540649414062436051e+02);
INSERT INTO "dataLog" VALUES(2,4,'2014-04-26 15:17',268304384.0);
DROP TABLE "eventLog";
CREATE TABLE "eventlog" (
"datapointId"  INTEGER,
"deviceid"  INTEGER NOT NULL,
"timestamp"  TEXT NOT NULL,
"event"  TEXT NOT NULL
);
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:05','Modbus Read failed');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:05','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:05','Modbus Read failed');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:05','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:05','Modbus Read failed');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:05','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:10','Modbus Read failed');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:10','Modbus Read failed');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:10','Modbus Read failed');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 14:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(3,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(3,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(3,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(4,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(4,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(4,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(2,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(2,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(2,2,'2014-04-26 15:10','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 15:17','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 15:17','Error reading DataSource');
INSERT INTO "eventlog" VALUES(1,1,'2014-04-26 15:17','Error reading DataSource');
CREATE TABLE "deviceStatus" (
"deviceId"  INTEGER NOT NULL,
"status"  INTEGER NOT NULL,
"timeStamp"  TEXT NOT NULL,
PRIMARY KEY ("deviceId")
);
INSERT INTO "deviceStatus" VALUES(1,10,'2014-04-26 15:17');
INSERT INTO "deviceStatus" VALUES(2,0,'2014-04-26 15:17');
COMMIT;


