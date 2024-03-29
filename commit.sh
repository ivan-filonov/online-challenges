#!/bin/bash
if [ -z "$1" ];
then
echo need patch comment
exit
fi

MSG=$(echo $@)
PART=${PWD##*/}
if [ ${PART} == "online-challenges" ]; then
	PART="common"
fi
git commit -m "${PART}: ${MSG};" && git status
