#include <iostream>
#include <deque>
#include <binders.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <cassert>
#include <algorithm>
using namespace std;
#define buckets_count 3

//typedef struct tagACTION
//{
//    int from;
//    int to;
//    int water;
//}ACTION;
//struct BucketState
//{
//    int bucket_cap[BUCKETS_COUNT];
//    int bucket_s[BUCKETS_COUNT];
//    ACTION curAction;
//    bool CanTakeDumpAction(int from, int to){
//        assert((from>=0)&&(from<BUCKETS_COUNT));
//        assert((to>=0)&&(to<BUCKETS_COUNT));
//        if((from != to)
//           && !IsBucketEmpty(from)
//           && !IsBucketFull(to)){
//            return true;
//           }
//        return false;
//    }
//    bool IsBucketEmpty(int from){
//        if(bucket_s[from] == 0)
//            return true;
//        return false;
//    }
//    bool IsBucketFull(int to){
//        if(bucket_cap[to] <= bucket_s[to])
//            return true;
//        return false;
//    }
//};
//bool IsProcessedState(deque<BucketState> &states, const BucketState & newState){
//    deque<BucketState>::iterator it = states.end();
//    it = find_if(states.begin(), states.end(), bind2nd(std::ptr_fun(IsSameBucketState),newState));
//    return (it!=state.end());
//}
const int BUCKETS_COUNT = 3;

typedef struct tagACTION
{
    int from;
    int to;
    int water;
}ACTION;

struct BucketState
{
    BucketState();
    BucketState(const int *buckets);
    BucketState(const BucketState& state);
    BucketState& operator=(const BucketState& state);
    bool IsSameState(const BucketState& state) const;
    bool operator == (const BucketState& state);
    void SetAction(int w, int f, int t);
    void SetBuckets(const int *buckets);
    bool CanTakeDumpAction(int from, int to);
    bool IsBucketEmpty(int bucket);
    bool IsBucketFull(int bucket);
    void PrintStates();
    bool IsFinalState();
    bool DumpWater(int from, int to, BucketState& next);

    int bucket_s[BUCKETS_COUNT];
    ACTION curAction;
};
int bucket_capicity[BUCKETS_COUNT] = {8, 5, 3};
int bucket_init_state[BUCKETS_COUNT]  = {8, 0, 0};
int bucket_final_state[BUCKETS_COUNT] = {4, 4, 0};

BucketState::BucketState()
{
    SetBuckets(bucket_init_state);
    SetAction(8, -1, 0);
}

BucketState::BucketState(const int *buckets)
{
    SetBuckets(buckets);
    SetAction(8, -1, 0);
}

BucketState::BucketState(const BucketState& state)
{
    SetBuckets((const int *)state.bucket_s);
    SetAction(state.curAction.water, state.curAction.from, state.curAction.to);
}

BucketState& BucketState::operator=(const BucketState& state)
{
    SetBuckets((const int *)state.bucket_s);
    SetAction(state.curAction.water, state.curAction.from, state.curAction.to);
    return *this;
}

bool BucketState::IsSameState(const BucketState& state) const
{
    for(int i = 0; i < BUCKETS_COUNT; ++i)
    {
        if(bucket_s[i] != state.bucket_s[i])
            return false;
    }

    return true;
}

bool BucketState::operator == (const BucketState& state)
{
    for(int i = 0; i < BUCKETS_COUNT; ++i)
    {
        if(bucket_s[i] != state.bucket_s[i])
            return false;
    }

    return true;
}

void BucketState::SetAction(int w, int f, int t)
{
    curAction.water = w;
    curAction.from  = f;
    curAction.to    = t;
}

void BucketState::SetBuckets(const int *buckets)
{
    for(int i = 0; i < BUCKETS_COUNT; ++i)
    {
        bucket_s[i] = buckets[i];
    }
}

bool BucketState::IsBucketEmpty(int bucket)
{
    assert((bucket >= 0) && (bucket < BUCKETS_COUNT));

    return (bucket_s[bucket] == 0);
}

bool BucketState::IsBucketFull(int bucket)
{
    assert((bucket >= 0) && (bucket < BUCKETS_COUNT));

    return (bucket_s[bucket] >= bucket_capicity[bucket]);
}

void BucketState::PrintStates()
{
    std::cout << "Dump " << curAction.water << " water from "
         << curAction.from + 1 << " to " << curAction.to + 1 << ", ";
    std::cout << "buckets water states is : ";

    for(int i = 0; i < BUCKETS_COUNT; ++i)
    {
        std::cout << bucket_s[i] << " ";
    }
    std::cout << std::endl;
}

bool BucketState::IsFinalState()
{
    return IsSameState(BucketState(bucket_final_state));
}

bool BucketState::CanTakeDumpAction(int from, int to)
{
    assert((from >= 0) && (from < BUCKETS_COUNT));
    assert((to >= 0) && (to < BUCKETS_COUNT));
    /*不是同一个桶，且from桶中有水，且to桶中不满*/

    if( (from != to)
        && !IsBucketEmpty(from)
        && !IsBucketFull(to) )
    {
        return true;
    }

    return false;
}

/*从from到to倒水，返回实际倒水体积*/
bool BucketState::DumpWater(int from, int to, BucketState& next)
{
    next.SetBuckets(bucket_s);
    int dump_water = bucket_capicity[to] - next.bucket_s[to];
    if(next.bucket_s[from] >= dump_water)
    {
        next.bucket_s[to] += dump_water;
        next.bucket_s[from] -= dump_water;
    }
    else
    {
        next.bucket_s[to] += next.bucket_s[from];
        dump_water = next.bucket_s[from];
        next.bucket_s[from] = 0;
    }
    if(dump_water > 0) /*是一个有效的倒水动作?*/
    {
        next.SetAction(dump_water, from, to);
        return true;
    }

    return false;
}
bool IsSameBucketState(BucketState state1, BucketState state2)
{
    return state1.IsSameState(state2);
}

bool IsProcessedState(deque<BucketState>& states, const BucketState& newState)
{
    deque<BucketState>::iterator it = states.end();

    it = find_if( states.begin(), states.end(),
                  bind2nd(ptr_fun(IsSameBucketState), newState) );

    //it = find_if(states.begin(), states.end(), bind1st(mem_fun_ref(&BucketState::IsSameState), newState));

    return (it != states.end());
}

void PrintResult(deque<BucketState>& states)
{
    cout << "Find Result : " << endl;
    for_each(states.begin(), states.end(),
             mem_fun_ref(&BucketState::PrintStates));
    cout << endl << endl;
}

bool IsCurrentActionValid(BucketState& current, int from, int to)
{
    /*不是同一个桶，且from桶中有水，且to桶中不满*/
    if( (from != to)
        && !current.IsBucketEmpty(from)
        && !current.IsBucketFull(to) )
    {
        return true;
    }

    return false;
}

void SearchState(deque<BucketState>& states);

void SearchStateOnAction(deque<BucketState>& states, BucketState& current, int from, int to)
{
    if(IsCurrentActionValid(current, from, to))
    {
        BucketState next;
         /*从from到to倒水，如果成功，返回倒水后的状态*/
        bool bDump = current.DumpWater(from, to, next);
        if(bDump && !IsProcessedState(states, next))
        {
            states.push_back(next);
            SearchState(states);
            states.pop_back();
        }
    }
}

void SearchState(deque<BucketState>& states)
{
    BucketState current = states.back(); /*每次都从当前状态开始*/
    if(current.IsFinalState())
    {
        PrintResult(states);
        return;
    }

    /*使用两重循环排列组合6种倒水状态*/
    for(int j = 0; j < buckets_count; ++j)
    {
        for(int i = 0; i < buckets_count; ++i)
        {
            SearchStateOnAction(states, current, i, j);
        }
    }
}


int main(int argc, char* argv[])
{
    deque<BucketState> states;
    BucketState init;

    states.push_back(init);
    SearchState(states);

    assert(states.size() == 1); /*穷举结束后states应该还是只有一个初始状态*/

    return 0;
}
