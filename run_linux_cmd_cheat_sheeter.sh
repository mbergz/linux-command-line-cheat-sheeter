tmp_cs_file="/tmp/cs_command_output.txt"

# Run executable with forwarded flags which will write to tmp_cs_file
linux_cmd_cheat_sheeter "$@"

if [ $? -ne 0 ]; then
   return
fi

function append_to_history() {
   if [ -n "$ZSH_VERSION" ]; then
      print -s "$1"
   elif [ -n "$BASH_VERSION" ]; then
      history -s "$1"
   else
      echo "Unknown shell. Exiting"
   fi
}

# If command was chosen and written to file, let user exeute it and save to history
if [ -s $tmp_cs_file ]; then
   file_command=$(<"$tmp_cs_file")
   echo "$file_command"

   echo -e "\033[31mExecute? [y or enter]\033[0m"
   read userInput
   if [[ -z "$userInput" || "$userInput" == "y" || "$userInput" == "Y" ]]; then
      eval $file_command
      append_to_history "$file_command"
   fi
fi

