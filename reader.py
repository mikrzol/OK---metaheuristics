import subprocess
import sys
import winsound
import argparse

frequency = 550  # Set Frequency To 2500 Hertz
duration = 1000  # Set Duration To 1000 ms == 1 second

open('results.txt', 'w').close()

# argv[nazwa programu, ile razy zrobić program, wielkość populacji dla GA]
for i in range(0, int(sys.argv[1])):
    process = subprocess.run(f'main_v3.exe {sys.argv[2]}')
    in_file = open("log.txt")
    out_file = open("results.txt", "a")
    scores = [[], []]
    mode = 0

    for line in in_file:
        if(line.startswith("CHILDREN")):
            mode = 1
        if(line.startswith("S:") and mode == 1):
            path = set(in_file.readline().rstrip().split())
            if(len(path) < 100):
                print("DUPLICATE ELEMENTS FOUND IN CHILDREN!!!")
                print(path)
        if(line.startswith("SCORE")):
            scores[mode].append(int(line.split()[2]))
    out_file.write(f"{i+1}\nPARENTS AVERAGE = {sum(scores[0])/len(scores[0]):.2f}\nCHILDREN AVERAGE = {sum(scores[1])/len(scores[1]):.2f}\n")
    out_file.write("========================================================================\n")
    in_file.close()
    out_file.close()

winsound.Beep(frequency, duration)
winsound.Beep(frequency, duration)
winsound.Beep(frequency, duration)