#include<iostream>
using namespace std;
#include"java_Arraylist.hpp"
#include"mystring.hpp"

int main() {
	/*
	1.mystring
	2.mypointers	ָ��
	3.myvector
	4.mylist	˫������
	5.myset������� ��ϣ��
	6.mymap������� ��ϣ��
	
	
	*/

	/*
	��֪���᲻�����˿��ҵ���Ŀ����Ŀǰʵ�ֵ�˳���� mystring myvector mylist
	��Ϊ���Ǳ�ѧ��д�ģ�����ǰ��ѧ�����ұ�ע���Ķ�������㲻�ᱸע�ˡ�
	��Ŀǰ��Ȼ�ϴ�����github�����ǻ�û��д�ꡣ��ΪһЩ����ԭ���Ҳ��ò�����̨�������л�д�������ϴ�����github��Ϊͬ���ֶ�
	*/
	mystring str1 = "һ��������";
	mystring str2 = "abccdefwddccdfdsdsccddfsdfsccdsdfsd";
	//cout << str2.substring(2,8)<< endl;

	vector<mystring> v = str2.split("cd");
	for (vector<mystring>::iterator it = v.begin();it != v.end();it++) {
		cout << (*it) << endl;
	}
}