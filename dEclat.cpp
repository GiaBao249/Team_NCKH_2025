#include<bits/stdc++.h>
using namespace std;
// the friquent item data
struct node{
    vector<int> item;
    vector<int> diff;
    int sp;
};

vector<node> fri_item;
map< vector<int> , int > con; // save friquent items to calculate confidence

// print confidence
void print_confidence(vector<int> a_b, int  b, double sp){
    cout<<"{ ";

    for(int i=0;i<a_b.size();++i)
        if(a_b[i] != b)
            cout<<a_b[i]<<"  ";

    cout << "}   ---> " << b << "     Confidence : "<<sp;
cout << '\n';
}

// print friquent item
void print(vector<node> items) {
    cout << "the size of the friquent item is " << items[0].item.size() << '\n' ;
    for (auto x : items) {
        cout << "{ ";
        for (int i = 0; i < x.item.size(); ++i) {
            cout << x.item[i];
            if (i < x.item.size()-1)
                cout << ", ";
        }
        cout << " }  support = "  << x.sp << "\n";
    }
}

// build the vertical transactions
map<vector<int>, vector<int>> build(vector<vector<int> > transaction){
    map<vector<int>, vector<int> > rs;
    for(int i=0;i<transaction.size();++i){
        for(auto j:transaction[i]){
            rs[{j}].push_back(i+1);
        }
    }
return rs;
}

// calculating diffset of friquent 1-items
vector<int> cal_diff_1(vector<int> a, int n){
        vector<int> rs;
        map<int,int> tmp;
        for(auto i:a){
            ++tmp[i];
        }
        for(int i=1;i<=n;++i){
            if(!tmp[i]){
                rs.push_back(i);
            }
        }
        return rs;
}

// build the friquent 1-item
vector<node> build_1( map<vector<int>, vector<int> > tran,int n){
    vector<node> rs;
    for(auto i:tran){
        node tmp;
        tmp.item = i.first;
        tmp.sp =i.second.size();
        tmp.diff = cal_diff_1(i.second,n);
        rs.push_back(tmp);
        fri_item.push_back(tmp);
        con[tmp.item] = tmp.sp;
    }
    print(rs);
    return rs;
}

// build the friquent (k+1)-item from two friquent k-items
vector<int> find_diff(vector<int> a, vector<int> b){

    vector<int> rs;
    map<int,int> tmp;

    for(auto i:a)++tmp[i];

    for(auto i:b)
        if(!tmp[i])
            rs.push_back(i);

    return rs;
}

// creating a item from two items
vector<int> merge_item(vector<int> a, vector<int> b){
    vector<int> rs;
    map<int,int> tmp;
    int cnt =0;
    for(auto i:a)++tmp[i];

    for(int i=0;i<b.size()-1;++i){
            if(a[i] != b[i])return rs;
            // if the first k-1 elements of  A are not the same as the first k-1 elements of B, return a empty vector
    }
    if(a[a.size()-1] < b[b.size()-1]){
        a.push_back(b[b.size()-1]);
        // if the k element of both A and B are not the same. add it to A
    }

    return a;

}

// the main function of dEclat algorithm
void DiffEclat(vector<node> lk, int MinSp, int n){

    if(lk.empty())return ;  // if the friquent k-item is empty, the function will stop
    vector<node> n_lk;// the friquent (k+1)-item
    map<vector<int> , int > cnt;
    bool f=1;

    // build the friquent (k+1)-item
    for (int i=0;i<lk.size()-1;++i)
        for (int j=i+1;j<lk.size();++j){

            node tmp;
            tmp.item = merge_item(lk[i].item, lk[j].item);

            if(tmp.item.size() == 0 || cnt[tmp.item])continue;
            cnt[tmp.item]=1;

            tmp.diff = find_diff(lk[i].diff, lk[j].diff);
            tmp.sp = lk[i].sp - tmp.diff.size();
            con[tmp.item] = tmp.sp;

            if (tmp.sp >=MinSp ) {
                n_lk.push_back(tmp);
                fri_item.push_back(tmp);
                f=0;
            }

        }

    if(!f)print(n_lk);
    DiffEclat(n_lk,MinSp,n);
}


double cal_confidence(vector<int> a_b, int b,int n){
    if(a_b.size() <= 1)return 0;
    vector<int> a;
    for(auto i:a_b)
        if(i != b){
            a.push_back(i);
        }

    double sp_a_b = (double)con[a_b]/n;
    double sp_a = (double)con[a]/n;
    if(!sp_a)return 0;
    return (double) sp_a_b/sp_a;
}
void confidence( double MinSupport,int n){

    for(auto i:con){
        if(i.first.size() == 1)continue;
        for(auto j:i.first){

            double  tmp = cal_confidence(i.first, j,n);
            if(tmp >= MinSupport){
                    print_confidence(i.first, j, tmp);
            }
        }
    }

}

int main() {
    int n;
    cout << "nhap n: ";
    cin >> n;
    vector <vector<int> > transaction(n);

    for(int i=0;i<n;++i){
        int k, tmp;
        cout << "nhap so luong item: ";
        cin >> k;
        cout << " nhap items: ";
        for(int j=0;j<k;++j){
            cin >> tmp;
            transaction[i].push_back(tmp);
        }
        sort(transaction[i].begin(), transaction[i].end());
    }
    cout << "nhap min Support: ";
    double MinSp;
    cin >> MinSp;

    DiffEclat(  build_1( build(transaction),n) , MinSp*n, n);
    confidence(MinSp,n);


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
0.5


*/
