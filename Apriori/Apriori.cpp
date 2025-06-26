#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(ll i=0; i<n; i++)
#define f1(i,n) for(ll i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;

struct item{
    // Variable
    set<string>se;

    // Constructor
    item(){}
    item(string str){ add(str); }
    item(vector<string>vt){ for(string x: vt) add(x); }

    // Operator
    bool operator<(const item &o)const{ return se < o.se; }

    // Function
    void add(string str){ se.insert(str); }
    string display(){
        string str="{";
        if(se.size()>0){
            for(string x: se) str+= x+",";
            str.pop_back();
        }
        str+="}";
        return str;
    }

};

const ll maxN = 1e4+7;

ll n;
map<string,ll> a[maxN]; // De bai
set<item>se_all_item, L, L_k;
map<item,ll>mp_support;
double min_sup;

item try_merge(item it_a, item it_b){
    item it;
    for(string x: it_a.se) it.add(x);
    for(string x: it_b.se) it.add(x);

    // Ngoai tru truong hop chi co 2 phan tu
    if(it_a.se.size()==1 && it.se.size()==2) return it;

    // Kiem tra dieu kien item
    if(it.se.size()-1!=it_a.se.size()) return item();

    // Kiem tra apriori
    vector<string>vt;
    for(string x: it.se) {
        if(it_a.se.find(x)==it_a.se.end() || it_b.se.find(x)==it_b.se.end()){
            vt.pb(x);
        }
    }
    if(L.find(item(vt))==L.end()) return item();

    return it;
}

vector<item> convert_to_vector(set<item>se){
    vector<item>vt;
    for(auto x: se) vt.pb(x);
    return vt;
}
vector<string> convert_to_vector(item it){
    vector<string>vt;
    for(auto x: it.se) vt.pb(x);
    return vt;
}

ll cal_support(item it){
    if(mp_support.find(it)==mp_support.end()){
        ll cnt = 0;
        f1(i,n){
            ll res = 1;
            for(string x: it.se){
                if(a[i][x]==0) {
                    res=0;
                    break;
                }
            }
            cnt+=res;
        }
        mp_support[it] = cnt;
    }
    return mp_support[it];
}

void doc(){
    cin >> n >> min_sup; cin.ignore();
    f1(i,n){
        string str; getline(cin, str);
        stringstream ss(str);
        string s;

        while(getline(ss,s,',')) if(s!=""){
            a[i][s]=1;
            se_all_item.insert(item(s));
        }
    }
}

void init(){
    for(auto x: se_all_item){
        double db = cal_support(x)*1.0/n;
        if(db>=min_sup) L_k.insert(x);
    }
    L = L_k;
}

void lap(){
    set<item>L_temp;
    vector<item>vt = convert_to_vector(L_k);

    ll n_vt = vt.size();
    for(ll i=0; i<n_vt-1; i++){
        for(ll j=i+1; j<n_vt; j++){
            item it = try_merge(vt[i], vt[j]);
            if(it.se.empty()) continue;

            // Tim L_k
            double db = cal_support(it)*1.0/n;
            if(db>=min_sup) L_temp.insert(it);
        }
    }

    L_k = L_temp;
    for(auto x: L_k) L.insert(x);
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    doc(); // Doc file
    init(); // Khoi tao L
    while(L_k.size()>0) lap(); // Lap den khi L_k rong
    cout << "Danh sach L (Tap muc pho bien): ", el;

    for(auto x: L) cout << x.display() << ' ' << mp_support[x], el;
}
