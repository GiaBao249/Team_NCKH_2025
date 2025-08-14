#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(ll i=0; i<n; i++)
#define f1(i,n) for(ll i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
using namespace std::chrono;

struct item{
    vector<int>itemset;
    vector<int>tid;

    item(){}
    item(int id, vector<int> tid){
        itemset.pb(id);
        this->tid = tid;
    }
};

int minsup, n;
float min_sup = 0.9;

vector<vector<int>>transactions;
map<int,vector<int>>mp_1_itemset;
vector<item>freq, L;

void read_database(){
    string str;
    while(getline(cin, str)){
        n++;
        vector<int>tr;
        int num = 0;
        for(char c: str)
            if('0' <= c && c <= '9') num = num * 10 + (c - '0');
            else if(num>0){
                tr.pb(num);
                mp_1_itemset[num].pb(n);
                num = 0;
            }
        if(num>0) {
            tr.pb(num);
            mp_1_itemset[num].pb(n);
        }
        transactions.pb(tr);
    }
    minsup = min_sup * n;
}

item intersection(const item &a, const item &b){
    item c;
    c.itemset = a.itemset;
    c.itemset.pb(b.itemset[b.itemset.size()-1]);

    int i=0, j=0, m=a.tid.size(), n=b.tid.size();
    while(i<m && j<n){
        if(a.tid[i] == b.tid[j]){
            c.tid.pb(a.tid[i]);
            i++;
            j++;
        }
        else if (a.tid[i] < b.tid[j]) i++;
        else j++;
    }

    return c;
}

void eclat(vector<item>L){
    int n = L.size();
    for(int i=0; i<n; i++){
        freq.pb(L[i]);
        vector<item>new_L;
        for(int j=i+1; j<n; j++){
            item it = intersection(L[i], L[j]);
            if(it.tid.size()>=minsup) new_L.pb(it);
        }
        eclat(new_L);
    }
}

void solve(){
    // Find L1
    for(auto [u, v]: mp_1_itemset)
        if(v.size() >= minsup)
            L.pb(item(u, v));

    // Find Lk
    eclat(L);
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("chess.txt", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    read_database();
    solve();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << freq.size(), el;
    cout << duration.count() << " ms" , el;
}


