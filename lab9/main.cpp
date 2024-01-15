// 
//Поиск кратчайшего пути между парой вершин
//алгоритмом Беллмана - Форда
//Задан взвешенный ориентированный граф, состоящий из n вершин и m
//ребер.Вершины пронумерованы целыми числами от 1 до n.Необходимо
//найти длину кратчайшего пути из вершины с номером start в вершину с
//номером finish при помощи алгоритма Беллмана - Форда.Длина пути
//равна сумме весов ребер на этом пути.Обратите внимание, что в данном
//варианте веса ребер могут быть отрицательными, поскольку алгоритм
//умеет с ними работать.Граф не содержит петель, кратных ребер и
//циклов отрицательного веса.
//Входные данные
//В первой строке заданы 1 ≤ n ≤ 105, 1 ≤ m ≤ 3 * 105, 1 ≤ start ≤ n и 1 ≤finish ≤ n.
//В следующих m строках записаны ребра.Каждая строка
//содержит три числа – номера вершин, соединенных ребром, и вес
//данного ребра.Вес ребра – целое число от - 109 до 109.
//Выходные данные
//Необходимо вывести одно число – длину кратчайшего пути между
//указанными вершинами.Если пути между указанными вершинами не
//существует, следует вывести строку "No solution" (без кавычек).
//Пример
//Входной файл Выходной файл
//5 6 1 5
//1 2 2
//1 3 0
//3 2 -1
//2 4 1
//3 4 4
//4 5 5
//
//5

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef int64_t weight_t;

//--- EDGE --------------------------------------------------

struct Edge
{
    size_t BeginVertex;
    size_t EndVertex;
    weight_t Weight;
};

//--- MAIN --------------------------------------------------

int main()
{
    size_t vertexCount;
    size_t edgeCount;
    size_t startVertex;
    size_t finishVertex;

    cin >> vertexCount;                 // ввести количество вершин N (1 <= n <= 105)
    cin >> edgeCount;                   // ввести количество ребер M (1 <= n <= 105)
    cin >> startVertex;                 // ввести номер начальной вершины (1 <= start <= n). внимание! нумерация с 1
    cin >> finishVertex;                // ввести номер конечной вершины (1 <= finish <= n). внимание! нумерация с 1

    // проверка корректности ввода данных
    if (!((1 <= vertexCount) && (vertexCount <= 105)))
        throw ("vertexCount is invalid: " + to_string(vertexCount));
    if (!((1 <= edgeCount) && (edgeCount <= 105)))
        throw ("edgeCount is invalid: " + to_string(edgeCount));
    if (!((1 <= startVertex) && (startVertex <= vertexCount)))
        throw ("startVertex is invalid: " + to_string(startVertex));
    if (!((1 <= finishVertex) && (finishVertex <= vertexCount)))
        throw ("finishVertex is invalid: " + to_string(finishVertex));

    vector<Edge> edges(edgeCount);          // массив по количеству ребер
    for (size_t i = 0; i < edgeCount; i++)
    {
        size_t beginVertex;
        size_t endVertex;
        weight_t edgeWeight;

        cin >> beginVertex;                 // ввести номер начальной вершины ребра (1 <= start <= n). внимание! нумерация с 1
        cin >> endVertex;                   // ввести номер конечной вершины ребра (1 <= start <= n). внимание! нумерация с 1
        cin >> edgeWeight;                  // ввести вес ребра (-109 <= weight <= 109).

        // проверка корректности ввода данных
        if (!((1 <= beginVertex) && (beginVertex <= vertexCount)))
            throw ("beginVertex is invalid: " + to_string(beginVertex) + ", row no: " + to_string(i));
        if (!((1 <= endVertex) && (endVertex <= vertexCount)))
            throw ("endVertex is invalid: " + to_string(endVertex) + ", row no: " + to_string(i));
        if (!((-109 <= edgeWeight) && (edgeWeight <= 109)))
            throw ("edgeWeight is invalid: " + to_string(edgeWeight) + ", row no: " + to_string(i));

        Edge edge = {};                          // создать очередное ребро
        edge.BeginVertex = beginVertex;
        edge.EndVertex = endVertex;
        edge.Weight = edgeWeight;
        edges[i] = edge;                                // добавить ребро в коллекцию
    }

    const weight_t infiniteWeight = INT64_MAX;          // специальное значение - бесконечный вес пути по ребрам
    vector<int64_t> weights(vertexCount + (size_t)1);   // массив по количеству вершин. На самом деле количество больше на 1 элемент. 
                                                        // делается это для удобства. Элемент 0 не будем использовать. Тогда нумерация массива с 1.
    for (size_t i = 1; i <= vertexCount; i++)           // инициализация массива
        weights[i] = infiniteWeight;                    // изначально, во всех элементах - бесконечный вес пути
    weights[startVertex] = 0;                           // кроме элемента для стартовой вершины. там вес пути = 0

    for (size_t n = 0; n < edgeCount; n++)              // провернуть несколько раз
    {
        bool wereChanges = false;                       // оптимизация: изначально изменений не было
        for (size_t i = 0; i < edgeCount; i++)          // пройдемся по каждому ребру
        {
            Edge edge = edges[i];                       // очередное ребро
            weight_t weight = weights[edge.BeginVertex];
            if (weight != infiniteWeight)               // если не напоролись на бесконечный вес ребра
            {
                weight_t addedWeight = weight + edge.Weight;
                if (addedWeight < weights[edge.EndVertex])      // если новый вес пути меньше чем подсчитать ранее. учитывается заодно, что любое значение меньше бесконечного значения
                {
                    weights[edge.EndVertex] = addedWeight;
                    wereChanges = true;                 // оптимимзация: изменения были
                }
            }
        }
        if (wereChanges == false)                       // оптимизация: прекратить крутить, если не было изменений
            break;
    }

    if (weights[finishVertex] == infiniteWeight)        // если вес путь так и остался бесконечным
        cout << "No solution" << endl;                  // то так и сообщить
    else
        cout << weights[finishVertex] << endl;          // иначе показать этот вес пути
}
