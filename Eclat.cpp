#include<bits/stdc++.h>
using namespace std;

// the friquent item data
struct node{
    vector<int> item;
    vector<int> tid;
    double sp;
};

vector<node> fri_item;
map< vector<int> , double > con; // save friquent items to calculate confidence

// print confidence
void print_confidence(vector<int> a_b, int  b, double sp){
    cout<<"{ ";

    for(int i=0;i<a_b.size();++i)
        if(a_b[i] != b) cout<<a_b[i]<<"  ";

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
            if (i < x.item.size()-1) cout << ", ";
        }
        cout << " }  support = "  << x.sp << "\n";
    }
}

// build the vertical transactions
map<int, vector<int>> build(vector<vector<int> > transaction){
    map<int, vector<int> > rs;

    for(int i=0;i<transaction.size();++i)
        for(auto j:transaction[i])
            rs[j].push_back(i+1);

return rs;
}

// build the friquent 1-item
vector<node> build_1( map< int, vector<int> > tran, int n, double MinSp){
    vector<node> rs; // save friquent 1-items
    for(auto i:tran){
        node tmp; // the temporery friquent 1-item
        tmp.item = {i.first};
        tmp.sp = (double) i.second.size()/n ;
        tmp.tid = i.second;
        if( tmp.sp >= MinSp){
            rs.push_back(tmp);
            fri_item.push_back(tmp);
        }
        con[tmp.item] = tmp.sp;
    }
    print(rs);
    return rs;
}

// build the tid of the friquent (k+1)-item from two friquent k-items
vector<int> cal_tid(vector<int> a, vector<int> b){

    vector<int> rs;
    map<int,int> tmp;

    for(auto i:a)++tmp[i];

    for(auto i:b)
        if(tmp[i])
            rs.push_back(i);

    return rs;
}

// creating a item from two items.
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

// the main function of Eclat algorithm
void Eclat(vector<node> lk, double MinSp, int n){

    // if the friquent k-item is empty, the function will stop
    if(lk.empty())return ;


    vector<node> n_lk;  // the friquent (k+1)-item
    map<vector<int> , int > cnt;
    bool f=1;

    // build the friquent (k+1)-item
    for (int i=0;i<lk.size()-1;++i){
        for (int j=i+1;j<lk.size();++j){
            node tmp;
            tmp.item = merge_item(lk[i].item, lk[j].item);

            if( !tmp.item.size()  || cnt[tmp.item] )continue;
            cnt[tmp.item]=1;

            tmp.tid = cal_tid( lk[i].tid, lk[j].tid );
            tmp.sp = (double) tmp.tid.size()/n;
            con[tmp.item] = tmp.tid.size();

            if (tmp.sp >=MinSp ) {
                n_lk.push_back(tmp);
                fri_item.push_back(tmp);
                f=0;
            }

        }
    }

    if(!f)print(n_lk);
    Eclat(n_lk,MinSp,n);
}

// calculating the confidence
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

// the main function of confidence
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
    }
    cout << "nhap min Support: ";
    double MinSp;
    cin >> MinSp;

    Eclat(  build_1( build( transaction), n, MinSp ) , MinSp, n );
    confidence(MinSp,n);


    return 0;
}

