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

void output(string problem_name, int &K, int &Q, vector<vector<int>> &clients)
{
    cout << problem_name << " " << K << " " << Q << endl;

    for (int j = 0; j < clients.size() - 1; j++)
    {
        for (int k = 0; k < 7; k++)
        {
            cout << clients[j][k] << " ";
        }
        cout << endl;
    }
}

void readfile(const string &filename, vector<vector<int>> &clients, int parameters[], string &problem_name)
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
            if (lines >= 9)
            {
                indata >> i >> x >> y >> q >> e >> l >> d;
                //if (i == int('\n') && (lines < 15 || lines > 22))
                //{
                //    continue;
                //}
                //else
                //{
                    vector<int> v1{i, x, y, q, e, l, d};
                    clients.push_back(v1);
                //}
                
            }
        }
    }
    indata.close();
}

void savetofile(vector<vector<int>> &routes, const string &outputFile, double sumDistance, int routesNum)
{
    ofstream output;
    output.open(outputFile);
    if (routesNum != -1)
    {
        output << routesNum << " " << fixed << setprecision(5) << sumDistance << endl;
        for (int i = 0; i < routes.size(); i++)
        {
            for (int j = 0; j < routes[i].size(); j++)
            {
                output << routes[i][j] << " ";
            }
            output << endl;
        }
    }
    else
    {
        output << routesNum << endl;
    }
    
    output.close();
}

void readfile2(const string &filename, vector<vector<int>> &clients, int parameters[], string &problem_name)
{
    ifstream indata;
    indata.open(filename);

    if(!indata.is_open())
    {
        cout << "error" << endl;
    }
    string str;
    long tmp;
    getline(indata,str);
    getline(indata,str);
    getline(indata,str);
    getline(indata,str);

    indata >> parameters[0];
    indata >> parameters[1];

    getline(indata,str);
    getline(indata,str);
    getline(indata,str);
    getline(indata,str);

    int i, x, y, q, e, l, d;
    while(indata)
    {
        indata >> i >> x >> y >> q >> e >> l >> d;
        vector<int> v1{i, x, y, q, e, l, d};
        clients.push_back(v1);
    }
    indata.close();
}


int number_of_consuments, parameters[2];

string problem_name;

int main(int argc, char **argv)
{
    vector<vector<int>> clients;
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
        readfile2(input_filename, clients, parameters, problem_name);
    }

    number_of_consuments = clients.size() - 2;

    double road_time, waiting_time, total_route_len = 0, local_len, local_route_len = 0, back_time = 0;

    int number_of_routes = 0, local_capacity = 0, start_point = 0, end_point = 1;
    int j = 1;

    vector<int> local_route;
    while (j <= number_of_consuments)
    {
        
        local_len = 0;
        waiting_time = 0;
        road_time = distance(clients[start_point][1] - clients[end_point][1], clients[start_point][2] - clients[end_point][2]);
        // demand > magazyn
        if (clients[start_point][3] > parameters[1] || clients[end_point][3] > parameters[1])
        {
            number_of_routes = -1;
            break;
        }
        // czas podrozy magazyn - klient > czas okna magazynu
        if ((2 * distance(clients[start_point][1] - clients[0][1], clients[start_point][2] - clients[0][2]) + clients[start_point][6]) > clients[0][5])
        {
            number_of_routes = -1;
            break;
        }
        
        if ((2 * distance(clients[end_point][1] - clients[0][1], clients[end_point][2] - clients[0][2]) + clients[end_point][6]) > clients[0][5])
        {   
            number_of_routes = -1;
            break;
        }

        if (local_route_len + road_time < clients[end_point][4])
        {
            waiting_time = clients[end_point][4] - local_route_len - road_time;
        }
        
        local_len = road_time + waiting_time;
        local_capacity += clients[end_point][3];
        back_time = distance(clients[end_point][1] - clients[0][1], clients[end_point][2] - clients[0][2]);
        if (local_route_len + local_len <= clients[end_point][5] && local_capacity <= parameters[1] && local_route_len + local_len + back_time + clients[end_point][6] <= clients[0][5])
        {
            local_route_len += local_len + clients[end_point][6];
            local_route.push_back(clients[end_point][0]);
            start_point = end_point;
            end_point++;
            j++;
        }

        back_time = distance(clients[end_point - 1][1] - clients[0][1], clients[end_point - 1][2] - clients[0][2]);
        if (local_route.size() == 0 && local_route_len + local_len + back_time + clients[end_point][6] > clients[0][5])
        {
            number_of_routes = -1;
            break;
        }
        
        if (local_route_len >= clients[end_point][5] || local_capacity >= parameters[1] || j > number_of_consuments || local_route_len + local_len + back_time + clients[end_point][6] >= clients[0][5])
        {
            number_of_routes++;
            total_route_len += local_route_len + back_time;
            routes.push_back(local_route);
            local_route.clear();
            local_route_len = 0;
            local_capacity = 0;
            start_point = 0;
        }
        cout << local_route_len << endl;
   }
    
    savetofile(routes, output_filename, total_route_len, number_of_routes);
}