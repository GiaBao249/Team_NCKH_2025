#include<bits/stdc++.h>
using namespace std;

map<vector<int>, int> fri_item;
map<int, int> a_item;

// print friquent item
void print(vector<int> lk) {
    cout<<"{";
    for(int i=0;i<lk.size()-1;++i)cout<<lk[i]<<", ";
    cout<<lk[lk.size()-1];
    cout<<"}  ";
}

//print confidence
void print_confidence(vector<int> a_b, int  b, double con){
    cout<<"{";

    for (int i=0;i<a_b.size();++i)
        if (a_b[i] != b) cout<<a_b[i]<<"  ";

    cout << "}   ---> " << b << "     Confidence : "<<con;
    cout << '\n';
}

// check the small vector is the subvector of the big ot not
bool IsSub(vector<int> sub, vector<int> transaction){
    return includes(transaction.begin(), transaction.end(), sub.begin(), sub.end());
}

// build the friquent (k+1)-item from the friquent k-item
vector< vector<int> > merge_item(vector<vector<int> > lk){
	vector<vector<int> > rs;
    int k=lk[0].size();

    for(int i=0;i<lk.size()-1;++i)
    	for(int j=i+1;j<lk.size();++j){
    		vector<int> tmp1 = lk[i];
    		vector<int> tmp2 = lk[j];
    		bool f=1;
    		for(int l=0;l<k-1;++l)
    			if(tmp1[l] != tmp2[l]){
    				f=0;
    				break;
				}

			if(f && tmp1[k-1] < tmp2[k-1]){
				tmp1.push_back(tmp2[k-1]);
				rs.push_back(tmp1);
			}
		}

    return rs;

}

// build the friquent 1-item from transactions and the min support
vector<vector<int> > build_l1(vector< vector<int> > transactions,  int MinCount){
        map<int,int> count_item;

        for(auto i:transactions)
            for(auto j:i)
                ++count_item[j];

        vector<vector<int> > l1;
        for(auto i:count_item)
            if(i.second >= MinCount){
                l1.push_back({i.first});
                fri_item[{i.first}] = i.second;
            }

        for(auto i:l1){
            print(i);
            double sp = (double) count_item[i[0]] / transactions.size();
            cout << "Support: " << sp << '\n';
        }

        return l1;
}

//calculating the appearance time of an k-item
int cal_sp_each_item(vector<int> lk, vector< vector<int> > transactions){
        int rs=0;
        for(auto i:transactions){
			if(IsSub(lk,i))++rs;
		}
		return rs;
}

//calculating confidence
double cal_confidence(vector<int> a_b, int b, vector< vector<int> > transactions){
    if(a_b.size() <= 1)return 0;
    vector<int> a;

    for(auto i:a_b)
        if(i != b) a.push_back(i);


    double sp_a_b = (double)cal_sp_each_item(a_b,transactions)/transactions.size();
    double sp_a = (double)cal_sp_each_item(a,transactions)/transactions.size();
    if(!sp_a)return 0;
    return (double) sp_a_b/sp_a;
}

// create the association rule from the friquent item
void confidence( vector< vector<int> > transactions, double MinSupport){

    for(auto i:fri_item)
        for(auto j:i.first){

            double con = cal_confidence(i.first, j, transactions);
            if(con >= MinSupport) print_confidence(i.first, j, con);

        }

}

// the main function of Apriori algorithm
void Apriori(vector< vector<int> > lk,vector< vector<int> > transactions, int MinCount){

    if(lk.empty())return;
        int k = lk[0].size();

		vector<vector<int> > ck=merge_item(lk);

		if(ck.empty())return;

		map < vector<int> , int > count_k_item;

		for(auto i:ck){
            count_k_item[i] = cal_sp_each_item(i,transactions);
		}
		lk.clear();

		for(auto i:count_k_item){

			if(i.second >= MinCount){
				lk.push_back(i.first);
				fri_item[i.first] = i.second;
			}
		}

		if(!lk.empty()){
			cout << "mau thu "<<k+1<<"\n";
			for(auto i:lk){
				print(i);
				double sp = (double)count_k_item[i]/transactions.size();
				cout << "Support: " << sp << '\n';
			}
		}

		Apriori(lk,transactions,MinCount);
}




int main(){
	int n;
	cout << "nhap so lan giao dich: ";
	 cin >> n;
	vector< vector<int> > transactions(n),l1(n);
	for(int i=0;i<n;++i){
		int ni;
		cout << "nhap so luong hang: ";
		cin >> ni;
		cout << "nhap giao dich thu " << i+1 <<"[1,9]: ";

		for(int j=0;j<ni;++j){
			int tmp;
			cin >> tmp;
			transactions[i].push_back(tmp);
		}
		sort(transactions[i].begin(),transactions[i].end());
	}
	double MinSupport;
    cout<<"Nhap nguong do ho tro toi thieu [0,1]: ";
    cin>>MinSupport;
    int MinCount = MinSupport*n;

    Apriori(build_l1(transactions,  MinCount),transactions, MinCount);
    confidence(transactions,MinSupport);

	return 0;
}
