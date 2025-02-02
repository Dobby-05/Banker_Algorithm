#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;
/*银行家算法基础版本
示例输入：
5
3

7 5 3
3 2 2
9 0 2
2 2 2
4 2 2

0 1 0
2 0 0
3 0 2
2 1 1
0 0 2

3 3 2
*/

int n;// 进程数
int m;// 资源种类数
vector<vector<int>> Max;        // 最大需求矩阵        
vector<vector<int>> Allocation; // 已分配资源 
vector<vector<int>> Need;       // 需求矩阵       
vector<int> Available;          // 当前系统可用资源数
vector<int> Request;            // 请求向量
vector<int> Work;               // 工作向量
vector<bool> Finish;            // 是否满足
vector<char> RName;             // 资源名称 eg:A B C ...
int P = 0;                      // 请求资源的进程
vector<int> SafeQueue;              // 存安全队列
vector<bool> Recycled;       // 进程资源是否已经回收

void Init();
bool SafeContinue();
void addAvi();
bool SafeCheck(bool flag);
void PrintTable();
void RecoverSafe();
void BankerMethod();

// 初始化进程和资源
void init()
{
    cout << "请输入参与的进程数量：";
    cin >> n;
    cout << "请输入参与的资源数量：";
    cin >> m;
    int tmp_name;
    for (int i = 0; i < 26; i++){
        tmp_name = i + 'A';
        RName.push_back(tmp_name);
    }
    int value = 0;
    vector<int> tmp;
    cout << "请输入Max矩阵的内容：" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            cin >> value;
            tmp.push_back(value);
        }
        Max.push_back(tmp);
        tmp.clear();
    }
    
    cout << "请输入Allocation矩阵的内容：" << endl;
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < m; j++)
        {
            cin >> value;
            tmp.push_back(value);
        }
        Allocation.push_back(tmp);
        tmp.clear();

        for (int j = 0; j < m; j++)//需求资源数=最大需求数-已分配资源数
        {
            tmp.push_back(Max[i][j] - Allocation[i][j]);
        }
        Need.push_back(tmp);

        tmp.clear();
    }
    
    cout << "请输入系统各类可申请资源量：" << endl;
    for (int i = 0; i < m; i++)
    {
        cin >> value;
        Available.push_back(value);
    }
    //Work初始为available,Finish初始为false
    RecoverSafe();
}

// 安全性检查后，恢复Work和Finish
void RecoverSafe() {
    Work.clear();
    Finish.clear();
    //Work系统当前可提供的各类资源数目，初始为available
    for (int i = 0; i < m; i++) {
        Work.push_back(Available[i]);
    }
    //Finish表示系统是否有足够的资源分配给该进程，初始为false
    for (int i = 0; i < n; i++) {
        Finish.push_back(false);
    }
}

void BankerMethod() {
    cout << "请输入需要请求资源的进程编号：" << endl;
    for (int i = 0; i < n; i++) {
        cout << setfill(' ') << setw(3) << "P" << i << ":" << i << endl;
    }
   
    int tmpP = -1;
    bool flagP = false;
    while (!flagP) {
        cout << "进程编号:";
        cin >> tmpP;
        if (tmpP < 0 || tmpP >= n) { // 检查输入合法性
            cout << "此进程不存在，请重新输入！" << endl;
            flagP = false;
        }
        else {
            P = tmpP;
            flagP = true;
        }
    }

    // 确认请求资源Request
    Request.clear();
    int req;
    for (int i = 0; i < m; i++) {
        cout << "请输入进程P" << P << "请求的" << RName[i] << "资源的数量：";
        cin >> req;
        Request.push_back(req);
    }

    // 1.Request<=Need?
    bool flag = true;
    int countRN = 0;
    for (int i = 0; i < m; i++) {
        if (Request[i] <= Need[P][i]) {
            countRN++;
        }
        if (countRN == m) {
            flag = true;
        }else {
            
            flag = false;
        }
    }
    if (flag) {
        // 2.Request<=Available?
        int countRA = 0;
        for (int i = 0; i < m; i++) {
            if (Request[i] <= Available[i]) {
                countRA++;
            }
            if (countRA == m) {
                flag = true;
            }
            else {
                flag = false;
            }
        }
        if (flag)
        {
            // 3.系统试探着把资源分配给进程Pi
            for (int i = 0; i < m; i++) {
                Need[P][i] -= Request[i];
                Allocation[P][i] += Request[i];
                Available[i] -= Request[i];
                Work[i] = Available[i];
            }
            // T0时刻的资源分配表
            cout << "-----------------------为P" << P << "分配资源后的资源分配表------------------------" << endl;
            PrintTable();
            // 4.系统执行安全性算法
            cout << "------------------------P" << P << "申请资源时的安全性检查-------------------------" << endl;
            flag = SafeCheck(flag);

            // 如果安全性检查不通过，则回收进程请求的资源
            if (!flag) {
                for (int i = 0; i < m; i++) {
                    Available[i] += Request[i];
                    Allocation[P][i] -= Request[i];
                    Need[P][i] += Request[i];
                    Work[i] += Request[i];
                }
            }
            RecoverSafe();
        }
        else
            cout << "申请资源量超过Available，请重新输入" << endl;
    }
    else
        cout << "申请资源超过Need，请重新输入" << endl;
}

bool SafeContinue() {
    Finish.clear();
    Finish.resize(n, 0);
    SafeQueue.clear();
    SafeQueue.resize(n, 0);
    int Qi = 0;
    while (1) {
        bool work_vary = false;//跳出循环的条件
        for (int i = 0; i < n; i++) {
            // 已满足的进程跳过
            if (Finish[i]) {
                continue;
            }
            int j;
            for ( j = 0; j < m; j++) {
                if (Need[i][j] > Work[j])
                    break;
            }          
            if (j == m) {   // 当前资源可以被回收
                // 当前有可回收资源，标志位置true
                work_vary = true;
                for (j = 0; j < m; j++) {
                    Work[j] += Allocation[i][j];
                }
                Finish[i] = true;
                SafeQueue[Qi++] = i;
            }
        }
        if (work_vary == false) {
            break;
        }
    }
    for (int i = 0; i < n; i++) {
        if (!Finish[i]) {
            return false;
        }
    }
    return true;
}

bool SafeCheck(bool flag) {
    flag = SafeContinue();
    if (flag) {
        cout << "存在一个安全队列:" << endl;
        for (int i = 0; i < n; i++)
        {
            if (i) {
                cout << "->";
            }
            cout << SafeQueue[i];
        }
        addAvi();
        cout << endl;
        cout << "********************************系统处于安全状态********************************" << endl;
        return true;
    }
    else {
        cout << "*******************************系统处于不安全状态*******************************" << endl;
        return false;
    }
}
//判断分配完Need矩阵是否为0，为0则当前进程所持资源可回收
void addAvi() {
    Recycled.resize(n, 0);
    for (int i = 0; i < m; ++i) {
        if (Need[P][i])//至少有一个需求
            return;
    }
    if (Recycled[P] == 0) {
        for (int i = 0; i < m; ++i) {
            Available[i] += Allocation[P][i];
        }
        Recycled[P] = 1;
        cout << endl;
        cout << "-----------------------为P" << P << "回收资源后的资源分配表------------------------" << endl;
        PrintTable();
    }
}

void PrintTable() {
    cout << "    DATA    |" << setw(6 * m) << "Max" << "|" << setw(6 * m) << "Alloc" << "|" << setw(6 * m) << "Need" << "|" << setw(6 * m) << "Avail" << "|" << endl;
    cout << "  RESOURCE  |";

    for (int i = 0; i < m; i++) {
        cout << "  " << RName[i] << "   ";
    }
    cout << "|";
    for (int i = 0; i < m; i++) {
        cout << "  " << RName[i] << "   ";
    }
    cout << "|";
    for (int i = 0; i < m; i++) {
        cout << "  " << RName[i] << "   ";
    }
    cout << "|";
    for (int i = 0; i < m; i++) {
        cout << "  " << RName[i] << "   ";
    }
    cout << "|" << endl;
    for (int i = 0; i < n; i++){
        cout << "    P" << i << "      |";
        for (int j = 0; j < m; j++){
            cout << " " << setw(3) << Max[i][j] << "  ";
        }
        cout << "|";
        for (int j = 0; j < m; j++){
            cout << " " << setw(3) << Allocation[i][j] << "  ";
        }
        cout << "|";
        for (int j = 0; j < m; j++){
            cout << " " << setw(3) << Need[i][j] << "  ";
        }
        cout << "|";
        if (i == 0){
            for (int j = 0; j < m; j++)
                cout << " " << setw(3) << Available[j] << "  ";
            cout << "|";
        }
        cout << endl;
    }
}

int main() {
    cout << "********************************初始化进程和资源********************************" << endl;
    init();
    cout << "**********************************初始化已完成**********************************" << endl;
    // T0时刻的资源分配表
    cout << "---------------------------T0时刻的资源分配表----------------------------" << endl;
    PrintTable();
    // T0时刻的安全性检查
    cout << "---------------------------T0时刻的安全性检查----------------------------" << endl;
    bool tmp = true;
    bool flagInit = SafeCheck(tmp);
    // 恢复安全性检查向量
    RecoverSafe();

    // 银行家算法
    while (true)
    {
        cout << "是否要继续请求资源(y/n):" << endl;
        char YorN;
        cin >> YorN;
        if (YorN == 'n' || YorN == 'N') {
            break;
        }
        BankerMethod();
    }
    cout<< "************************************退出系统*************************************" << endl;
    return 0;
}

