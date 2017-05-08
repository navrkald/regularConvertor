#!/bin/sh
for file in `ls *.jpg *.png *.eps`; do 
	convert $file -alpha remove -flatten +matte $file
done;
