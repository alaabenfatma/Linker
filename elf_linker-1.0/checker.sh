###########################################
Tests=("foo.o" "checker.sh" "Examples_loader/example1.o")
###########################################
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
    Flag_etape1=0
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
verif_nb_ligne(){
    Flag=0
    first_N=$(readelf -S $1 | cut -d"]" -f1 | cut -d"[" -f2 | sed '1,5d' | cut -d' ' -f2 | sed  '/^des/d' | tr -d "\n\t" )
    second_N=$(./elf_dumper $1 | egrep "Numéro" | cut -d":" -f2 | tr -d "\n\t ")
    if [[ $first_N != $second_N ]] ; then
	    Flag=1
    fi
    return $Flag
}
verif_name(){
    Flag=0
    first=$(readelf -S $1 | cut -d"]" -f2 | cut -d" " -f2 | sed '1,5'd | sed '/^des/d' | tr -d "\n\t" )
    second=$(./elf_dumper $1 | egrep "Nom" | cut -d":" -f2 | sed '1,2'd | tr -d "\n\t ")
    if [[ $first != $second ]] ; then
	   Flag=1
    fi
    return $Flag
}
verif_type(){
    Flag=0
    compteur=1
    second=$(./elf_dumper $1 | egrep "Type" | sed '1d' | cut -d":" -f2 | tr -d "\t")
    for i in $second
    do
        first=$(readelf -S $1 | sed '1,4'd | egrep "]" | cut -d"]" -f2 | sed -n ${compteur}p | tr -d " ")
        #echo "$first" | egrep -q "$i"
        #echo "$first"
        #echo "$i"
        if [[ $? -ne 0 ]] ;then
            Flag=1
            return $Flag
        fi
        compteur=$((compteur+1))
    done
    return $Flag
}

etape2(){
    flag_test=0
    for f in "${Tests[@]}"; do
        echo "*******************************************"
        echo "Testing the file : $f"
        verif_nb_ligne $f
        val_ret=$?
        if [[ val_ret -eq 0 ]]
        then
            verif_name $f
            val_ret=$?
            if [[ val_ret -eq 0 ]]
            then
                verif_type $f
                val_ret=$?
                if [[ val_ret -eq 1 ]]
                then
                    flag_test=1
                    echo "WRONG TEST"
                    echo "Type des sections different"
                fi
            else
                flag_test=1
                echo "WRONG TEST"
                echo "Nom des sections different"
            fi
        else
            flag_test=1
            echo "WRONG TEST"
            echo "Nombre de sections different"
        fi
        if [[ flag_test -eq 0 ]]
        then
            echo "GOOD TEST"
        fi
    done
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
