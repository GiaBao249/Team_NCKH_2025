#include <bits/stdc++.h>
using namespace std;

struct node_tree {
    int id, cnt, pre, post;
    node_tree* parent;
    map<int, node_tree*> child;
    node_tree(int id, node_tree* parent = nullptr): id(id), parent(parent), cnt(1), pre(0), post(0) {}
};

struct n_list {
    int pre, post, cnt;
};

struct frequent_item {
    vector<int> item;
    int sp;
};

int min_sp, n;
node_tree* root=new node_tree(-1);
map<int, vector<n_list>> rs_n_list;
vector<frequent_item> result;

vector<pair<int, int>> calculate_support_1_item(const vector<vector<int>>& transaction) {
    map<int, int> m_cnt;
    vector<pair<int, int>> rs;

    for (const auto& i:transaction)
        for (int j:i)
            m_cnt[j]++;

    for (auto& [item, cnt]:m_cnt)
        if (cnt >= min_sp)
            rs.push_back({cnt, item});

    sort(rs.rbegin(), rs.rend());
    return rs;
}

void sort_transaction(vector<vector<int>>& transaction, const map<int, int>& order) {
    for (auto& i:transaction) {
        vector<int> tmp;

        for (int j:i)
            if (order.count(j))
                tmp.push_back(j);

        sort(tmp.begin(), tmp.end(), [&](int x, int y) {
            return order.at(x) < order.at(y);
        });

        i=tmp;
    }
}

vector<int> pre_build_tree(vector<vector<int>>& transaction) {

    vector<pair<int, int>> l1=calculate_support_1_item(transaction);
    vector<int> l2;
    map<int, int> l1_sort;

    for (int i=0;i<l1.size();++i) {
        l1_sort[l1[i].second]=i;
        l2.push_back(l1[i].second);
    }

    sort_transaction(transaction, l1_sort);
    return l2;
}

node_tree* insert_node(node_tree* cur, int id) {
    if (cur->child.count(id)) cur->child[id]->cnt+=1;
    else cur->child[id]=new node_tree(id, cur);

    return cur->child[id];
}

int pp=1;
void calculate_pre_post(node_tree* node) {
    node->pre=pp++;

    for (auto& [i, child]:node->child)
        calculate_pre_post(child);

    node->post=pp++;
}

void take_n_list(node_tree* node) {
    if (node->id!=-1) {
        n_list tmp{node->pre, node->post, node->cnt};
        rs_n_list[node->id].push_back(tmp);
    }

    for (auto& [_, child]:node->child)
        take_n_list(child);
}

void build_tree(const vector<vector<int>>& transaction) {

    for (const auto& i:transaction) {
        node_tree* cur=root;
        for (int id : i)
            cur=insert_node(cur, id);
    }

    calculate_pre_post(root);
    take_n_list(root);
}

vector<n_list> intersect_n_list(const vector<n_list>& a, const vector<n_list>& b) {
    vector<n_list> rs;
    int i=0,j=0;

    while( i<a.size() && j<b.size())
        if (a[i].pre<b[j].pre && a[i].post>b[j].post){
                rs.push_back({b[j].pre, b[j].post, min(a[i].cnt, b[j].cnt)});
                ++j;
        }
        else if (a[i].pre>=b[j].pre) ++j;
        else ++i;

    return rs;
}

int support(const vector<n_list>& item) {
    int total=0;
    for (const auto& i:item)
        total+=i.cnt;
    return total;
}

void ppc(const vector<int>& prefix, const vector<int>& item, map<int, vector<n_list>> l) {
    for (int i=0; i < item.size(); ++i) {
        vector<int> tmp_prefix=prefix;
        tmp_prefix.push_back(item[i]);
        vector<n_list> i_item = l[item[i]];
        int tmp_sp = support(i_item);

        if (tmp_sp >= min_sp) {
            result.push_back({tmp_prefix, tmp_sp});

            vector<int> new_item;
            map<int, vector<n_list>> new_l=l;

            for (int j=i+1; j<item.size(); ++j) {
                vector<n_list> j_item=l[item[j]];
                vector<n_list> inter=intersect_n_list(i_item, j_item);
                int sp=support(inter);

                if (sp>=min_sp) {
                    new_l[item[j]]=inter;
                    new_item.push_back(item[j]);
                }
            }

            ppc(tmp_prefix, new_item, new_l);
        }
    }
}

void print_result() {

    for (const auto& fi:result) {
        cout << "{";
        for (int j=0;j<fi.item.size();++j){
            cout << fi.item[j];
            if (j+1 < fi.item.size())
                cout << ",";
        }
        cout<<"} => "<<fi.sp<<"\n";
    }

}

int main(){
    cout << "nhap n: ";
    cin >> n;
    vector<vector<int>> transaction(n);

    for (int i=0;i<n;++i) {
        int k;
        cout << "nhap so luong item " << i + 1 << ": ";
        cin >> k;
        transaction[i].resize(k);
        cout << "nhap cac item: ";
        for (int j=0; j<k;++j)
            cin >> transaction[i][j];
    }

    cout << "Enter minimum support: ";
    cin >> min_sp;

    vector<int> order=pre_build_tree(transaction);
    build_tree(transaction);
    ppc({}, order, rs_n_list);
    print_result();

    return 0;
}

/*
6
4
1 2 4 5
3
2 3 5
4
1 2 4 5
4
1 2 3 5
5
1 2 3 4 5
3
2 3 4
3

10
3
1 2 3
3
1 2 4
3
2 3 5
4
1 3 4 5
3
2 4 5
4
1 2 3 5
4
2 3 4 5
4
1 2 4 5
3
1 3 5
3
2 3 4
3
*/
