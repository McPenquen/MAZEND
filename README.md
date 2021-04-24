# MAZEND
*Games Engineering Coursework*

### Getting started from git cloned repo
#### Update existing submodules
- `git submodule update --init --recursive`
#### Get Submodules
- `git submodule add https://github.com/SFML/SFML.git lib/sfml`
- `git submodule init`
- `git submodule update`

### Compiling with CMake
- Create a build folde outside of this folder
- Select this *MAZEND* folder as the *source code*
- Select the new created build folder as the *binaries*
- Compile for VS 2019
- Configure
- Configure a second time
- Generate
- Go to the build folder and open *MAZEND.sln* where you can continue developing
