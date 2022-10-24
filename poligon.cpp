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

double road(int a, int b)
{
    return sqrt(pow(a, 2) + pow(b, 2));
}

void output(string problem_name,int K,int Q,vector<vector<int>> &array){
        cout << problem_name << " " << K << " " << Q << endl;

        for(int j=0;j<array.size();j++)
        {
            for(int k=0; k < 7; k++)
            {
                cout << array[j][k] << " ";
            }
            cout << endl;
        }
}

int main()
{
    ifstream indata;
    // indata.open("test_Case/RC203.txt");
    indata.open("test_Case/C101.txt");

    string value;
    string problem_name;

    int lines = 1, K, Q;
    int i, x, y, q, e, l, d;
    int number_of_consuments;

    vector<vector<int>> array;

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
                indata >> K >> Q;
            }
            if (lines >= 10)
            {
                indata >> i >> x >> y >> q >> e >> l >> d;
                vector<int> v1{i,x,y,q,e,l,d};
                array.push_back(v1);
            }
        }
        indata.close();

        number_of_consuments = array.size() - 2;
    }

    double road_time,waiting_time,number_of_routes;
    cout << number_of_consuments << endl;
    for(int j=1;j <= number_of_consuments; j++){
        waiting_time=0;
        road_time = road(array[0][1] - array[j][1], array[0][2] - array[j][2]);
        if(road_time >= array[j][5])
        {
            number_of_routes = -1;
            break;
        }

        if(road_time < array[j][4])
        {
            waiting_time = array[j][4] - road_time;
        }

        if(road_time < array[j][5])
        {   
            number_of_routes = number_of_routes + (2 * road_time) + waiting_time + array[j][6];
        } 
    }
    cout << fixed << setprecision(5) << number_of_routes << endl;
}