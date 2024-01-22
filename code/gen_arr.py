#!/Users/vladaefremenko/miniconda3/bin/python3
# Use: ./gen_arr.py <length> <file_name>

import random
import sys
import numpy as np

if len(sys.argv) != 3:
    print ("Usage: ", sys.argv[0], " <length> <output_file>")
    exit()

f = open(sys.argv[2], 'w')

l = sys.argv[1]

f.write(l + "\n")
l = int(l)

array = []

for i in range(l):
    r = random.randrange(100000)
    f.write(str(r) + " ")
    array.append(r)

f.close()

np.save('input.npy', array)

print("Сгенерирован массив на", l, "элементов")