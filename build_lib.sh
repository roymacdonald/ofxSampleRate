#!/bin/bash

CYAN='\033[1;36m'
RED='\033[1;31m'
YELL='\033[1;33m'
NC='\033[0m'

check_make_dir(){
	if [ ! -d "$1" ]; then
		mkdir "$1"
	fi
}

copy_lib_to(){
	check_make_dir libs/libsamplerate/lib/"$1"
	cp libsamplerate/build/install/lib/"$2" libs/libsamplerate/lib/"$1"/	
	echo "copying libsamplerate/build/install/lib/$2 -> libs/libsamplerate/lib/$1/$2"
}

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR	


if ! command -v cmake &>/dev/null; then
      	echo -e "${RED}     You need to install CMake in order to compile whisper.cpp.     ${NC}"
	echo -e "${RED}     Download the installer from https://cmake.org/download/      ${NC}"
	echo -e "${RED}     On windows, make sure you use the MSI installer rather than the zip      ${NC}"
	echo -e "${RED}     and choose \"Add CMake to the system PATH for all users\"      ${NC}"
	echo -e "${RED}     Exiting now!      ${NC}"
	exit 1
fi

echo -e "${YELL}Cloning libsamplerate repository${NC}"

git clone --depth 1  https://github.com/libsndfile/libsamplerate.git

cd libsamplerate


echo -e "${YELL}Building libsamplerate${NC}"

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF -DLIBSAMPLERATE_EXAMPLES=OFF -DLIBSAMPLERATE_INSTALL=ON -DCMAKE_INSTALL_PREFIX=./install ..
make
make install

if [ $? -ne 0 ];then
	echo -e "${YELL}BUilding libsamplerate FAILED!. Exiting${NC}"
	exit 1
fi

echo -e "${YELL}Libsamplerate built successfully${NC}"

echo -e "${YELL}Copying files into addon${NC}"

cd $SCRIPT_DIR

check_make_dir libs/
check_make_dir libs/libsamplerate/
check_make_dir libs/libsamplerate/include/
check_make_dir libs/libsamplerate/lib/

cp libsamplerate/build/install/include/samplerate.h libs/libsamplerate/include/
if [[ "$OSTYPE" =~ ^msys ]]; then
	copy_lib_to vs libsamplerate.lib
	# check_make_dir libs/libsamplerate/lib/vs/
	# cp libsamplerate/build/install/lib/libsamplerate.lib libs/libsamplerate/lib/vs/
fi	
if [[ "$OSTYPE" =~ ^darwin ]]; then
	copy_lib_to osx libsamplerate.a
	# check_make_dir libs/libsamplerate/lib/osx/
	# cp libsamplerate/build/install/lib/libsamplerate.a libs/libsamplerate/lib/osx/
fi

if [[ "$OSTYPE" =~ ^linux ]]; then
	if [[ "$HOSTTYPE" =~ ^x86_64 ]]; then
		copy_lib_to linux64 libsamplerate.a
		# check_make_dir libs/libsamplerate/lib/linux64/
		# cp libsamplerate/build/install/lib/libsamplerate.a libs/libsamplerate/lib/linux64/
	else
		copy_lib_to linux libsamplerate.a
		# check_make_dir libs/libsamplerate/lib/linux/
		# cp libsamplerate/build/install/lib/libsamplerate.a libs/libsamplerate/lib/linux/
	fi
fi


if [ $? -ne 0 ];then
	echo -e "${YELL}Installing libsamplerate FAILED!. Exiting${NC}"
	exit 1
fi

echo -e "${YELL}Removing libsamplerate build files${NC}"
rm -rf libsamplerate

echo -e "${YELL}Libsamplerate installed successfully${NC}"


