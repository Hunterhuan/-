#include <iostream>
#include <cstring>
using namespace std;

const char *chnNumChar[] = {"零","一","二","三","四","五","六","七","八","九"};
const char *chnUnitSection[] = {"","万","亿","万亿"};
const char *chnUnitChar[] = {"","十","百","千"};

void SectionToChinese(unsigned int section, string &chnStr){
    string strIns;
    int unitPos = 0;
    bool zero = true;
    while(section>0){
        int v = section%10;
        if(v==0){
            if((section==0)||!zero){
                zero = true;
                chnStr.insert(0,chnNumChar[0]);
            }
        }
        else{
            zero = false;
            strIns = chnNumChar[v];
            strIns += chnUnitChar[unitPos];
            chnStr.insert(0,strIns);
        }
        unitPos++;
        section = section /10;
    }
}

void NumberToChinese(unsigned int num, string &chnStr){
    if(num==0){
        chnStr = "零";
        return;
    }
    int unitPos = 0;
    string strIns;
    bool needZero = false;
    while(num>0){
        unsigned int section = num % 10000;
        if(needZero){
            chnStr.insert(0,chnNumChar[0]);
        }
        strIns = "";
        SectionToChinese(section,strIns);
        strIns += (section!=0) ?chnUnitSection[unitPos]:chnUnitSection[0];
        chnStr.insert(0,strIns);
        needZero = (section<1000)&&(section>0);
        num = num/10000;
        unitPos++;
    }
}
int main()
{
    string res = "";
    unsigned int num = 154570254;
    NumberToChinese(num,res);
    cout<<res;
    return 0;
}
