#!/bin/bash

for ch in 4e
	do
	for cat in 0
	do
	text2workspace.py hzz4l_${ch}_Cat${cat}_13TeV_onshell.txt -o hzz4l_${ch}_Cat${cat}_13TeV_onshell.root -P HiggsAnalysis.CombinedLimit.HighmassModel:HighmassModel --PO=mwAsPOI -v 3
	combine -M MultiDimFit hzz4l_${ch}_Cat${cat}_13TeV_onshell.root -n scan -t -1 --saveToys --setPhysicsModelParameters mean_pole=125,sigma_pole=0.004,r=1.,rvbf_ggh=1 --setPhysicsModelParameterRanges mean_pole=120,130:sigma_pole=0.0001,5.0001 --algo=grid --points=10

	mv hzz4l_${ch}_Cat${cat}_13TeV_onshell.txt ./datacard/
	mv ./hzz4l_${ch}_Cat${cat}_13TeV_onshell.root ./hzz4l_4mu_Cat0_13TeV_onshell.root.dot ./temp_root/
	mv ./higgsCombinescan.MultiDimFit.mH120.123456.root ./higgsCombinescan.MultiDimFit.mH120.123456_${ch}_Cat${cat}.root
	done
done
