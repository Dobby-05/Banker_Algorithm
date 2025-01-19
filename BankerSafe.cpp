#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;
/*���м��㷨�����汾
ʾ�����룺
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

int n;// ������
int m;// ��Դ������
vector<vector<int>> Max;        // ����������        
vector<vector<int>> Allocation; // �ѷ�����Դ 
vector<vector<int>> Need;       // �������       
vector<int> Available;          // ��ǰϵͳ������Դ��
vector<int> Request;            // ��������
vector<int> Work;               // ��������
vector<bool> Finish;            // �Ƿ�����
vector<char> RName;             // ��Դ���� eg:A B C ...
int P = 0;                      // ������Դ�Ľ���
vector<int> SafeQueue;              // �氲ȫ����
vector<bool> Recycled;       // ������Դ�Ƿ��Ѿ�����

void Init();
bool SafeContinue();
void addAvi();
bool SafeCheck(bool flag);
void PrintTable();
void RecoverSafe();
void BankerMethod();

// ��ʼ�����̺���Դ
void init()
{
    cout << "���������Ľ���������";
    cin >> n;
    cout << "������������Դ������";
    cin >> m;
    int tmp_name;
    for (int i = 0; i < 26; i++){
        tmp_name = i + 'A';
        RName.push_back(tmp_name);
    }
    int value = 0;
    vector<int> tmp;
    cout << "������Max��������ݣ�" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            cin >> value;
            tmp.push_back(value);
        }
        Max.push_back(tmp);
        tmp.clear();
    }
    
    cout << "������Allocation��������ݣ�" << endl;
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < m; j++)
        {
            cin >> value;
            tmp.push_back(value);
        }
        Allocation.push_back(tmp);
        tmp.clear();

        for (int j = 0; j < m; j++)//������Դ��=���������-�ѷ�����Դ��
        {
            tmp.push_back(Max[i][j] - Allocation[i][j]);
        }
        Need.push_back(tmp);

        tmp.clear();
    }
    
    cout << "������ϵͳ�����������Դ����" << endl;
    for (int i = 0; i < m; i++)
    {
        cin >> value;
        Available.push_back(value);
    }
    //Work��ʼΪavailable,Finish��ʼΪfalse
    RecoverSafe();
}

// ��ȫ�Լ��󣬻ָ�Work��Finish
void RecoverSafe() {
    Work.clear();
    Finish.clear();
    //Workϵͳ��ǰ���ṩ�ĸ�����Դ��Ŀ����ʼΪavailable
    for (int i = 0; i < m; i++) {
        Work.push_back(Available[i]);
    }
    //Finish��ʾϵͳ�Ƿ����㹻����Դ������ý��̣���ʼΪfalse
    for (int i = 0; i < n; i++) {
        Finish.push_back(false);
    }
}

void BankerMethod() {
    cout << "��������Ҫ������Դ�Ľ��̱�ţ�" << endl;
    for (int i = 0; i < n; i++) {
        cout << setfill(' ') << setw(3) << "P" << i << ":" << i << endl;
    }
   
    int tmpP = -1;
    bool flagP = false;
    while (!flagP) {
        cout << "���̱��:";
        cin >> tmpP;
        if (tmpP < 0 || tmpP >= n) { // �������Ϸ���
            cout << "�˽��̲����ڣ����������룡" << endl;
            flagP = false;
        }
        else {
            P = tmpP;
            flagP = true;
        }
    }

    // ȷ��������ԴRequest
    Request.clear();
    int req;
    for (int i = 0; i < m; i++) {
        cout << "���������P" << P << "�����" << RName[i] << "��Դ��������";
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
            // 3.ϵͳ��̽�Ű���Դ���������Pi
            for (int i = 0; i < m; i++) {
                Need[P][i] -= Request[i];
                Allocation[P][i] += Request[i];
                Available[i] -= Request[i];
                Work[i] = Available[i];
            }
            // T0ʱ�̵���Դ�����
            cout << "-----------------------ΪP" << P << "������Դ�����Դ�����------------------------" << endl;
            PrintTable();
            // 4.ϵͳִ�а�ȫ���㷨
            cout << "------------------------P" << P << "������Դʱ�İ�ȫ�Լ��-------------------------" << endl;
            flag = SafeCheck(flag);

            // �����ȫ�Լ�鲻ͨ��������ս����������Դ
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
            cout << "������Դ������Available������������" << endl;
    }
    else
        cout << "������Դ����Need������������" << endl;
}

bool SafeContinue() {
    Finish.clear();
    Finish.resize(n, 0);
    SafeQueue.clear();
    SafeQueue.resize(n, 0);
    int Qi = 0;
    while (1) {
        bool work_vary = false;//����ѭ��������
        for (int i = 0; i < n; i++) {
            // ������Ľ�������
            if (Finish[i]) {
                continue;
            }
            int j;
            for ( j = 0; j < m; j++) {
                if (Need[i][j] > Work[j])
                    break;
            }          
            if (j == m) {   // ��ǰ��Դ���Ա�����
                // ��ǰ�пɻ�����Դ����־λ��true
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
        cout << "����һ����ȫ����:" << endl;
        for (int i = 0; i < n; i++)
        {
            if (i) {
                cout << "->";
            }
            cout << SafeQueue[i];
        }
        addAvi();
        cout << endl;
        cout << "********************************ϵͳ���ڰ�ȫ״̬********************************" << endl;
        return true;
    }
    else {
        cout << "*******************************ϵͳ���ڲ���ȫ״̬*******************************" << endl;
        return false;
    }
}
//�жϷ�����Need�����Ƿ�Ϊ0��Ϊ0��ǰ����������Դ�ɻ���
void addAvi() {
    Recycled.resize(n, 0);
    for (int i = 0; i < m; ++i) {
        if (Need[P][i])//������һ������
            return;
    }
    if (Recycled[P] == 0) {
        for (int i = 0; i < m; ++i) {
            Available[i] += Allocation[P][i];
        }
        Recycled[P] = 1;
        cout << endl;
        cout << "-----------------------ΪP" << P << "������Դ�����Դ�����------------------------" << endl;
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
    cout << "********************************��ʼ�����̺���Դ********************************" << endl;
    init();
    cout << "**********************************��ʼ�������**********************************" << endl;
    // T0ʱ�̵���Դ�����
    cout << "---------------------------T0ʱ�̵���Դ�����----------------------------" << endl;
    PrintTable();
    // T0ʱ�̵İ�ȫ�Լ��
    cout << "---------------------------T0ʱ�̵İ�ȫ�Լ��----------------------------" << endl;
    bool tmp = true;
    bool flagInit = SafeCheck(tmp);
    // �ָ���ȫ�Լ������
    RecoverSafe();

    // ���м��㷨
    while (true)
    {
        cout << "�Ƿ�Ҫ����������Դ(y/n):" << endl;
        char YorN;
        cin >> YorN;
        if (YorN == 'n' || YorN == 'N') {
            break;
        }
        BankerMethod();
    }
    cout<< "************************************�˳�ϵͳ*************************************" << endl;
    return 0;
}

