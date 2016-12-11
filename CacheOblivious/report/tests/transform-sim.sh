#!/bin/sh


function trans {
	echo "awk -f trans.awk $1 > $1.csv"
	awk -f trans.awk $1 > $1.csv
}

echo starting with $#
while [ $# -ne 0 ] 
do
	trans $1
	shift
done
echo end
