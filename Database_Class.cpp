#include<iostream>
using namespace std;

class Table {
public:
    int ID;
    string Name;
    int Age;

    Table(int id, int age, string name) {
        this->ID = id;
        this->Name = name;
        this->Age = age;
    }

    Table() {
        this->ID = 0;
        this->Name = "";
        this->Age = 0;
    }
};