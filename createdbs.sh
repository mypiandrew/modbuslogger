#!/bin/bash

echo "Creating New DBs"
sqlite3 db_data.db <db_data.sql
sqlite3 db_config.db <db_config.sql

