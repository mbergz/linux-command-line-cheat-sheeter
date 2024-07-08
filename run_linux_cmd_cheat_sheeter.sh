#!/bin/bash

tmp_cs_file="/tmp/cs_command_output.txt"

# Run executable with forwarded flags which will write to tmp_cs_file
linux_cmd_cheat_sheeter "$@"

# If command was chosen and written to file, let user exeute it and save to history
if [ -s $tmp_cs_file ]; then
   file_command=$(<"$tmp_cs_file")
   echo "Command is \"$file_command\""

   echo -e "\033[31mExecute? [y or enter]\033[0m\n"
   read userInput
   if [[ -z "$userInput" || "$userInput" == "y" || "$userInput" == "Y" ]]; then
      eval $file_command
      history -s "$file_command"
   fi
   
fi

