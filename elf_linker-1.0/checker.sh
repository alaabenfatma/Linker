Tests=("foo.o" "/bin/ls" "checker.sh")
for f in "${Tests[@]}"; do
    echo "Testing the file : $f"
    first=$(readelf -h $f | egrep "Magique|Magic" | tr -d "\n\t ")
    second=$(./elf_dumper $f | egrep "Magique|Magic" | tr -d "\n\t ")
    french_word="Magique"
    first=${first/Magic/$french_word}
    second=${second/Magic/$french_word}
    echo "$first"
    echo "$second"
    if [[ $first == $second ]] ; then
        echo "GOOD TEST"
    else
        echo "WRONG TEST"
    fi
done