#!/bin/bash

if [ "$1" == 'planck' ]; then
	qmk config user.keyboard=planck/rev6
elif [ "$1" == 'ctrl' ]; then
	qmk config user.keyboard=massdrop/ctrl
else
	echo "Usage: $0 [planck|ctrl]"
fi
