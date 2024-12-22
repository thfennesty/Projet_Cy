#!/bin/bash

function display_help {
    for i in $@
    do
        if [ "$i" = "-h" ]
        then
            echo "================== Welcome to the help page ===================="
            echo
            echo "We are here to assist you in analyzing various stations (HV-A stations, HV-B stations, LV posts), to determine whether they are in a state of overproduction or underproduction of energy, as well as to evaluate what proportion of their energy is consumed by companies and individuals."
            echo 
            echo "To achieve this, you simply need to enter three things, with one or more options at each step:
            
                1. The file path 
                        option: Enter the input file path.
                        
                Define your observation parameters:
                
                2. The type of station to analyze ''
                        options: 'hvb','hva' or 'lv'.
                3. The client category to examine consumption ''
                        options: companies 'comp', individuals 'indiv', or all 'all'."
            echo
            echo "Note: HVB and HVA cannot be associated with the 'indiv' or 'all' options!
            
                You can return to this help page by entering -h as an argument."
            echo 
            echo "===================================================================="
            exit 0
        fi
    done
}

# Check if the input file path is provided
if [ -z "$1" ]; then
    echo "Error: No input file path provided."
    display_help
    exit 1
fi

# Check if the file exists and copy it to the input directory
if [ ! -f "$1" ]; then
    echo "Error: The file '$1' does not exist."
    display_help
    exit 1
fi

echo "Input file path: $1"

while [ ! -d input ] ; do
    mkdir input
done
cp "$1" input/input_file_copy.csv
echo "The input file has been copied to this location: input/input_file_copy.csv"

# Set the column number based on station type
if [ "$2" = "hvb" ]; then
    column=1
elif [ "$2" = "hva" ]; then
    column=2
elif [ "$2" = "lv" ]; then
    column=3
else
    echo "Error : Option '$2' not recognized. Valid options are 'hvb', 'hva' or 'lv'."
    display_help
    exit 1
fi

# Set the consumption type
if [ "$3" = "comp" ]; then
    consumption=1
elif [ "$3" = "indiv" ]; then
    consumption=2
elif [ "$3" = "all" ]; then
    consumption=3
else
    echo "Error : Option '$3' not recognized. Valid options are 'comp', 'indiv' or 'all'."
    display_help
    exit 1
fi

# Error check for invalid combinations
if [ $column -eq 1 ] || [ $column -eq 2 ] && [ $consumption -ne 1 ]; then
    echo "Error : options 'all' and 'indiv' cannot be combined with HVB or HVA stations."
    display_help
    exit 2
fi

echo "Final parameters: Input file is $1, Station choice: $2, Consumption choice: $3"

# Files management
if [ ! -d tmp ]; then
    mkdir tmp
else 
    rm -f tmp/*
fi

# Filter of the csv file

FILTERED_FILE="tmp/filtered_data.csv"

awk -F ";" -v station="$2" -v consumer="$3" '
BEGIN { OFS = ":" }
{
    # Filter data based on station and consumer type
    if (station == "hvb" && consumer == "comp" && $2 != "-") {
        print $2, $7, $8
    } else if (station == "hva" && consumer == "comp" && $3 != "-") {
        print $3, $7, $8
    } else if (station == "lv" && consumer == "comp" && $4 != "-" && ($5 != "-" || $7 != "-")) {
        print $4, $7, $8
    } else if (station == "lv" && consumer == "indiv" && $4 != "-" && ($6 != "-" || $7 != "-")) {
        print $4, $7, $8
    } else if (station == "lv" && consumer == "all" && $4 != "-") {
        print $4, $7, $8
    }
}' "$1" > "$FILTERED_FILE"

if [[ ! -s "$FILTERED_FILE" ]]; then
    echo "Erreur : No data found matching filters."
    exit 1
fi

# Add the number of lines in the first line
line_count=$(wc -l < "$FILTERED_FILE")
sed -i "1i$line_count" "$FILTERED_FILE"

# Switch to C program compilation management
ProgramC="./main"  
FileSource="./main.c"  
Makefile="./Makefile"

if [ ! -f "$ProgramC" ]; then
    echo "The executable '$ProgramC' does not yet exist, so we can start compiling..."

    if [ ! -d codeC ]; then
        echo "Error: codeC folder not found. Compilation impossible"
        exit 1
    fi
    if [ ! -f "$FileSource" ]; then
        echo "Error: The source file '$FileSource' cannot be found. Compilation impossible"
        exit 1
    fi
    if [ ! -f "$Makefile" ]; then
        echo "Error: The makefile '$Makefile' cannot be found. Compilation impossible"
        exit 1
    fi

    # Start compilation with 'make'
    cd ./codeC  
    make
    cd ..      

    if [ $? -ne 0 ]; then
        echo "Error: Compilation failed."
        exit 2
    fi
else
    echo "The '$ProgramC' executable already exists, no need to compile it."
fi

start_time=$(date +%s.%N)


echo "Calculating the sum of consumers..."
./main "$FILTERED_FILE" > tmp/sum_consumers.txt 

end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "Length of treatment: ${duration}sec."

# After calculating the consumer sums, we can add the results to the output CSV file and sort by increasing capacity.
output_name="tmp/${2}_${3}.csv"

case $2 in
    "hvb") opt_station="Station HVB" ;;
    "hva") opt_station="Station HVA" ;;
    "lv") opt_station="Station LV" ;;
esac

case $3 in
    "comp") opt_consump="Consommation (entreprises)" ;;
    "indiv") opt_consump="Consommation (particuliers)" ;;
    "all") opt_consump="Consommation (tous)" ;;
esac

echo "$opt_station:Capacité:$opt_consump" > "$output_name"

# Add capacity value and sum of connected consumers directly to final file
while read ligne; do
    station=$(echo $ligne | cut -d' ' -f1)
    capacity=$(echo $ligne | cut -d' ' -f2)
    consump=$(echo $ligne | cut -d' ' -f3)
    echo "$station:$capacity:$consump" >> "$output_name"
done < tmp/sum_consumers.txt

sort -t':' -k2 -n "$output_name" -o "$output_name" # sort by increasing capacity

if [ ! -d tests ]; then
    mkdir tests
fi 
cat $output_name >> tests/final_output_file.csv

# Special treatment for the lv_all options
if [ "$output_name" = "tmp/lv_all.csv" ] ; then
    awk -F: '{ diff = $2 - $3; print $0 ":" diff }' "$output_name" | sort -t':' -k4 -n > tmp/lv_trie.txt
    head -n 10 tmp/lv_trie.txt > tmp/10_plus.txt
    tail -n 10 tmp/lv_trie.txt > tmp/10_moins.txt
    cat tmp/10_plus.txt tmp/10_moins.txt > tmp/lv_all_minmax.csv
    cat tmp/lv_all_minmax.csv >> tests/final_output_file.csv
fi
