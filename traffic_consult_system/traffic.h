#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<fstream>

#define MAX 20

using namespace std;

//ע����һ�������ܹ��˷ɻ����߻𳵵�����һ�����У�����������е���һ��������һ��·
//    ��һ��·��û���κν�ͨ����ʱ����Ӧ·������ֵΪNULL
//    ��һ��·��ֻ��һ�ֽ�ͨ����ʱ����Ӧ·��������û�е����ֽ�ͨ���������Ϊ99999������ʱ��Ϊ99999
//    ���ݽ�Ϊ���˹��죬����ʵ�������

struct city{
	int num;//���б��
	string city_name;//������
	int wait_time_train;//����תʱ�䣬��λΪ����
	int wait_time_plane;//�ɻ���תʱ�䣬��λΪ����
	string plane_setoff;//�ɻ����ʱ��
	string train_setoff;//��������ʱ��
	city(string name, int time_train, int time_plane, int x, string plane_setoff, string train_setoff) :city_name(name),
		wait_time_train(time_train), wait_time_plane(time_plane), num(x), plane_setoff(plane_setoff), train_setoff(train_setoff){}
};



struct road{
	int cost_plane;//һ�����е���һ�����г����ɻ��ķ��ã���λΪ�����
	int time_plane;//һ�����е���һ�����г����ɻ���ʱ�䣬��λΪ����
	int cost_train;//һ�����е���һ�����г����𳵵ķ��ã���λΪ�����
	int time_train;//һ�����е���һ�����г����𳵵�ʱ�䣬��λΪ����
	road(int cost_plane, int time_plane, int cost_train, int time_train) : cost_plane(cost_plane),
		time_plane(time_plane), cost_train(cost_train), time_train(time_train){}
};

string time_add(string time1, string time2){
	string res;
	if (time1[0] > 0){
		int minute = stoi(time1.substr(0, 2)) * 60 + stoi(time1.substr(3, 2)) + stoi(time2);
		int hour = minute / 60;
		int minute2 = minute % 60;
		if (hour < 10){
			res += "0";
			res += to_string(hour);
		}
		else if (hour < 24){
			res += to_string(hour);
		}
		else if (hour < 34){
			res += "����";
			hour = hour - 24;
			res += "0";
			res += to_string(hour);
		}
		else{
			res += "����";
			hour = hour - 24;
			res += to_string(hour);
		}

		res += ":";

		if (minute2 < 10){
			res += "0";
			res += to_string(minute2);
		}
		else{
			res += to_string(minute2);
		}
		return res;
	}
	else{
		res += "����";
		int minute = stoi(time1.substr(4, 2)) * 60 + stoi(time1.substr(7, 2)) + stoi(time2);
		int hour = minute / 60;
		int minute2 = minute % 60;
		if (hour < 10){
			res += "0";
			res += to_string(hour);
		}
		else{
			res += to_string(hour);
		}

		res += ":";

		if (minute2 < 10){
			res += "0";
			res += to_string(minute2);
		}
		else{
			res += to_string(minute2);
		}
		return res;
	}
}


class Graph{
private:

	vector<city*> citys;//����
	road* roads[MAX][MAX];//һ�����е���һ�����е�·,���ڽӾ���洢
	int city_num;//��ǰ��������
	int mincost_plane[MAX][MAX];
	int mincost_train[MAX][MAX];
	int mincost_plane_path[MAX][MAX];
	int mincost_train_path[MAX][MAX];
	int mintime_plane[MAX][MAX];
	int mintime_plane_path[MAX][MAX];
	int mintime_train[MAX][MAX];
	int mintime_train_path[MAX][MAX];
	int newroads_plane[MAX][MAX];//���ڼ�����С��ת����
	int newpaths_plane[MAX][MAX];//���ڼ�����С��ת����
	int newroads_train[MAX][MAX];//���ڼ�����С��ת����
	int newpaths_train[MAX][MAX];//���ڼ�����С��ת����

public:
	Graph() :roads(), city_num(0), mincost_plane(), mincost_train(),
		mincost_plane_path(), mincost_train_path(), mintime_plane(), mintime_plane_path(),
		mintime_train(), mintime_train_path(), newroads_plane(), newpaths_plane(), newroads_train(), newpaths_train(){}


	void load_city(string s){
		city* c = new city(s.substr(2, 4),
			stoi(s.substr(19, 2)), stoi(s.substr(22)), stoi(s.substr(0, 2)), s.substr(7, 5), s.substr(13, 5));
		citys.push_back(c);
		city_num++;
		return;
	}



	void load_road(string s){
		road* r = new road(stoi(s.substr(11, 9)), stoi(s.substr(20, 9)), stoi(s.substr(29, 9)), stoi(s.substr(38)));
		string s1 = s.substr(0, 4);
		string s2 = s.substr(5, 4);
		int i = search_city(s1);
		int j = search_city(s2);
		roads[i][j] = r;
		return;
	}


	int load_graph(){
		string s;
		vector<string> a;
		vector<string> b;
		ifstream file_city;
		file_city.open("city.txt");
		if (!file_city){
			cerr << "���ļ�����" << endl;
			return 0;
		}

		while (getline(file_city, s)){
			a.push_back(s);
			s.clear();
		}

		file_city.close();
		for (int i = 0; i < a.size(); i++){
			load_city(a[i]);
		}//���ļ����������

		ifstream file_road;
		file_road.open("road.txt");
		if (!file_road){
			cerr << "���ļ�����" << endl;
			return 0;
		}


		while (getline(file_road, s)){
			b.push_back(s);
			s.clear();
		}

		for (int i = 0; i < b.size(); i++){
			load_road(b[i]);
		}//���ļ�������·��

		file_road.close();
		return 1;
	}

	void file_out_city(){
		ofstream file_out("city.txt");
		for (int i = 0; i < city_num; i++){
			if (citys[i] != NULL){
				file_out << citys[i]->num << " " << citys[i]->city_name << " " << citys[i]->plane_setoff << " "<<
					citys[i]->train_setoff <<" "<< citys[i]->wait_time_train<<" " << citys[i]->wait_time_plane << endl;
			}
		}
		file_out.close();
	}


	void file_out_road(){
		ofstream file_out("road.txt");
		for (int i = 0; i < city_num; i++){
			for (int j = 0; j < city_num; j++){
				if (roads[i][j] != NULL){
					file_out << citys[i]->city_name << " " << citys[j]->city_name << "  ";
					if (roads[i][j]->cost_plane != 99999){
						if (roads[i][j]->cost_plane < 100)
							file_out << roads[i][j]->cost_plane << "       ";
						else  file_out << roads[i][j]->cost_plane << "      ";
					}
					else file_out << roads[i][j]->cost_plane << "    ";
					
					if (roads[i][j]->time_plane != 99999){
						if (roads[i][j]->time_plane < 100)
							file_out << roads[i][j]->time_plane << "       ";
						else  file_out << roads[i][j]->time_plane << "      ";
					}
					else file_out << roads[i][j]->time_plane << "    ";
		            
					if (roads[i][j]->cost_train!= 99999){
						if (roads[i][j]->cost_train < 100)
							file_out << roads[i][j]->cost_train << "       ";
						else  file_out << roads[i][j]->cost_train << "      ";
					}
					else file_out << roads[i][j]->cost_train << "    ";

					file_out << roads[i][j]->time_train << endl;

				}
			}
		}
		file_out.close();
	}

	int search_city(string name){
		for (int i = 0; i < citys.size(); i++){
			if (citys[i] != NULL && citys[i]->city_name == name){
				return citys[i]->num;//�ó��д��ڣ����ظó��б�ţ�
			}
		}
		return -1;
	}



	int search_road(string name_start, string name_end){
		int i = search_city(name_start);
		int j = search_city(name_end);
		if (roads[i][j] == NULL)
			return -1;//��·�������ڣ�����-1
		else return 1;
	}


	int insert_city(){
		cout << "��������е����ƣ�";
		string name;
		cin >> name;
		int k = search_city(name);
		if (k != -1){
			cout << "�ó����Ѿ����ڣ���";
			return 0;
		}
		cout << "������ó��зɻ�����ת�Ⱥ�ʱ��(��λ�����ӣ���";
		int time_plane;
		cin >> time_plane;
		cout << "������ó��л𳵵���ת�Ⱥ�ʱ�䣨��λ�����ӣ���";
		int time_train;
		cin >> time_train;
		cout << "������ó��зɻ������ʱ�䣨��ʽxx:xx����";
		string plane_setoff;
		cin >> plane_setoff;
		cout << "������ó��л𳵵ĳ���ʱ�䣨��ʽxx:xx����";
		string train_setoff;
		cin >> train_setoff;
		city* s = new city(name, time_train, time_plane, city_num, plane_setoff, train_setoff);
		city_num++;
		citys.push_back(s);
		cout << "���ӳ��гɹ�����";
		file_out_city();
		return 1;
	}//���ӳ���



	int delete_city(){
		cout << "������Ҫɾ���ĳ��е����ƣ�";
		string name;
		cin >> name;
		int i = search_city(name);
		if (i == -1){
			cout << "�����ڸó��У���";
			return 0;
		}
		else{
			delete citys[i];
			citys[i] = NULL;
			file_out_city();
			for (int j = 0; j < city_num; j++){
				if (roads[i][j] != NULL){
					delete roads[i][j];
					roads[i][j] = NULL;
				}
			}//ɾȥ��ó����������·
			
			for (int j = 0; j < city_num; j++){
				if (roads[j][i] != NULL){
					delete roads[j][i];
					roads[j][i] = NULL;
				}
			}//ɾȥ��ó����������·
			cout << "ɾ�����гɹ�����";
			file_out_road();
			return 1;
		}
	}//ɾ�����м��������·




	int delete_road(){
		cout << "������Ҫɾ��·���ĳ������У�";
		string name_start;
		cin >> name_start;
		cout << "������Ҫɾ��·����Ŀ�ĳ��У�";
		string name_end;
		cin >> name_end;
		int k = search_road(name_start, name_end);
		if (k == -1){
			cout << "Ҫɾ����·�������ڣ���";
			return 0;
		}
		else{
			int i = search_city(name_start);
			int j = search_city(name_end);
			delete roads[i][j];
			roads[i][j] = NULL;
			cout << "ɾ��·���ɹ�����";
			file_out_road();
			return 1;
		}
	}



	int insert_road(){
		int j, k;
		cout << "������������·���ĳ������У�";
		string name_start;
		cin >> name_start;
		cout << "������������·����Ŀ�ĳ��У�";
		string name_end;
		cin >> name_end;
		j = search_city(name_start);
		k = search_city(name_end);
		if (j == -1 || k == -1){
			cout << "������ϵͳ�в����ڣ��޷����·������";
			return 0;
		}
		if (search_road(name_start, name_end) == 1){
			cout << "��·���Ѿ����ڣ���";
			return 0;
		}

		cout << "������������е�Ŀ�ĳ��г����ɻ�����ʱ�䣨��λ:���ӣ���";
		int time_plane;
		cin >> time_plane;
		cout << "������������е�Ŀ�ĳ��г����ɻ��ķ��ã���λ:����ң���";
		int cost_plane;
		cin >> cost_plane;
		cout << "������������е�Ŀ�ĳ��г���������ʱ�䣨��λ:���ӣ���";
		int time_train;
		cin >> time_train;
		cout << "������������е�Ŀ�ĳ��г�����������ã���λ:����ң���";
		int cost_train;
		cin >> cost_train;
		roads[j][k] = new road(cost_plane, time_plane, cost_train, time_train);
		cout << "���·���ɹ�����";
		file_out_road();
		return 1;
	}//����·


	void update(){
		for (int i = 0; i < city_num; i++){
			for (int j = 0; j < city_num; j++){
				if (roads[i][j] != NULL)
					mincost_plane[i][j] = roads[i][j]->cost_plane;
				else mincost_plane[i][j] = 99999;

				if (i != j && roads[i][j] != NULL && roads[i][j]->cost_plane != 99999) mincost_plane_path[i][j] = i;
				else mincost_plane_path[i][j] = -1;
			}
		}


		for (int k = 0; k < city_num; k++){
			for (int i = 0; i < city_num; i++){
				for (int j = 0; j < city_num; j++){
					if (mincost_plane[i][k] + mincost_plane[k][j] < mincost_plane[i][j]){
						mincost_plane[i][j] = mincost_plane[i][k] + mincost_plane[k][j];
					   if(roads[k][j] != NULL)  mincost_plane_path[i][j] = k;
					}
				}
			}

		}//�ɻ����ٻ��Ѿ���ĸ���


		for (int i = 0; i < city_num; i++){
			for (int j = 0; j < city_num; j++){
				if (roads[i][j] != NULL)
					mincost_train[i][j] = roads[i][j]->cost_train;
				else mincost_train[i][j] = 99999;
				if (i != j && roads[i][j] != NULL && roads[i][j]->cost_train != 99999) mincost_train_path[i][j] = i;
				else mincost_train_path[i][j] = -1;
			}
		}


		for (int k = 0; k < city_num; k++){
			for (int i = 0; i < city_num; i++){
				for (int j = 0; j < city_num; j++){
					if (mincost_train[i][k] + mincost_train[k][j] < mincost_train[i][j]){
						mincost_train[i][j] = mincost_train[i][k] + mincost_train[k][j];
						if (roads[k][j] != NULL)  mincost_train_path[i][j] = k;
					}
				}
			}
		}//�����ٻ��Ѿ���ĸ���


		for (int i = 0; i < city_num; i++){
			for (int j = 0; j < city_num; j++){
				if (roads[i][j] != NULL)
					mintime_plane[i][j] = roads[i][j]->time_plane;
				else mintime_plane[i][j] = 99999;
				if (i != j && roads[i][j] != NULL && roads[i][j]->time_plane != 99999)  mintime_plane_path[i][j] = i;
				else mintime_plane_path[i][j] = -1;
			}
		}

		for (int k = 0; k < city_num; k++){
			for (int i = 0; i < city_num; i++){
				for (int j = 0; j < city_num; j++){
					if (citys[k] != NULL){
						if (mintime_plane[i][k] + citys[k]->wait_time_plane + mintime_plane[k][j] < mintime_plane[i][j]){
							mintime_plane[i][j] = mintime_plane[i][k] + citys[k]->wait_time_plane + mintime_plane[k][j];
							if (roads[k][j] != NULL) mintime_plane_path[i][j] = k;
						}
					}
				}
			}
		}//�ɻ�����ʱ����󣨿��ǵȺ�ʱ�䣩�ĸ���

		for (int i = 0; i < city_num; i++){
			for (int j = 0; j < city_num; j++){
				if (roads[i][j] != NULL)
					mintime_train[i][j] = roads[i][j]->time_train;
				else mintime_train[i][j] = 99999;
				if (i != j && roads[i][j] != NULL && roads[i][j]->time_train != 99999)  mintime_train_path[i][j] = i;
				else mintime_train_path[i][j] = -1;
			}
		}


		for (int k = 0; k < city_num; k++){
			for (int i = 0; i < city_num; i++){
				for (int j = 0; j < city_num; j++){
					if (citys[k] != NULL){
						if (mintime_train[i][k] + citys[k]->wait_time_train + mintime_train[k][j] < mintime_train[i][j]){
							mintime_train[i][j] = mintime_train[i][k] + citys[k]->wait_time_train + mintime_train[k][j];
							if (roads[k][j] != NULL) mintime_train_path[i][j] = k;
						}
					}
				}
			}
		}//������ʱ����󣨿��ǵȺ�ʱ�䣩�ĸ���

		for (int i = 0; i < city_num; i++){
			for (int j = 0; j < city_num; j++){

				if (roads[i][j] != NULL && roads[i][j]->time_plane != 99999) newroads_plane[i][j] = 1;
				else newroads_plane[i][j] = 99999;

				if (i != j && roads[i][j] != NULL && roads[i][j]->time_plane != 99999) newpaths_plane[i][j] = i;
				else newpaths_plane[i][j] = -1;
			}
		}

		for (int k = 0; k < city_num; k++){
			for (int i = 0; i < city_num; i++){
				for (int j = 0; j < city_num; j++){
					if (newroads_plane[i][k] + newroads_plane[k][j] < newroads_plane[i][j]){
						newroads_plane[i][j] = newroads_plane[i][k] + newroads_plane[k][j];
						if (roads[k][j] != NULL) newpaths_plane[i][j] = k;
					}
				}
			}
		}//�ɻ�������ת��������ĸ���

		for (int i = 0; i < city_num; i++){
			for (int j = 0; j < city_num; j++){

				if (roads[i][j] != NULL && roads[i][j]->time_train != 99999) newroads_train[i][j] = 1;
				else newroads_train[i][j] = 99999;

				if (i != j && roads[i][j] != NULL && roads[i][j]->time_train != 99999) newpaths_train[i][j] = i;
				else newpaths_train[i][j] = -1;
			}
		}

		for (int k = 0; k < city_num; k++){
			for (int i = 0; i < city_num; i++){
				for (int j = 0; j < city_num; j++){
					if (newroads_train[i][k] + newroads_train[k][j] < newroads_train[i][j]){
						newroads_train[i][j] = newroads_train[i][k] + newroads_train[k][j];
						if (roads[k][j] != NULL) newpaths_train[i][j] = k;
					}
				}
			}
		}//��������ת��������ĸ���


	}//����Floyd�㷨���½�ͨ��ѯϵͳ�������¼���𳵺ͷɻ������ٻ��Ѿ�������ʱ�����������ת��������


	void show_path_mincost_plane(int city_start, int city_end){
		stack<int> s;
		int k = city_end;
		while (k != city_start){
			s.push(k);
			k = mincost_plane_path[city_start][k];
		}
		string time = citys[k]->plane_setoff;

		cout << "��" << time << "��" << citys[k]->city_name << "����,����"
			<< roads[k][s.top()]->time_plane << "����,��" << time_add(time, to_string(roads[k][s.top()]->time_plane))
			<< "����" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_plane));


		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_plane));
			if (!s.empty()){
				cout << "��ת�Ⱥ�" << citys[k]->wait_time_plane << "���Ӻ���" << time << "��" << citys[k]->city_name << "����,����" << roads[k][s.top()]->time_plane << "���ӣ���" << time_add(time, to_string(roads[k][s.top()]->time_plane)) << "����" << citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_plane));
			}
			else break;
		}
	}


	void show_path_mincost_train(int city_start, int city_end){
		stack<int> s;
		int k = city_end;
		while (k != city_start){
			s.push(k);
			k = mincost_train_path[city_start][k];
		}

		string time = citys[k]->train_setoff;

		cout << "��" << time << "��" << citys[k]->city_name << "����,����"
			<< roads[k][s.top()]->time_train << "����,��" << time_add(time, to_string(roads[k][s.top()]->time_train))
			<< "����" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_train));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_train));
			if (!s.empty()){
				cout << "��ת�Ⱥ�" << citys[k]->wait_time_train << "���Ӻ���" << time << "��" << citys[k]->city_name << "����,����"
					<< roads[k][s.top()]->time_train << "���ӣ���" << time_add(time, to_string(roads[k][s.top()]->time_train)) << "����"
					<< citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_train));
			}
			else break;
		}
	}

	void show_path_mintime_train(int city_start, int city_end){
		stack<int> s;
		int k = city_end;
		while (k != city_start){
			s.push(k);
			k = mintime_train_path[city_start][k];
		}

		string time = citys[k]->train_setoff;

		cout << "��" << time << "��" << citys[k]->city_name << "����,����"
			<< roads[k][s.top()]->time_train << "����,��" << time_add(time, to_string(roads[k][s.top()]->time_train))
			<< "����" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_train));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_train));
			if (!s.empty()){
				cout << "��ת�Ⱥ�" << citys[k]->wait_time_train << "���Ӻ���" << time << "��" << citys[k]->city_name << "����,����"
					<< roads[k][s.top()]->time_train << "���ӣ���" << time_add(time, to_string(roads[k][s.top()]->time_train)) << "����"
					<< citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_train));
			}
			else break;
		}
	}

	void show_path_mintime_plane(int city_start, int city_end){
		stack<int> s;
		int k = city_end;
		while (k != city_start){
			s.push(k);
			k = mintime_plane_path[city_start][k];
		}

		string time = citys[k]->plane_setoff;

		cout << "��" << time << "��" << citys[k]->city_name << "����,����"
			<< roads[k][s.top()]->time_plane << "����,��" << time_add(time, to_string(roads[k][s.top()]->time_plane))
			<< "����" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_plane));


		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_plane));
			if (!s.empty()){
				cout << "��ת�Ⱥ�" << citys[k]->wait_time_plane << "���Ӻ���" << time << "��" << citys[k]->city_name << "����,����"
					<< roads[k][s.top()]->time_plane << "���ӣ���" << time_add(time, to_string(roads[k][s.top()]->time_plane)) << "����"
					<< citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_plane));
			}
			else break;
		}
	}


	void consult_mincost(string city_start, string city_end, string transportation){
		if (transportation == "�ɻ�"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "������ѯ�ĳ�����ϵͳ�в����ڣ���" << endl;
				return;
			}
			if (mincost_plane[i][j] == 99999) cout << "�޷������ɻ���" << city_start << "����" << city_end<<endl;
			else{
				cout << "������Ҫ���ѵ����ٷ���Ϊ��" << mincost_plane[i][j] << "Ԫ" << endl;
				cout << "·��Ϊ��" << endl;
				show_path_mincost_plane(i, j);
			}
			return;
		}

		else if (transportation == "��"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "������ѯ�ĳ�����ϵͳ�в����ڣ���" << endl;
				return;
			}
			if (mincost_train[i][j] == 99999) cout << "�޷������𳵴�" << city_start << "����" << city_end << endl;
			else{
				cout << "������Ҫ���ѵ����ٷ���Ϊ��" << mincost_train[i][j] << "Ԫ" << endl;
				cout << "·��Ϊ��" << endl;
				show_path_mincost_train(i, j);	
			}
			return;
		}

		else{
			cout << "�޴˽�ͨ���ߣ���";
			return;
		}
	}


	void consult_mintime(string city_start, string city_end, string transportation){
		if (transportation == "�ɻ�"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "������ѯ�ĳ�����ϵͳ�в����ڣ���" << endl;
				return;
			}
			if (mintime_plane[i][j] == 99999) cout << "�޷������ɻ���" << city_start << "����" << city_end << endl;
			else{
				cout << "������Ҫ���ѵ�����ʱ��Ϊ��" << mintime_plane[i][j] / 60 << "Сʱ" << mintime_plane[i][j] % 60 << "����" << endl;
				cout << "·��Ϊ��" << endl;
				show_path_mintime_plane(i, j);
			}
			return;
		}

		else if (transportation == "��"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "������ѯ�ĳ�����ϵͳ�в����ڣ���" << endl;
				return;
			}
			if (mintime_train[i][j] == 99999) cout << "�޷������𳵴�" << city_start << "����" << city_end << endl;
			else{
				cout << "������Ҫ���ѵ�����ʱ��Ϊ��" << mintime_train[i][j] / 60 << "Сʱ" << mintime_train[i][j] % 60 << "����" << endl;
				cout << "·��Ϊ��" << endl;
				show_path_mintime_train(i, j);
			}
			return;
		}

		else{
			cout << "�޴˽�ͨ���ߣ���";
			return;
		}
	}




	void show_path_mintransit_plane(int city_start, int city_end){
		stack<int> s;
		int k = city_end;
		while (k != city_start){
			s.push(k);
			k = newpaths_plane[city_start][k];
		}

		string time = citys[k]->plane_setoff;

		cout << "��" << time << "��" << citys[k]->city_name << "����,����"
			<< roads[k][s.top()]->time_plane << "����,��" << time_add(time, to_string(roads[k][s.top()]->time_plane))
			<< "����" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_plane));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_plane));
			if (!s.empty()){
				cout << "��ת�Ⱥ�" << citys[k]->wait_time_plane << "���Ӻ���" << time << "��" << citys[k]->city_name << "����,����"
					<< roads[k][s.top()]->time_plane << "���ӣ���" << time_add(time, to_string(roads[k][s.top()]->time_plane)) << "����"
					<< citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_plane));
			}
			else break;
		}
	}




	void show_path_mintransit_train(int city_start, int city_end){
		stack<int> s;
		int k = city_end;
		while (k != city_start){
			s.push(k);
			k = newpaths_train[city_start][k];
		}

		string time = citys[k]->train_setoff;

		cout << "��" << time << "��" << citys[k]->city_name << "����,����"
			<< roads[k][s.top()]->time_train << "����,��" << time_add(time, to_string(roads[k][s.top()]->time_train))
			<< "����" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_train));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_train));
			if (!s.empty()){
				cout << "��ת�Ⱥ�" << citys[k]->wait_time_train << "���Ӻ���" << time << "��" << citys[k]->city_name << "����,����"
					<< roads[k][s.top()]->time_train << "���ӣ���" << time_add(time, to_string(roads[k][s.top()]->time_train)) << "����"
					<< citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_train));
			}
			else break;
		}

	}



	void consult_mintransit(string city_start, string city_end, string transportation){
		if (transportation == "�ɻ�"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "������ѯ�ĳ�����ϵͳ�в����ڣ���" << endl;
				return;
			}
			if (newroads_plane[i][j] == 99999) cout << "�޷������ɻ���" << city_start << "����" << city_end << endl;
			else{
				int count = 0;
				int k = j;

				while (k != i){
					k = newpaths_plane[i][k];

					count++;
				}
				count--;
				cout << "������ת����Ϊ��" << count << "��" << endl;
				cout << "·��Ϊ��" << endl;
				show_path_mintransit_plane(i, j);
			}
			return;
		}

		else if (transportation == "��"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "������ѯ�ĳ�����ϵͳ�в����ڣ���" << endl;
				return;
			}
			if (newroads_train[i][j] == 99999) cout << "�޷������𳵴�" << city_start << "����" << city_end << endl;
			else{
				int count = 0;
				int k = j;
				while (k != i){
					k = newpaths_train[i][k];
					count++;
				}
				count--;
				cout << "������ת����Ϊ��" << count << "��" << endl;
				cout << "·��Ϊ��" << endl;
				show_path_mintransit_train(i, j);
			}
			return;
		}

		else{
			cout << "�޴˽�ͨ���ߣ���";
			return;
		}

	}

	
};
