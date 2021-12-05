#!/bin/sh

touch AB1.csv
rm AB*.csv
unzip BDEChannelMap.zip

for i in 1 2 3 4 5 6 7 8; do
    dos2unix AB${i}.csv
    dos2unix -c mac AB${i}.csv
done

touch packed_spreadsheet_nolabels.txt
rm packed_spreadsheet_nolabels.txt
cat AB*.csv | grep -v Strip |  sed -e "s/,/ /g" > packed_spreadsheet_nolabels.txt
./vdcbmakemap < packed_spreadsheet_nolabels.txt > vdcbce_chanmap_v2.txt
./maptest 3200 > extrachans3200.txt
./maptest -1 > maptest.output
cat vdcbce_chanmap_v2.txt extrachans3200.txt > vdcbce_chanmap_v2_dcchan3200.txt
