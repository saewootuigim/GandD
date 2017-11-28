#!/bin/bash
RAW_DATA="Pc64.00psi.csv"
date_formatted=$(date "+%m/%d/%y %H:%M:%S")
echo "Running G and D project. " $date_formatted
python3 GandD.py ./raw_data/$RAW_DATA
