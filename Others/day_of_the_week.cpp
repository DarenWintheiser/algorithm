//
// pn + r (n ����������) ��ɽ���������Τ�����x �ʾ�Ȥʤ�Ǿ�������
//
// verified:
//   Codeforces 304 DIV2 183 - B
//


#include <iostream>
using namespace std;

// ���쥴�ꥪ������
// ���쥴�ꥪ��ˤ������ꥦ������1582ǯ10��15������Ȥ�������â����-4712ǯ1��1����0�Ȥʤ�褦�ˤ����
int GregorianDay(int y, int m, int d) {
    y += 4800;
    if (m < 3) {--y; m += 12;}
    return 365*y+y/4-y/100+y/400+(153*m-457)/5+d-32083;
}

// ��ꥦ����ˤ������ꥦ������-4712ǯ1��1����0���ܤȤ�������
int JulianDay(int y, int m, int d) {
    y += 4716;
    if (m < 3) {--y; m += 12;}
    return 365*y+y/4+(153*m-457)/5+d-1402;
}

// ���쥴�ꥪ��β��Ǥ���������:1����:2����:3����:4����:5����:6����:0��
int dayWeek(int y, int m, int d) {
    return (GregorianDay(y, m, d) + 5) % 7;
}

int main() {
    cout << dayWeek(2019, 11, 27) << endl;
}
