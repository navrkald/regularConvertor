#!/bin/sh
for file in `ls *.jpg *.png *.eps ../graphs/*.jpg ../classDiagrams/*.png`; do 
	file $file | grep "RGBA"
done;
