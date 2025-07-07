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

struct PPCode{
    int pre, post, cnt;
};

struct NList{
    vector<int>items;
    vector<PPCode>ppcs;
    int cnt;

    NList(){cnt = 0;}

    string get_item(){
        string str = "{";
        if(items.size()>0){
            str+=to_string(items[0]);
            for(int i=1; i<items.size(); i++) str += "," + to_string(items[i]);
        }
//        str += + ":" + to_string(cnt) + "}";
        str += "}";
        return str;
    }
};

struct Node{
    int id, pre, post, cnt;
    map<int,Node*>children;

    Node(){ id = pre = post = -1; cnt = 1;}
    Node *insert_by_id(int id){
        if(children.find(id)!=children.end()){
            children[id]->cnt++;
        }
        else{
            Node *new_child = new Node();
            new_child->id = id;
            children[id] = new_child;
        }

        return children[id];
    }
};

int n, minsup, idPre, idPost; // So giao dich trong csdl, nguong toi thieu, Pre, Post
float min_sup = 0.7; // Nguong toi thieu vo`i 0<x<1
vector<vector<int>>transactions; // Danh sach cac giao dich
Node *root = new Node();

unordered_map<int,int>sup_1_itemset;
unordered_map<int,NList>mp_nlists;
vector<NList>vt_nlists;
vector<int>F, F1, F2;
vector<NList>freq;

void dfs_pre_post(Node *curr){
    curr->pre = idPre++;
    for(auto [u,v]: curr->children) dfs_pre_post(v);
    curr->post = idPost++;
}

void dfs_build_nlists(Node *curr){
    if(curr!=root){
        NList &nlist = mp_nlists[curr->id];
        if(nlist.items.empty())
            nlist.items.pb(curr->id);

        nlist.ppcs.pb({curr->pre, curr->post, curr->cnt});
        nlist.cnt += curr->cnt;
    }
    for(auto [u,v]: curr->children) dfs_build_nlists(v);
}

NList intersection(const NList &a, const NList &b){
    NList c;
    int i = 0, j = 0;
    while(i<a.ppcs.size() && j<b.ppcs.size()){
        if(a.ppcs[i].pre < b.ppcs[j].pre){
            if (a.ppcs[i].post > b.ppcs[j].post){
                if(c.ppcs.size() && c.ppcs.back().pre == a.ppcs[i].pre){
                    c.ppcs.back().cnt += b.ppcs[j].cnt;
                }
                else{
                    c.ppcs.pb({a.ppcs[i].pre, a.ppcs[i].post, b.ppcs[j].cnt});
                }
                c.cnt += b.ppcs[j].cnt;
                j++;
            } else i++;
        } else j++;
    }
    c.items = b.items;
    c.items.pb(a.items.back());
    return c;
}

void eclat(vector<NList>vt_nlists){
    int n = vt_nlists.size();
    for(int i=0; i<n; i++){
        freq.pb(vt_nlists[i]);
        vector<NList>new_vt_nlists;
        for(int j=i+1; j<n; j++){
            NList nl = intersection(vt_nlists[j], vt_nlists[i]);
            if(nl.cnt>=minsup) new_vt_nlists.pb(nl);
        }
        eclat(new_vt_nlists);
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
            sup_1_itemset[x]++;
        }

        transactions.pb(transaction);
        n++;
    }
    minsup = min_sup * n;

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

    // Xay dung cay PPC
    for(vector<int> &transaction: transactions){
        Node *curr = root;
        for(int &x: transaction) curr = curr->insert_by_id(x);
    }

    // De quy tim Pre, Post
    dfs_pre_post(root);

    // Tao mp_nlists
    dfs_build_nlists(root);

    // Chuyen mp_nlists => vt_nlists
    for(auto &[u,v]: mp_nlists) vt_nlists.pb(v);

    // Sap xep lai vt_nlists
    sort(all(vt_nlists), [&](const NList a, const NList b){
        if(sup_1_itemset[a.items[0]]!=sup_1_itemset[b.items[0]])
            return sup_1_itemset[a.items[0]]<sup_1_itemset[b.items[0]];
        return a.items[0]>b.items[0];
    });

    // Goi eclats
    eclat(vt_nlists);
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("chess.txt", "r", stdin);
    freopen("code1.ans", "w", stdout);
    #endif // demo

    auto start = high_resolution_clock::now();
    doc(); // Doc transaction trong csdl
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    for(NList &x: freq) cout << x.get_item(), el;
    cout << freq.size(), el;
    cout << duration.count() << " ms" , el;
}
