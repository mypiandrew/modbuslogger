#!/bin/bash

echo "Creating New DBs"
sqlite3 -echo db_data.db <db_data.sql
sqlite3 -echo db_config.db <db_config.sql

