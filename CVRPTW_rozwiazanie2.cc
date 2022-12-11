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
#include <random>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>

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

void readfile(const string &filename, vector<vector<int>> &clients, int parameters[], vector<int> &magazine)
{
    ifstream indata;
    indata.open(filename);

    string value;

    int lines = 1;
    int i, x, y, q, e, l, d, c = 0;

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

            vector<int> v1{i, x, y, q, e, l, d, c};
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

int findStartPoint(vector<vector<int>> &clients, int numberOfClients)
{
    int startPoint = -1, bigStartWindow = 10000000, bigEndWindow = 0;
    for (int i = 0; i < numberOfClients; i++)
    {
        if (clients[i][4] < bigStartWindow && clients[i][7] == 0)
        {
            startPoint = i;
            bigEndWindow = clients[i][5];
            bigStartWindow = clients[i][4];
        }
    }
    return startPoint;
}

int findBadSituations(vector<vector<int>> &clients, int numberOfClients, vector<int> &magazine, int parameters[])
{
    int yes = 1;
    double baseDistance = 0, waitingTime = 0;
    for (int i = 0; i < numberOfClients; i++)
    {
        waitingTime = 0;
        baseDistance = distance(clients[i][1] - magazine[1], clients[i][2] - magazine[2]);
        if (baseDistance > clients[i][5])
        {
            yes = 0;
            break;
        }
        if (clients[i][4] > baseDistance)
        {
            waitingTime = clients[i][4] - baseDistance;
        }
        if (baseDistance + baseDistance + clients[i][6] + waitingTime > magazine[5])
        {
            yes = 0;
            break;
        }
        if (clients[i][3] > parameters[1])
        {
            yes = 0;
            break;
        }
    }
    return yes;
}

bool exists(vector<vector<int>> &clients, int numberOfClients)
{
    bool exist = false;
    for (int c = 0; c < numberOfClients; c++)
    {
        if (clients[c][7] == 0)
        {
            exist = true;
            break;
        }
    }
    return exist;
}

void shuffleVector(vector<vector<int>> &clients)
{
    auto rng = default_random_engine{};
    shuffle(clients.begin(), clients.end() - 1, rng);
}

void settingZeros(vector<vector<int>> &clients, int numberOfClients)
{
    for (int x = 0; x < numberOfClients; x++)
    {
        clients[x][7] = 0;
    }
}

int findRandomValue(vector<vector<int>> &clients, int numberOfClients)
{
    int random;
    while (true)
    {
        random = rand() % numberOfClients;
        if (clients[random][7] == 0)
        {
            break;
        }
        else
        {
            continue;
        }
    }
    return random;
}

int getIndex(vector<vector<int>> &clients, int numberOfClients, int K)
{
    int i = 0;
    for (i; i < numberOfClients; i++)
    {
        if (clients[i][0] == K)
            break;
    }
    return i;
}

int number_of_consuments, parameters[2], numberOfIterations = 200;

int main(int argc, char **argv)
{
    vector<vector<int>> clients;
    vector<int> magazine;
    vector<vector<int>> routes;
    vector<int> routesLocally;

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    if (argc < 2)
    {
        cout << "Usage: exe_file [input data filename] [output data filename] [iterations-optional]" << endl;
        exit(0);
    }

    if (argc >= 2 && argc <= 4)
    {
        if (argc > 3)
        {
            numberOfIterations = atoi(argv[3]);
        }
        readfile(input_filename, clients, parameters, magazine);
    }
    else
    {
        cout << "To many argumnets" << endl;
        cout << "Usage: exe_file [input data filename] [output data filename] [iterations-optional]" << endl;
        exit(0);
    }

    // Liczba klientów
    number_of_consuments = clients.size() - 1;
    int whereToStart, localCapacity = 0, isFine, bestDown, bestUp, nextClient, dol, gora, endUP, iterations = 0, numOfRoutes;
    double localRoute = 0, route, comeBack, mini, distanceDown = 0, distanceUp = 0, adder, routeUP, routeDown, totalRoute = 0, waitTime = 0;
    double lenOfRoutes, timeStart;

    // Sytuacje błedne
    isFine = findBadSituations(clients, number_of_consuments, magazine, parameters);
    if (!isFine)
    {
        savetofile(routes, output_filename, totalRoute, -1);
        exit(0);
    }

    int neighbour = (number_of_consuments / 4);
    if(neighbour == 0 ) neighbour = 1;
    
    srand((unsigned)time(NULL));
    timeStart = clock();
    while (iterations < numberOfIterations)
    {
        while (true)
        {
            localCapacity = 0;
            localRoute = 0;
            routesLocally.clear();
            waitTime = 0;

            // Pierwszy wierzchołek znalezienie/zmiana statusu/pollicznie poczatkowej trasy/dodanie to listy
            whereToStart = findRandomValue(clients, number_of_consuments);
            // whereToStart = findStartPoint(clients, number_of_consuments);
            clients[whereToStart][7] = 1;
            localRoute = distance(magazine[1] - clients[whereToStart][1], magazine[2] - clients[whereToStart][2]);
            routesLocally.push_back(clients[whereToStart][0]);

            if (localRoute < clients[whereToStart][4])
            {
                waitTime = clients[whereToStart][4] - localRoute;
            }
            localRoute += waitTime + clients[whereToStart][6];
            localCapacity = clients[whereToStart][3];
            while (true)
            {
                // Szukanie od punktu w dol
                mini = 100000000;
                adder = 0;
                bestDown = -1;
                dol = 0;

                if (dol != whereToStart)
                {
                    dol = whereToStart - neighbour;
                    if (dol < 0)
                    {
                        dol = 0;
                    }
                    while (dol < whereToStart)
                    {
                        route = distance(clients[dol][1] - clients[whereToStart][1], clients[dol][2] - clients[whereToStart][2]);
                        comeBack = distance(clients[dol][1] - magazine[1], clients[dol][2] - magazine[2]);
                        if (localRoute + route <= clients[dol][5] && clients[dol][7] == 0 && localCapacity + clients[dol][3] <= parameters[1] && localRoute + route + clients[dol][6] + comeBack <= magazine[5])
                        {
                            waitTime = clients[dol][4] - localRoute - route;
                            if (abs(waitTime) < mini)
                            {
                                mini = waitTime;
                                bestDown = dol;
                                distanceDown = waitTime;
                                routeDown = route;
                            }
                            if (abs(waitTime) == mini)
                            {
                                if (route < routeDown)
                                {
                                    mini = waitTime;
                                    bestDown = dol;
                                    distanceDown = waitTime;
                                    routeDown = route;
                                }
                            }
                        }
                        dol++;
                    }
                }

                // Szukanie od punktu w gore
                mini = 100000000;
                bestUp = -1;
                gora = 0;
                if (whereToStart < number_of_consuments - 1)
                {
                    gora = whereToStart + 1;
                    endUP = whereToStart + neighbour;
                    if (endUP >= number_of_consuments)
                    {
                        endUP = number_of_consuments;
                    }
                    while (gora < endUP)
                    {
                        route = distance(clients[gora][1] - clients[whereToStart][1], clients[gora][2] - clients[whereToStart][2]);
                        comeBack = distance(clients[gora][1] - magazine[1], clients[gora][2] - magazine[2]);
                        if (localRoute + route <= clients[gora][5] && clients[gora][7] == 0 && localCapacity + clients[gora][3] <= parameters[1] && localRoute + route + clients[gora][6] + comeBack <= magazine[5])
                        {
                            waitTime = clients[gora][4] - localRoute - route;
                            if (mini > abs(waitTime))
                            {
                                mini = waitTime;
                                bestUp = gora;
                                distanceUp = waitTime;
                                routeUP = route;
                            }
                            if (mini == abs(waitTime))
                            {
                                if (route < routeUP)
                                {
                                    mini = waitTime;
                                    bestUp = gora;
                                    distanceUp = waitTime;
                                    routeUP = route;
                                }
                            }
                        }
                        gora++;
                    }
                }
                // Nie mozna odnalezc kolejnych wierzcholkow spelniajacych warunki
                if (bestDown == -1 && bestUp == -1)
                {
                    break;
                }

                // Jeżlie nie istnieje droga w doł lub w góre, robimy aby była ona gorsza(czyli wieksza)
                if (bestDown == -1)
                {
                    distanceDown = distanceUp + 100;
                }
                if (bestUp == -1)
                {
                    distanceUp = distanceDown + 100;
                }

                if (distanceDown > distanceUp && clients[bestUp][7] == 0)
                {
                    nextClient = bestUp;
                    routesLocally.push_back(clients[nextClient][0]);
                    clients[nextClient][7] = 1;
                    if (distanceUp > 0)
                    {
                        adder = distanceUp + routeUP;
                    }
                    else
                    {
                        adder = routeUP;
                    }

                    localCapacity += clients[nextClient][3];
                    localRoute += adder + clients[nextClient][6];
                    whereToStart = nextClient;
                }

                if (distanceDown <= distanceUp && clients[bestDown][7] == 0)
                {
                    nextClient = bestDown;
                    routesLocally.push_back(clients[nextClient][0]);
                    clients[nextClient][7] = 1;

                    if (distanceDown > 0)
                    {
                        adder = distanceDown + routeDown;
                    }
                    else
                    {
                        adder = routeDown;
                    }
                    localCapacity += clients[nextClient][3];
                    localRoute += adder + clients[nextClient][6];
                    whereToStart = nextClient;
                }
                if (localRoute >= magazine[5] || localCapacity >= parameters[1])
                {
                    break;
                }
            }

            int lastVertex, lastClient, idx;

            lastVertex = routesLocally.size() - 1;
            lastClient = routesLocally[lastVertex];
            idx = getIndex(clients, number_of_consuments, lastClient);
            comeBack = distance(clients[idx][1] - magazine[1], clients[idx][2] - magazine[2]);

            localRoute += comeBack;
            routes.push_back(routesLocally);
            totalRoute += localRoute;
            if (!exists(clients, number_of_consuments))
            {
                break;
            }
            else
            {
                continue;
            }
        }

        if (iterations == 1)
        {
            savetofile(routes, output_filename, totalRoute, routes.size());
            numOfRoutes = routes.size();
            lenOfRoutes = totalRoute;
        }

        if (iterations != 1 && totalRoute < lenOfRoutes)
        {
            savetofile(routes, output_filename, totalRoute, routes.size());
            numOfRoutes = routes.size();
            lenOfRoutes = totalRoute;
        }
        settingZeros(clients, number_of_consuments);
        shuffleVector(clients);
        routes.clear();
        totalRoute = 0;
        iterations++;
        if ((clock() - timeStart) / CLOCKS_PER_SEC >= 120)
            break;
    }
    // Wypisywanie
    // cout << fixed << setprecision(5) << totalRoute << endl;
    // for (int x = 0; x < clients.size()-1; x++)
    // {
    //     for (int y = 0; y < clients[x].size(); y++)
    //     {
    //         cout << clients[x][y] << " ";
    //     }
    //     cout << endl;
    // }
}