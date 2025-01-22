#include<iostream>
using namespace std;
#include"mystring.hpp"
#include"myvector.hpp"
#include"mylist.hpp"
#include"my_unique_ptr.hpp"
#include"my_share_ptr.hpp"
#include"my_open_hashtable.hpp"
#include <chrono>
#include <string>
#include <unordered_map>
#include"my_unordered_map.hpp"

int main() {
	/*
	1.mystring
	2.mypointers	指针
	3.myvector
	4.mylist	双向链表
	5.myset（红黑树 哈希表）
	6.mymap（红黑树 哈希表）
	
	
	*/

	/*
	不知道会不会有人看我的项目，我目前实现的顺序是 mystring myvector mylist
	因为我是边学边写的，所以前面学到并且备注过的东西后面便不会备注了。
	我目前虽然上传到了github，但是还没有写完。因为一些特殊原因，我不得不在两台电脑上切换写，所以上传到了github作为同步手段
	*/
	
	/*
	mystring str1 = "一二三四五";
	mystring str2 = "abccdefwddccdfdsdsccddfsdfsccdsdfsd";
	//cout << str2.substring(2,8)<< endl;
	

	mystring str2 = "abccdefwddccdfdsdsccddfsdfsccdsdfsd";
	vector<mystring> v = str2.split("cd");
	for (vector<mystring>::iterator it = v.begin();it != v.end();it++) {
		cout << (*it) << endl;
	}*/

	/*
	myvector<mystring> v;
	v.push_back("一");
	v.push_back("二");
	v.push_back("三");
	for (myvector<mystring>::iterator it = v.begin(); it != v.end(); it++) {
		cout << (*it) << endl;
	}
	*/
	/*
	mylist<mystring> my_list;
	my_list.insertBack("一");
	my_list.insertBack("2");
	my_list.insertBack("3");
	for (mylist<mystring>::iterator it = my_list.begin(); it != my_list.end(); it++) {
		cout << (*it) << endl;
	}
	*/
	/*
	my_unique_ptr<mystring> str = my_unique_ptr<mystring>::makemy_unique_ptr("独立指针");
	cout << *str << endl;
	my_unique_ptr<mystring> str1 = str;
	*/
	/*
	my_share_ptr<mystring> str1 = my_share_ptr<mystring>(new mystring("共享指针1"));
	
	my_share_ptr<mystring> str2 = str1;

	my_share_ptr<mystring> str3 = str1;

	cout << str2.use_count()<< endl;
	*/

	/*
	my_open_hashtable<string, mystring> *moh = new my_open_hashtable<string, mystring>();
	moh->insertdata("一", "张三");
	moh->insertdata("二", "李四");
	moh->insertdata("三", "王五");

	for (my_open_hashtable<string, mystring>::iterator it = moh->begin(); it != moh->end(); it++) {
		cout << (*it).first << "--" << (*it).second << endl;
	}*/
	/*
	my_unordered_map<std::string, int> hashtable(10000);
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10000; ++i)
	{
		hashtable.insert(std::to_string(i),i);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> insert_time = end - start;
	std::cout << "Insert time: \t" << insert_time.count() << "\tseconds" << std::endl;

	

	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10000; ++i)
	{
		hashtable.find(std::to_string(i));
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> find_time = end - start;
	std::cout << "find time: \t" << find_time.count() << "\tseconds" << std::endl;


	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10000; ++i)
	{
		hashtable.erase(std::to_string(i));
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> erase_time = end - start;
	std::cout << "erase time: \t" << erase_time.count() << "\tseconds" << std::endl;


	unordered_map<std::string, int> map(10000);
	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10000; ++i)
	{
		map.insert({ std::to_string(i), i });
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> insert_time1 = end - start;
	std::cout << "Insert time: \t" << insert_time1.count() << "\tseconds" << std::endl;



	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10000; ++i)
	{
		map.find(std::to_string(i));
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> find_time1 = end - start;
	std::cout << "find time: \t" << find_time1.count() << "\tseconds" << std::endl;


	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 10000; ++i)
	{
		map.erase(std::to_string(i));
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> erase_time1 = end - start;
	std::cout << "erase time: \t" << erase_time1.count() << "\tseconds" << std::endl;
	*/

	/*
		比较结果：
		Insert time:    0.0178895       seconds
		find time:      0.0058469       seconds
		erase time:     0.005223        seconds
		Insert time:    0.013245        seconds
		find time:      0.0052772       seconds
		erase time:     0.0074544       seconds	
	*/
}










































