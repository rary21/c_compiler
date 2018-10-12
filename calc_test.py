import fileinput
import sys

for line in fileinput.input():
    line = line.translate(None, ';')
    sys.stdout.write(str(eval(line)) + '\n')