#include <iostream>
#include <cstring>
using namespace std;

typedef struct
{
    string name;
    int value;
    bool secUnit;
}CHN_NAME_VALUE;
typedef struct
{
    string name;
    int value;
}CHN_NAME_VALUE2;
CHN_NAME_VALUE2 chnValue[] = {{"��",0},{"һ",1},{"��",2},{"��",3},{"��",4},{"��",5},{"��",6},{"��",7},{"��",8},{"��",9}};
CHN_NAME_VALUE chnValuePair[] = {{"ʮ",10,false},{"��",100,false},{"ǧ",1000,false},{"��",10000,true},{"��",100000000,true}};

int ChinesetoValue(string chnstr){
    for(int i=0;i<10;++i){
        if(chnValue[i].name==chnstr)
            return chnValue[i].value;
    }
    return -1;
}
int ChinesetoUnit(string str,bool &secUnit){
    for(int i=0;i<5;++i){
        if(str==chnValuePair[i].name){
            secUnit = chnValuePair[i].secUnit;
            return chnValuePair[i].value;
        }
    }
}
unsigned int Chinesetoint(const string &chnString){
    unsigned int rtn = 0;
    unsigned int section = 0;
    int number = 0;
    bool secUnit = false;
    string::size_type pos = 0;
    while(pos<chnString.length()){
        int num = ChinesetoValue(chnString.substr(pos,2));
        if(num>=0){
            number = num;
            pos += 2;
            if(pos>=chnString.length()){
                section += number;
                rtn += section;
                break;
            }
        }
        else{
            int unit = ChinesetoUnit(chnString.substr(pos,2),secUnit);
            if(secUnit){
                section = (section+number)*unit;
                rtn += section;
                section = 0;
            }
            else{
                section += (number * unit);
            }
            number = 0;
            pos += 2;
            if(pos>=chnString.length()){
                rtn +=section;
                break;
            }
        }
    }
    return rtn;
}
int main()
{
    string num = "��ǧ����ǧ";
    cout<<Chinesetoint(num);
    return 0;
}
