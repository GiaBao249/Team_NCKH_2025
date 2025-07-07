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

struct dbv{
    vector<int>bit_vt;
    int pos;
};

struct item{
    vector<int>itemset;
    dbv item_dbv;

    item(){}

    item(int a, dbv b){
        itemset.pb(a);
        item_dbv = b;
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

int n, minsup, lookup[256];
float min_sup = 0.5;
vector<vector<int>>transactions;
vector<item>items, freq;

void init_lookup(){
    f0(i, 256){
        int val = i, cnt = 0;
        while(val){
            cnt += (val & 1);
            val >>= 1;
        }
        lookup[i] = cnt;
    }
}

int compute_support(const dbv &a){
    int s = 0;
    for(int byte: a.bit_vt) s += lookup[byte & 255]; // Chi dung 8 bit cuoi
    return s;
}

dbv intersection(const dbv &a, const dbv &b){
    dbv c;
    int pos = max(a.pos, b.pos); // Find max pos
    int i = (a.pos < b.pos ? b.pos - a.pos : 0);
    int j = (a.pos < b.pos ? 0 : a.pos - b.pos);
    int cnt = (a.bit_vt.size() - i < b.bit_vt.size() - j ?
               a.bit_vt.size() - i: b.bit_vt.size() - j);
    if(cnt*8<minsup) return dbv();
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
    if(cnt*8<minsup) return dbv();
    c.pos = pos;
    f0(k,cnt) c.bit_vt.pb(a.bit_vt[i++] & b.bit_vt[j++]);

    return c;
}

item intersection(const item &a, const item &b){
    item c;
    c.itemset = b.itemset;
    c.itemset.pb(a.itemset.back());
    c.item_dbv = intersection(a.item_dbv, b.item_dbv);
    return c;
}

void eclat(vector<item>items){
    int n = items.size();
    for(int i=0; i<n; i++){
        freq.pb(items[i]);
        vector<item>new_items;
        for(int j=i+1; j<n; j++){
            item new_item = intersection(items[j], items[i]);
            if(compute_support(new_item.item_dbv)>=minsup)
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
    int num_bytes = (n+7)/8;

    f0(i,transactions.size()){
        const vector<int> &tr = transactions[i];
        int byte_pos = i/8;
        int bit_pos = i%8;
        for(int x: tr){
            if(mp[x].empty()) mp[x].resize(num_bytes, 0);
            mp[x][byte_pos] |= (1<<(7-bit_pos));
        }
    }

    for(auto &[u,v]: mp){
        int pos = 0;
        while(pos<v.size() && v[pos]==0) pos++;
        if(pos==v.size()) continue;

        dbv dbv_node;
        dbv_node.pos = pos;
        for(int i=pos; i<v.size(); i++) dbv_node.bit_vt.pb(v[i]);

        int sup = compute_support(dbv_node);
        if(sup>=minsup) items.pb(item(u,dbv_node));
    }
}

void solve(){
    // Sap xep cac items theo thu tu sup giam dan
    sort(all(items), [&](const item a, const item b){
        int sup_a = compute_support(a.item_dbv);
        int sup_b = compute_support(b.item_dbv);
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


