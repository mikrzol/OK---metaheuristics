import subprocess
import sys
import winsound
import argparse

frequency = 250  # Set Frequency To 2500 Hertz
duration = 500  # Set Duration To 1000 ms == 1 second

open('results.txt', 'w').close()

# argv[nazwa programu, ile razy zrobić program, wielkość populacji dla GA]
for i in range(0, int(sys.argv[1])):
    in_file = open("test.txt")
    out_file = open("results.txt", "a")
    process = subprocess.run(f'main_v2.exe {sys.argv[2]}')
    scores = [[], []]
    mode = 0

    for line in in_file:
        if(line.startswith("CHILDREN")):
            mode = 1
        if(line.startswith("S")):
            scores[mode].append(int(line.split()[2]))
    out_file.write(f"{i}\nPARENTS MIN = {min(scores[0])}\nCHILDREN MIN = {min(scores[1])}\n")
    out_file.write("========================================================================\n")
    in_file.close()
    out_file.close()

winsound.Beep(frequency, duration)