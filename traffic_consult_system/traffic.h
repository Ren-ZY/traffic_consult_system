#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<fstream>

#define MAX 20

using namespace std;

//注：若一个城市能够乘飞机或者火车到达另一个城市，则简称这个城市到另一个城市有一条路
//    当一条路上没有任何交通工具时，对应路径矩阵值为NULL
//    当一条路上只有一种交通工具时，对应路径矩阵中没有的那种交通工具其费用为99999，花费时间为99999
//    数据皆为本人构造，与事实并不相符

struct city{
	int num;//城市编号
	string city_name;//城市名
	int wait_time_train;//火车中转时间，单位为分钟
	int wait_time_plane;//飞机中转时间，单位为分钟
	string plane_setoff;//飞机起飞时间
	string train_setoff;//过车出发时间
	city(string name, int time_train, int time_plane, int x, string plane_setoff, string train_setoff) :city_name(name),
		wait_time_train(time_train), wait_time_plane(time_plane), num(x), plane_setoff(plane_setoff), train_setoff(train_setoff){}
};



struct road{
	int cost_plane;//一个城市到另一个城市乘坐飞机的费用，单位为人民币
	int time_plane;//一个城市到另一个城市乘坐飞机的时间，单位为分钟
	int cost_train;//一个城市到另一个城市乘坐火车的费用，单位为人民币
	int time_train;//一个城市到另一个城市乘坐火车的时间，单位为分钟
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
			res += "次日";
			hour = hour - 24;
			res += "0";
			res += to_string(hour);
		}
		else{
			res += "次日";
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
		res += "次日";
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

	vector<city*> citys;//城市
	road* roads[MAX][MAX];//一个城市到另一个城市的路,用邻接矩阵存储
	int city_num;//当前城市数量
	int mincost_plane[MAX][MAX];
	int mincost_train[MAX][MAX];
	int mincost_plane_path[MAX][MAX];
	int mincost_train_path[MAX][MAX];
	int mintime_plane[MAX][MAX];
	int mintime_plane_path[MAX][MAX];
	int mintime_train[MAX][MAX];
	int mintime_train_path[MAX][MAX];
	int newroads_plane[MAX][MAX];//用于计算最小中转次数
	int newpaths_plane[MAX][MAX];//用于计算最小中转次数
	int newroads_train[MAX][MAX];//用于计算最小中转次数
	int newpaths_train[MAX][MAX];//用于计算最小中转次数

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
			cerr << "打开文件出错！" << endl;
			return 0;
		}

		while (getline(file_city, s)){
			a.push_back(s);
			s.clear();
		}

		file_city.close();
		for (int i = 0; i < a.size(); i++){
			load_city(a[i]);
		}//从文件中输入城市

		ifstream file_road;
		file_road.open("road.txt");
		if (!file_road){
			cerr << "打开文件出错！" << endl;
			return 0;
		}


		while (getline(file_road, s)){
			b.push_back(s);
			s.clear();
		}

		for (int i = 0; i < b.size(); i++){
			load_road(b[i]);
		}//从文件中输入路径

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
				return citys[i]->num;//该城市存在，返回该城市编号；
			}
		}
		return -1;
	}



	int search_road(string name_start, string name_end){
		int i = search_city(name_start);
		int j = search_city(name_end);
		if (roads[i][j] == NULL)
			return -1;//该路并不存在，返回-1
		else return 1;
	}


	int insert_city(){
		cout << "请输入城市的名称：";
		string name;
		cin >> name;
		int k = search_city(name);
		if (k != -1){
			cout << "该城市已经存在！！";
			return 0;
		}
		cout << "请输入该城市飞机的中转等候时间(单位：分钟）：";
		int time_plane;
		cin >> time_plane;
		cout << "请输入该城市火车的中转等候时间（单位：分钟）：";
		int time_train;
		cin >> time_train;
		cout << "请输入该城市飞机的起飞时间（格式xx:xx）：";
		string plane_setoff;
		cin >> plane_setoff;
		cout << "请输入该城市火车的出发时间（格式xx:xx）：";
		string train_setoff;
		cin >> train_setoff;
		city* s = new city(name, time_train, time_plane, city_num, plane_setoff, train_setoff);
		city_num++;
		citys.push_back(s);
		cout << "增加城市成功！！";
		file_out_city();
		return 1;
	}//增加城市



	int delete_city(){
		cout << "请输入要删除的城市的名称：";
		string name;
		cin >> name;
		int i = search_city(name);
		if (i == -1){
			cout << "不存在该城市！！";
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
			}//删去与该城市相关联的路
			
			for (int j = 0; j < city_num; j++){
				if (roads[j][i] != NULL){
					delete roads[j][i];
					roads[j][i] = NULL;
				}
			}//删去与该城市相关联的路
			cout << "删除城市成功！！";
			file_out_road();
			return 1;
		}
	}//删除城市及其关联的路




	int delete_road(){
		cout << "请输入要删除路径的出发城市：";
		string name_start;
		cin >> name_start;
		cout << "请输入要删除路径的目的城市：";
		string name_end;
		cin >> name_end;
		int k = search_road(name_start, name_end);
		if (k == -1){
			cout << "要删除的路径不存在！！";
			return 0;
		}
		else{
			int i = search_city(name_start);
			int j = search_city(name_end);
			delete roads[i][j];
			roads[i][j] = NULL;
			cout << "删除路径成功！！";
			file_out_road();
			return 1;
		}
	}



	int insert_road(){
		int j, k;
		cout << "请输入需增加路径的出发城市：";
		string name_start;
		cin >> name_start;
		cout << "请输入需增加路径的目的城市：";
		string name_end;
		cin >> name_end;
		j = search_city(name_start);
		k = search_city(name_end);
		if (j == -1 || k == -1){
			cout << "城市在系统中不存在，无法添加路径！！";
			return 0;
		}
		if (search_road(name_start, name_end) == 1){
			cout << "该路径已经存在！！";
			return 0;
		}

		cout << "请输入出发城市到目的城市乘坐飞机所需时间（单位:分钟）：";
		int time_plane;
		cin >> time_plane;
		cout << "请输入出发城市到目的城市乘坐飞机的费用（单位:人民币）：";
		int cost_plane;
		cin >> cost_plane;
		cout << "请输入出发城市到目的城市乘坐火车所需时间（单位:分钟）：";
		int time_train;
		cin >> time_train;
		cout << "请输入出发城市到目的城市乘坐火车所需费用（单位:人民币）：";
		int cost_train;
		cin >> cost_train;
		roads[j][k] = new road(cost_plane, time_plane, cost_train, time_train);
		cout << "添加路径成功！！";
		file_out_road();
		return 1;
	}//增加路


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

		}//飞机最少花费矩阵的更新


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
		}//火车最少花费矩阵的更新


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
		}//飞机最少时间矩阵（考虑等候时间）的更新

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
		}//火车最少时间矩阵（考虑等候时间）的更新

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
		}//飞机最少中转次数矩阵的更新

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
		}//火车最少中转次数矩阵的更新


	}//利用Floyd算法更新交通咨询系统，即重新计算火车和飞机的最少花费矩阵，最少时间矩阵，最少中转次数矩阵


	void show_path_mincost_plane(int city_start, int city_end){
		stack<int> s;
		int k = city_end;
		while (k != city_start){
			s.push(k);
			k = mincost_plane_path[city_start][k];
		}
		string time = citys[k]->plane_setoff;

		cout << "于" << time << "从" << citys[k]->city_name << "出发,经过"
			<< roads[k][s.top()]->time_plane << "分钟,于" << time_add(time, to_string(roads[k][s.top()]->time_plane))
			<< "到达" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_plane));


		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_plane));
			if (!s.empty()){
				cout << "中转等候" << citys[k]->wait_time_plane << "分钟后于" << time << "从" << citys[k]->city_name << "出发,经过" << roads[k][s.top()]->time_plane << "分钟，于" << time_add(time, to_string(roads[k][s.top()]->time_plane)) << "到达" << citys[s.top()]->city_name << endl;
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

		cout << "于" << time << "从" << citys[k]->city_name << "出发,经过"
			<< roads[k][s.top()]->time_train << "分钟,于" << time_add(time, to_string(roads[k][s.top()]->time_train))
			<< "到达" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_train));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_train));
			if (!s.empty()){
				cout << "中转等候" << citys[k]->wait_time_train << "分钟后于" << time << "从" << citys[k]->city_name << "出发,经过"
					<< roads[k][s.top()]->time_train << "分钟，于" << time_add(time, to_string(roads[k][s.top()]->time_train)) << "到达"
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

		cout << "于" << time << "从" << citys[k]->city_name << "出发,经过"
			<< roads[k][s.top()]->time_train << "分钟,于" << time_add(time, to_string(roads[k][s.top()]->time_train))
			<< "到达" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_train));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_train));
			if (!s.empty()){
				cout << "中转等候" << citys[k]->wait_time_train << "分钟后于" << time << "从" << citys[k]->city_name << "出发,经过"
					<< roads[k][s.top()]->time_train << "分钟，于" << time_add(time, to_string(roads[k][s.top()]->time_train)) << "到达"
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

		cout << "于" << time << "从" << citys[k]->city_name << "出发,经过"
			<< roads[k][s.top()]->time_plane << "分钟,于" << time_add(time, to_string(roads[k][s.top()]->time_plane))
			<< "到达" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_plane));


		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_plane));
			if (!s.empty()){
				cout << "中转等候" << citys[k]->wait_time_plane << "分钟后于" << time << "从" << citys[k]->city_name << "出发,经过"
					<< roads[k][s.top()]->time_plane << "分钟，于" << time_add(time, to_string(roads[k][s.top()]->time_plane)) << "到达"
					<< citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_plane));
			}
			else break;
		}
	}


	void consult_mincost(string city_start, string city_end, string transportation){
		if (transportation == "飞机"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "您所咨询的城市在系统中不存在！！" << endl;
				return;
			}
			if (mincost_plane[i][j] == 99999) cout << "无法乘坐飞机从" << city_start << "到达" << city_end<<endl;
			else{
				cout << "您所需要花费的最少费用为：" << mincost_plane[i][j] << "元" << endl;
				cout << "路线为：" << endl;
				show_path_mincost_plane(i, j);
			}
			return;
		}

		else if (transportation == "火车"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "您所咨询的城市在系统中不存在！！" << endl;
				return;
			}
			if (mincost_train[i][j] == 99999) cout << "无法乘坐火车从" << city_start << "到达" << city_end << endl;
			else{
				cout << "您所需要花费的最少费用为：" << mincost_train[i][j] << "元" << endl;
				cout << "路线为：" << endl;
				show_path_mincost_train(i, j);	
			}
			return;
		}

		else{
			cout << "无此交通工具！！";
			return;
		}
	}


	void consult_mintime(string city_start, string city_end, string transportation){
		if (transportation == "飞机"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "您所咨询的城市在系统中不存在！！" << endl;
				return;
			}
			if (mintime_plane[i][j] == 99999) cout << "无法乘坐飞机从" << city_start << "到达" << city_end << endl;
			else{
				cout << "您所需要花费的最少时间为：" << mintime_plane[i][j] / 60 << "小时" << mintime_plane[i][j] % 60 << "分钟" << endl;
				cout << "路线为：" << endl;
				show_path_mintime_plane(i, j);
			}
			return;
		}

		else if (transportation == "火车"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "您所咨询的城市在系统中不存在！！" << endl;
				return;
			}
			if (mintime_train[i][j] == 99999) cout << "无法乘坐火车从" << city_start << "到达" << city_end << endl;
			else{
				cout << "您所需要花费的最少时间为：" << mintime_train[i][j] / 60 << "小时" << mintime_train[i][j] % 60 << "分钟" << endl;
				cout << "路线为：" << endl;
				show_path_mintime_train(i, j);
			}
			return;
		}

		else{
			cout << "无此交通工具！！";
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

		cout << "于" << time << "从" << citys[k]->city_name << "出发,经过"
			<< roads[k][s.top()]->time_plane << "分钟,于" << time_add(time, to_string(roads[k][s.top()]->time_plane))
			<< "到达" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_plane));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_plane));
			if (!s.empty()){
				cout << "中转等候" << citys[k]->wait_time_plane << "分钟后于" << time << "从" << citys[k]->city_name << "出发,经过"
					<< roads[k][s.top()]->time_plane << "分钟，于" << time_add(time, to_string(roads[k][s.top()]->time_plane)) << "到达"
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

		cout << "于" << time << "从" << citys[k]->city_name << "出发,经过"
			<< roads[k][s.top()]->time_train << "分钟,于" << time_add(time, to_string(roads[k][s.top()]->time_train))
			<< "到达" << citys[s.top()]->city_name << endl;
		time = time_add(time, to_string(roads[k][s.top()]->time_train));

		while (!s.empty()){
			k = s.top();
			s.pop();
			time = time_add(time, to_string(citys[k]->wait_time_train));
			if (!s.empty()){
				cout << "中转等候" << citys[k]->wait_time_train << "分钟后于" << time << "从" << citys[k]->city_name << "出发,经过"
					<< roads[k][s.top()]->time_train << "分钟，于" << time_add(time, to_string(roads[k][s.top()]->time_train)) << "到达"
					<< citys[s.top()]->city_name << endl;
				time = time_add(time, to_string(roads[k][s.top()]->time_train));
			}
			else break;
		}

	}



	void consult_mintransit(string city_start, string city_end, string transportation){
		if (transportation == "飞机"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "您所咨询的城市在系统中不存在！！" << endl;
				return;
			}
			if (newroads_plane[i][j] == 99999) cout << "无法乘坐飞机从" << city_start << "到达" << city_end << endl;
			else{
				int count = 0;
				int k = j;

				while (k != i){
					k = newpaths_plane[i][k];

					count++;
				}
				count--;
				cout << "最少中转次数为：" << count << "次" << endl;
				cout << "路线为：" << endl;
				show_path_mintransit_plane(i, j);
			}
			return;
		}

		else if (transportation == "火车"){
			int i = search_city(city_start);
			int j = search_city(city_end);
			if (i == -1 || j == -1){
				cout << "您所咨询的城市在系统中不存在！！" << endl;
				return;
			}
			if (newroads_train[i][j] == 99999) cout << "无法乘坐火车从" << city_start << "到达" << city_end << endl;
			else{
				int count = 0;
				int k = j;
				while (k != i){
					k = newpaths_train[i][k];
					count++;
				}
				count--;
				cout << "最少中转次数为：" << count << "次" << endl;
				cout << "路线为：" << endl;
				show_path_mintransit_train(i, j);
			}
			return;
		}

		else{
			cout << "无此交通工具！！";
			return;
		}

	}

	
};
