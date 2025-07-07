#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(int i=0; i<int(n); i++)
#define f1(i,n) for(int i=1; i<=int(n); i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
using namespace std::chrono;

struct edbv{
    vector<int>bit_vt;
    int pos;
};

struct item{
    vector<int>itemset;
    edbv item_edbv;

    item(){}

    item(int a, edbv b){
        itemset.pb(a);
        item_edbv = b;
    }

    string get_itemset(){
        string str = "{";
        if(itemset.size()>0){
            str+=to_string(itemset[0]);
            for(int i=1; i<int(itemset.size()); i++) str += "," + to_string(itemset[i]);
        }
        str+="}";
        return str;
    }
};

int n, minsup;
float min_sup = 0.5;
vector<vector<int>>transactions;
vector<item>items, freq;
map<int,vector<int>>lookup;

void init_lookup(){
    f0(i, 65536){
        vector<int>res;
        int val = i;
        for(int cnt=1; cnt<=16 && val>0; cnt++){
            if(val & 1) res.pb(cnt);
            val >>= 1;
        }
        lookup[i] = res;
    }
}

int compute_support(const edbv &a){
    int s = 0;
    for(int x: a.bit_vt) s += lookup[x].size(); // Cong
    return s;
}

edbv intersection(const edbv &a, const edbv &b){
    edbv c;
    int pos = max(a.pos, b.pos); // Find max pos
    int i = (a.pos < b.pos ? b.pos - a.pos : 0);
    int j = (a.pos < b.pos ? 0 : a.pos - b.pos);
    int cnt = (a.bit_vt.size() - i < b.bit_vt.size() - j ?
               a.bit_vt.size() - i: b.bit_vt.size() - j);
//    if(cnt*8<minsup) return edbv();
    while(cnt>0 && (a.bit_vt[i] & b.bit_vt[j])==0){
        i++;
        j++;
        pos++;
        cnt--;
    }
    int i1 = i + cnt - 1;
    int j1 = j + cnt - 1;
    while(cnt>0 && (a.bit_vt[i1] & b.bit_vt[j1])==0){
        i1--;
        j1--;
        cnt--;
    }
//    if(cnt*8<minsup) return edbv();
    c.pos = pos;
    f0(k,cnt) c.bit_vt.pb(a.bit_vt[i++] & b.bit_vt[j++]);

    return c;
}

item intersection(const item &a, const item &b){
    item c;
    // Gan itemset cho c
    c.itemset = b.itemset;
    c.itemset.pb(a.itemset.back());

    // Gan edbv cho c
    c.item_edbv = intersection(a.item_edbv, b.item_edbv);
    return c;
}

void eclat(vector<item>items){
    int n = items.size();
    for(int i=0; i<n; i++){
        freq.pb(items[i]);
        vector<item>new_items;
        for(int j=i+1; j<n; j++){
            item new_item = intersection(items[j], items[i]);
            if(compute_support(new_item.item_edbv)>=minsup)
                new_items.pb(new_item);
        }
        eclat(new_items);
    }
}

void doc(){
    string str, s; // Chuoi mot dong, chuoi tung item

    while(getline(cin, str)){
        stringstream ss(str);

        vector<int>transaction;
        while(getline(ss,s,' ')){
            int x = stoi(s);
            transaction.pb(x);
        }

        transactions.pb(transaction);
        n++;
    }

    minsup = min_sup * n;
}

void build_1_itemset(){
    unordered_map<int,vector<int>>mp;
    int num_bytes = (n+15)/16;

    f0(i,transactions.size()){
        const vector<int> &tr = transactions[i];
        int byte_pos = i/16;
        int bit_pos = i%16;
        for(int x: tr){
            if(mp[x].empty()) mp[x].resize(num_bytes, 0);
            mp[x][byte_pos] |= (1<<(15-bit_pos));
        }
    }

    for(auto &[u,v]: mp){
        int pos = 0;
        while(pos<v.size() && v[pos]==0) pos++;
        if(pos==v.size()) continue;

        edbv edbv_node;
        edbv_node.pos = pos;
        for(int i=pos; i<v.size(); i++) edbv_node.bit_vt.pb(v[i]);

        int sup = compute_support(edbv_node);
        if(sup>=minsup) items.pb(item(u,edbv_node));
    }
}

void solve(){
    // Sap xep cac items theo thu tu sup giam dan
    sort(all(items), [&](const item a, const item b){
        int sup_a = compute_support(a.item_edbv);
        int sup_b = compute_support(b.item_edbv);
        if(sup_a!=sup_b) return sup_a<sup_b;
        return a.itemset[0]>b.itemset[0];
    });


    eclat(items);
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("chess.txt", "r", stdin);
//    freopen("code.txt", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    init_lookup();
    doc();
    build_1_itemset();
    solve();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
//    for(auto &x: freq) cout << x.get_itemset(), el;
    cout << freq.size(), el;
    cout << duration.count() << " ms" , el;
}


