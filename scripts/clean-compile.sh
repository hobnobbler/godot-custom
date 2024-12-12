#!/usr/bin/env zsh


godot_branch="master"
app_name="godot-clipper2"

new_app_name="$app_name-$godot_branch"
dir_this_repo=$(cd "$(dirname "$0")/.." && pwd)
dir_build="$HOME/dev/build/$new_app_name"
dir_local_copy="$dir_build/../local-copy"
rm -rf "$dir_build"
if [[ -d "$dir_build" ]]; then
	echo "Error: Failed to delete build directory at '$dir_build'."
	exit 1
fi
mkdir -p "$dir_build"

figlet Clone Godot
if [[ ! -d "$dir_local_copy/godot" ]]; then
  git clone git@github.com:godotengine/godot.git "$dir_local_copy/godot"
fi
cp -R "$dir_local_copy/godot" "$dir_build/godot"
cd "$dir_build/godot" || exit
git checkout "$godot_branch"
git pull

figlet Clone clipper2
if [[ ! -d "$dir_local_copy/clipper2" ]]; then
  git clone git@github.com:AngusJohnson/Clipper2.git "$dir_local_copy/clipper2"
fi
cp -R "$dir_local_copy/clipper2" "$dir_build/clipper2"

figlet Copy Lib Files
mkdir -p "$dir_build/godot/modules/clipper2/Clipper2Lib"
cp -r "$dir_this_repo/godot-modules/clipper2/"* "$dir_build/godot/modules/clipper2"
cp -r "$dir_build/clipper2/CPP/Clipper2Lib/src" "$dir_build/godot/modules/clipper2/Clipper2Lib/src"
cp -r "$dir_build/clipper2/CPP/Clipper2Lib/include/clipper2" "$dir_build/godot/modules/clipper2/Clipper2Lib/src/clipper2"

# Build with mono enabled, but no bundle yet. (An app bundle is the form of the app that can be opened from the Applications folder.)
figlet Compile
scons platform=macos arch=arm64 dev_mode=yes target=editor module_mono_enabled=yes module_clipper2_enabled=yes
#scons platform=macos arch=arm64 dev_mode=yes target=editor module_mono_enabled=yes
if [[ $? -ne 0 ]]; then
	echo "Error: Command failed with a nonzero exit code."
	exit 1
fi
# Generate the mono glue. This is required to build the mono bundle.
figlet Generate Mono Glue
./bin/godot.macos.editor.arm64.mono --headless --generate-mono-glue modules/mono/glue
if [[ $? -ne 0 ]]; then
	echo "Error: Command failed with a nonzero exit code."
	exit 1
fi
#### Build Managed Libraries.
figlet Build Libs
./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin
if [[ $? -ne 0 ]]; then
	echo "Error: Command failed with a nonzero exit code."
	exit 1
fi
# Build bundle.
figlet Build App Bundle
scons platform=macos arch=arm64 dev_mode=yes target=editor module_mono_enabled=yes module_clipper2_enabled=yes generate_bundle=yes
#scons platform=macos arch=arm64 dev_mode=yes target=editor module_mono_enabled=yes generate_bundle=yes
if [[ $? -ne 0 ]]; then
	echo "Error: Command failed with a nonzero exit code."
	exit 1
fi

# Copy app to applications folder, overwriting possible existing one.
figlet Copy App
app_name=$(find "$dir_build/godot/bin/" -name "*.app" -type d -exec basename {} \;)
if [[ ! -d "$dir_build/godot/bin/$app_name" ]]; then
	echo "Error: App bundle not found at path '$dir_build/godot/bin/$app_name'"
	exit 1
fi

final_app_name=$app_name
if [[ -n "$new_app_name" ]]; then
	final_app_name="$new_app_name.app"
fi
max_retries=10
i=0

while [ $i -lt $max_retries ]; do
	if [ -n "$(pgrep -f "$final_app_name")" ]; then
		echo "$final_app_name is open. Attempting to close."
		pkill -f "$final_app_name"
		sleep 5
	else
		echo "$final_app_name has closed successfully."
		break
	fi
	i=$((i + 1))
done

if [ -n "$(pgrep -f "$final_app_name")" ]; then
	echo "Error: $final_app_name is still running after $max_retries attempts."
	exit 1
fi

rm -rf "/Applications/$final_app_name"
cp -rf "$dir_build/godot/bin/$app_name" "/Applications/$final_app_name"
open "/Applications/$final_app_name"