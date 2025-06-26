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

ll n;
double min_sup;
map<item,set<ll>>tid_list, L, L_k;

vector<string> convert_to_vector(item it){
    vector<string>vt;
    for(auto x: it.se) vt.pb(x);
    return vt;
}
vector<item> convert_to_vector(map<item,set<ll>>mp){
    vector<item>vt;
    for(auto [u,v]: mp) vt.pb(u);
    return vt;
}

set<ll>intersection(set<ll>se_a, set<ll>se_b){
    set<ll>se;
    for(ll x: se_a) if(se_b.find(x)!=se_b.end()) se.insert(x);
    return se;
}

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

void doc(){
    cin >> n >> min_sup; cin.ignore();
    f1(i,n){
        string str; getline(cin, str);
        stringstream ss(str);
        string s;
        while(getline(ss,s,',')) if(s!=""){
            item it(s);
            tid_list[it].insert(i);
        }
    }
}

void init(){
    for(auto [u,v]: tid_list){
        double db = v.size()*1.0/n;
        if(db>=min_sup) L[u] = v;
    }
    L_k = L;
}

void Lap(){
    map<item,set<ll>>L_temp;
    vector<item>vt_item = convert_to_vector(L_k);

    ll n_vt_item = vt_item.size();
    for(ll i=0; i<n_vt_item-1; i++){
        for(ll j=i+1; j<n_vt_item; j++){
            item it = try_merge(vt_item[i], vt_item[j]);
            if(it.se.empty()) continue;

            set<ll>se = intersection(L_k[vt_item[i]], L_k[vt_item[j]]);
            double db = se.size()*1.0/n;
            if(db>=min_sup) L_temp[it]=se;
        }
    }

    L_k = L_temp;
    for(auto [u,v]: L_k) L[u] = v;
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    doc(); // Doc file
    init(); // Khoi tao L
    while(L_k.size()>0) Lap(); // Lap den khi L_k rong

    cout << "Danh sach L (Tap muc pho bien): ", el;
    for(auto [u,v]: L){
        item it = u;
        set<ll>se = v;
        cout << it.display() << " => ";
        for(ll x: se) cout << x << ' ';
        el;
    }
}
