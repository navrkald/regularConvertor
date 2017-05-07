#!/bin//sh
for file in `ls *.jpg *.png *.eps`; do 
	file $file | grep RGBA 
done;
