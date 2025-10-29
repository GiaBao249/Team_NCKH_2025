#include<bits/stdc++.h>
#define ll long long
using namespace std;

int n;
vector<vector<int> > transaction;

struct dbv{
    vector<bool>  bit;
    int pos;

    dbv merge_bit( dbv a, dbv b ){
        dbv rs;
        int cnt = min( a.bit.size(), b.bit.size() );
        for( int i=0; i<cnt; ++i )
            rs.bit.push_back(a[i] & b[i] );
        return rs;
    }

};

struct k_item{
    vector<int> items;
    dbv bit;


    k_item merge_item( k_item a, k_item b ){

        k_item rs;
        rs.items = a.items;
        rs.items.push_back(b.items.back());
        rs.bit = a.merge_item( a.bit, b.bit );
        return rs;

    }

    void build_1_item(){

    }

};
