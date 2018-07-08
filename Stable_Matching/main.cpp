#include <iostream>

using namespace std;
#define UNIT_COUNT 10
//Gale-Shapley �㷨 n^2 ���Ӷ�

typedef struct tagPartner
{
    char *name;
    int next;   //��һ���������
    int current; //��ǰ
    int pCount; //ƫ���б��е�������
    int perfect[UNIT_COUNT];  //ƫ���б�
}PARTNER;

int FindFreePartner(PARTNER *boys, int count){
    for(int i=0;i<count;++i){
        if(boys[i].current == -1 && boys[i].next == 0)
            return i;
    }
    return -1;
}
bool IsAllPartnerMatch(PARTNER *boys, int count){
    for(int i=0;i<count;++i){
        if(boys[i].current == -1)
            return false;
    }
    return true;
}
int GetPerfectPosition(PARTNER *partner, int id)
{
    for(int i=0;i< partner->pCount;i++){
        if(partner->perfect[i] == id){
            return i;
        }
    }
    return 0x7fffffff;
}

bool Gale_Shapley(PARTNER *boys, PARTNER *girls, int count)
{
    int bid = FindFreePartner(boys, count);
    while(bid >=0)
    {
        int gid = boys[bid].perfect[boys[bid].next];
        if(girls[gid].current == -1){
            boys[bid].current = gid;
            girls[gid].current = bid;
        }
        else{
            int bpid = girls[gid].current;
            if(GetPerfectPosition(&girls[gid],bpid)>GetPerfectPosition(&girls[gid],bid)){
                boys[bpid].current = -1;
                boys[bid].current = gid;
                girls[gid].current = bid;
            }
        }
        boys[bid].next++;
        bid = FindFreePartner(boys, count);
    }
    return IsAllPartnerMatch(boys, count);
}

//�ж��Ƿ��ȶ�
bool IsStableMatch(PARTNER *boys, PARTNER *girls){
    for(int i=0;i<UNIT_COUNT ; ++i){
        int gpos = GetPerfectPosition(&boys[i], boys[i].current);
        for(int k=0; k<gpos;k++){
            int gid = boys[i].perfect[k];
            int bpos = GetPerfectPosition(&girls[gid], i);
            int cpos = GetPerfectPosition(&girls[gid], girls[gid].current);
            if(bpos< cpos)
                return false;
        }
    }
    return true;
}


int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
