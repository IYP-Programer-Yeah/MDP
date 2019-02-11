Basedir="$(dirname "$(realpath "$0")")"
if [ -z ${var+x} ];
then
	Prefix="$Basedir/../../Build"
fi

InstallPath="$Prefix/Include/MDP/"

mkdir "$InstallPath" -p
cp "$Basedir/../../../../../Messenger/Include/." "$InstallPath" -r
