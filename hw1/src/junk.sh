#!/bin/bash
################################################################################
# junk.sh
################################################################################

help_flag=0
list_flag=0
purge_flag=0

file_path="$HOME/cs3157/hw1/src/junk.sh"

help_display() {
cat<< HELP
Usage: $(basename "$file_path") [-hlp] [list of files]
     -h: Display help.
     -l: List junked files.
     -p: Purge all files.
     [list of files] with no other arguments to junk those files.
HELP
}

while getopts ":hlp" option; do
    case "$option" in
       h) if [ $list_flag = 1 ] || [ $purge_flag = 1 ]; then
	       printf "Error: Too many options enabled." >&2
	       help_display
	  fi
	  help_flag=1
          ;;
       l) if [ $help_flag = 1 ] || [ $purge_flag = 1 ]; then
	  	printf "Error: Too many options enabled." >&2
		help_display
	  fi
	  list_flag=1
          ;;
       p) if [ $list_flag = 1 ] || [ $help_flag = 1 ]; then
          	printf "Error: Too many options enabled." >&2
		help_display
	  fi
	  purge_flag=1 #spacing weird		  
  	  ;;
       ?) printf "Error: Unknown option '-%s'.\n" "$OPTARG" >&2
	  help_display     
          exit 1
          ;;
    esac
done

if [[ $(($help_flag + $list_flag + $purge_flag)) -gt 0 ]];then	
	shift $((OPTIND-1))
	if [ $# = 1 ]; then
		printf "Error: Too many options enabled." >&2
		help_display
		exit 1
	fi
fi
#create junk directory if it does not exist
if [ ! -d "$HOME/.junk" ];then
	echo "doesnt exist"
	mkdir "$HOME"/.junk
fi

if [ $help_flag = 1 ];then
	help_display
	exit 0
fi

if [ $list_flag = 1 ];then
	ls -lAf ~/.junk
	exit 0
fi

if [ $purge_flag = 1 ];then
	find $HOME/.junk -type f -delete
	exit 0
fi

for file in $*; do
	if [ -f "$file" ];then
		mv "$file" ~/.junk
        else 
 		printf "Warning: $file not found" >&2
	fi
done 
exit 0		

