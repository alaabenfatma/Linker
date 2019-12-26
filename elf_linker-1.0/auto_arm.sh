for i in $(seq 1 12); 
do 
readelf -x $i $1
done