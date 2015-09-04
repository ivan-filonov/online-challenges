#!/bin/sh
if [ "$HOSTNAME" = vbox-gentoo ]; then
	rsync -av --delete-after /home/user/projects/online-challenges/ rsync://192.168.75.10/projects/online-challenges
#elif [ "$HOSTNAME" = vbox-gentoo-2 ]; then
#	rsync -av --delete-after rsync://192.168.75.11/projects/online-challenges /home/user/projects/online-challenges/
fi
