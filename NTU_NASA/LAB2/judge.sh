#! /usr/bin/env bash

# Usage: 
# ./judge PROBLEM_ID C_FILE

# TODO: retrieve the arguments
# Note that you can use the variable name you like :)
# But please do not modify the output messages
prob_id=$1
c_file=$2

echo "------ JudgeGuest ------"
echo "Problem ID: ${prob_id}"
echo "Test on: ${c_file}"
echo "------------------------"

# TODO: create a temporary directory in current directory
# Hint: `man mktemp`
tmpDIR=$(mktemp -d -p ./)
cd ${tmpDIR}
# TODO: compile the source C code, the output executable,
# should output to your temporary directory
gcc -s ../${c_file} -o a &>/dev/null
# TODO: check the compile status
if [ $? -ne 0 ]; then
    echo "Compile Error"
    cd ..
	rm -r ${tmpDIR}
	exit
fi

# TODO: run a loop, keep trying download testdatas
i=0
while true; do

    # TODO: download input and output file
	wget -q --no-check-certificate https://judgegirl.csie.org/downloads/testdata/${prob_id}/${i}.{in,out} 
    # TODO: `break` if get a 404
    if [ $? -ne 0 ]; then
        break
    fi

    # TODO: execute with the input file
	timeout 1 ./a < ${i}.in > res
	
	tle=$?
	
	if [ $tle -eq 0 ]; then
		diff res ${i}.out &>/dev/null
	fi
	
	ac=$?
		

    # TODO: finish those conditions, ${i} is the testcase number (zero-based)
    if [ $tle -ne 0 ]; then
        echo -e "${i}\tTime Limit Exceeded"
    elif [ $ac -eq 0 ]; then
        echo -e "${i}\tAccepted"
    else
        echo -e "${i}\tWrong Answer"
    fi

    i=$((i+1))
done

# TODO: remove the temporary directory
cd ..
rm -r ${tmpDIR}
