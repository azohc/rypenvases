#! /bin/bash

#script para compilar y ejecutar el código
n=18
array=()
more=1
rm "inputs/input_n=$n.txt"

while true
do
    while [[ more -eq 1 ]]
    do
        for j in {1..18}
        do
            echo -n "$(( ( RANDOM % 10 ) + 1 )) " >> "inputs/input_n=$n.txt"
        done

        rng="$(cat inputs/input_n=$n.txt)"

        contained=0
        for item in "${array[@]}"; do   #check if present
            [[ $rng == "$item" ]] && echo "$rng present in the array" && contained=1 && dist+=1
        done

        if [[ $contained -eq 0 ]]; then         # if rng is not in list
            array=("${array[@]}" "$rng") # add to list
            more=0        # dont gen more
            echo "$rng added to list" 
            # print list
            for item in "${array[@]}"; do  
               echo $item
            done
        fi
    done

    g++ -g -std=c++0x main.cpp -o main 
    ./main 18 1
    res1=$?
    ./main 18 2
    res2=$?

    if [ $res1 -ne $res2 ]; then
        exit
    fi
    more=1
    rm "inputs/input_n=$n.txt"
done
# g++ -g -std=c++0x main.cpp -o main && ./main
# rm main