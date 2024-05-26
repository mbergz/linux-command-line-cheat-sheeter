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
This will output an executable called 'cs'.

To use from anywhere, place in e.g. /usr/local/bin
````
sudo mv cs /usr/local/bin
````


