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

using namespace std;

int road(int a, int b){
    double len;
    return len = sqrt(pow(a,2)+pow(b,2));
}


int main(){
    ifstream indata;
    indata.open("test_case2.txt");
    
    string value;
    string problem_name;
    int lines = 1,K,Q; 
    int i,x,y,q,e,l,d; 
    int number_of_consuments, number_of_routes;

    vector<vector<int>> array;

    if(!indata)
    {
        cout << "No file" << endl;
    }
    else
    {
                while (!indata.eof())
        {   
            getline(indata,value);
            lines++;
            if(lines == 2){
                problem_name = value;
            }
            if(lines == 5){
                indata >> K >> Q;
            }
            if(lines >= 10){
                indata >> i >> x >> y >> q >> e >> l >> d;
                vector<int> v1;
                v1.push_back(i);
                v1.push_back(x);
                v1.push_back(y);
                v1.push_back(q);
                v1.push_back(e);
                v1.push_back(l);
                v1.push_back(d);   
                array.push_back(v1);             
            }
        }
        indata.close();

        number_of_consuments = array.size()-1;
        
        /*
        cout << problem_name << " " << K << " " << Q << endl;

        for(int j=0;j<array.size();j++)
        {
            for(int k=0; k < 7; k++)
            {
                cout << array[j][k] << " ";
            }
            cout << endl;
        }
        */
        
        
    }
    double road_time;
    for(int j=1;j <= number_of_consuments; j++){
        road_time = road(array[0][1]-array[j][1],array[0][2]-array[j][2]);
        if(array[j][4] < road_time){
            number_of_routes = -1;
            break;
        }
    }
    cout << number_of_routes << endl;
}