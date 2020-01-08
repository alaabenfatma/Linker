Tests=("Examples_loader/example1.o" "Examples_loader/example2.o")
affiche_etape(){
    echo "Choisir une Etape à vérifier: "
    echo "  1 - Etape 1    5 - Etape 5"
    echo "  2 - Etape 2    6 - Tout"
    echo "  3 - Etape 3    7 - afficher choix"
    echo "  4 - Etape 4    8 - fin"
    echo "Etape à vérifier:"
}
option(){
    echo "Choisir une tache : "
    echo " - vérifier"
    echo " - quitter"

}
######## Etape 1 ######
etape1(){
    Flag_etape1=0
    for f in "${Tests[@]}"; do
        echo "*******************************************"
        echo "Testing the file : $f"
        first=$(readelf -h $f | egrep "Magique|Magic" | tr -d "\n\t ")
        second=$(./$1 -h $f | egrep "Magique|Magic" | tr -d "\n\t ")
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
#######################
######## Etape 2 ######
verif_nb_ligne(){
    Flag=0
    first_N=$(readelf -S $1 | cut -d"]" -f1 | cut -d"[" -f2 | sed '1,4d' | cut -d' ' -f2 | sed  '/^des/d' | tr -d "\n\t" )
    second_N=$(./$2 -S $1 | egrep "Numéro" | cut -d":" -f2 | tr -d "\n\t ")
    if [[ $first_N != $second_N ]] ; then
	    Flag=1
    fi
    return $Flag
}

verif_name(){
    Flag=0
    first=$(readelf -S $1 | cut -d"]" -f2 | cut -d" " -f2 | sed '1,5'd | sed '/^des/d' | tr -d "\n\t" )
    second=$(./$2 -S $1 | egrep "Nom" | cut -d":" -f2 | tr -d " \t\n")
    if [[ $first != $second ]] ; then
	   Flag=1
    fi
    return $Flag
}

verif_type(){
    Flag=0
    compteur=1
    second=$(./$2 -S $1 | egrep "Type" | sed '1d' | cut -d":" -f2 | tr -d "\t")
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
    prog=$1
    Flag_etape2=0
    for f in "${Tests[@]}"; do
        echo "*******************************************"
        echo "Testing the file : $f"
        verif_nb_ligne $f $prog
        val_ret=$?
        if [[ val_ret -eq 0 ]]
        then
            verif_name $f $prog
            val_ret=$?
            if [[ val_ret -eq 0 ]]
            then
                verif_type $f $prog
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
#######################
######## Etape 3 ######


etape3(){
    prog=$1
    Flag_etape3=0
    for f in "${Tests[@]}"; do
        echo
    done
}

#######################
######## Etape 4 ######
nombre_de_section(){
    Flag_nb_section=0
    first=$(./$2 -s $1 | sed '1,2'd | wc -l )
    nb_entree=$(./$2 -s $1 | egrep "Table des symboles" | cut -d" " -f7)
    if [ $first -ne $nb_entree ]
    then
        Flag_nb_section=1
        #nombre d'entrée de la tables des symboles différent"
    else
        second=$(readelf -s $1 | egrep "La table de symboles" | cut -d" " -f7)
        if [ $first -ne $second ]
        then
           Flag_nb_section=1 
        fi
    fi
    return $Flag_nb_section
}
type_etape_4(){
    Flag_type=0
    list_type1=$(./$2 -s $1 | egrep "Type" | cut -d":" -f5 | cut -d"|" -f1 | tr -d " " )
    list_type2=$(readelf -s $1 | sed "1,3"d | tr -s " " | cut -d" " -f5)
    if [[ $list_type1 != $list_type2 ]]
    then
        Flag_type=1
    fi
    return $Flag_type
}
lien_etape_4(){
    Flag_lien=0
    list_lien1=$(./$2 -s $1 | egrep "Type" | cut -d":" -f6 | cut -d"|" -f1 | tr -d " " )
    list_lien2=$(readelf -s $1 | sed "1,3"d | tr -s " " | cut -d" " -f6)
    if [[ $list_lien1 != $list_lien2 ]]
    then
        Flag_lien=1
    fi
    return $Flag_lien
}
verif_nom_etape_4(){
    Flag_nom=0
    list_nom1=$(./$2 -s $1| egrep "Nom" | cut -d":" -f9)
    list_nom2=$(readelf -s $1 | sed "1,3"d | tr -s " " | cut -d" " -f9)
    echo "$list_nom1"
    echo "**********"
    echo "$list_nom2"
    echo "**********"
    if [[ $list_nom1 != $list_nom2 ]]
    then
        Flag_nom=1
    fi
    return $Flag_nom
}
etape4(){
    prog=$1
    Flag_etape4=0
    for f in "${Tests[@]}"; do
        echo "*******************************************"
        echo "Testing the file : $f"
        nombre_de_section $f $prog
        val_ret=$?
        if [ $val_ret -eq 0 ]
        then
            type_etape_4 $f $prog
            val_ret=$?
            if [ $val_ret -eq 0 ]
            then
                lien_etape_4 $f $prog
                val_ret=$?
                if [ $val_ret -eq 0 ]
                then
                    verif_nom_etape_4 $f $prog
                    val_ret=$?
                    if [ $val_ret -eq 1 ]
                    then
                        Flag_etape4=1
                        echo "Noms de la section mauvais"
                    fi
                else
                    Flag_etape4=1
                    echo "Lien de la section mauvais"
                fi
            else
                Flag_etape4=1
                echo "Typage de la section mauvais"
            fi
        else
            Flag_etape4=1
            echo "nombre d'entrée de la tables des symboles différent"
        fi
        if [ $Flag_etape4 -eq 0 ]
        then
            echo "GOOD TEST"
        else
            echo "WRONG TEST"
        fi
    done
}
#######################
######## MAIN #########
tmp=0

while [ $tmp -eq 0 ]
do
    option
    read tache
    if [ $tache = "vérifier" ]
    then
        echo "nom du programme : "
        read programme;

        affiche_etape
        read no_etape;
        tmp2=0
        while [ $tmp2 -eq 0 ]
        do        
            case $no_etape in
                    1)
                    etape1 $programme
                    echo "*******************************************"
                    affiche_etape
                    read no_etape;
                    ;;
                    2)
                    etape2 $programme
                    echo "*******************************************"
                    affiche_etape
                    read no_etape;
                    ;;
                    3)
                    etape3 $programme
                    echo "*******************************************"
                    affiche_etape
                    read no_etape;
                    ;;
                    4)
                    etape4 $programme
                    echo "*******************************************"
                    affiche_etape
                    read no_etape;
                    ;;
                    5)
                    etape5
                    ;;
                    6)
                    ensemble
                    ;;
                    7)
                    affiche_etape
                    ;;
                    8)
                    tmp2=1
                    ;;
                    *)
            esac
        done
    elif [ $tache = "quitter" ]
    then
        tmp=1
    else
        echo "mauvaise commande"
    fi
done
