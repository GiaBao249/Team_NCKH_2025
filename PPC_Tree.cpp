#include<bits/stdc++.h>
#define el '\n'
#define FOR(i ,a ,n) for(int i = a ; i <= n; i++)
using namespace std;
const int MAXN = 2e5 + 5;
int n , minsup;
struct itemset{
    string tid;
    vector<int> items;
};
itemset database[MAXN];
vector<int> L1;
vector<pair<int , int>> L2;
vector<itemset> f1;
unordered_map<int , int> cnt_freq_f1;
bool out[MAXN]; // xử lí các số có freq < minsup để loại trừ trước khi thêm vào cây
bool cmp(int a , int b){
    if(cnt_freq_f1[a] == cnt_freq_f1[b]){
        return a < b;
    }
    return cnt_freq_f1[a] > cnt_freq_f1[b];
}
// xây dựng cây ppc
struct PPC_Tree{
    int name; // số đại diện cho PPC_Tree
    map<int , PPC_Tree*> child; // lưu trữ PPC_Tree con để đi xuống
    int cnt; // số lượng xuất hiện của phần tử
    int pre; // vị trí được duyệt theo pre-travel
    int post; // vị trí được duyệt theo post-travel
    PPC_Tree(){
        cnt = 0;
        pre = 0;
        post = 0;
    }
    PPC_Tree(int _name , int _cnt , int _pre , int _post){
        name = _name;
        cnt = _cnt;
        pre = _pre;
        post = _post;
    }
};
struct PPC_Code{
    int pre , post , cnt;
};
PPC_Tree *root;
map<int , vector<PPC_Code>> NL1;
// đọc database
int StringToInt(string s){
    int res = 0;
    for(char c : s){
        res = res * 10 + (c - '0');
    }
    return res;
}

void docfile(){
    cin >> n >> minsup;
    //cout << n << ' ' << minsup << el;
    cin.ignore();
    FOR(i ,1 ,n){
        string line;
        getline(cin , line);
        stringstream ss(line);
        string s;
        bool ok = 0;
        while(ss >> s){
            if(!ok) database[i].tid = s , ok = 1;
            else database[i].items.push_back(StringToInt(s));
        }
    }
}
// hàm insert vào cây
void insertPPC(vector<int> u){
    PPC_Tree *p = root;
    for(auto v : u){
        //if(out[v]) continue;
        if(!p->child.count(v)) {
            p->child[v] = new PPC_Tree(v , 0 , 0 , 0);
        }
        p = p->child[v];
        p->cnt++;
    }
}
// tìm tập F1 trong database và tính supports
unordered_set<int> se;
void prepare_F1(){
    // lưu tần suất của các items
    FOR(i , 1 , n) for(auto x : database[i].items){
        cnt_freq_f1[x]++;
    }
    // xử lí đẩy sắp xếp dựa trên cnt_freq_f1 trên từng thành phần tid , lọc theo minsup
    for(auto [x , y] : cnt_freq_f1){
        if(y < minsup && !out[x]){
            out[x] = 1;
        }
    }
    
    // L1 : tập đã lọc ra các thành phần < minsup và đã đc sort
    FOR(i , 1 , n){
        for(auto x : database[i].items){
            if(!out[x] && !se.count(x)){
                L1.push_back(x);
                se.insert(x);
            }
        }
    }
    
    // Sắp xếp L1 theo support giảm dần
    sort(L1.begin(), L1.end(), cmp);
    
    // Xây dựng cây PPC
    for (int i = 1; i <= n; ++i) {
        vector<int> v;
        for (int x : database[i].items) if (!out[x]) v.push_back(x);
        sort(v.begin(), v.end(), cmp);
        if (v.size()) insertPPC(v);
    }
}

// duyệt pre , post
int pre_cnt = 0, post_cnt = 0;
void prepost_travel(PPC_Tree *u){
    u->pre = pre_cnt++;
    for(auto [v, child] : u->child){
        prepost_travel(child);
    }
    u->post = post_cnt++;
}

// Xây dựng N-list dựa trên L1
void builtNList1(PPC_Tree *u){
    if(u->name != -1){
        // thêm vào NL1 <(pre , post) : cnt>
        NL1[u->name].push_back({u->pre, u->post, u->cnt});
    } 
    for(auto v : u->child){
        builtNList1(v.second);
    }
}

//Xây dựng N-list 2 items dựa trên L1
int posL1[MAXN]; // lưu L1[i] -> i : tiện trong việc truy xuất để lưu vào mảng temp
vector<vector<int>> temp; // lưu trữ giá trị khi merge phần tử
void prepareNlist2(){
    temp.resize(L1.size(), vector<int>(L1.size(), 0));
    for(int i = 0 ; i < L1.size() ; i++){
        posL1[L1[i]] = i;
    }
}
void buildL2(PPC_Tree *u , vector<int> &daxuathien){
    if(u->name != -1){
        for(auto x : daxuathien){ // duyệt qua các phần tử đã xuất hiện trong đường đi của PPC_Tree u kiểm tra xem nó có phải tổ tiên không
            for(auto y : NL1[x]){
                if(y.pre < u->pre && y.post > u->post) // kiểm tra có phải tổ tiên không
                    temp[posL1[x]][posL1[u->name]] += u->cnt; // merge các cặp giống nhau (bf : 1 , bf : 2 , bf : 3) -> bf : 6
            }
        }
    }
    daxuathien.push_back(u->name);
    for(auto v : u->child){
        buildL2(v.second , daxuathien);
    }
    daxuathien.pop_back();
}

void locL2(){
    FOR(i , 0 , L1.size() - 1) FOR(j , 0 , L1.size() - 1){
        if(i != j && temp[i][j] >= minsup){
            vector<int> itemset = {L1[i], L1[j]};
            sort(itemset.begin(), itemset.end());
            L2.push_back({itemset[0] , itemset[1]});
        }
    }
}

vector<PPC_Code> giao(vector<PPC_Code> A, vector<PPC_Code> B) {
    vector<PPC_Code> C;
    int i = 0, j = 0;
    while (i < (int)A.size() && j < (int)B.size()) {
        auto &x = A[i];
        auto &y = B[j];
        if (x.pre == y.pre && x.post == y.post) {
            // cùng nút
            C.push_back({x.pre, x.post, min(x.cnt, y.cnt)});
            i++; j++;
        }
        else if (x.pre < y.pre && x.post > y.post) {
            // x tổ tiên y
            C.push_back({y.pre, y.post, min(x.cnt, y.cnt)});
            j++;
        }
        else if (y.pre < x.pre && y.post > x.post) {
            // y tổ tiên x
            C.push_back({x.pre, x.post, min(x.cnt, y.cnt)});
            i++;
        }
        else {
            // không tương thích, dịch con trỏ của mảng có pre nhỏ hơn
            if (x.pre < y.pre) i++;
            else j++;
        }
    }

    vector<PPC_Code> ans;
    for (auto &e : C) {
        if (!ans.empty() && ans.back().pre == e.pre && ans.back().post == e.post) {
            ans.back().cnt += e.cnt;
        } else {
            ans.push_back(e);
        }
    }
    return ans;
}


map<pair<int , int> , vector<PPC_Code>> NL2;
void buildNL2(){
    for(auto [i , j] : L2){ // duyệt qua các phần tử đã được ghép lại  
        vector<PPC_Code> g = giao(NL1[i] , NL1[j]); 
        // tinhs sup
        int sup = 0;
        for(auto x : g){
            sup += x.cnt; // sup dựa trên việc tổng tổ tiên với phần tử hiện tại đóng góp bao nhiêu lần
        }
        if(sup >= minsup){
            NL2[{i , j}] = g; // sinh ra được 1 tập có 2 phần tử i , j
        }
    }
}

// chỉ gộp được khi a.size = b.size và có cùng k - 1 kí tự giống nhau
bool check(vector<int> a , vector<int> b){
    if(a.size() != b.size()) return false;
    if(a.size() == 2){
        return a[0] == b[0] && a[1] < b[1];
    }
    FOR(i , 0 , a.size() - 2){
        if(a[i] != b[i]){
            return false;
        }
    }
    return a[a.size() - 1] < b[a.size() - 1];
}
vector<vector<int>> f;

void mining_L(vector<vector<int>> Lk, map<vector<int>, vector<PPC_Code>> NLk) {
    vector<vector<int>> Lk1;
    map<vector<int>, vector<PPC_Code>> NLk1;
    int n = Lk.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (check(Lk[i], Lk[j])) {
                vector<int> temp = Lk[i];
                temp.push_back(Lk[j].back());
                
                vector<PPC_Code> nlist = giao(NLk.at(Lk[i]), NLk.at(Lk[j]));
                int sup = 0;
                for (auto& code : nlist) sup += code.cnt;
                
                if (sup >= minsup) {
                    Lk1.push_back(temp);
                    NLk1[temp] = nlist;
                    f.push_back(temp);
                }
            }
        }
    }
    
    if (!Lk1.empty()) {
        mining_L(Lk1, NLk1);
    }
}
// chạy ở tập NL2 để tìm xem những phần tử tạo nên NL3 , 4 , 5 ... K
void Sinh_K_items(){
    for(auto item : L1){
        f.push_back({item});
    }
    for(auto [a, b] : L2){
        f.push_back({a, b});
    }
    vector<vector<int>> curL; 
    map<vector<int>, vector<PPC_Code>> curNL;
    for(auto [a, b] : L2){
        curL.push_back({a, b});
        curNL[{a, b}] = NL2[{a, b}];
    }
    if (!curL.empty()) {
        mining_L(curL, curNL);
    }
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(0);
    freopen("ppc_input.in" , "r" , stdin);
    freopen("ppc_output.out" , "w" , stdout);
    root = new PPC_Tree(-1 , 0 , 0 , 0); // -1 = null
    docfile();
    prepare_F1();
    pre_cnt = 0 , post_cnt = 0;
    prepost_travel(root);  
    for(auto x : L1) se.insert(x);
    builtNList1(root);
    prepareNlist2();
    vector<int> daxuathien;
    buildL2(root , daxuathien);
    locL2();
    buildNL2();
    Sinh_K_items();
    for(auto x : f){
        for(int i = 0; i < x.size(); i++){
            cout << x[i] << ' ';
        }
        cout << el;
    }
}