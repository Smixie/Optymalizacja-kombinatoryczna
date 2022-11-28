/*
    i - numer wierzcholka
    x,y - wierzcholki
    q - zapotrzebowanie na towar
    e,l - poczatek(e) i koniec(l) okna czasowego dostawy
    d - czas rozladunku
    Q - pojemnosc ciezarowek

    odbiorca < Q
    czas przyjazdu < e -> za wczesnie wiec czeka
    czas przyjazdu > l -> za pozno nie mozna miec takiego rozwiazania

    rozladunek moze sie zakonczyc po czasie l

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <iomanip>
#include <stdio.h>

using namespace std;

double distance(int a, int b)
{
    return sqrt(pow(a, 2) + pow(b, 2));
}

void output(string problem_name, int &K, int &Q, vector<vector<int>> &array)
{
    cout << problem_name << " " << K << " " << Q << endl;

    for (int j = 0; j < array.size() - 1; j++)
    {
        for (int k = 0; k < 7; k++)
        {
            cout << array[j][k] << " ";
        }
        cout << endl;
    }
}

void readfile(const string &filename, vector<vector<int>> &array, int parameters[], string &problem_name)
{
    ifstream indata;
    indata.open(filename);

    string value;

    int lines = 1;
    int i, x, y, q, e, l, d;

    if (!indata)
    {
        cout << "No file" << endl;
    }
    else
    {
        while (!indata.eof())
        {
            getline(indata, value);
            lines++;
            if (lines == 2)
            {
                problem_name = value;
            }
            if (lines == 5)
            {
                indata >> parameters[0] >> parameters[1];
            }
            if (lines >= 10)
            {
                indata >> i >> x >> y >> q >> e >> l >> d;
                vector<int> v1{i, x, y, q, e, l, d};
                array.push_back(v1);
            }
        }
    }
    indata.close();
}

void savetofile(vector<vector<int>> &routes, const string &outputFile, double sumDistance, int routesNum)
{
    ofstream output;
    output.open(outputFile);
    // cout << routesNum << " " << fixed << setprecision(5) << sumDistance << endl;
    output << routesNum << " " << fixed << setprecision(5) << sumDistance << endl;
    for (int i = 0; i < routes.size(); i++)
    {
        for (int j = 0; j < routes[i].size(); j++)
        {
            // cout << routes[i][j] << " ";
            output << routes[i][j] << " ";
        }
        // cout << endl;
        output << endl;
    }
    output.close();
}

int number_of_consuments, parameters[2];

string problem_name;

int main(int argc, char **argv)
{
    vector<vector<int>> array;
    vector<vector<int>> routes;

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    if (argc < 2)
    {
        cout << "Usage: exe_file [input data filename] [output data filename]" << endl;
        exit(0);
    }

    if (argc >= 2)
    {
        readfile(input_filename, array, parameters, problem_name);
    }

    number_of_consuments = array.size() - 2;

    // output(problem_name,parameters[0],parameters[1],array);
    double road_time, waiting_time, route_len = 0, local_len, local_route_len = 0, come_back = 0;
    int number_of_routes = 0, local_cap = 0, start = 0, end = 1;
    int j = 1;
    vector<int> v;
    while (j <= number_of_consuments)
    {
        local_len = 0;
        waiting_time = 0;
        road_time = distance(array[start][1] - array[end][1], array[start][2] - array[end][2]);
        if (local_route_len + road_time <= array[end][4]) // czas oczekiwania
        {
            waiting_time = array[end][4] - local_route_len - road_time;
        }

        local_len = road_time + waiting_time;
        local_cap += array[end][3];

        come_back = distance(array[end][1] - array[0][1], array[end][2] - array[0][2]);
        // droga jesli moze istniec
        if (local_route_len + local_len < array[end][5] && local_cap <= parameters[1] && local_route_len + local_len + come_back + array[end][6] < array[0][5])
        {
            local_route_len += local_len + array[end][6];
            v.push_back(array[end][0]);
            start = end;
            end++;
            j++;
        }

        come_back = distance(array[end - 1][1] - array[0][1], array[end - 1][2] - array[0][2]);
        // nie moze wrocic na czas do magazynu / nie wpisane do vektora
        if (v.size() == 0 && local_route_len + local_len + come_back + array[end][6] >= array[0][5])
        {
            number_of_routes = -1;
            break;
        }
        // nie mozna zrobic dluzszej trasy wiec robimy trase
        if (local_route_len > array[end][5] || local_cap > parameters[1] || j > number_of_consuments || local_route_len + local_len + come_back + array[end][6] >= array[0][5])
        {
            number_of_routes++;
            route_len += local_route_len + come_back;
            routes.push_back(v);
            v.clear();
            local_route_len = 0;
            local_cap = 0;
            start = 0;
        }
    }
    // cout << number_of_routes << " " << fixed << setprecision(5) << route_len << endl;
    // for (int j = 0; j < routes.size(); j++)
    // {
    //     for (int k = 0; k < routes[j].size(); k++)
    //     {
    //         cout << routes[j][k] << " ";
    //     }
    //     cout << endl;
    // }
    savetofile(routes, output_filename, route_len, number_of_routes);
}