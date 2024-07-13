# Linux Command Line Cheat Sheeter
This project will print out samples of common linux commands like
find, grep etc.

Available samples of specific commands will be printed out and navigable by arrow keys or shift-tab/tab.

Then after selection, the command is editable on all caret positions where input can be changed, like any path, flags, or search terms.

## Build
See the Makefile for details.

To build, simply run
````
make
````
In the current directory.

To install and use from anywhere (by typing ````cs````), run
````
make install
````

### History to current shell
After running this executable, the constructed command should be executed (if present) and in order to get the executed command added to history, a couple of tricks were required.

First of, the result of C executable will be written to a temporary file and not executed by C program itself. Then, this temporary file will be read by the run script (which is a wrapper to execute the script). This run script is sourced by the current shell which will execute the command and add it to current shell's history.

#### Install steps
The install step in makefile is defined by the [install.sh](install.sh) script.
This script will move both the executable and the [run_linux_cmd_cheat_sheeter.sh](run_linux_cmd_cheat_sheeter.sh) to $HOME/.local/bin.
It will also create alias in ~/.bashrc or ~/.zshrc which will be used to run the program.
This alias will source the run script and forward arguments.
