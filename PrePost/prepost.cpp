#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(int i=0; i<n; i++)
#define f1(i,n) for(int i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;

struct node{
    int id, cnt, pre, post;
    node *parent;
    map<int, node*> childs;

    node(int id, node *parent = nullptr): id(id), parent(parent), cnt(1), pre(0), post(0){};
};

struct nlist_node{
    int pre, post, cnt;
};

typedef vector<nlist_node>nlist;

int n, min_sup;
vector<vector<int>>transactions;
map<int, int> mp_cnt_sup;
vector<int>L1; // Tap muc thuong xuyen cap 1
map<int,vector<node*>>mp_nlists;
map<int,nlist>nlists;
vector<pair<vector<int>,int>>frequent;
node *head = new node(-1);

bool cmp(const int &a, const int &b){
    int mp_a = mp_cnt_sup[a];
    int mp_b = mp_cnt_sup[b];
    if(mp_a!=mp_b) return mp_a>mp_b;
    return a<b;
}

void doc(){
    cin >> n >> min_sup; cin.ignore();
    f0(i,n){
        string str; getline(cin, str);
        stringstream ss(str);
        string s;
        vector<int>transaction;
        while(getline(ss,s,' ')) {
            int val = stoi(s);
            transaction.pb(val);
            mp_cnt_sup[val]++; // tinh gia tri support
        }
        transactions.pb(transaction);
    }

    // Tim L1
    for(auto [u,v]: mp_cnt_sup) if(v>=min_sup) L1.pb(u);
    sort(all(L1), [&](int a, int b){
        int mp_a = mp_cnt_sup[a];
        int mp_b = mp_cnt_sup[b];
        if(mp_a!=mp_b) return mp_a>mp_b;
        return a<b;
    });

    // Tim thu tu cua cac phan tu trong L1
    map<int,int>mp_order_L1;
    f0(i,int(L1.size())) mp_order_L1[L1[i]] = i;

    // Sap xep transaction
    for(auto &transaction: transactions){
        vector<int>vt_fill;
        for(int x: transaction) if(mp_cnt_sup[x]>=min_sup) vt_fill.pb(x);
        sort(all(vt_fill), [&](int a, int b){
            return mp_order_L1[a]<mp_order_L1[b];
        });
        transaction = vt_fill;
    }
}

node *insert_to_tree(node *curr, int id){
    if(curr->childs.count(id)) curr->childs[id]->cnt++;
    else{
        node *new_node = new node(id, curr);
        mp_nlists[id].pb(new_node);
        curr->childs[id] = new_node;
    }
    return curr->childs[id];
}

void dfsPrePost(node *curr, int &idx){
    curr->pre = idx++;
    for(auto [u,v]: curr->childs) dfsPrePost(v, idx);
    curr->post = idx++;
}

void build(){
    // Xay dung cay ppc
    for(auto &transaction: transactions) {
        node *curr = head;
        for(int x: transaction) curr = insert_to_tree(curr, x);
    }

    // Tim pre, pos
    int idx = 1;
    node *curr = head;
    dfsPrePost(curr, idx);

    // Tao nlist
    for(auto [u,v]: mp_nlists){
        for(node *x: v)
            nlists[u].pb({x->pre, x->post, x->cnt});
    }
}

int support(nlist nl){
    int cnt = 0;
    for(auto x: nl) cnt += x.cnt;
    return cnt;
}

nlist intersect(nlist a, nlist b){
    nlist c;
    for(nlist_node x: a){
        for(nlist_node y: b){
            if(x.pre<y.pre && x.post>y.post)
                c.pb({y.pre, y.post, min(x.cnt, y.cnt)});
        }
    }
    return c;
}

void prepost(vector<int> prefix, vector<int>items, map<int,nlist>nlists){
    int n = items.size();
    for(int i=0; i<n; i++){
        vector<int>new_prefix = prefix;
        new_prefix.pb(items[i]);
        nlist i_nlist = nlists[items[i]];

        if(support(i_nlist)>=min_sup){
            frequent.pb({new_prefix, support(i_nlist)});

            vector<int>suffix;
            map<int,nlist>new_nlists = nlists;
            for(int j=i+1; j<n; j++){
                nlist j_nlist = nlists[items[j]];
                nlist giao = intersect(i_nlist, j_nlist);
                if(support(giao)>=min_sup){
                    new_nlists[items[j]] = giao;
                    suffix.pb(items[j]);
                }
            }

            prepost(new_prefix, suffix, new_nlists);
        }
    }
}

void printresult(){
    for(auto &x: frequent){
        vector<int>fillter = x.first;
        sort(all(fillter));
        x.first = fillter;
    }
    sort(all(frequent));
    for(auto [u,v]: frequent){
        int cnt;
        string str = "{";
        cnt = v;
        if(u.size()>0){
            str += to_string(u[0]);
            for(int i=1; i<u.size(); i++)
                str += "," + to_string(u[i]);
        }
        str+="}";
        cout << str << " => " << cnt, el;
    }
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    doc();
    build();
    prepost({}, L1, nlists);
    printresult();
}


