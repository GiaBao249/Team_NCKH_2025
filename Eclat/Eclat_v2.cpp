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
    vector<string>name;
    vector<ll>tids;

    item(){}
    item(vector<string>name, vector<ll>tids){
        this->name = name;
        this->tids = tids;
    }

    void print(){
        string str = "{";
        ll n = name.size();
        if(n>0){
            str += name[0];
            for(ll i=1; i<n; i++) str += "," + name[i];
        }
        str += "}";
        cout << str << " => ";
        for(ll x: tids) cout << x << ' ';
        el;
    }
};

ll n, min_sup;
deque<pair<string, vector<ll>>>items; // First List Item ==>> only one item
vector<item>frequent; // List Item Frequency

vector<ll> giao(vector<ll>a, vector<ll>b){
    vector<ll>res;
    ll m = a.size(), n = b.size(), i = 0, j = 0;
    while(i<m && j<n){
        if(a[i]==b[j]) {
            res.pb(a[i]);
            i++;
            j++;
        }
        else if(a[i]<b[j]) i++;
        else j++;
    }
    return res;
}

void doc(){
    cin >> n >> min_sup; cin.ignore();
    map<string, vector<ll>> mp_item;
    f1(i,n){
        string str; getline(cin, str);
        stringstream ss(str);
        string s;

        while(getline(ss,s,',')) if(s!=""){
            mp_item[s].pb(i);
        }
    }

    for(auto [u,v]: mp_item){
        vector<ll>vt_v = v;
        items.pb({u, vt_v});
    }
}

void eclat(const item &prefix, const deque<pair<string, vector<ll>>> &items){
    ll n = items.size();
    deque<pair<string, vector<ll>>> new_items = items;
    for(ll i = 0; i < n; i++) {
        item new_prefix = prefix;
        if(new_prefix.name.size()==0) new_prefix = item(vector<string>{items[i].first}, items[i].second);
        else{
            new_prefix.tids = giao(new_prefix.tids, items[i].second);
            new_prefix.name.pb(items[i].first);
        }

        new_items.pop_front();
        if(ll(new_prefix.tids.size())>=min_sup){
            frequent.pb(new_prefix);
            eclat(new_prefix, new_items);
        }
    }
}

void solve_eclat(){
    item prefix;
    eclat(prefix, items);

    // In ket qua
    cout << "Danh sach tap muc thuong xuyen: ", el;
    for(item x: frequent) x.print();
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh_2.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo
    doc();
    solve_eclat();
}


