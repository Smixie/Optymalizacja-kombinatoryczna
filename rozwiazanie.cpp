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
    int a,b,c,d,e,f,g;

    vector<int> custNO;
    vector<int> Xcord;
    vector<int> Ycord;
    vector<int> demand;
    vector<int> ready;
    vector<int> duo;
    vector<int> service;
    if(!indata){
        cout << "No file error" << endl;
    }else{
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
                indata >> a >> b >> c >> d >> e >> f >> g;
                custNO.push_back(a);
                Xcord.push_back(b);
                Ycord.push_back(c);
                demand.push_back(d);
                ready.push_back(e);
                duo.push_back(f);
                service.push_back(g);
            }
        }
        cout << problem_name << " " << K << " " << Q << endl; 
        indata.close();

        for(int i=0;i<custNO.size();i++){
            cout << custNO[i] << " " << Xcord[i] << " " << Ycord[i] << " " << demand[i] << " " << ready[i] << " " << duo[i] << " " << service[i] << endl;
        }
        }
}