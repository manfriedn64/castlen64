#!/usr/bin/python3
import sys, os, numpy as np

assert(len(sys.argv) > 1)
assert(os.path.isfile(sys.argv[1]))
    
filesplit = os.path.split(sys.argv[1])
folder = filesplit[0]
filename = os.path.splitext(filesplit[1])[0]
file_out = os.path.join(folder, filename+'.blv')

with open(sys.argv[1], 'r') as csvfile:
    lines = csvfile.readlines()
    height = len(lines)
    width = len(lines[0].replace("\n", "").replace("\r", ""))
    level =  np.ones(width*height+2, dtype=np.uint8)
    level[0] = width
    level[1] = height
    cpt=0
    for line in lines:
        line = line.replace("\n", "").replace("\r", "")
        for char in range(0,width):
            position = cpt * width + char
            #print(str(position))
            #print(line[char])
            if line[char] == " ":
                value = 11
            elif line[char] == "t": # torch
                value = 57
            elif line[char] == "w": # walltorch
                value = 58
            elif line[char] == "c": # chandelier
                value = 49
            elif line[char] == "b": # barrel
                value = 46
            elif line[char] == "f": # small flag
                value = 51
            elif line[char] == "g": # big flag
                value = 52
            elif line[char] == "p": # pot
                value = 53
            elif line[char] == "♣": # plant in pot
                value = 55

            elif line[char] == "▓":
                value = 34
                if cpt > 1 and level[position - width + 2] in (34, 57, 51):
                    value = 33
            elif line[char] == "▒":
                value = 35
            elif line[char] == "X":
                value = 13
            elif line[char] == "┌":
                value = 16
            elif line[char] == "┬":
                value = 31
            elif line[char] == "┐":
                value = 17
            elif line[char] == "├":
                value = 28
            elif line[char] == "┼":
                value = 18
            elif line[char] == "┤":
                value = 30
            elif line[char] == "└":
                value = 14
            elif line[char] == "┴":
                value = 27
            elif line[char] == "┘":
                value = 15
            elif line[char] == "│":
                if cpt == 0 or level[position - width + 2] not in [32, 31, 18, 28, 22, 16, 17, 29, 30]:
                    print(str(cpt)+'x'+str(char))
                    value = 29
                elif cpt == height - 1:
                    value = 19
                else:
                    value = 32
            elif line[char] == "─":
                value = 20
                if char > 0     and line[char-1] not in ["─", "┴", "┬", "┼", "├", "┌", "└"]:
                    value = 21
                if char < width and line[char+1] not in ["─", "┴", "┬", "┼", "┤", "┘", "┐"]:
                    value = 22
            elif line[char] == "▼":
                value = 9
            elif line[char] == "▲":
                value = 23
            elif line[char] == "~":
                value = 36
            elif line[char] == "¤":
                value = 37
            elif line[char] == "¶":
                value = 38
                if cpt > 0 and level[position - width + 2] == 33:
                    value = 39
            else:
                value = 11
            if cpt > 1 and line[char] not in ["│", "┴", "┼", "├", "┤", "┘", "└"] and level[position - width + 2] == 32:
                level[position - width + 2] = 19
            #print(str(position)+ ' = '+str(value))
            level[position+2] = value
        cpt += 1
    print(str(level[2+18]))
    level.byteswap().tofile(file_out)

