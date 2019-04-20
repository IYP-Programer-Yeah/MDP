#!/bin/bash
Basedir="$(dirname "$(realpath "$0")")"
if [ -z ${Prefix+x} ];
then
	Prefix="$Basedir/../../Build"
fi

InstallPath="$Prefix/Include/MDP/"

mkdir "$InstallPath" -p
cp "$Basedir/../../../../../MDP/Include/." "$InstallPath" -r
