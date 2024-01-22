import numpy as np
import time

# тест для C & Linux pthreads
def test_c_pthread(expected_c):
    actual_c = np.loadtxt("c_pthreads_res.txt")
    actual_c = [int(item) for item in actual_c]
    if np.array_equal(actual_c, expected_c[0]):
        print("C & Linux pthreads - SUCCESS")
    else:
        print("C & Linux pthreads - FAIL")

# тест для C & MPI
def test_c_mpi(expected_c):
    actual_c = np.loadtxt("c_mpi_res.txt")
    actual_c = [int(item) for item in actual_c]
    if np.array_equal(actual_c, expected_c[0]):
        print("C & MPI - SUCCESS")
    else:
        print("C & MPI - FAIL")

# тест для Python & MPI
def test_py_mpi(expected_c):
    actual_c = np.loadtxt("py_mpi_res.txt")
    actual_c = [int(item) for item in actual_c]
    if np.array_equal(actual_c, expected_c[0]):
        print("Python & MPI - SUCCESS")
    else:
        print("Python & MPI - FAIL")

# тест для C & OpenMP
def test_c_openmp(expected_c):
    actual_c = np.loadtxt("c_openmp_res.txt")
    actual_c = [int(item) for item in actual_c]
    if np.array_equal(actual_c, expected_c[0]):
        print("C & OpenMP - SUCCESS")
    else:
        print("C & OpenMP - FAIL")


if __name__ == '__main__':
    array = []

    with open("input.txt") as f:
        _ = f.readline()
        array.append([int(x) for x in f.readline().split()])

    # start = time.time()
    expected_c = np.sort(array)
    # end = time.time()

    print("------- Tests -------")
    test_c_pthread(expected_c)
    test_c_mpi(expected_c)
    test_py_mpi(expected_c)
    test_c_openmp(expected_c)

    # elapsed = end - start
    # print(elapsed)

    f.close