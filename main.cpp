#include<iostream>
using namespace std;
#include"java_Arraylist.hpp"
#include"mystring.hpp"

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
	mystring str1 = "一二三四五";
	mystring str2 = "abccdefwddccdfdsdsccddfsdfsccdsdfsd";
	//cout << str2.substring(2,8)<< endl;

	vector<mystring> v = str2.split("cd");
	for (vector<mystring>::iterator it = v.begin();it != v.end();it++) {
		cout << (*it) << endl;
	}
}