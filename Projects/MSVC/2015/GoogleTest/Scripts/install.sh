Basedir="$(dirname "$(realpath "$0")")"
if [ -z ${var+x} ];
then
	Prefix="$Basedir/../../Build"
fi

InstallPath="$Prefix/Include/"

mkdir "$InstallPath" -p
cp "$Basedir/../../../../../Dependencies/googletest/googletest/include/." "$InstallPath" -r
