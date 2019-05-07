#! /bin/bash

#script para compilar y ejecutar el código

array=()
dist=0
res=9193186188426240000
more=1
while [[ $dist -ne $res ]] 
do
    while [[ more -eq 1 ]]
    do
        for j in {1..35}
        do
            echo -n "$(( ( RANDOM % 9 ) + 1 )) " >> inputs/input_n=35.txt
        done

        rng="$(cat inputs/input_n=35.txt)"

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

    g++ -g -std=c++0x main.cpp -o main && ./main 35
    if [ $? -ne 3 ]; then
        exit
    fi
    more=1
    rm inputs/input_n=35.txt
done
# g++ -g -std=c++0x main.cpp -o main && ./main
# rm main