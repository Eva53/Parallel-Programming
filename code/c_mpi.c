#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Перестановка двух чисел
void swap(int* arr, int i, int j)
{
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

// Сортировка для массива arr[] с индексом начала start и концом end
void quicksort(int* arr, int start, int end)
{
    int pivot, index;

    if (end <= 1)
        return;

    pivot = arr[start + end / 2]; // Выбор pivot - среднего элемента
    swap(arr, start, start + end / 2); // Перестановка местами с первым

    // Индекс разделения
    index = start;

    // Выполнение итерации по диапазону [start, end]
    for (int i = start + 1; i < start + end; i++) {
        // Перестановка местами, если элемент меньше, чем элемент pivot
        if (arr[i] < pivot) {
            index++;
            swap(arr, i, index);
        }
    }

    // Установка pivot на место
    swap(arr, start, index);

    // Рекурсивный вызов для подразделов массива
    quicksort(arr, start, index - start);
    quicksort(arr, index + 1, start + end - index - 1);
}

// Объединение двух массивов
int* merge(int* arr1, int n1, int* arr2, int n2)
{
    int* result = (int*)malloc((n1 + n2) * sizeof(int));
    int i = 0;
    int j = 0;
    int k;

    for (k = 0; k < n1 + n2; k++) {
        if (i >= n1) {
            result[k] = arr2[j];
            j++;
        }
        else if (j >= n2) {
            result[k] = arr1[i];
            i++;
        }
        // Индексы в границах, поскольку i < n1 && j < n2
        else if (arr1[i] < arr2[j]) {
            result[k] = arr1[i];
            i++;
        }
        else {
            result[k] = arr2[j];
            j++;
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    int number_of_elements;
    int* data = NULL;
    int chunk_size, own_chunk_size;
    int* chunk;
    FILE* file = NULL;
    double time_taken;

    MPI_Status status;

    if (argc != 3) {
        printf("ERROR: needs 2 files: 'input.txt' and 'c_mpi_res.txt'\n");
        exit(-1);
    }

    int number_of_process, rank_of_process;
    int rc = MPI_Init(&argc, &argv); // Инициализация MPI

    if (rc != MPI_SUCCESS) {
        printf("ERROR: creating MPI program is failed\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &number_of_process); // Определение числа процессов
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_of_process); // Определение номера процесса

    // Главный процесс
    if (rank_of_process == 0) {
        file = fopen(argv[1], "r");

        if (file == NULL) {
            printf("ERROR: file wasn't opened\n");
            exit(-1);
        }

        fscanf(file, "%d", &number_of_elements);

        // Размер блока
        chunk_size = (number_of_elements % number_of_process == 0) ?
            (number_of_elements / number_of_process) :
            (number_of_elements / (number_of_process - 1));

        data = (int*)malloc(number_of_process * chunk_size * sizeof(int));

        // Чтение массива из файла
        for (int i = 0; i < number_of_elements; i++) {
            fscanf(file, "%d", &data[i]);
        }

        // Заполнение data нулями
        for (int i = number_of_elements; i < number_of_process * chunk_size; i++) {
            data[i] = 0;
        }

        fclose(file);
        file = NULL;
    }

    // Блокировка всего процесса до достижения этой точки (синхронизация)
    MPI_Barrier(MPI_COMM_WORLD);

    // Запуск таймера
    time_taken -= MPI_Wtime();

    // Передача размера всем процессам из корневого процесса
    MPI_Bcast(&number_of_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Размер блока
    chunk_size = (number_of_elements % number_of_process == 0) ?
        (number_of_elements / number_of_process) :
        (number_of_elements / (number_of_process - 1));

    // Общий размер блока
    chunk = (int*)malloc(chunk_size * sizeof(int));

    // Распределение данных о размере патрона по всему процессу
    MPI_Scatter(data, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    free(data);
    data = NULL;

    // Размер собственного блока
    own_chunk_size = (number_of_elements >= chunk_size * (rank_of_process + 1)) ?
        chunk_size : (number_of_elements - chunk_size * rank_of_process);

    //  Сортировка массива для каждого фрагмента, вызванного процессом
    quicksort(chunk, 0, own_chunk_size);

    // Объединение фрагментов
    for (int step = 1; step < number_of_process; step = 2 * step) {
        if (rank_of_process % (2 * step) != 0) {
            MPI_Send(chunk, own_chunk_size, MPI_INT, rank_of_process - step, 0, MPI_COMM_WORLD);
            break;
        }

        if (rank_of_process + step < number_of_process) {
            int received_chunk_size = (number_of_elements >= chunk_size * (rank_of_process + 2 * step))
                ? (chunk_size * step)
                : (number_of_elements - chunk_size * (rank_of_process + step));

            int* chunk_received;
            chunk_received = (int*)malloc(received_chunk_size * sizeof(int));
            MPI_Recv(chunk_received, received_chunk_size,
                MPI_INT, rank_of_process + step, 0,
                MPI_COMM_WORLD, &status);

            data = merge(chunk, own_chunk_size, chunk_received, received_chunk_size);

            free(chunk);
            free(chunk_received);
            chunk = data;
            own_chunk_size = own_chunk_size + received_chunk_size;
        }
    }

    // Остановка таймера
    time_taken += MPI_Wtime();

    // Запись результата в файл
    if (rank_of_process == 0) {
        file = fopen(argv[2], "w+b");

        if (file == NULL) {
            printf("ERROR: file wasn't opened\n");
            exit(-1);
        }

        for (int i = 0; i < own_chunk_size; i++) {
            fprintf(file, "%d ", chunk[i]);
        }

        fclose(file);

        printf("------- MPI - C -------\n");
        printf("%d nums, %d procs: %f secs\n",
            number_of_elements, number_of_process, time_taken);
    }

    MPI_Finalize();
    return 0;
}
