# godot-custom
Repo for building custom versions of Godot from source - contains code for custom modules and scripts for compiling Godot with external libraries.

# Usage

***Clone this repo and run the compilation script in `scripts` directory.***

# Compile Godot Mono from Source

## Prerequisites:
1. Clone Godot: \
```git clone git@github.com:godotengine/godot.git```
   - Check out version branch.  (You can build master, but that might cause unpredictable bugs.):\
   ```git checkout 4.4```<br><br>

1. Install mono:\
```brew install mono```

## Compile

1. **Build with mono enabled, but no bundle yet:**\
```scons platform=macos arch=arm64 dev_mode=yes target=editor module_mono_enabled=yes module_clipper2_enabled=yes```
    - (An app bundle is the form of the app that can be opened from the Applications folder.)<br><br>

1. **Generate the mono glue:**\
```<godot_binary> --headless --generate-mono-glue modules/mono/glue```
   - Should look something like this:\
   ```./bin/godot.macos.editor.arm64.mono  --headless --generate-mono-glue modules/mono/glue```
   - This is required to build the mono bundle.<br><br>

1. **Build Managed Libraries:**\
```./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin```<br><br>

1. **Build app bundle:**\
```scons platform=macos arch=arm64 dev_mode=yes target=editor module_mono_enabled=yes module_clipper2_enabled=yes generate_bundle=yes```
   - Add this arg if Godot doesn't automatically pick up the mono installation location:\
   ```mono_prefix=/opt/homebrew/Cellar/mono/6.12.0.206```
   - ^ (Be sure to use the actual path, not this one here.)<br><br>

1. **Copy app bundle to Applications folder:**\
   ```cp -f bin/godot_macos_editor_mono.app /Applications/godot_macos_editor_mono.app```<br><br>

1. **Start app and build project.**
   - The Godot .NET SDK version might need to be changed at the top of the csproj file.
     - For Godot 4.4, I had to change it from ```4.4.0-dev``` to ```4.4.0-dev.1```
   - C# libraries may need to be reinstalled. 
     - Example syntax: ```dotnet add package NetTopologySuite```
     - Execute package installation commands from the Godot project directory, not the Godot engine source repo.
