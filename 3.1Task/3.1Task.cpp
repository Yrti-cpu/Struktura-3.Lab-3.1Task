#include <iostream>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <algorithm>
#define N_1 60000 
#define N_2 90000 
#define N_3 120000
#define N_4 150000



unsigned long long cnt_arr = 0, cnt_help = 0;

void QuickSort(int* a, int n)
{
    int x, w, i, j;
    x = a[n / 2];
    i = 0; j = n - 1;
    do
    {
        while (++cnt_arr && a[i] < x) i++;
        while (++cnt_arr && x < a[j]) j--;
        if (++cnt_help && i <= j)
        {
            w = a[i]; a[i] = a[j]; a[j] = w;
            i++; j--;
        }
    }
    while (++cnt_help && i < j);
    if (++cnt_help && j > 0)
        QuickSort(a, j + 1);
    if (++cnt_help && i < n - 1)
        QuickSort(a + i, n - i);
}

void Merge(int a[], int split, int n) {
    /* Слияние упорядоченных частей массива в буфер temp
      с дальнейшим переносом содержимого temp обратно в массив a */
    int pos1 = 0;	/* текущая позиция чтения из первой последовательности a[0]...a[split] */
    int pos2 = split;	/*текущая позиция чтения из второй последовательности a[split]...a[n]*/
    int pos3 = 0;  /*текущая позиция записи в temp*/

    int* temp = (int*)malloc(sizeof(int) * n);
    while (pos1 < split && pos2 < n) 	/*идет слияние, пока есть хоть один элемент в каждой последовательности*/
    {
        if (a[pos1] < a[pos2])
            temp[pos3++] = a[pos1++];
        else
            temp[pos3++] = a[pos2++];
    }
    /* одна последовательность закончилась - копировать остаток другой в конец буфера */
    while (pos2 < n)  			 /* пока вторая последовательность не пуста */
        temp[pos3++] = a[pos2++];
    while (pos1 < split)  		/* пока первая последовательность не пуста */
        temp[pos3++] = a[pos1++];

    /* скопировать буфер temp обратно в a */
    for (pos3 = 0; pos3 < n; pos3++)
        a[pos3] = temp[pos3];

    free(temp);
}

void MergeSortRecursive(int* a, int n)
{
    int split;                   /* индекс, по которому делим массив */

    if (n > 1)                 /* если есть более 1 элемента */
    {
        split = n / 2;
        MergeSortRecursive(a, split);       /* сортировать левую половину */
        MergeSortRecursive(a + split, n - split);/* сортировать правую половину */
        Merge(a, split, n);    /* слить результаты в общий массив */
    }
}

void NaturalMergeSort(int* a, int n)
{
    int split;                   /* индекс, по которому делим массив */
    int last, end, i, * p = a, * tmp;
    char flag = 0, sorted = 0;
    int pos1, pos2, pos3;
    tmp = (int*)malloc(n * sizeof(int));
    do                 /* если есть более 1 элемента */
    {
        end = n; pos2 = pos3 = 0;
        do
        {
            p += pos2; end = n - pos3;
            for (split = 1; split < end && p[split - 1] <= p[split]; split++); /*первая серия*/
            if (split == n) { sorted = 1; break; }
            pos1 = 0; pos2 = split;
            while (pos1 < split && pos2 < end) 	/*идет слияние, пока есть хоть один элемент в каждой серии*/
            {
                if (p[pos1] < p[pos2])
                    tmp[pos3++] = p[pos1++];
                else
                {
                    tmp[pos3++] = p[pos2++];
                    if (p[pos2] < p[pos2 - 1]) break;
                }
            }
            /* одна последовательность закончилась - копировать остаток другой в конец буфера */
            while (pos2 < end && tmp[pos3 - 1] <= p[pos2])  			 /* пока вторая последовательность не пуста */
                tmp[pos3++] = p[pos2++];
            while (pos1 < split)  		/* пока первая последовательность не пуста */
                tmp[pos3++] = p[pos1++];
        } while (pos3 < n);
        if (sorted) break;
        p = tmp;
        tmp = a;
        a = p;
        flag = !flag;
    } while (split < n);
    if (flag)
    {
        for (pos1 = 0; pos1 < n; pos1++)
            tmp[pos1] = a[pos1];
        free(a);
    }
    else
        free(tmp);
}

typedef struct {
    int* beg; /* адрес начала последовательности */
    int len; /* длина последовательности */
}segment;

/* вычисление длины последовательности в зависимости от N*/
/* ответ должен быть в диапазоне (32,64]*/
int get_min_size(int n)
{
    int r = 0;
    while (n >= 64) {
        n >>= 1;
        r |= n & 1;
    }
    return n + r;
}

/*разворот массива*/
void reverse(int* a, int n)
{
    int i, j, tmp;
    for (i = 0, j = n - 1; i < j; i++, j--)
        if (a[i] != a[j])
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
    for (; i < n; ++i)
    {
        x = a[i];
        for (j = i; j && a[j - 1] > x; --j)
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

    if (seg[0].len < seg[1].len)
    {
        temp = (int*)malloc(sizeof(int) * split);
        /*копируем первую последовательность во вспомогательный массив*/
        for (pos1 = 0; pos1 < split; pos1++)
            temp[pos1] = a[pos1];
        pos1 = 0;
        while (pos1 < split && pos2 < n) 	/*идет слияние, пока есть хоть один элемент в каждой последовательности*/
            if (temp[pos1] <= a[pos2])
                a[pos3++] = temp[pos1++];
            else
                a[pos3++] = a[pos2++];
        /* одна последовательность закончилась - копировать остаток другой в конец буфера */
        /* даже если вторая последовательность не пуста, она уже на месте */
        while (pos1 < split)  		/* пока первая последовательность не пуста */
            a[pos3++] = temp[pos1++];
    }
    else
    {
        temp = (int*)malloc(sizeof(int) * seg[1].len);
        /*копируем вторую последовательность во вспомогательный массив*/
        for (pos1 = 0, pos2 = split; pos2 < n; )
            temp[pos1++] = a[pos2++];
        /*в этом случае слияние производится справа налево*/
        pos1 = split - 1; pos2 = seg[1].len - 1; pos3 = n - 1;
        while (pos1 >= 0 && pos2 >= 0) 	/*идет слияние, пока есть хоть один элемент в каждой последовательности*/
            if (temp[pos2] > a[pos1])
                a[pos3--] = temp[pos2--];
            else
                a[pos3--] = a[pos1--];
        /* одна последовательность закончилась - копировать остаток другой в конец буфера */
        /* даже если первая последовательность не пуста, она уже на месте */
        while (pos2 >= 0)  		/* пока вторая последовательность не пуста */
            a[pos3--] = temp[pos2--];
    }
    free(temp);
    seg[0].len = n; /*фиксируем длину полученной последовательности в стеке*/
}

/*слияние близких по размеру последовательностей*/
int try_merge(segment* seg, int top)
{
    int x, y, z;
    while (top > 0) /*пока в стеке больше одного элемента*/
    {
        x = seg[top].len;
        y = seg[top - 1].len;
        z = top > 1 ? seg[top - 2].len : 0;
        if (top > 1 && z <= x + y) /*если в стеке не меньше трех элементов и третий сверху маловат*/
        {
            if (z < x)
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
            if (y <= x) /*если предпоследний не больше последнего*/
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
    int min_size = get_min_size(n);
    int size;
    int i, j = 0;
    /*стек координат последовательностей*/
    segment* seg = (segment*)malloc(((n - 1) / min_size + 1) * sizeof(segment));
    int t = -1; /*вершина стека*/

    /*формирование упорядоченных последовательностей*/
    for (i = 0; i < n; i += size)
    {
        for (j = i + 1; j < n && a[j - 1] >= a[j]; j++); /*поиск обратно упорядоченной последовательности*/
        if (j != i + 1)
            reverse(a + i, j - i);          /*переворачиваем найденную обратную последовательность*/
        if (j != n && j - i < min_size)
        {
            size = n - i < min_size ? n - i : min_size;
            insertion_sort(a + i, size, j - i); /*адрес начала фрагмента, его размер, размер упорядоченной части*/
            j = i + size;
        }
        for (; j < n && a[j - 1] <= a[j]; j++); /*ищем конец последовательности, если она не закончилась*/
        /*запоминаем адрес начала и длину последовательности в стеке*/
        seg[++t].beg = a + i;
        size = seg[t].len = j - i;
        t = try_merge(seg, t);
    }
    /*слияние всех оставшихся последовательностей*/
    while (t > 0)
    {
        merge_copy_less(&seg[t - 1]); /*сливаем две последние последовательности*/
        t--; /*количество последовательностей уменьшилось*/
    }
    free(seg);
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


void sorting(int* arr, int N, void (*sort)(int*, int), std::string name)
{
    int* current_array = new int[N];
    copy_array(N, current_array, arr);
    std::cout << std::endl << std::endl << "   " << name << " for N = " << N << std::endl;
    std::cout << " _____________________________________" << std::endl;
    std::cout << "|                                     |" << std::endl;
    std::cout << "   Disordered" << std::endl;
    sort(current_array, N);
    std::cout << "Число сравнений элементов массива " << cnt_arr << " Число вспомогательных сравнений " << cnt_help << "\n";
    std::cout << "   Ordered (min->max)" << std::endl;
    cnt_arr = cnt_help = 0;
    sort(current_array, N);
    std::cout << "Число сравнений элементов массива " << cnt_arr << " Число вспомогательных сравнений " << cnt_help << "\n";
    cnt_arr = cnt_help = 0;
    std::reverse(current_array, current_array + N);
    std::cout << "   Reverse ordered (max->min)" << std::endl;
    sort(current_array, N);
    std::cout << "Число сравнений элементов массива " << cnt_arr << " Число вспомогательных сравнений " << cnt_help << "\n";
    std::cout << "|_____________________________________|" << std::endl;
    cnt_arr = cnt_help = 0;
    delete[] current_array;

}




int main()
{
    setlocale(LC_ALL, "rus");
	std::ifstream file("test_numbers.txt");
	int* array = new int[N_4];
	int values[4] = { N_1,N_2,N_3,N_4 };

	void (*sort[4])(int*, int) = { QuickSort, MergeSortRecursive, NaturalMergeSort, TimSort };
	std::string names[4] = { "QuickSort", "MergeSortRecursive", "NaturalMergeSort", "TimSort" };

	array_input_data(array, N_4, file);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			sorting(array, values[i], sort[j], names[j]);
		}
	}

	file.close();
	delete[] array;
	return 0;
    

}

