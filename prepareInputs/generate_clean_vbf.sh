#!/bin/bash
let quad=$1
for ch in 4e 2e2mu 4mu
	do 
	icat=0
	while [ $icat -lt $quad ]
	do 
	root -b -l -q loadLib.C clean.c++\(\"${ch}\",${icat},1\) 
	root -l -b -q loadLib.C vbf.c\(\"${ch}\",${icat},1\)
	((icat++))	
		
	done
done
mv ./workspace125_onshell/ ./workspace125_onshell_Cat$quad/
cp clean.c clean.c_Cat$quad
cp vbf.c vbf.c_Cat$quad 
