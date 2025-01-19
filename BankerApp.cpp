#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;
/*���м��㷨Ӧ�ð汾 
ʾ�����룺 
3

1

1000

300
400
0

200
100
300
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
vector<char> Name;             // ��Դ���� eg:A B C ...
int P = 0;                      // ������Դ�Ľ���
vector<int> SafeQueue;              // �氲ȫ����
vector<bool> Recycled;       // ������Դ�Ƿ��Ѿ�����
int choice;

void Menu();
void Init();
bool SafeContinue();
void addAvi();
bool SafeCheck();
void PrintTable();
void RecoverSafe();
void BankerMethod();
void AddP();
void AddR();
void AutoBank();

// ��ʼ�����̺���Դ
void init()
{
    cout << "������������ҵ�ͻ�������";
    cin >> n;
    cout << "���������Ĵ�����Դ������";
    cin >> m;
    int tmp_name;
    for (int i = 0; i < 26; i++) {
        tmp_name = i + 'A';
        Name.push_back(tmp_name);
    }

    int value = 0;
    cout << "������ϵͳ�����ʼ��Դ����" << endl;
    for (int i = 0; i < m; i++) {
        cin >> value;
        Available.push_back(value);
    }
    vector<int> tmp;
    cout << "������Allocation��������ݣ�" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> value;
            tmp.push_back(value);
            Available[j] -= value;
        }
        Allocation.push_back(tmp);
        tmp.clear();
    }

    cout << "������Need��������ݣ�" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> value;
            tmp.push_back(value);
        }
        Need.push_back(tmp);
        tmp.clear();

        //������Դ��=���������-�ѷ�����Դ��
        for (int j = 0; j < m; j++)
        {
            tmp.push_back(Need[i][j] + Allocation[i][j]);
        }
        Max.push_back(tmp);

        tmp.clear();
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
    cout << "��������Ҫ������Դ����ҵ��ţ�" << endl;
    for (int i = 0; i < n; i++) {
        cout << setfill(' ') << setw(3) << Name[i] << ":" << i << endl;
    }

    int tmpP = -1;
    bool flagP = false;
    while (!flagP) {
        cout << "��ҵ���:";
        cin >> tmpP;
        if (tmpP < 0 || tmpP >= n) { // �������Ϸ���
            cout << "����ҵ�����ڣ����������룡" << endl;
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
        cout << "��������ҵ" << P << "�����" << i << "��Դ��������";
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
        }
        else {

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
            cout << "-----------------------Ϊ��ҵ" << P << "������Դ�����Դ�����------------------------" << endl;
            PrintTable();
            // 4.ϵͳִ�а�ȫ���㷨
            cout << "------------------------��ҵ" << P << "������Դʱ�İ�ȫ�Լ��-------------------------" << endl;
            flag = SafeCheck();

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
            cout << "*******************************������Դ������Available������������*******************************" << endl;
    }
    else
        cout << "*******************************������Դ����Need������������*******************************" << endl;
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
            for (j = 0; j < m; j++) {
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

bool SafeCheck() {
    bool flag;
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
        cout << "-----------------------Ϊ��ҵ" << P << "������Դ�����Դ�����------------------------" << endl;
        PrintTable();
    }
}

void PrintTable() {
    cout << "    DATA    |" << setw(6 * m) << "Max" << "|" << setw(6 * m) << "Alloc" << "|" << setw(6 * m) << "Need" << "|" << setw(6 * m) << "Avail" << "|" << endl;
    cout << "  RESOURCE  |";

    for (int i = 0; i < m; i++) {
        cout << "  " << i << "   ";
    }
    cout << "|";
    for (int i = 0; i < m; i++) {
        cout << "  " << i << "   ";
    }
    cout << "|";
    for (int i = 0; i < m; i++) {
        cout << "  " << i << "   ";
    }
    cout << "|";
    for (int i = 0; i < m; i++) {
        cout << "  " << i << "   ";
    }
    cout << "|" << endl;
    for (int i = 0; i < n; i++) {
        cout << "     " << Name[i] << "      |";
        for (int j = 0; j < m; j++) {
            cout << " " << setw(3) << Max[i][j] << "  ";
        }
        cout << "|";
        for (int j = 0; j < m; j++) {
            cout << " " << setw(3) << Allocation[i][j] << "  ";
        }
        cout << "|";
        for (int j = 0; j < m; j++) {
            cout << " " << setw(3) << Need[i][j] << "  ";
        }
        cout << "|";
        if (i == 0) {
            for (int j = 0; j < m; j++)
                cout << " " << setw(3) << Available[j] << "  ";
            cout << "|";
        }
        cout << endl;
    }
}
//���������
void AddP() {
    bool flagAvail = true;
    cout << "�����������ҵ��Ŀ";
    int temp;
    cin >> temp;
    n += temp;

    // ���浱ǰ״̬�Ա㳷��
    vector<int> oldAvailable = Available;
    vector<vector<int>> oldAllocation = Allocation;
    vector<vector<int>> oldNeed = Need;
    vector<vector<int>> oldMax = Max;

    int value = 0;
    cout << "������������ҵ��Allocation��������ݣ�" << endl;
    for (int i = 0; i < temp; i++) {
        vector<int> tmp;
        for (int j = 0; j < m; j++) {
            cin >> value;
            tmp.push_back(value);
            Available[j] -= value;
            if (Available[j] < 0) {
                flagAvail = false;
            }
        }
        Allocation.push_back(tmp);
    }

    cout << "������������ҵNeed��������ݣ�" << endl;
    for (int i = 0; i < temp; i++) {
        vector<int> tmp;
        for (int j = 0; j < m; j++) {
            cin >> value;
            tmp.push_back(value);
        }
        Need.push_back(tmp);

        vector<int> maxTmp;
        for (int j = 0; j < m; j++) {
            maxTmp.push_back(Need[n - i - 1][j] + Allocation[n - i - 1][j]);
        }
        Max.push_back(maxTmp);
    }

    RecoverSafe();
    bool flag = SafeCheck();
    if (!flag||!flagAvail) {
        cout << "���ʧ�ܣ����ڳ�������..." << endl;
        // ��������
        Available = oldAvailable;
        Allocation = oldAllocation;
        Need = oldNeed;
        Max = oldMax;
        n -= temp; // ���ٽ�������
    }
    else {
        cout << "��ӳɹ�" << endl;
    }
}
//�����Դ
void AddR() {
    bool flagAvail = true;
    // ���浱ǰ״̬�Ա㳷��
    vector<int> oldAvailable = Available;
    vector<vector<int>> oldAllocation = Allocation;
    vector<vector<int>> oldNeed = Need;
    vector<vector<int>> oldMax = Max;

    cout << "���������������Դ��Ŀ";
    int temp;
    cin >> temp;
    m += temp;

    int value = 0;
    cout << "���������������ʼ��Դ����" << endl;
    for (int i = 0; i < temp; i++) {
        cin >> value;
        Available.push_back(value);
    }

    cout << "����������Allocation��������ݣ�" << endl;
    for (int i = 0; i < n; i++) {
        vector<int> tmp;
        for (int j = 0; j < temp; j++) {
            cin >> value;
            tmp.push_back(value);
            Available[m - j - 1] -= value;
            if (Available[m - j - 1] < 0) {
                flagAvail = false;
            }
        }
        Allocation[i].insert(Allocation[i].end(), tmp.begin(), tmp.end());
    }

    cout << "����������Need��������ݣ�" << endl;
    for (int i = 0; i < n; i++) {
        vector<int> tmp;
        for (int j = 0; j < temp; j++) {
            cin >> value;
            tmp.push_back(value);
        }
        Need[i].insert(Need[i].end(), tmp.begin(), tmp.end());

        vector<int> maxTmp;
        for (int j = 0; j < temp; j++) {
            maxTmp.push_back(Need[i][m - j - 1] + Allocation[i][m - j - 1]);
        }
        Max[i].insert(Max[i].end(), maxTmp.begin(), maxTmp.end());
    }

    RecoverSafe();
    bool flag = SafeCheck();
    if (!flag||!flagAvail) {
        cout << "���ʧ�ܣ����ڳ�������..." << endl;
        // ��������
        Available = oldAvailable;
        Allocation = oldAllocation;
        Need = oldNeed;
        Max = oldMax;
        m -= temp; // ������Դ��������
    }
    else {
        cout << "��ӳɹ�" << endl;
    }
}

void Menu() {
    cout << "��ѡ��ǰ���ܣ�0-6����" << endl;
    cout << "1.�鿴ϵͳ��ǰ��Դʹ�����" << endl;
    cout << "2.������ǰϵͳ��ȫ��" << endl;
    cout << "3.������Դ" << endl;
    cout << "4.�����ҵ" << endl;
    cout << "5.�����Դ" << endl;
    cout << "0.�˳�����" << endl;
}

int main() {
    int flaginit = true;
    do {
        cout << "********************************��ʼ�����̺���Դ********************************" << endl;
        init();
        cout << "**********************************��ʼ�������**********************************" << endl;
        // T0ʱ�̵���Դ�����
        cout << "---------------------------T0ʱ�̵���Դ�����----------------------------" << endl;
        PrintTable();
        // T0ʱ�̵İ�ȫ�Լ��
        cout << "---------------------------T0ʱ�̵İ�ȫ�Լ��----------------------------" << endl;
        flaginit = SafeCheck();
        if (!flaginit) {
            cout << "��ʼ��ʧ�ܣ����ڳ�������..." << endl;
            Max.clear();
            Allocation.clear();
            Need.clear();
            Work.clear();
            Available.clear();
            Finish.clear();
        }
    } while (!flaginit);

    // �ָ���ȫ�Լ������
    RecoverSafe();

    while (1) {
        cout << endl;
        Menu();
        cin >> choice;
        switch (choice) {
        case 0://0.�˳�����
            system("cls");
            cout << "���ѳɹ��˳�ϵͳ����ӭ�´�ʹ��" << endl;
            exit(0);
        case 1://1.�鿴ϵͳ��ǰ��Դʹ�����
            PrintTable();
            break;
        case 2://2.������ǰϵͳ��ȫ��
            SafeCheck();
            break;
        case 3://3.������Դ
            BankerMethod();
            break;
        case 4://4.��ӽ���
            AddP();
            break;
        case 5://5.�����Դ
            AddR();
            break;
        default:
            cout << "��Ч��ѡ�������ѡ��" << endl;
        }
    }
    return 0;
}

