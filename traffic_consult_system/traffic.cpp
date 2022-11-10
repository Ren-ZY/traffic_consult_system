#include"traffic.h"
#include<Windows.h>


int main();

string password = "admin";

Graph G;

void Administer()/* ��ʾ����Ա����ϵͳѡ����� */
{

	string newpassword;
	string x;
	int j = 0;
	char choice;
	int t;
	cout << "�������½����(Ĭ��Ϊadmin���ɸ���)�����������˳�" << endl << endl << "����:";

	while (1)
	{
		x.clear();
		cin >> x;

		if (x != password)
		{
			j++;
			if (j == 3){
				cout << endl << "��������������Σ�ϵͳ�Զ��˳�����";
				Sleep(1500);
				exit(0);
			}
			cout << endl << "����������������룡��" << endl << "���룺";
		}
		else break;
	}

	system("cls");

	while (1)
	{

		cout << endl << endl << endl;
		cout << "*************************************************************************************" << endl << endl;
		cout << "������������������������������������ѡ�������Ŀ������������������������������������" << endl;
		cout << "����������  1 = ��������                                                    ��������" << endl;
		cout << "����������  2 = ���ļ��ж�������                                            ��������" << endl;
		cout << "����������  3 = ���ӳ���                                                    ��������" << endl;
		cout << "����������  4 = ɾ������                                                    ��������" << endl;
		cout << "����������  5 = ����·��                                                    ��������" << endl;
		cout << "����������  6 = ɾ��·��                                                    ��������" << endl;
		cout << "����������  7 = ����ϵͳ                                                    ��������" << endl;
		cout << "����������  8 = ������һ���˵�                                              ��������" << endl;
		cout << "����������������������������������������������������������������������������������" << endl << endl;
		cout << "************************************************************************************" << endl << endl;
		cout << "ע�⣺ÿ����ɾ�������ϵͳ����" << endl << endl;
		cout << "��ѡ��?" << endl;

		cin >> choice;

		switch (choice)
		{
		case '1':
			system("cls");
			cout << "�����������룺";
			cin >> newpassword;
			password.clear();
			password = newpassword;
			cout << "��������ɹ�����";
			Sleep(1500);
			system("cls");

			break;

		case '2':
			system("cls");
			t = G.load_graph();
			if (t == 1){
				cout << "����ɹ�����";
				Sleep(1500);

			}
			else{
				cout << "����ʧ�ܣ���";
				Sleep(1500);

			}
			system("cls");
			break;

		case '3':
			system("cls");
			t = G.insert_city();
			Sleep(1500);
			system("cls");
			break;

		case '4':
			system("cls");
			t = G.delete_city();
			Sleep(1500);
			system("cls");
			break;

		case '5':
			system("cls");
			t = G.insert_road();
			Sleep(1500);
			system("cls");
			break;

		case '6':
			system("cls");
			t = G.delete_road();
			Sleep(1500);
			system("cls");
			break;

		case '7':
			system("cls");
			G.update();
			cout << "����ϵͳ�ɹ�����";
			Sleep(1500);
			system("cls");
			break;

		case '8':
			system("cls");
			main();
			break;

		default:
			cout << "�밴��ʾ�������룡��";
			Sleep(1500);
			system("cls");
		}
	}
}


void User(){
	char choice;
	string trans;
	string city_start;
	string city_end;
	string temp;
	while (1)
	{

		cout << endl << endl << endl;
		cout << "*************************************************************************************" << endl << endl;
		cout << "������������������������������������ѡ����ѯ��Ŀ������������������������������������" << endl;
		cout << "����������  1 = ���ٻ�����ѯ                                                ��������" << endl;
		cout << "����������  2 = ����ʱ����ѯ                                                ��������" << endl;
		cout << "����������  3 = ������ת������ѯ                                            ��������" << endl;
		cout << "����������  4 = ������һ���˵�                                              ��������" << endl;
		cout << "������������������������������������������������������������������������������������" << endl << endl;
		cout << "************************************************************************************" << endl << endl;
		cout << "��ѡ��?" << endl;
		cin >> choice;

		switch (choice){
		case '1':
			system("cls");
			cout << "������������У�";
			cin >> city_start;
			cout << "������Ŀ�ĳ���:";
		    cin >> city_end;
			cout << "��ѡ��ͨ���ߣ�";
			cin >> trans;
			G.consult_mincost(city_start, city_end, trans);
			city_start.clear();
			city_end.clear();
			trans.clear();
			cout << endl << endl << "��������ֵ����...........";
			cin >> temp;
			temp.clear();
			system("cls");
			break;

		case '2':
			system("cls");
			cout << "������������У�";
			cin >> city_start;
			cout << "������Ŀ�ĳ���:";
			cin >> city_end;
			cout << "��ѡ��ͨ���ߣ�";
			cin >> trans;
			G.consult_mintime(city_start, city_end, trans);
			city_start.clear();
			city_end.clear();
			trans.clear();
			cout << endl << endl << "��������ֵ����...........";
			cin >> temp;
			temp.clear();
			system("cls");
			break;

		case '3':
			system("cls");
			cout << "������������У�";
			cin >> city_start;
			cout << "������Ŀ�ĳ���:";
			cin >> city_end;
			cout << "��ѡ��ͨ���ߣ�";
			cin >> trans;
			G.consult_mintransit(city_start, city_end, trans);
			city_start.clear();
			city_end.clear();
			trans.clear();
			cout << endl << endl << "��������ֵ����...........";
			cin >> temp;
			temp.clear();
			system("cls");
			break;

		case '4':
			system("cls");
			main();
			break;

		default:
			cout << "�밴��ʾ�������룡��";
			Sleep(1500);
			system("cls");
		}

	}
}



int main(){

	char choice;
	while (1)
	{
		system("cls");
		cout << endl << endl;
		cout << "**********************************��ӭʹ�ý�ͨ��ѯϵͳ********************************" << endl << endl;
		cout << "��������������������������������������������������������������������������������������" << endl << endl;
		cout << "����������������    1 = �������Ա����ϵͳ                          ������������������" << endl;
		cout << "����������������    2 = �����û���ѯϵͳ                            ������������������" << endl;
		cout << "����������������    3 = �˳�                                        ������������������" << endl << endl;
		cout << "��������������������������������������������������������������������������������������" << endl << endl;
		cout << "**************************************************************************************" << endl << endl;
		cout << "��ѡ��?" << endl;
		cin >> choice;

		switch (choice)
		{

		case '1': system("cls"); Administer(); break; //����Ա 


		case '2': system("cls"); User(); break; //�û� 


		case '3':
			cout << "��лʹ��!!";
			Sleep(1500);
			exit(1); break;

		default:
			cout << "�밴��ʾ�������룡��";
			Sleep(1500);
			system("cls");
			break;
		}
	}

	return 0;
}
