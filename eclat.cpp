#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(int i=0; i<n; i++)
#define f1(i,n) for(int i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
using us_name = vector<string>;
using us_tids = vector<int>;

struct item{
    us_name name;
    us_tids tids;

    item(){}
    item(us_name name, us_tids tids){
        this->name = name;
        this->tids = tids;
    }

    void print(){
        string str = "{";
        int n = name.size();
        if(n>0){
            for(string x: name) str += x + ",";
            str.pop_back();
        }
        str += "}";
        cout << str << " => ";
        for(int x: tids) cout << x << ' ';
        el;
    }
};

int n, min_sup; // So dong, min_support
vector<vector<string>>transactions; // Danh sach tat ca giao dich
vector<item>frequent; // Danh sach tap muc thuong xuyen
map<us_name, us_tids> mp_item; // Danh sach tap muc da chuyen doi sang dang hang ngang
vector<item> vt_item; // Danh sach tap muc da chuyen doi tu mp_item -> vt_item

void doc(){
    cin >> n >> min_sup; cin.ignore(); // Doc n va min_sup
    f0(i,n){ // Doc tat ca giao dich trong file
        string str; getline(cin, str);
        stringstream ss(str);
        string s;

        vector<string>transaction;
        while(getline(ss,s,',')) if(s!=""){
            transaction.pb(s);
        }
        transactions.pb(transaction);
    }

    // Chuyen doi transaction sang mp_item
    f0(i,int(transactions.size())) {
        for(string x: transactions[i]){
            mp_item[us_name{x}].pb(i);
        }
    }

    // Chuyen doi tu mp_item -> vt_item
    for(auto x: mp_item){
        us_name name = x.first;
        us_tids tids = x.second;
        vt_item.pb(item(name, tids));
    }
}

// Tim tap giao
us_tids intersect(vector<int>a, vector<int>b){
    vector<int>res;
    int m = a.size(), n = b.size(), i = 0, j = 0;
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


void eclat_recursive(const us_name &prefix, const vector<item> &items){
    int n = items.size();
    for(int i=0; i<n; i++){
        us_name new_prefix = prefix;
        new_prefix.pb(items[i].name[0]);

        us_tids tid_list = items[i].tids;
        if(int(tid_list.size())>=min_sup){
            frequent.pb({new_prefix, tid_list});

            vector<item> new_items;
            for(int j=i+1; j<n; j++){
                us_name new_name = items[j].name;
                us_tids new_tids = intersect(tid_list, items[j].tids);
                if(int(new_tids.size())>=min_sup) new_items.pb(item(new_name, new_tids));
            }

            eclat_recursive(new_prefix, new_items);
        }
    }
}

void eclat(){
    us_name prefix;
    eclat_recursive(prefix, vt_item);
}

void print_result(){
    cout << "Danh sach tap muc thuong xuyen: ", el;
    for(item x: frequent) x.print();
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    doc();
    eclat();
    print_result();

    return 0;
}
