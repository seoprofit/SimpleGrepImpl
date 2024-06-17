#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
" sdsa grep_test.txt VAR"
)


testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "    $FAIL   $SUCCESS    $COUNTER  success grep $t"
    else
      (( FAIL++ ))
      echo "    $FAIL   $SUCCESS     $COUNTER fail grep $t"
    fi
    rm test_s21_grep.log test_grep.log
}


for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done


for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done


for var1 in v c l n h 
do
    for var2 in v c l n h 
    do
        for var3 in v c l n h 
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done


for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done



echo "  FAIL: $FAIL "
echo "     SUCCESS: $SUCCESS "
echo "ALL: $COUNTER"
