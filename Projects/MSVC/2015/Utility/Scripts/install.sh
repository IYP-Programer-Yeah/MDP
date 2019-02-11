Basedir="$(dirname "$(realpath "$0")")"
if [ -z ${Prefix+x} ];
then
	export Prefix="$Basedir/../../Build"
fi
./../../../../../Dependencies/Utility/Projects/MSVC/2015/Utility/Scripts/install.sh