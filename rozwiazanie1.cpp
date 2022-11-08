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

void savetofile(vector<vector<int>> &routes, const string &outputFile, double sumDistance)
{
    ofstream output;
    output.open(outputFile);
    cout << routes.size() << " " << fixed << setprecision(5) << sumDistance << endl;
    output << routes.size() << " " << fixed << setprecision(5) << sumDistance << endl;
    for (int i = 0; i < routes.size(); i++)
    {
        for (int j = 0; j < routes[i].size(); j++)
        {
            cout << routes[i][j] << " ";
            output << routes[i][j] << " ";
        }
        cout << endl;
        output << endl;
    }
    output.close();
}

void savetofileifwrong(const string &outputFile, int numOFroads)
{
    ofstream output;
    output.open(outputFile);
    output << numOFroads << endl;
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
        cout << "Put a propper syntax: a [input filename] [output filename]" << endl;
        exit(0);
    }

    if (argc >= 2)
    {
        readfile(input_filename, array, parameters, problem_name);
    }

    number_of_consuments = array.size() - 2;

    // output(problem_name,parameters[0],parameters[1],array);
    double road_time, waiting_time, route_len, number_of_routes = 0;

    for (int j = 1; j <= number_of_consuments; j++)
    {
        waiting_time = 0;
        road_time = distance(array[0][1] - array[j][1], array[0][2] - array[j][2]);
        if (road_time >= array[j][5])
        {
            number_of_routes = -1;
            break;
        }

        if (road_time < array[j][4])
        {
            waiting_time = array[j][4] - road_time;
        }

        if (road_time < array[j][5])
        {
            number_of_routes = number_of_routes + (2 * road_time) + waiting_time + array[j][6];
            vector<int> v{array[j][0]};
            routes.push_back(v);
        }
    }
    if (number_of_routes != -1)
    {
        savetofile(routes, output_filename, number_of_routes);
    }
    else
    {
        savetofileifwrong(output_filename, number_of_routes);
    }
}