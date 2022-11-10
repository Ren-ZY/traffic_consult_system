#include"traffic.h"
#include<Windows.h>


int main();

string password = "admin";

Graph G;

void Administer()/* 显示管理员管理系统选择界面 */
{

	string newpassword;
	string x;
	int j = 0;
	char choice;
	int t;
	cout << "请输入登陆密码(默认为admin，可更改)，错误三次退出" << endl << endl << "密码:";

	while (1)
	{
		x.clear();
		cin >> x;

		if (x != password)
		{
			j++;
			if (j == 3){
				cout << endl << "密码输入错误三次，系统自动退出！！";
				Sleep(1500);
				exit(0);
			}
			cout << endl << "输入错误，请重新输入！！" << endl << "密码：";
		}
		else break;
	}

	system("cls");

	while (1)
	{

		cout << endl << endl << endl;
		cout << "*************************************************************************************" << endl << endl;
		cout << "※※※※※※※※※※※※※※※※※请选择管理项目※※※※※※※※※※※※※※※※※※" << endl;
		cout << "※※※※※  1 = 更改密码                                                    ※※※※" << endl;
		cout << "※※※※※  2 = 从文件中读入数据                                            ※※※※" << endl;
		cout << "※※※※※  3 = 增加城市                                                    ※※※※" << endl;
		cout << "※※※※※  4 = 删除城市                                                    ※※※※" << endl;
		cout << "※※※※※  5 = 增加路径                                                    ※※※※" << endl;
		cout << "※※※※※  6 = 删除路径                                                    ※※※※" << endl;
		cout << "※※※※※  7 = 更新系统                                                    ※※※※" << endl;
		cout << "※※※※※  8 = 返回上一级菜单                                              ※※※※" << endl;
		cout << "※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※" << endl << endl;
		cout << "************************************************************************************" << endl << endl;
		cout << "注意：每次增删后请更新系统！！" << endl << endl;
		cout << "请选择?" << endl;

		cin >> choice;

		switch (choice)
		{
		case '1':
			system("cls");
			cout << "请输入新密码：";
			cin >> newpassword;
			password.clear();
			password = newpassword;
			cout << "更改密码成功！！";
			Sleep(1500);
			system("cls");

			break;

		case '2':
			system("cls");
			t = G.load_graph();
			if (t == 1){
				cout << "载入成功！！";
				Sleep(1500);

			}
			else{
				cout << "载入失败！！";
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
			cout << "更新系统成功！！";
			Sleep(1500);
			system("cls");
			break;

		case '8':
			system("cls");
			main();
			break;

		default:
			cout << "请按提示重新输入！！";
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
		cout << "※※※※※※※※※※※※※※※※※请选择咨询项目※※※※※※※※※※※※※※※※※※" << endl;
		cout << "※※※※※  1 = 最少花费咨询                                                ※※※※" << endl;
		cout << "※※※※※  2 = 最少时间咨询                                                ※※※※" << endl;
		cout << "※※※※※  3 = 最少中转次数咨询                                            ※※※※" << endl;
		cout << "※※※※※  4 = 返回上一级菜单                                              ※※※※" << endl;
		cout << "※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※" << endl << endl;
		cout << "************************************************************************************" << endl << endl;
		cout << "请选择?" << endl;
		cin >> choice;

		switch (choice){
		case '1':
			system("cls");
			cout << "请输入出发城市：";
			cin >> city_start;
			cout << "请输入目的城市:";
		    cin >> city_end;
			cout << "请选择交通工具：";
			cin >> trans;
			G.consult_mincost(city_start, city_end, trans);
			city_start.clear();
			city_end.clear();
			trans.clear();
			cout << endl << endl << "输入任意值返回...........";
			cin >> temp;
			temp.clear();
			system("cls");
			break;

		case '2':
			system("cls");
			cout << "请输入出发城市：";
			cin >> city_start;
			cout << "请输入目的城市:";
			cin >> city_end;
			cout << "请选择交通工具：";
			cin >> trans;
			G.consult_mintime(city_start, city_end, trans);
			city_start.clear();
			city_end.clear();
			trans.clear();
			cout << endl << endl << "输入任意值返回...........";
			cin >> temp;
			temp.clear();
			system("cls");
			break;

		case '3':
			system("cls");
			cout << "请输入出发城市：";
			cin >> city_start;
			cout << "请输入目的城市:";
			cin >> city_end;
			cout << "请选择交通工具：";
			cin >> trans;
			G.consult_mintransit(city_start, city_end, trans);
			city_start.clear();
			city_end.clear();
			trans.clear();
			cout << endl << endl << "输入任意值返回...........";
			cin >> temp;
			temp.clear();
			system("cls");
			break;

		case '4':
			system("cls");
			main();
			break;

		default:
			cout << "请按提示重新输入！！";
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
		cout << "**********************************欢迎使用交通咨询系统********************************" << endl << endl;
		cout << "※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※" << endl << endl;
		cout << "※※※※※※※※    1 = 进入管理员管理系统                          ※※※※※※※※※" << endl;
		cout << "※※※※※※※※    2 = 进入用户咨询系统                            ※※※※※※※※※" << endl;
		cout << "※※※※※※※※    3 = 退出                                        ※※※※※※※※※" << endl << endl;
		cout << "※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※" << endl << endl;
		cout << "**************************************************************************************" << endl << endl;
		cout << "请选择?" << endl;
		cin >> choice;

		switch (choice)
		{

		case '1': system("cls"); Administer(); break; //管理员 


		case '2': system("cls"); User(); break; //用户 


		case '3':
			cout << "感谢使用!!";
			Sleep(1500);
			exit(1); break;

		default:
			cout << "请按提示重新输入！！";
			Sleep(1500);
			system("cls");
			break;
		}
	}

	return 0;
}
