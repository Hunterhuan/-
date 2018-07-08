#include <iostream>
#include <stdio.h>
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <cassert>
using namespace std;

typedef enum tagBoatLocation
{
    LOCAL = 0,
    REMOTE
}BOAT_LOCATION;

typedef enum tagActionName
{
    ONE_MONSTER_GO = 0,
    TWO_MONSTER_GO,
    ONE_MONK_GO,
    TWO_MONK_GO,
    ONE_MONSTER_ONE_MONK_GO,
    ONE_MONSTER_BACK,
    TWO_MONSTER_BACK,
    ONE_MONK_BACK,
    TWO_MONK_BACK,
    ONE_MONSTER_ONE_MONK_BACK,
    INVALID_ACTION_NAME,
}ACTION_NAME;

typedef struct tagActionEffection
{
    ACTION_NAME act;
    BOAT_LOCATION boat_to; //���ƶ��ķ���
    int move_monster; //�˴��ƶ�����������
    int move_monk; //�˴��ƶ��ĺ�������
}ACTION_EFFECTION;

struct ItemState
{
    ItemState();
    ItemState(const ItemState& state);
    ItemState& operator=(const ItemState& state);
    bool IsSameState(const ItemState& state) const;
    void SetState(int lmonster, int lmonk, int rmonster, int rmonk, BOAT_LOCATION bl);
    void PrintStates();
    bool IsFinalState();
    bool CanTakeAction(ACTION_EFFECTION& ae) const;
    bool IsValidState();

    int local_monster;
    int local_monk;
    int remote_monster;
    int remote_monk;
    BOAT_LOCATION boat; /*LOCAL or REMOTE*/
    ACTION_NAME curAct;
};

typedef struct tagActionDescription
{
    ACTION_NAME act;
    const char *description; //��Ӧ������������������
}ACTION_DESCRIPTION;


const char *GetActionDescription(ACTION_NAME act);

const int monster_count = 3;
const int monk_count    = 3;

ItemState::ItemState()
{
    SetState(monster_count, monk_count, 0, 0, LOCAL);
    curAct = INVALID_ACTION_NAME;
}

ItemState::ItemState(const ItemState& state)
{
    SetState(state.local_monster, state.local_monk,
             state.remote_monster, state.remote_monk, state.boat);
    curAct = state.curAct;
}

ItemState& ItemState::operator=(const ItemState& state)
{
    SetState(state.local_monster, state.local_monk,
             state.remote_monster, state.remote_monk, state.boat);
    curAct = state.curAct;

    return *this;
}

bool ItemState::IsSameState(const ItemState& state) const
{
    return ( (local_monster == state.local_monster)
             && (local_monk == state.local_monk)
             && (remote_monster == state.remote_monster)
             && (remote_monk == state.remote_monk)
             && (boat == state.boat) );
}

void ItemState::SetState(int lmonster, int lmonk, int rmonster, int rmonk, BOAT_LOCATION bl)
{
    local_monster  = lmonster;
    local_monk     = lmonk;
    remote_monster = rmonster;
    remote_monk    = rmonk;
    boat           = bl;
}

void ItemState::PrintStates()
{
    std::cout << GetActionDescription(curAct) << std::endl;
}

bool ItemState::IsFinalState()
{
    return ( (local_monster == 0) && (local_monk == 0)
             && (remote_monster == monster_count) && (remote_monk == monk_count)
             && (boat == REMOTE) );
}

bool ItemState::CanTakeAction(ACTION_EFFECTION& ae) const
{
    if(boat == ae.boat_to)
        return false;
    if((local_monster + ae.move_monster) < 0 || (local_monster + ae.move_monster) > monster_count)
        return false;
    if((local_monk + ae.move_monk) < 0 || (local_monk + ae.move_monk) > monk_count)
        return false;

    return true;
}

bool ItemState::IsValidState()
{
    if((local_monk > 0) && (local_monster > local_monk))
    {
        return false;
    }
    if((remote_monk > 0) && (remote_monster > remote_monk))
    {
        return false;
    }

    return true;
}

ACTION_DESCRIPTION actDesc[] =
{
    { ONE_MONSTER_GO ,            "One monster go over river"              },
    { TWO_MONSTER_GO ,            "Two monster go over river"              },
    { ONE_MONK_GO ,               "One monk go over river"                 },
    { TWO_MONK_GO ,               "Two monk go over river"                 },
    { ONE_MONSTER_ONE_MONK_GO ,   "One monster and one monk go over river" },
    { ONE_MONSTER_BACK ,          "One monster go back"                    },
    { TWO_MONSTER_BACK ,          "Two monster go back"                    },
    { ONE_MONK_BACK ,             "One monk go back"                       },
    { TWO_MONK_BACK ,             "Two monk go back"                       },
    { ONE_MONSTER_ONE_MONK_BACK , "One monster and one monk go back"       }
};


const char *GetActionDescription(ACTION_NAME act)
{
    int actCount = sizeof(actDesc) / sizeof(actDesc[0]);
    for(int i = 0; i < actCount; ++i)
    {
        if(actDesc[i].act == act)
        {
            return actDesc[i].description;
        }
    }

    return "Unknown action";
}

ACTION_EFFECTION actEffect[] =
{
    { ONE_MONSTER_GO ,            REMOTE, -1,  0 },
    { TWO_MONSTER_GO ,            REMOTE, -2,  0 },
    { ONE_MONK_GO ,               REMOTE,  0, -1 },
    { TWO_MONK_GO ,               REMOTE,  0, -2 },
    { ONE_MONSTER_ONE_MONK_GO ,   REMOTE, -1, -1 },
    { ONE_MONSTER_BACK ,          LOCAL ,  1,  0 },
    { TWO_MONSTER_BACK ,          LOCAL ,  2,  0 },
    { ONE_MONK_BACK ,             LOCAL ,  0,  1 },
    { TWO_MONK_BACK ,             LOCAL ,  0,  2 },
    { ONE_MONSTER_ONE_MONK_BACK , LOCAL ,  1,  1 }
};

bool IsSameItemState(ItemState state1, ItemState state2)
{
    return state1.IsSameState(state2);
}

bool IsProcessedState(std::deque<ItemState>& states, ItemState& newState)
{
    std::deque<ItemState>::iterator it = states.end();

    it = find_if( states.begin(), states.end(),
                  std::bind2nd(std::ptr_fun(IsSameItemState), newState) );

    return (it != states.end());
}

void PrintResult(std::deque<ItemState>& states)
{
    std::cout << "Find Result : " << std::endl;
    for_each(states.begin(), states.end(), std::mem_fun_ref(&ItemState::PrintStates));
    std::cout << std::endl << std::endl;
}

bool MakeActionNewState(const ItemState& curState, ACTION_EFFECTION& ae, ItemState& newState)
{
    if(curState.CanTakeAction(ae))
    {
        newState = curState;
        newState.local_monster  += ae.move_monster;
        newState.local_monk     += ae.move_monk;
        newState.remote_monster -= ae.move_monster;
        newState.remote_monk    -= ae.move_monk;
        newState.boat    = ae.boat_to;
        newState.curAct  = ae.act;

        return true;
    }

    return false;
}

void SearchState(std::deque<ItemState>& states);

void SearchStateOnNewAction(std::deque<ItemState>& states,
                             ItemState& current, ACTION_EFFECTION& ae)
{
    ItemState next;
    if(MakeActionNewState(current, ae, next))
    {
        if(next.IsValidState() && !IsProcessedState(states, next))
        {
            states.push_back(next);
            SearchState(states);
            states.pop_back();
        }
    }
}

void SearchState(std::deque<ItemState>& states)
{
    ItemState current = states.back(); /*ÿ�ζ��ӵ�ǰ״̬��ʼ*/
    if(current.IsFinalState())
    {
        PrintResult(states);
        return;
    }

    /*������10�ֶ����ֱ��뵱ǰ״̬���*/
    for(int i = 0; i < sizeof(actEffect) / sizeof(actEffect[0]); i++)
    {
        SearchStateOnNewAction(states, current, actEffect[i]);
    }
}


int main(int argc, char* argv[])
{
    std::deque<ItemState> states;
    ItemState init;

    states.push_back(init);
    SearchState(states);

    assert(states.size() == 1); /*��ٽ�����statesӦ�û���ֻ��һ����ʼ״̬*/

    return 0;
}
