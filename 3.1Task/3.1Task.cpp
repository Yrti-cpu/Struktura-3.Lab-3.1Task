﻿#include <iostream>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <algorithm>
#define N_1 60000 
#define N_2 90000 
#define N_3 120000
#define N_4 150000

static unsigned long long primary = 0, secondary = 0;

void NonRecursiveQuickSort(int* a, int n)
{
    primary = secondary = 0;
    clock_t start = clock();
    const int M = log(n) / log(2) + 1;
    int i, j, left, right, s, x, w;
    struct stack { int left, right; } *stack;
    stack = (struct stack*)malloc(M * sizeof(struct stack));
    s = 0;
    stack[0].left = 0;
    stack[0].right = n - 1;

    do /*выбор из стека последнего запроса*/
    {
        left = stack[s].left;
        right = stack[s].right;
        s--;
        do /*разделение а[left]… a[right]*/
        {
            i = left; j = right;
            x = a[(left + right) / 2];
            do
            {
                while (++primary && a[i] < x) i++;
                while (++primary && x < a[j]) j--;
                if (++secondary && i <= j)                              
                {
                    w = a[i]; a[i] = a[j]; a[j] = w;
                    i++; j--;
                }
            } while (++secondary && i < j);
            if (++secondary && i < right && ++secondary && right - i >= j - left) /*если правая часть не меньше левой*/
            { /*запись в стек границ правой части*/
                s++;
                stack[s].left = i;
                stack[s].right = right;
                right = j; /*теперь left и right ограничивают левую часть*/
            }
            else if (++secondary && j > left && ++secondary && j - left > right - i) /*если левая часть больше правой*/
            { /*запись в стек границ левой части*/
                s++;
                stack[s].left = left;
                stack[s].right = j;
                left = i; /*теперь left и right ограничивают правую часть*/
            }
            else left = right; /*делить больше нечего, интервал "схлопывается"*/
        } while (++secondary && left < right);
    } while (++secondary && s > -1);
    clock_t end = clock();
    int memory = 8 * sizeof(int) + sizeof(struct stack*) + M * sizeof(struct stack);
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "   time = " << time << " seconds" << std::endl << "   memory = " << memory << " bytes" << std::endl << "   primary operations = " << primary << std::endl << "   secondary operations = " << secondary << std::endl << std::endl;
    free(stack);
}


void MergeSortNonRecursive(int* a, int n)
{
    primary = secondary = 0;
    clock_t start = clock();
    int* temp = (int*)malloc(sizeof(int) * n);	/* вспомогательный массив */
    int* p1, 	/* адрес элемента в одной серии */
        * p2, 	/* адрес элемента в другой серии */
        * p3, 	/* адрес элемента в формируемом массиве */
        length = 1, 		/* длина серии */
        len1 = 0, len2 = 0, 	/* текущие длины серий */
        * end_a,				/* адрес конца массива a */
        * end_temp;			/* адрес конца массива temp */
    char flag = 0;			/* признак того, откуда куда идет перезапись: 0 - из сортируемого массива во вспомогательный, 1 - из вспомогательного в исходный */
    int memory = 6 * sizeof(int *) + 3 * sizeof(int) + sizeof(char) + sizeof(int) * n;
    while (++secondary && length < n)	/* пока длина серии меньше длины массива */
    {
        p1 = a; p2 = a + length; p3 = temp; end_a = a + n; end_temp = temp + n; /* указатели ставим на стартовые позиции */
        while (++secondary && p3 < end_temp)			/* пока переписаны не все элементы массива */
        {
            len1 = 0;			/* в первой серии всегда будут элементы, устанавливаем счетчик в 0 */
            if (++secondary && p2 < end_a) 	/* если на вторую серию элементов в массиве хватает, */
                len2 = 0;		/* то счетчик тоже устанавливаем в 0 */
            else
                len2 = length;	/* а если элементов не хватает, то серия не будет принимать участие в переписывании */
            while (++secondary && len1 < length && ++secondary && len2 < length && ++secondary && p2 < end_a)   /* пока в обеих сериях есть элементы */
            {
                if (++primary && *p1 <= *p2)						/* если значение в первой серии не больше, чем во второй, */
                    *p3++ = *p1++, len1++;			/* то переписываем его в другой массив, увеличиваем счетчик изъятых элементов */
                else
                    *p3++ = *p2++, len2++;			/* иначе переписываем значение из второй серии */
            }
            while (++secondary && len1 < length && ++secondary && p3 < end_temp)  	/* пока первая серия не пуста */
                *p3++ = *p1++, len1++;					/* переписываем ее до конца во второй массив */
            while (++secondary && len2 < length && ++secondary && p3 < end_temp)   	/* пока вторая серия не пуста */
                *p3++ = *p2++, len2++;					/* переписываем ее до конца во второй массив */
            p1 += length; 			/* переставляем указатели на первые элементы следующих серий */
            p2 += length;
        }
        length *= 2;			/* увеличиваем длину серии вдвое */
        p3 = a;			/* меняем местами входной и выходной массивы */
        a = temp;
        temp = p3;
        flag = !flag; 	/* меняем значение флага */
    }
    if (++secondary && flag)	/* если флаг установлен, то упорядоченная последовательность находится во вспомогательном массиве */
    {
        for (p1 = temp, p2 = a; ++secondary && p1 < temp + n; *p1++ = *p2++);	/* переписываем ее в исходный */
        free(a);												/* освобождаем память от вспомогательного массива */
    }
    else
        free(temp);	/* иначе на вспомогательный массив указыает temp, освобождаем память от вспомогательного массива */
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "   time = " << time << " seconds" << std::endl << "   memory = " << memory << " bytes" << std::endl << "   primary operations = " << primary << std::endl << "   secondary operations = " << secondary << std::endl << std::endl;
}


void NaturalMergeSort(int* a, int n)
{
    primary = secondary = 0;
    clock_t start = clock();
    int split;                   /* индекс, по которому делим массив */
    int last, end, i, * p = a, * tmp;
    char flag = 0, sorted = 0;
    int pos1, pos2, pos3;
    tmp = (int*)malloc(n * sizeof(int));
    int memory = 7 * sizeof(int) + 2 * sizeof(int*) + 2 * sizeof(char) + n * sizeof(int);
    do                 /* если есть более 1 элемента */
    {
        end = n; pos2 = pos3 = 0;
        do
        {
            p += pos2; end = n - pos3;
            for (split = 1; ++secondary && split < end && ++primary && p[split - 1] <= p[split]; split++); /*первая серия*/
            if (++secondary && split == n) { sorted = 1; break; }
            pos1 = 0; pos2 = split;
            while (++secondary && pos1 < split && ++secondary && pos2 < end) 	/*идет слияние, пока есть хоть один элемент в каждой серии*/
            {
                if (++primary && p[pos1] < p[pos2])
                    tmp[pos3++] = p[pos1++];
                else
                {
                    tmp[pos3++] = p[pos2++];
                    if (++primary && p[pos2] < p[pos2 - 1]) break;
                }
            }
            /* одна последовательность закончилась - копировать остаток другой в конец буфера */
            while (++secondary && pos2 < end && ++primary && tmp[pos3 - 1] <= p[pos2])  			 /* пока вторая последовательность не пуста */
                tmp[pos3++] = p[pos2++];
            while (++secondary && pos1 < split)  		/* пока первая последовательность не пуста */
                tmp[pos3++] = p[pos1++];
        } while (++secondary && pos3 < n);
        if (++secondary && sorted) break;
        p = tmp;
        tmp = a;
        a = p;
        flag = !flag;
    } while (++secondary && split < n);
    if (++secondary && flag)
    {
        for (pos1 = 0; ++secondary && pos1 < n; pos1++)
            tmp[pos1] = a[pos1];
        free(a);
    }
    else
        free(tmp);
    clock_t final = clock();
    double time = (double)(final - start) / CLOCKS_PER_SEC;
    std::cout << "   time = " << time << " seconds" << std::endl << "   memory = " << memory << " bytes" << std::endl << "   primary operations = " << primary << std::endl << "   secondary operations = " << secondary << std::endl << std::endl;

}

int memory = 0, maxi = 0;
typedef struct {
    int* beg; /* адрес начала последовательности */
    int len; /* длина последовательности */
}segment;

/* вычисление длины последовательности в зависимости от N*/
/* ответ должен быть в диапазоне (32,64]*/
int get_min_size(int n)
{
    int r = 0;
    while (++secondary && n >= 64) {
        n >>= 1;
        r |= n & 1;
    }
    return n + r;
}

/*разворот массива*/
void reverse(int* a, int n)
{
    int i, j, tmp;
    for (i = 0, j = n - 1; ++secondary && i < j; i++, j--)
        if (++primary && a[i] != a[j])
        {
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
}

/* сортировкa вставками */
void insertion_sort(int* a, int n, int i) /*адрес начала массива, его размер, размер уже упорядоченной части*/
{
    int j, x;
    for (; ++secondary && i < n; ++i)
    {
        x = a[i];
        for (j = i; ++secondary && j && ++primary && a[j - 1] > x; --j)
            a[j] = a[j - 1];
        a[j] = x;
    }
}
/*слияние с предварительным копированием во вспомогательный массив более короткой последовательности*/
void merge_copy_less(segment* seg)
{
    int* a = seg[0].beg; /*адрес начала первой из сливаемых последовательностей*/
    int split = seg[0].len; /*длина первой последовательности*/
    int n = split + seg[1].len; /*длина результирующей последовательности*/
    int pos1 = 0;	/* текущая позиция чтения из первой последовательности a[0]...a[split] */
    int pos2 = split;	/*текущая позиция чтения из второй последовательности a[split]...a[n]*/
    int pos3 = 0;  /*текущая позиция записи в результирующей последовательности*/
    int* temp;
    int mem = 0;
    if (++primary && seg[0].len < seg[1].len)
    {
        temp = (int*)malloc(sizeof(int) * split);
        mem = sizeof(int) * split;
        if (mem > maxi)
        {
            maxi = mem;
        }
        /*копируем первую последовательность во вспомогательный массив*/
        for (pos1 = 0; ++secondary && pos1 < split; pos1++)
            temp[pos1] = a[pos1];
        pos1 = 0;
        while (++secondary && pos1 < split && ++secondary && pos2 < n) 	/*идет слияние, пока есть хоть один элемент в каждой последовательности*/
            if (++primary && temp[pos1] <= a[pos2])
                a[pos3++] = temp[pos1++];
            else
                a[pos3++] = a[pos2++];
        /* одна последовательность закончилась - копировать остаток другой в конец буфера */
        /* даже если вторая последовательность не пуста, она уже на месте */
        while (++secondary && pos1 < split)  		/* пока первая последовательность не пуста */
            a[pos3++] = temp[pos1++];
    }
    else
    {
        temp = (int*)malloc(sizeof(int) * seg[1].len);
        mem =  sizeof(int) * seg[1].len;
        if (mem > maxi)
        {
            maxi = mem;
        }
        /*копируем вторую последовательность во вспомогательный массив*/
        for (pos1 = 0, pos2 = split; ++secondary && pos2 < n; )
            temp[pos1++] = a[pos2++];
        /*в этом случае слияние производится справа налево*/
        pos1 = split - 1; pos2 = seg[1].len - 1; pos3 = n - 1;
        while (++secondary && pos1 >= 0 && ++secondary && pos2 >= 0) 	/*идет слияние, пока есть хоть один элемент в каждой последовательности*/
            if (++primary && temp[pos2] > a[pos1])
                a[pos3--] = temp[pos2--];
            else
                a[pos3--] = a[pos1--];
        /* одна последовательность закончилась - копировать остаток другой в конец буфера */
        /* даже если первая последовательность не пуста, она уже на месте */
        while (++secondary && pos2 >= 0)  		/* пока вторая последовательность не пуста */
            a[pos3--] = temp[pos2--];
    }
    free(temp);
    seg[0].len = n; /*фиксируем длину полученной последовательности в стеке*/
}

/*слияние близких по размеру последовательностей*/
int try_merge(segment* seg, int top)
{
    int x, y, z;
    while (++secondary && top > 0) /*пока в стеке больше одного элемента*/
    {
        x = seg[top].len;
        y = seg[top - 1].len;
        z = top > 1 ? seg[top - 2].len : 0;
        ++secondary;
        if (++secondary && top > 1 && ++secondary && z <= x + y) /*если в стеке не меньше трех элементов и третий сверху маловат*/
        {
            if (++secondary && z < x)
            {
                merge_copy_less(&seg[top - 2]); /*сливаем ZY*/
                seg[top - 1] = seg[top]; /*корректируем стек*/
            }
            else
            {
                merge_copy_less(&seg[top - 1]); /*сливаем YX*/
            }
            top--;  /*стало на одну последовательность меньше*/
        }
        else
            if (++secondary && y <= x) /*если предпоследний не больше последнего*/
            {
                merge_copy_less(&seg[top - 1]); /*сливаем YX*/
                top--;
            }
            else
                break;
    }
    return top;
}

void TimSort(int* a, int n)
{
    maxi = memory = primary = secondary = 0;
    clock_t start = clock();
    int min_size = get_min_size(n);
    int size;
    int i, j = 0;
    /*стек координат последовательностей*/
    segment* seg = (segment*)malloc(((n - 1) / min_size + 1) * sizeof(segment));
    int t = -1; /*вершина стека*/
    memory = 13 * sizeof(int) + ((n - 1) / min_size + 1) * sizeof(segment) + sizeof(segment*) + 2 * sizeof(int*);

    /*формирование упорядоченных последовательностей*/
    for (i = 0; ++secondary && i < n; i += size)
    {
        for (j = i + 1; ++secondary && j < n && ++primary && a[j - 1] >= a[j]; j++); /*поиск обратно упорядоченной последовательности*/
        if (++secondary && j != i + 1)
            reverse(a + i, j - i);          /*переворачиваем найденную обратную последовательность*/
        if (++secondary && j != n && ++secondary && j - i < min_size)
        {
            size = n - i < min_size ? n - i : min_size;
            ++secondary;
            insertion_sort(a + i, size, j - i); /*адрес начала фрагмента, его размер, размер упорядоченной части*/
            j = i + size;
        }
        for (; ++secondary && j < n && ++primary && a[j - 1] <= a[j]; j++); /*ищем конец последовательности, если она не закончилась*/
        /*запоминаем адрес начала и длину последовательности в стеке*/
        seg[++t].beg = a + i;
        size = seg[t].len = j - i;
        t = try_merge(seg, t);
    }
    /*слияние всех оставшихся последовательностей*/
    while (++secondary && t > 0)
    {
        merge_copy_less(&seg[t - 1]); /*сливаем две последние последовательности*/
        t--; /*количество последовательностей уменьшилось*/
    }
    free(seg);
    clock_t final = clock();
    double time = (double)(final - start) / CLOCKS_PER_SEC;
    std::cout << "   time = " << time << " seconds" << std::endl << "   memory = " << memory + maxi << " bytes" << std::endl << "   primary operations = " << primary << std::endl << "   secondary operations = " << secondary << std::endl << std::endl;

}

void array_input_data(int* mas, int n, std::ifstream& f)
{
    int cnt = 0, a = 0;
    if (f.is_open())
    {
        while (cnt < n)
        {
            f >> mas[cnt++];
        }
    }
    f.seekg(0);//rewind
}

void copy_array(int N, int* in, int* out)
{
    for (int i = 0; i < N; i++)
        in[i] = out[i];
}

void repeat(std::ofstream& f, int povtor, int* array)
{
    int* mas = new int[N_4];
    int x = 0;
    for (int i = 0; i < N_4 / povtor; i++)
    {
        x = array[i];
        for (int j = 0; j < povtor; j++)
        {
            mas[i * povtor + j] = x;
        }
    }
    std::random_shuffle(mas, mas + N_4);
    for (int i = 0; i < N_4; i++)
    {
        f << mas[i];
        f << " ";
    }
}

void sorting(int* arr, int N, void (*sort)(int*, int), std::string name)
{
    std::cout << std::endl << std::endl << " " << name << " for N = " << N << std::endl;
    std::cout << " __________________________________" << std::endl;
    std::cout << "|                                  |" << std::endl;
    std::cout << "   Disordered:" << std::endl;
    sort(arr, N);
    std::cout << "   Ordered (min->max):" << std::endl;
    sort(arr, N);
    std::reverse(arr, arr + N);
    std::cout << "   Reverse ordered (max->min):" << std::endl;
    sort(arr, N);
    std::cout << "|__________________________________|" << std::endl;   

}




int main()
{
	std::ifstream file("test_numbers.txt");
	int* array = new int[N_4];
	int values[4] = { N_1,N_2,N_3,N_4 };
    int povtor[4] = { 10,100,500,1000 };

	void (*sort[4])(int*, int) = { NonRecursiveQuickSort, MergeSortNonRecursive, NaturalMergeSort, TimSort };
	std::string names[4] = { "NonRecursiveQuickSort", "MergeSortNonRecursive", "NaturalMergeSort", "TimSort" };
    std::string files[4] = { "1.txt", "2.txt", "3.txt", "4.txt" };

	array_input_data(array, N_4, file);
    std::cout << "==============================" << std::endl;
    std::cout << "Basic level" << std::endl;
    std::cout << "==============================" << std::endl;
	for (int i = 0; i < 4; i++)
	{
        int* current_array = new int[values[i]];
		for (int j = 0; j < 4; j++)
		{
            copy_array(values[i], current_array, array);
			sorting(current_array, values[i], sort[j], names[j]);
		}
        delete[] current_array;
	}

	file.close();
    std::cout << "\n==============================" << std::endl;
    std::cout << "High level" << std::endl;
    std::cout << "==============================" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        std::ofstream f(files[i]);
        repeat(f, povtor[i], array);

        f.close();
    }

    for (int i = 0; i < 4; i++)
    {
        std::ifstream file_with_repeat(files[i]);
        array_input_data(array, N_4, file_with_repeat);
        int* copy = new int[N_4];
        for (int j = 0; j < 4; j++)
        {
            std::cout << std::endl << " " << "Repeat keys = " << povtor[i];
            copy_array(N_4, copy, array);
            sorting(copy, N_4, sort[j], names[j]);
        }
        delete[] copy;
    }
	return 0;
    

}

