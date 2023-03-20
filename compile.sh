#!/bin/bash

# Colors
RED="\e[31m"
YELLOW='\e[33m'
GREEN='\e[32m'
NORMAL='\e[0m'


InstallRequirements() {
  echo -e "${RED}Install requirements${NORMAL}"
  sleep 1
  read -p "Do you want install Python3? (y/n) " answer
  case ${answer:0:1} in
      y|Y )
          sudo apt-get install python3
      ;;
      * )
      ;;
  esac
  read -p "Do you want install Boost? (y/n) " answer
    case ${answer:0:1} in
        y|Y )
            sudo apt-get install libboost-all-dev
        ;;
        * )
        ;;
    esac
  echo -e "${YELLOW}Install and create virtual environment ${NORMAL}"
  echo -e "${YELLOW}Install requirements ${NORMAL}"
  python3 -m pip install -r requirements.txt
  echo -e "${GREEN}Done ${NORMAL}"
}


Сompile() {
  mkdir -p build
  cd build || exit
  cmake ..
  cmake --build .
}


Clean() {
  echo -e "${YELLOW}Cleaning...${NORMAL}"
  if [ -d "build" ]; then rm -Rf build; fi
  if [ -d "venv" ]; then rm -Rf venv; fi
  if [ -f "integers_50M.txt" ]; then rm integers_50M.txt; fi
  echo -e "${GREEN}Done${NORMAL}"
}


Help() {
  echo "Usage: compile.sh [options]"
  echo "Options:"
  echo "  -h, --help"
  echo "  -c, --compile: Compile only"
  echo "  --clean: Clean compiled files and build directory"
  echo "  -r, --requirements: Install requirements"
}


if [ $# -eq 0 ]; then
  Help
  exit 1
fi


# Check if there is more than one argument
if [ $# -gt 1 ]; then
  echo -e "${RED}Error: Too many arguments${NORMAL}"
  Help
  exit 1
fi


# Check if the argument is -h or --help
if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
  Help
  exit 0
fi


# Check if the argument is -c or --compile
if [ "$1" = "-c" ] || [ "$1" = "--compile" ]; then
  Сompile
  exit 0
fi


# Check if the argument is -r or --requirements
if [ "$1" = "-r" ] || [ "$1" = "--requirements" ]; then
  InstallRequirements
  exit 0
fi


# Check if the argument is clean
if [ "$1" = "--clean" ]; then
  Clean
  exit 0
fi


# Check if the argument is not valid
if [[ ! " ${valid_args[*]} " == *" $1 "* ]]; then
  echo -e "${RED}Error: Invalid argument${NORMAL}"
  Help
  exit 1
fi