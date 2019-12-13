first=$(readelf -h foo.o | grep Magique | tr -d " \t\n\r" )
second=$(./elf_dumper foo.o | grep Magique| tr -d " \t\n\r" )
echo "$first"
echo "$second"

if [[ $first == $second ]] ; then
    echo "GOOD TEST"
else
    echo "WRONG TEST"
fi