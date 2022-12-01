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
#include <chrono>

using namespace std;
using namespace std::chrono;

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

void readfile(const string &filename, vector<vector<int>> &clients, int parameters[], vector<int> &magazine)
{
    ifstream indata;
    indata.open(filename);

    string value;

    int lines = 1;
    int i, x, y, q, e, l, d,c=0;

    if (!indata)
    {
        cout << "No file" << endl;
    }
    else
    {
        getline(indata, value);
        getline(indata, value);
        getline(indata, value);
        getline(indata, value);
        indata >> parameters[0] >> parameters[1];
        getline(indata, value);
        getline(indata, value);
        getline(indata, value);
        getline(indata, value);
        indata >> i >> x >> y >> q >> e >> l >> d;
    
        magazine.push_back(i);
        magazine.push_back(x);
        magazine.push_back(y);
        magazine.push_back(q);
        magazine.push_back(e);
        magazine.push_back(l);
        magazine.push_back(d);

        while (indata)
        {
            indata >> i >> x >> y >> q >> e >> l >> d;
            
            vector<int> v1{i, x, y, q, e, l, d,c};
            clients.push_back(v1);
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

int findStartPoint(vector<vector<int>> &clients,int numberOfClients)
{
    int startPoint, bigStartWindow = 10000000,bigEndWindow=0;
    for(int i=0;i<=numberOfClients/2;i++)
    {
        if (clients[i][4] < bigStartWindow && clients[i][7]==0)
        {
            startPoint = clients[i][0];
            bigEndWindow = clients[i][5];
            bigStartWindow = clients[i][4];
        }
        if (clients[i][4] == bigStartWindow && clients[i][7]==0)
        {
            if (clients[i][5] > bigEndWindow)
            {
                startPoint = clients[i][0];
                bigEndWindow = clients[i][5];
                bigStartWindow = clients[i][4];
            }
        }
    }
    return startPoint;
}

int findBadSituations(vector<vector<int>> &clients,int numberOfClients,vector<int> &magazine,int parameters[])
{
    int yes=1;
    double baseDistance=0, waitingTime=0;
    for(int i=0;i<numberOfClients;i++)
    {
        waitingTime = 0;
        baseDistance = distance(clients[i][1]-magazine[1],clients[i][2]-magazine[2]);
        if(baseDistance > clients[i][5])
        {
            yes= 0;
            break;
        }
        if(clients[i][4] > baseDistance){
            waitingTime = clients[i][4] - baseDistance;
        }
        if(baseDistance + baseDistance + clients[i][6] + waitingTime> magazine[5])
        {
            yes = 0;
            break;
        }
        if(clients[i][3] > parameters[1])
        {
            yes = -0;
            break;
        }
    }
    return yes;
}
int number_of_consuments, parameters[2],magazine[7];

int main(int argc, char **argv)
{
    vector<vector<int>> clients;
    vector<int> magazine;
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
        readfile(input_filename, clients, parameters, magazine);
    }

    number_of_consuments = clients.size() - 1;
    int whereToStart, isFine;
    double totalSumDistance=0;

    isFine = findBadSituations(clients,number_of_consuments,magazine,parameters);
    if (!isFine)
    {
        savetofile(routes,output_filename,totalSumDistance,-1);
        exit(0);
    }

    whereToStart = findStartPoint(clients,number_of_consuments);
    double distanceBetweenClients=0,localBestRoute=1000000000000;
    int can=1,bestClientId=0;
    while(can <= 15)
    {
        localBestRoute=1000000000;
        cout << whereToStart << endl;
        int endPoint = whereToStart+5;
        if (endPoint > number_of_consuments){
            endPoint = number_of_consuments;
        }
        for(int k=whereToStart+1;k < endPoint+1;k++)
        {
            if (clients[k][7] == 0){
                distanceBetweenClients = distance(clients[k][1]-clients[whereToStart][1],clients[k][2]-clients[whereToStart][2]);
                if (distanceBetweenClients < localBestRoute)
                {
                    localBestRoute = distanceBetweenClients;
                    bestClientId = k;
                }
                if (distanceBetweenClients == localBestRoute)
                {
                    if (clients[k][5] > clients[bestClientId][5])
                    {
                        localBestRoute = distanceBetweenClients;
                        bestClientId = k;
                    }
                }
            }       
        }
        whereToStart = bestClientId;
        can++;
    }

    
    //savetofile(routes, output_filename, total_route_len, number_of_routes);
}