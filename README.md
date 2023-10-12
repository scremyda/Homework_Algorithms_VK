# 1.3. Проверка битов
Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
Иначе записать FAIL.
Необходимо использование битовых операций.
Использование арифметических операций запрещено.

### Формат ввода
Входное число лежит в диапазоне 0..232-1 и вводится в десятичном виде.

### Формат вывода
Строка OK или FAIL


# 2.1. Следующий элемент

Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. Время работы поиска k для каждого элемента B[i]: O(log(k)). n, m ≤ 10000.

### Формат ввода
В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.

# 3.2. Дек с динамическим буфером

Реализовать дек с динамическим зацикленным буфером.

Обрабатывать команды push * и pop *.

### Формат ввода
В первой строке количество команд n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Если дана команда pop *, то число b - ожидаемое значение. Если команда pop * вызвана для пустой структуры данных, то ожидается “-1”.

# 4.1. Слияние массивов

Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.

### Требования:

Время работы O(N * logK)
Куча должна быть реализована в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
Куча должна быть динамической.
### Формат ввода
Сначала вводится количество массивов K.
Затем по очереди размер каждого массива и элементы массива.
Каждый массив упорядочен по возрастанию.

### Формат вывода
Итоговый отсортированный массив.

# 6. Порядковая статистика и параметры множества

Дано множество целых чисел из [0..109] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10% перцентиль
медиана
90% перцентиль
### Требования:

К дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.
Формат ввода
Сначала вводится кол-во элементов массива n. После сам массив.

### Формат вывода
Параметры множества в порядке:

10% перцентиль
медиана
90% перцентиль

# 7.3. Binary MSD для long long

Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 1000000. Отсортировать массив методом MSD по битам (бинарный QuickSort).