#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(int i=0; i<n; i++)
#define f1(i,n) for(int i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
using namespace std::chrono;

unordered_map<int,float> w_item;
unordered_map<int,float> w_ts;

struct PPCode{
    int pre, post;
    float weight;
};

struct WNList{
    vector<int>items;
    vector<PPCode>ppcs;
    float weight;

    WNList(){weight = 0.0;}
    WNList(vector<int>items){this->items = items; weight = 0.0;}

    string get_item(){
        string str = "{";
        if(items.size()>0){
            str+=to_string(items[0]);
            for(int i=1; i<items.size(); i++) str += "," + to_string(items[i]);
        }
        str+="}";
        return str;
    }

    bool operator==(const WNList &x) const{
        return items == x.items;
    }
};

struct Node{
    int id, pre, post;
    float weight;
    map<int,Node*>children;

    Node(){ id = pre = post = -1; weight = 0.0;}
    Node *insert_by_id(int id, int ts){
        if(children.find(id)!=children.end()){
            children[id]->weight += w_ts[ts];
        }
        else{
            Node *new_child = new Node();
            new_child->id = id;
            new_child->weight = w_ts[ts];
            children[id] = new_child;
        }

        return children[id];
    }
};

int n, idPre, idPost; // So giao dich trong csdl, nguong toi thieu, Pre, Post
float minsup;
float min_sup = 0.5; // Nguong toi thieu voi 0<x<1
float sum_ts = 0.0;
vector<vector<int>>transactions; // Danh sach cac giao dich
Node *root = new Node();

unordered_map<int,float>sup_1_itemset;
unordered_map<int,WNList>mp_nlists;
vector<WNList>F1, F2; // Tap muc thuong xuyen cap i
vector<WNList>freq;

// Bo bien de sinh
vector<int>vt_sinh;
vector<vector<int>>vt_sinh_list;
int n_sinh;

void dfs(int u){
    for(int i=0; i<=1; i++) {
        vt_sinh.pb(i);
        if(u==n_sinh) vt_sinh_list.pb(vt_sinh);
        else dfs(u+1);
        vt_sinh.pop_back();
    }
}

void Sinh_Tap(const vector<int>& itemset, const vector<int>& candidate, float w_base){
    n_sinh = candidate.size();
    vt_sinh_list.clear();
    vt_sinh.clear();
    dfs(1);

    for(const vector<int>& x: vt_sinh_list){
        vector<int> cand = itemset;
        f0(i,n_sinh) switch(x[i]){
            case 1: cand.pb(candidate[i]); break;
        }
        if(cand!=itemset){
            WNList wn(cand);
            wn.weight = w_base;
            freq.pb(wn);
        }
    }
}

void dfs_pre_post(Node *curr){
    curr->pre = idPre++;
    for(auto [u,v]: curr->children) dfs_pre_post(v);
    curr->post = idPost++;
}

void dfs_build_nlists(Node *curr){
    if(curr!=root){
        WNList &nlist = mp_nlists[curr->id];
        if(nlist.items.empty())
            nlist.items.pb(curr->id);

        nlist.ppcs.pb({curr->pre, curr->post, curr->weight});
        nlist.weight += curr->weight;
    }
    for(auto [u,v]: curr->children) dfs_build_nlists(v);
}

WNList intersection(const WNList &a, const WNList &b){
    WNList c;
    int i = 0, j = 0;
    while(i<(int)a.ppcs.size() && j<(int)b.ppcs.size()){
        if(a.ppcs[i].pre < b.ppcs[j].pre){
            if (a.ppcs[i].post > b.ppcs[j].post){
                if(c.ppcs.size() && c.ppcs.back().pre == a.ppcs[i].pre){
                    c.ppcs.back().weight += b.ppcs[j].weight;
                }
                else{
                    c.ppcs.pb({a.ppcs[i].pre, a.ppcs[i].post, b.ppcs[j].weight});
                }
                c.weight += b.ppcs[j].weight;
                j++;
            } else i++;
        } else j++;
    }
    c.items = b.items;
    c.items.pb(a.items.back());
    return c;
}

int eclat_depth = 0;

void eclat(vector<WNList>vt_nlists, WNList nl_check){
    int n = vt_nlists.size();
    for(int i=0; i<n; i++) {
        if(vt_nlists[i]==nl_check){
            break;
        }
        freq.pb(vt_nlists[i]);
        vector<WNList>new_vt_nlists;
        vector<int>equivalent_item;
        WNList nl_equivalent_item;

        for(int j=i+1; j<n; j++){
            WNList P = intersection(vt_nlists[j], vt_nlists[i]);
            if(P.weight/sum_ts>=minsup){
                const float EPS = 1e-9f;
                if (fabs(P.weight - vt_nlists[i].weight) <= EPS) {
                    equivalent_item.pb(vt_nlists[j].items.back());
                    if(nl_equivalent_item.items.size()==0) nl_equivalent_item = P;
                }
                else{
                    equivalent_item.clear();
                    nl_equivalent_item = WNList();
                }
                new_vt_nlists.pb(P);
            }
        }
        eclat_depth++;
        eclat(new_vt_nlists, nl_equivalent_item);
        eclat_depth--;
        if(equivalent_item.size())
            Sinh_Tap(vt_nlists[i].items, equivalent_item, nl_equivalent_item.weight);
    }
}

void calculate_sup_1_item(){
    for(int ts=0; ts<n; ts++){
        for(int x : transactions[ts]){
            sup_1_itemset[x] += w_ts[ts+1]/sum_ts;
        }
    }

    vector<pair<int,float>> supv(sup_1_itemset.begin(), sup_1_itemset.end());
    sort(supv.begin(), supv.end(), [](auto &L, auto &R){
        if(L.second!=R.second) return L.second>R.second;
        return L.first<R.first;
    });
}

void doc(){
    string str, s; // Chuoi mot dong, chuoi tung item
    int tid=1;

    while(getline(cin, str)){
        if(str.size()==0) continue;
        stringstream ss(str);
        float sum_witem = 0.0;
        vector<int>transaction;
        while(getline(ss,s,' ')){
            if(s.size()==0) continue;
            int x = stoi(s);
            transaction.pb(x);
            sum_witem += w_item[x];
        }
        w_ts[tid] = transaction.empty()? 0.0f : sum_witem/(float)transaction.size();
        sum_ts += w_ts[tid++];
        transactions.pb(transaction);
        n++;
    }
    minsup = min_sup;
    calculate_sup_1_item();
    // Loc va sap xep lai cac giao dich
    for(auto &transaction: transactions){

        vector<int>tr;

        // Loc cac itemset >= minsup
        for(int &x: transaction) if(sup_1_itemset[x]>=minsup) tr.pb(x);

        // Sap xep lai transaction theo thu tu cua F1
        sort(all(tr), [&](const int a, const int b){
             if(sup_1_itemset[a]!=sup_1_itemset[b]) return sup_1_itemset[a]>sup_1_itemset[b];
             return a<b;
        });

        transaction = tr;
    }
}

void doc_2(){
    int n_w, tmp_item;
    float item_w, tmp_w;
    cin >> n_w;
    f0(i, n_w){
        cin >> tmp_item >> tmp_w;
        w_item[tmp_item] = tmp_w;
    }
}

void build_tree(){
    // Xay dung cay PPC
    int ts = 1;
    for(auto &transaction: transactions){
        Node *curr = root;
        for(int x: transaction) {
            curr = curr->insert_by_id(x, ts);
        }
        ts++;
    }

    // De quy tim Pre, Post
    idPre = idPost = 0;
    dfs_pre_post(root);
}

void build_1_itemset(){
    // Tao mp_nlists
    dfs_build_nlists(root);

    // Chuyen mp_nlists => F1
    for(auto &[u,v]: mp_nlists) {
        F1.pb(v);
    }

}

void solve(){
    // Sap xep lai F1
    sort(all(F1), [&](const WNList a, const WNList b){
        if(sup_1_itemset[a.items[0]]!=sup_1_itemset[b.items[0]])
            return sup_1_itemset[a.items[0]]<sup_1_itemset[b.items[0]];
        return a.items[0]>b.items[0];
    });

    // Goi eclat
    eclat(F1, WNList());
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("C:\\Users\\Acer\\.templateengine\\Research_science\\test.txt", "r", stdin);
//  freopen("code.ans", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    doc_2(); // Doc trong so cua cac item
    doc(); // Doc transaction trong csdl
    build_tree();
    build_1_itemset();
    solve();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << freq.size(), el;
    cout << duration.count() << " ms" , el;

}
