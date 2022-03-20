#!/usr/bin/env bash

files=()
startTime=-1
endTime=-2
helpMsg="usage: ./p1.sh -s <Unix timestamp > -e <Unix timestamp > <input file >
ex: ./ p1.sh -s 1133642864 -e 1133643505 log1.txt log3.txt"
isInt='^[0-9]+$'

while getopts ":s:e:?" argv
do
	case $argv in
	s)
		startTime=$OPTARG;;
	e)
		endTime=$OPTARG;;
	:)
		echo $helpMsg;
		exit;;
	*)
		echo $helpMsg;
		exit;;
	?)
		echo $helpMsg;
		exit;;
	esac
done

shift "$(($OPTIND - 1))"

for var in $@; do
	files+=($var)
done 

if  [[ ! $startTime =~ $isInt ]] || [[ ! $endTime =~ $isInt ]] || [ $startTime -gt $endTime ] || [ $startTime = -1 ] || [ $endTime = -2 ] || [ ${#files[@]} -eq 0 ]; then
	echo $helpMsg
	exit	
fi

res=()

lineRegex="\[(.{24})\]\s(\[(notice|error)\])\s(.*)"

for file in ${files[@]}; do

	while IFS= read -r line; do
	
	if [[ $line =~ $lineRegex ]]; then 
		date="${BASH_REMATCH[1]}"
		typeOfNotify="${BASH_REMATCH[2]}"
		msg="${BASH_REMATCH[4]}"
	fi

	date=$(date -d "$date" +%s)
	
	if [ $date -ge $startTime ] && [ $date -le $endTime ]; then
		res+=("$date $typeOfNotify $msg")
	fi
			
	done < "$file"

done

# res=$(echo $res | xargs -n1 | sort | xargs)

IFS=$'\n' res=($(sort <<<"${res[*]}"))

for line in ${res[@]}; do
	echo $line
done

