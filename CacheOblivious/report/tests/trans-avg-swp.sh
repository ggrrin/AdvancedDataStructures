#!/bin/sh


function trans {
	echo "awk -f average_swap.awk $1 > $1S"
	awk -f average_swap.awk $1 > $1S
}

echo starting with $#
while [ $# -ne 0 ] 
do
	trans $1
	shift
done
echo end
