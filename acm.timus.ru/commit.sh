#!/bin/bash
git add -A

if [ -z "$1" ];
then
echo need patch comment
exit
fi

MSG=$(echo $@)
git commit -m "acm.timus.ru: ${MSG};"
