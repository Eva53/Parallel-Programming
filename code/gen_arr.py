import sys
import numpy as np

def generate_and_save_array(size):
    array = np.random.randint(0, 100000, size)

    with open('input.txt', 'w') as txt_file:
        txt_file.write(str(size) + '\n')
        txt_file.write(' '.join(map(str, array)))

    np.save('input.npy', array)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ./gen_arr.py <array_size>")
        sys.exit(1)

    try:
        array_size = int(sys.argv[1])
    except ValueError:
        print("Error: array size must be an integer")
        sys.exit(1)

    generate_and_save_array(array_size)
    print(f"Сгенерирован массив на {array_size} элементов")
