#!/bin/bash

echo "Transfering DB"

uploadName=db_data`date +%y%m%d%H%M%S`.db
mv db_data.db ./upload/$uploadName

echo "Creating New DB"
sqlite3 db_data.db <db_data.sql
chmod 666 db_data.db

echo "Spawning Upload"

# Run Python upload

