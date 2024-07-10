#!/bin/bash

target=linux_cmd_cheat_sheeter
target_script=run_linux_cmd_cheat_sheeter.sh
install_dir=$HOME/.local/bin
mkdir -p $install_dir

echo "Installing linux cmd cheat sheeter..."
echo "Moving executable c file to $install_dir"
cp $target $install_dir
echo "Moving run script to $install_dir"
cp $target_script $install_dir


alias="alias cs='source run_linux_cmd_cheat_sheeter.sh'"

if [ -f "$HOME/.zshrc" ]; then
    echo "zshrc found"
    if ! grep -Fxq "$alias" ~/.zshrc; then
        echo "Not in zshrc. Adding"
        echo "$alias" >> ~/.zshrc
        echo "Installation successful. Run source ~/.zshrc to complete"
    else 
        echo "Alias already exists in ~/.zshrc"
        echo "Installation successful"
    fi
elif [ -f "$HOME/.bashrc" ]; then
    echo "bashrc found"
    if ! grep -Fxq "$alias" ~/.bashrc; then
        echo "Not in bashrc. Adding"
        echo "$alias" >> ~/.bashrc
        echo "Installation successful. Run source ~/.bashrc to complete"
     else 
        echo "Alias already exists in ~/.bashrc"
        echo "Installation successful"
    fi
else
    echo "Unknown shell. Exiting"
fi