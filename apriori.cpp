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

int n, minsup;
float min_sup = 0.9;
vector<set<int>>transactions;

map<int,int>C1;
vector<int>L1;

set<vector<int>>C2;
vector<vector<int>>L2;

vector<vector<int>>Ck;
vector<vector<int>>Lk;

int cal_sup(vector<int>vt){
    int cnt = 0;
    for(set<int>tr: transactions){
        int have_item = 1;
        for(int x: vt){
            have_item = have_item && (tr.find(x)!=tr.end());
            if(!have_item) break;
        }
        cnt += have_item;
    }
    return cnt;
}

vector<int> intersection(vector<int>a, vector<int>b){
    int len = a.size();
    vector<int>c;
    c.pb(a[0]);
    c.pb(b[0]);
    for(int i=1; i<len; i++)
        if(a[i]!=b[i]) {
            c.clear();
            return c;
        }
        else c.pb(a[i]);
    vector<int>itemset({a[0], b[0]});
    if(C2.find(itemset)==C2.end()) c.clear();
    return c;
}

void read_database(){
    string str;
    while(getline(cin, str)){
        int num = 0;
        set<int>tr;
        for(char c: str){
            if ('0'<=c && c<='9') num = num * 10 + (c - '0');
            else if(num) {
                C1[num]++;
                tr.insert(num);
                num=0;
            }
        }
        if(num) {
            C1[num]++;
            tr.insert(num);
            num=0;
        }
        transactions.pb(tr);
        n++;
    }
    minsup = n * min_sup;
}

void solve(){
    // Find L1
    for(auto [u,v]: C1) if (v>=minsup) L1.pb(u);

    // Find L2
    int len_n = L1.size();
    for(int i=0; i<len_n-1; i++){
        for(int j=i+1; j<len_n; j++){
            vector<int>items({L1[i], L1[j]});
            int cnt = cal_sup(items);
            if(cnt>=minsup){
                C2.insert(items);
                L2.pb(items);
            }
        }
    }

    // Find Lk
    Ck = L2;
    while(Ck.size()){
        vector<vector<int>>C;
        int len = Ck.size();
        for(int i=0; i<len-1; i++){
            for(int j=i+1; j<len; j++){
                vector<int>items = intersection(Ck[i], Ck[j]);
                if(items.size() && cal_sup(items)>=minsup){
                    C.pb(items);
                    Lk.pb(items);
                }
            }
        }
        Ck = C;
    }
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("chess.txt", "r", stdin);
//    freopen("code.ans", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    read_database();
    solve();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << L1.size() + L2.size() + Lk.size(), el;
    cout << duration.count() << " ms" , el;
}
