#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(int i=0; i<n; i++)
#define f1(i,n) for(int i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
using us_name = set<string>;
using us_tids = set<int>;

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
            mp_item[us_name{x}].insert(i);
        }
    }


}

us_tids intersect(us_tids a, us_tids b){
    us_tids res;
    for(int x: a) if(b.find(x)!=b.end()) res.insert(x);
    return res;
}


void eclat_recursive(const us_name &prefix, const map<us_name, us_tids> &items){
    for(auto it = items.begin(); it!=items.end(); it++){
        us_name new_prefix = prefix;
        new_prefix.insert(it->first.begin(), it->first.end());

        us_tids tid_list = it->second;
        if(tid_list.size()>=min_sup){
            frequent.pb({new_prefix, tid_list});

            map<us_name, us_tids> new_items;
            auto it2 = it; it2++;
            for(auto it3 = it2; it3!=items.end(); it3++){
                us_name new_name = it3->first;
                us_tids new_tids = intersect(tid_list, it3->second);
                if(new_tids.size()>=min_sup) new_items[new_name] = new_tids;
            }

            eclat_recursive(new_prefix, new_items);
        }

    }
}

void eclat(){
    us_name prefix;
    eclat_recursive(prefix, mp_item);
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
