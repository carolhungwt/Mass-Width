#!/bin/bash

for ch in 4e 2e2mu 4mu
        do
        for icat in 0 1 2
        do
                root -b -l -q loadLib.C vbf.c\(\"${ch}\",${icat},1\)
        done
done
