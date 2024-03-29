// 3. Максимальный треугольник
//Заданы длины N отрезков, необходимо выбрать три таких отрезка,
//которые образовывали бы треугольник с максимальной площадью.
//Формат входных данных : на первой строке находится число N, за
//которым следует N строк с целыми числами - длинами отрезков.Формат
//выходных данных : если никакого треугольника из заданных отрезков
//составить нельзя — 0, в противном случае на первой строке площадь
//треугольника с тремя знаками после запятой, на второй строке — длины
//трёх отрезков, составляющих этот треугольник.Длины должны быть
//отсортированы.
//Пример :
//    Входной файл Выходной файл
//    4
//    2
//    1
//    3
//    5
//
//    0
//
//    Входной файл Выходной файл
//    4
//    3
//    5
//    3
//    4
//
//    6.000
//    3 4 5
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;


//--- HERON -------------------------------------------------
// формула Герона:  S = sqrt (p*(p-a)*(p-b)*(p-c)), где p = (a+b+c)/2
long double calculateHeron(long double a, long double b, long double c)
{
	// вернем 0 если треугольник не возможен
	if (a + b < c) return 0.0;
	if (b + c < a) return 0.0;
	if (a + c < b) return 0.0;
	long double p = (a + b + c) / 2.0;
	return sqrtl(p * (p - a) * (p - b) * (p - c));
}

//--- MAIN --------------------------------------------------

int main()
{
	size_t edgeCount;
	cin >> edgeCount;							// вводим количество отрезков
	if (edgeCount < 3)							// если меньше 3 отрезков
	{
		cout << "0" << endl;					// то треугольника не получится создать
		return 0;
	}

	vector<uint64_t> edges(edgeCount);
	//---- в цикле заполняем массив отрезков
	for (size_t i = 0; i < edgeCount; i++)
		cin >> edges[i];						// вводим очередной отрезок

	sort(edges.begin(), edges.end());			// отсортировать массив отрезков.. считаем, что сортировка со сложностью O(n)

	long double maxHeron = 0.0;								// 0 - это значение, когда треугольник не получился
	size_t indexOfMax = 0;
	for (size_t i = edges.size() - 1; i >= 2; i--)		// второй проход по массиву с конца до 2-го элемента. сложность O(n)
	{
		long double heron = calculateHeron((long double)edges[i - 2], (long double)edges[i - 1], (long double)edges[i]);
		if (heron > maxHeron)							// если новая площадь больше
		{
			maxHeron = heron;							// запоминаем площадь и индекс тройки отрезков
			indexOfMax = i;
		}
	}

	if (maxHeron == 0.0)								// если максимальный элемент имеет нулевое значение
	{
		cout << "0" << endl;							// то это значит, что ни один треугольник не получился
		return 0;
	}

	// итак, треугольник получился - один или несколько. треугольник с максимальной учетверенным квадратом площади найден.
	cout.precision(3);
	cout << fixed << maxHeron << endl;						// выведем площадь и тройку отрезков
	cout << edges[indexOfMax - 2] << " " << edges[indexOfMax - 1] << " " << edges[indexOfMax] << endl;

	return 0;
}
