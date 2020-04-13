#!/bin/bash

#Reset Terminal to parallel attributes
echo -n -e "\x1f\x2f\x42" 

#You best start with a command to set the position
echo -n -e "\x1fAA Cursorposition: 0x1f YX (Y,X=Zeichenposition von A-)" 

#0x18 deletes the rest of the line


#Colors can be set with 0x80-0x87 and 0x90-0x97
echo -n -e "\x1fBAForeground: \x81 81 \x82 82 \x83 83 \x84 84 \x85 85 \x86 86 \x87 87"
echo -n -e "\x1fCABackground: \x91 91 \x92 92 \x93 93 \x94 94 \x95 95 \x96 96 \x97 97 \x90"

#BTX supports 4 text sizes

echo -n -e "\x1fDAGroessen: 0x8c\x8d0x8d\x8e0x8e\x8f0x8f\x8c\x18"

#You can delete to the end of the line or the whole screen
echo -n -e "\x1fEA 0x18 loescht zum Ende der Zeile  0x0c loescht den ganzen Schirm \x18" 

