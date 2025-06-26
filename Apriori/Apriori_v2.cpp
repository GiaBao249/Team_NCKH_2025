#define demo
#include <bits/stdc++.h>
#define el cout << '\n'
#define tm_opt ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define f0(i,n) for(int i=0; i<n; i++)
#define f1(i,n) for(int i=1; i<=n; i++)
#define pb push_back
#define all(a) a.begin(),a.end()
using namespace std;
using item_name = vector<string>;

int n, min_sup, k = 0; // So dong, min_support, do dai item hien tai cua frequent
vector<set<string>>transactions; // Danh sach tat ca giao dich
vector<item_name>frequent; // Danh sach tap muc thuong xuyen
vector<item_name>candidate; // Danh sach tap muc thanh vien (Candidate)
map<item_name, int> mp_item; // Danh sach tap muc da chuyen doi sang dang hang ngang

// Ham nay de hien thi ten cua item
string get_item_name(item_name name){
    string str = "{";
    if(name.size()>0){
        for(string x: name) str += x+",";
        str.pop_back();
    }
    str+="}";
    return str;
}

// Ham doc du lieu trong file
void doc(){
    cin >> n >> min_sup; cin.ignore(); // Doc n va min_sup
    f0(i,n){ // Doc tat ca giao dich trong file
        string str; getline(cin, str);
        stringstream ss(str);
        string s;

        set<string>transaction;
        while(getline(ss,s,',')) if(s!=""){
            transaction.insert(s);
        }
        transactions.pb(transaction);
    }

    // Chuyen transaction thanh mp_item
    for(set<string> transaction: transactions){
        for(string str: transaction){
            mp_item[item_name{str}]++;
        }
    }

    // Chuyen mp_item sang candidate va frequent
    for(auto [u,v]: mp_item) if(v>=min_sup) {
        frequent.pb(u);
        candidate.pb(u);
    }

    // Tang do dai hien tai len 1
    k++;
}

// Kiem tra k-1 phan tu dau co giong nhau khong
bool check_same_k_1(item_name a, item_name b, item_name &c){
    f0(i,k-1) if(a[i]!=b[i]) return false;
    c = a;
    c.pb(b[k-1]);
    return true;
}

// Tim support cua item
int cal_sup(item_name name){
    if(mp_item.find(name)!=mp_item.end()) return mp_item[name];

    int cnt = 0;
    for(set<string>transaction : transactions){
        int tmp = 0;
        // Duyet qua tat ca cac ten nam trong name
        for(string x: name){
            if(transaction.find(x)!=transaction.end()) tmp++;
            else break;
        }
        cnt += (tmp==int(name.size()));
    }
    mp_item[name] = cnt;

    return cnt;
}

// Chay thuat toan apriori
void apriori(){
    int n = candidate.size();
    vector<item_name>tmp;

    for(int i=0; i<n-1; i++){
        for(int j=i+1; j<n; j++){
            item_name new_item;
            if(!check_same_k_1(candidate[i], candidate[j], new_item)) continue;

            if(cal_sup(new_item)>=min_sup){
                frequent.pb(new_item);
                tmp.pb(new_item);
            }
        }
    }

    candidate = tmp;
}

// In ket qua
void print_result(){
    cout << "Danh sach tap muc thuong xuyen: ", el;
    for(item_name x: frequent) cout << get_item_name(x) << " => " << mp_item[x] , el;
}

signed main(){
    tm_opt;
    #ifdef demo
    freopen("sinh.inp", "r", stdin);
    freopen("code.ans", "w", stdout);
    #endif // demo

    doc();
    while(candidate.size()>0) {
        apriori();
        k++;
    }
    print_result();
}


