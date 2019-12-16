affiche_etape(){
    echo "Choisir une Etape à vérifier: "
    echo "  1 - Etape 1"
    echo "  2 - Etape 2"
    echo "  3 - Etape 3"
    echo "  4 - Etape 4"
    echo "  5 - Etape 5"
    echo "  6 - Tout"
    echo "Etape à vérifier:"
}
etape1(){
    Tests=("foo.o" "/bin/ls" "checker.sh")
    for f in "${Tests[@]}"; do
        echo "*******************************************"
        echo "Testing the file : $f"
        first=$(readelf -h $f | egrep "Magique|Magic" | tr -d "\n\t ")
        second=$(./elf_dumper $f | egrep "Magique|Magic" | tr -d "\n\t ")
        french_word="Magique"
        first=${first/Magic/$french_word}
        second=${second/Magic/$french_word}
        if [[ -n $first  &&  -n $second ]]
        then
            echo "$first"
            echo "$second"
        fi
        if [[ $first == $second ]] ; then
            echo "GOOD TEST"
        else
            echo "WRONG TEST"
        fi
    done
}
etape2(){
    echo "etape2"
}
etape3(){
    echo "etape3"
}
etape4(){
    echo "etape4"
}
etape5(){
    echo "etape5"
}
ensemble(){
    echo "Ensemble des étapes"
}

affiche_etape
read no_etape;
case $no_etape in
        1)
        etape1
        ;;
        2)
        etape2
        ;;
        3)
        etape3
        ;;
        4)
        etape4
        ;;
        5)
        etape5
        ;;
        6)
        ensemble
        ;;
        *)
esac
        
