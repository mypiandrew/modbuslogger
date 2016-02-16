all: log

log: 
	gcc log.c -o log -lsqlite3 -lmodbus	

clean:
	rm -rf *.o log
