#!/bin/bash
Basedir="$(dirname "$(realpath "$0")")"
if [ -z ${Prefix+x} ];
then
	Prefix="$Basedir/../../Build"
fi

InstallPath="$Prefix/Include/"

mkdir "$InstallPath" -p
cp "$Basedir/../../../../../Dependencies/googletest/googletest/include/." "$InstallPath" -r
