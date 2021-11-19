#!/bin/sh

touch AB1.csv
rm AB*.csv
unzip BDEChannelMap.zip
dos2unix -c mac AB2.csv
cat AB*.csv | grep -v Strip |  sed -e "s/,/ /g" > packed_spreadsheet_nolabels.txt
./vdcbmakemap < packed_spreadsheet_nolabels.txt > vdcbce_chanmap_v1.txt
./maptest 3200 > extrachans3200.txt
./maptest -1 > maptest.output
cat vdcbce_chanmap_v1.txt extrachans3200.txt > vdcbce_chanmap_v1_dcchan3200.txt
./vertsliceshift < vdcbce_chanmap_v1.txt > vdcbce_chanmap_v1_verticalslice.txt
./vertsliceshift < vdcbce_chanmap_v1_dcchan3200.txt > vdcbce_chanmap_v1_dcchan3200_verticalslice.txt
