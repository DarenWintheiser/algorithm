//
// Nim AI (three moutaions)
//   win pattern: xor = 0
//


#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    // 3 �����ФθĿ�������
    int A, B, C;
    cout << "The number of the first pile: ";
    cin >> A;
    cout << "The number of the second pile: ";
    cin >> B;
    cout << "The number of the third pile: ";
    cin >> C;

    // ���ȸ�����ά
    string ans;
    cout << "Are you first? (yes / no): " << endl;
    cin >> ans;
    bool yours = (ans == "yes" ? true : false);

    // �Ф���ʤ��ʤ�ޤǥץ쥤����
    while (A + B + C > 0) {
        cout << "-----------------" << endl;
        
        // �ץ쥤�䡼�Υ����� (�㳰�����Ͼ�ά)
        if (yours) {
            // �Фλ��ȼ���ФθĿ�������
            int which, num;
            cout << "Your Turn" << endl;
            cout << "Current state: ("
                 << A << ", "
                 << B << ", "
                 << C << ")" << endl;
            cout << "Which pile? (1 or 2 or 3): ";
            cin >> which;
            cout << "The number of stones? : ";
            cin >> num;

            // �ФθĿ����ѹ�
            if (which == 1) A -= num;
            else if (which == 2) B -= num;
            else C -= num;    
        }
        // AI �Υ�����
        else {
            cout << "AI's turn" << endl;
            cout << "Current state: ("
                 << A << ", "
                 << B << ", "
                 << C << ")" << endl;

            // A, B, C �� XOR �¤� S �Ȥ���
            int S = A ^ B ^ C;

            // S > 0 �ΤȤ��Ͼ����ѥ�����ˤ���
            if (S > 0) {
                if ((A ^ S) < A) A ^= S;
                else if ((B ^ S) < B) B ^= S;
                else C ^= S;
            }
            // ����ʳ��ξ��ϥ�������Ф򸺤餹
            else {
                if (A > 0) A = rand() % A;
                else if (B > 0) B = rand() % B;
                else C = rand() % C;
            }

            cout << "After AI's turn: ("
                 << A << ", "
                 << B << ", "
                 << C << ")" << endl;
        }

        // ���֤������ؤ�
        yours = !yours;
    }

    if (yours) cout << "You lose..." << endl;
    else cout << "You win!" << endl;
}
