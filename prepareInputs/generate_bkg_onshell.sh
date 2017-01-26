#!/bin/bash
mkdir -p ./workspace_2d_bkg_onshell
for ch in 4e 2e2mu 4mu
do 
	for cat in 0 1
	do 
	root -l -q -b loadLib.C bkgWorkspace.c++\(\"${ch}\",$cat,0\)
	done
done
