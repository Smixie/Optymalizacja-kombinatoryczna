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

using namespace std;

int main(){
    ifstream indata;
    indata.open("test_case.txt");
    
    string value;
    string problem_name;
    int lines = 1,K,Q; 
    int i,x,y,q,e,l,d; 

    vector<int> custNO;
    vector<int> Xcord;
    vector<int> Ycord;
    vector<int> demand;
    vector<int> ready;
    vector<int> duo;
    vector<int> service;

    if(!indata)
    {
        cout << "No file" << endl;
    }
    else
    {
        while (!indata.eof())
        {   
            getline(indata,value);
            cout << lines << " ";
            cout << value << endl;
            lines++;
            if(lines == 2){
                problem_name = value;
            }
            if(lines == 5){
                indata >> K >> Q;
            }
            if(lines >= 10){
                indata >> i >> x >> y >> q >> e >> l >> d;
                custNO.push_back(i);
                Xcord.push_back(x);
                Ycord.push_back(y);
                demand.push_back(q);
                ready.push_back(e);
                duo.push_back(l);
                service.push_back(d);
            }
        }
        cout << problem_name << " " << K << " " << Q << endl; 
        indata.close();

        for(int j=0;j<custNO.size();j++){
            cout << custNO[j] << " " << Xcord[j] << " " << Ycord[j] << " " << demand[j] << " " << ready[j] << " " << duo[j] << " " << service[j] << endl;
        }
    }
}