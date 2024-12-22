#!/bin/bash

#Option d’aide (-h), si présente, toutes les autres options sont ignorées
function afficher_aide {
    for i in $@
    do
        if [$i = "-h"]
        then
            echo "================== Bienvenue sur la page d'aide ===================="
            echo
            echo "Nous sommes ici pour vous accompagner dans l'analyser de différentes stations (stationsHV-A, HV-B, postes LV), 
                afin de déterminer si elles sont en situation de surproduction ou de sous-production d’énergie, ainsi que 
                d’évaluer quelle proportion de leur énergie est consommée par les entreprises et les particuliers."
            echo 
            echo "Pour ce faire vous devez entrer simplement 3 choses, avec une ou plusieurs options à chaque étape:
            
                1. Le chemin du fichier 
                        option : Entrez le chemin d'entrée du fichier.
                        
                Définissez vos paramètres d’observation :
                
                2. Le type de station à analyser ''
                        options : 'hvb','hva' ou les 'lv'.
                3. La catégorie du client à examiner la consommation ''
                        options : celles des entreprises 'comp', de particuliers 'indiv', ou de tous 'all'."
            echo
            echo"Attention, les HVB et HVA ne peuvent être assimilées aux options indiv ni all !
            
                Notez que vous pouvez revenir sur cette page d'aide si vous entrez -h en argument."
            echo 
            echo "===================================================================="
            exit 0
        fi
    done
}


#1)LES DIFFERENTS PARAMETRES DU SCRIPT 


#chemin du fichier de données
if [ -f "$1" ]; then
    echo "Le fichier d'entré existe et se trouve à : $1"
else
    echo "Erreur : Le fichier $1 n'existe pas."
    affiche_aide
    echo "Durée du traitement : 0.0sec."
    exit 1
fi

#type de station à traiter   
if [ $2="hvb" ]
then
    colonne=1
elif [ $2="hva" ]
    colonne=2
elif [ $2="lv" ]
    colonne=3
else
    echo "Erreur : Option '$2' non reconnue. Les options valides sont 'hvb', 'hva' ou 'lv'."
    affiche_aide
    echo "Durée du traitement : 0.0sec."
    exit 1
fi
#type de consommateur à traiter
if [ $3="comp" ]
then
    consom=1
elif [ $3="indiv" ]
    consom=2
elif [ $3="all" ]
    consom=3
else
    echo "Erreur : Option '$3' non reconnue. Les options valides sont 'comp', 'indiv' ou 'all'."
    affiche_aide
    echo "Durée du traitement : 0.0sec."
    exit 1
fi

if [ $colonne = 1 ] || [ $colonne = 2 ] && [ $consom != "comp" ]
then
    echo "Erreur : Les options 'all' et 'indiv' ne sont pas autorisées pour les stations HVA et HVB."
    afiche_aide
    echo "Durée du traitement : 0.0sec."
    exit 2
fi

#script SHELL doit s’assurer que toutes les options obligatoires sont présentes
echo "Le fichier d'entrée est : $1"
echo "Choix de la station: $2"
echo "Choix de consommation : $3"



#FILTRAGE du fichier DATA_CWIRE.csv

if [ "$colonne" = 1 ]; then # Filtrer les lignes où la colonne 'HV-B Station' est remplie
    grep -E "^[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*" "$1" > tmp_filtrer.csv
elif [ "$colonne" = 2 ]; then 
    grep -E "^[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*" "$1" > tmp_filtrer.csv
elif [ "$colonne" = 3 ]; then 
    grep -E "^[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*" "$1" > tmp_filtrer.csv
fi

if [ "$consom" =1 ]; then # Filtrer les lignes où 'Company' est renseigné
    grep -E "^[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*" tmp_filtrer.csv > tmp_filtrer_comp.csv
elif [ "$consom" = 2 ]; then # Filtrer les lignes où 'Individual' est renseigné
    grep -E "^[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*;[^;]*" tmp_filtrer.csv > tmp_filtrer_indiv.csv
elif [ "$consom" = 3 ]; then # Conserver toutes les lignes (avec ou sans consommateur)  n'appliques aucun filtrage 
    cp tmp_filtrer.csv tmp_filtrer_all.csv
fi

rm tmp_filtrer.csv


ProgrammeC="./codeC/mon_programme"   #Variable contenant le nom de l'exécutable à vérifier.
FichierSource="./codeC/mon_programme.c"  #Variable contenant le chemin vers le fichier source C à compiler.
Makefile="./codeC/Makefile"  # Makefile pour la compilation

# Vérifier si l'exécutable existe
if [ ! -f "$ProgrammeC" ]; then
    echo "L'exécutable \"$ProgrammeC\" n'existe pas encore, on peut donc lancer la compilation..."
    
    if [ ! -d codeC ]; then
        echo "Erreur : dossier codeC introuvable. Compilation impossible"
        exit 1
    fi
    # Vérification de l'existence du fichier source avant la compilation
    if [ ! -f "$FichierSource" ]; then
        echo "Erreur : Le fichier source \"$FichierSource\" est introuvable. Compilation impossible"
        exit 1
    fi
    # Vérification de l'existence du Makefile avant la compilation
    if [ ! -f "$Makefile" ]; then
        echo "Erreur : Le make \"$Makefile\" est introuvable. Compilation impossible"
        exit 1
    fi
    
    # Lancer la compilation avec 'make' 
    cd ./codeC  # Aller dans le dossier contenant le Makefile
    make       # Lancer make pour compiler
    cd ..      # Revenir au dossier initial
    
    # Vérifier si la compilation a réussi
    if [ $? -ne 0 ]; then
        echo "Erreur : La compilation a échoué."
        exit 2
    fi
else
    echo "L'exécutable \"$ProgrammeC\" existe déjà, pas besoin de compliler."
fi



#Une fois la compilation effectuée le scrit pourra effectuer le traitement demandé en argument

# VERIFIER LA DUREE DU TRAITEMENT ne contient pas la phase de compilation

temps_depart=$(date +%s.%N)

# Traitement demandé en argument
echo "Calcul de la somme des consommateurs..."
./calcul_consommation "tmp_filtrer_comp.csv" "$consom" > tmp/somme_consommateurs.txt


temps_fin=$(date +%s.%N)
duree=`echo "$temps_fin - $temps_depart" | bc`
echo "Durée du traitement : ${duree}sec."


#Gestion du dosier tmp
if [ ! -d tmp ]; then
    mkdir tmp
else 
    cd tmp
    rm -f tmp/*
    cd ..
fi

if [ ! -d tests ]; then
    mkdir tests
fi



#Après avoir calculé les sommes des consommateurs, nous pouvons ajouter les résultats au fichier CSV de sortie et trier par capacité croissante.

nom_fichier="tmp/${2}_${3}.csv" #nom du fichier csv de sortie

case $2 in
    "hvb") opt_station="Station HVB" ;;
    "hva") opt_station="Station HVA" ;;
    "lv") opt_station="Station LV" ;;
esac

case $3 in
    "comp") opt_consom="Consommation (entreprises)" ;;
    "indiv") opt_consom="Consommation (particuliers)" ;;
    "all") opt_consom="Consommation (tous)" ;;
esac

echo "$opt_station:Capacité:$opt_consom" > "$nom_fichier"


#Ajouter la a valeur de capacité et la somme des consommateurs branchés directement au fichier final
while read ligne; do
    station=$(echo $ligne | cut -d' ' -f1)
    capacite=$(echo $ligne | cut -d' ' -f2)
    consommation=$(echo $ligne | cut -d' ' -f3)
    echo "$station:$capacite:$consommation" >> "$nom_fichier"
done < tmp/somme_consommateurs.txt

#Traitement spécial pour l'option lv_all
if [ "$opt_station" = "Station LV" ] && [ "$opt_consom" = "Consommation (tous)" ]; then
    # Calculer la différence entre capacité et consommation et trier par cette différence
    awk -F: '{ diff = $2 - $3; print $0 ":" diff }' "$nom_fichier" | sort -t':' -k4 -n > tmp/lv_trie.txt

    # Extraire les 10 postes avec la plus grande consommation et les 10 postes avec la moins grande consommation
    head -n 10 tmp/lv_trie.txt > tmp/10_plus.txt
    tail -n 10 tmp/lv_trie.txt > tmp/10_moins.txt

    # Combiner les résultats et les mettre dans un fichier final
    cat tmp/10_plus.txt tmp/10_moins.txt > tmp/lv_all_minmax.csv
fi

sort -t':' -k2 -n "$nom_fichier" -o "$nom_fichier" #trier par ordre capacité croissant


rm -f tmp_filtrer_comp.csv tmp_filtrer_indiv.csv tmp_filtrer_all.csv tmp/somme_consommateurs.txt
