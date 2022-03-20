#!/usr/bin/env bash

path="."
layer=2147483647
flags=""
greps=""
sym=0
curLink=()
sorted=""
printed=()
paths=()


function compare {
	for ((i = 0; i < $2; ++i)); do
		if [[ $1 -ef  ${curLink[$i]} ]]; then
		
			return 1;
		fi 
	done	

	return 0
}

function checkPrintVal {
	for ((i = 1; i < $1; ++i)); do
		if [[ ${printed[$i]} = 0 ]]; then
			printed[$i]=0;
			local red2=$(seq -s "-" $((3 * $(($i - 1)) + 1)) | sed 's/[0-9]//g')
			echo "|$red2 ${paths[$i]}"
		fi
	done
}


function recursivePrintFile {
	
	if [ $2 = $layer ]; then
		return 
	fi
	
	
	local curFiles=($(ls $flags $1 | LC_ALL=C sort $sorted ))
	
	local red=$(seq -s "-" $((3 * $(($2 - 1)) + 1)) | sed 's/[0-9]//g')
	for file in ${curFiles[@]}; do
		
		if [ $file == "."  ] ||  [ $file == ".." ]; then
			continue
		fi
		
		if [[ ${file::0} = "/" ]]; then
			tmpFile="$file"
		else
			tmpFile="$1/$file"
		fi

		if [ -L "$tmpFile" ]; then
			linkInfo="`readlink $tmpFile`"
			if [ $sym = 1 ]; then
				echo "|$red $file -> $linkInfo"
			else
				compare "$tmpFile" $2
				
				if [ $? = 1 ]; then
					if [ $2 = $(($layer - 1)) ]; then
						echo "|$red $file"
					else
						echo "|$red $file (loop)"
					fi
				else
					echo "|$red $file"
					curLink[$2]="$tmpFile" 
					recursivePrintFile "$tmpFile" $(($2+1))
				fi
				
			fi
		
		
		else
			printed[$2]=0
			paths[$2]=$file
			
			if [ ${#greps} != 0 ]; then
				if [[ $file == *$greps* ]]; then
					checkPrintVal $2
					printed[$2]=1
					echo "|$red $file"
				fi
			else
				echo "|$red $file"
			fi

			
		
			if [ -d "$1/$file" ]; then
				curLink[$2]="$1/$file"
				
				recursivePrintFile "$1/$file" $(($2+1))		

			fi
		fi
	
	done
}



if [ $# -ge 2 ] && ([ ${#1} != 2 ] || [ ${1::1} != '-' ]); then
	path=$1
	shift
fi

while getopts "l:sari:?:" argv; do
	case $argv in
	l)
		layer=$(($OPTARG + 2));;
	r)
		sorted+=" -r";;
	a)
		flags+=" -a";;
	i)
		greps+="$OPTARG";;
	s)
		sym=1;;
	esac
done


echo $path
curLink[0]=$path

recursivePrintFile $path 1
