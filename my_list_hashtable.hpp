#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<utility>
#include<Iterator>
#include<algorithm>


//����ַ����ϣ��
/*
����ַ����ָ����ά��һ��vector������洢һ����������Щ�����ֽ�Ͱ�����ݾʹ洢��Ͱ�
����ַ��ͻʱ��ֻ�Ǵ������ǻ���뵽ͬһ��������Ĳ�ͬλ��

����ַ��ʵ�ּ򵥣��ɴ��������ͻ����Ϊ�ײ����������Բ���ɾ���ȽϿ졣
*/
template<typename Tkey,typename Tval>
class my_list_hashtable {
private:
	/*
		����ַ����ϣ����vector�д洢������Щ������ΪͰ����������ʱ��
		���ݼ���ó����±��ҵ���Ӧ������Ȼ����뵽�����С�
		��������ȫһ�µ�����ʱ��ֱ�Ӹ���
	*/
	using Bucket = std::list< std::pair<Tkey, Tval>>;
	using v_Bucket = std::vector<Bucket>;
	v_Bucket buckets;


	//�����ϣֵ
	size_t get_bucket_index(Tkey& key) {
		if constexpr (std::is_same_v<Tkey,int>) {
			return key % buckets.size();
		}
		else if constexpr (std::is_same_v<Tkey, std::string>) {
			size_t hashvalue = 0;
			for (size_t i = 0; i < key.size; i++)
			{
				char c = key[i];
				hashvalue = hashvalue * 31 + c;
			}
			return hashvalue % buckets.size();
		}
		else {
			//��������
			//������õ���hash������()���ţ��������������������÷�
			return std::hash<Tkey>()(key) % buckets.size();
		}
	}


public:
	//���죬����13��ֵ
	my_list_hashtable()
		:buckets(13){}

	//�������
	void insert(Tkey& key,Tval& val) {
		size_t index = get_bucket_index(key);
		pair<Tkey, Tval> p(key,val);
		bool isexist = false;
		if (index < buckets.size()) {
			std::list<std::pair<Tkey, Tval>>& buckets_one = buckets.at(index);
			for (auto it = buckets_one.begin(); it != buckets_one.end(); it++) {
				if (it->first == key) {
					it->second = val;
					isexist = true;
					break;
				}
			}
		}
		if (!isexist) {
			if (index >= buckets.size()) {
				buckets.resize(index + 1);
			}
			std::list<std::pair<Tkey, Tval>>& buckets_one = buckets.at(index);
			buckets_one.push_back(p);
		}
	}

	//����
	Tval* find_v(Tkey key) {
		size_t index = get_bucket_index(key);
		if (index < buckets.size()) {
			std::list<std::pair<Tkey, Tval>>& buckets_one = buckets.at(index);
			for (auto it = buckets_one.begin(); it != buckets_one.end(); it++) {
				if (it->first == key) {
					return &it->second;
				}
			}
		}
		return nullptr;
	}

	//ɾ��
	bool delete_k(Tkey key) {
		size_t index = get_bucket_index(key);
		if (index < buckets.size()) {
			std::list<std::pair<Tkey, Tval>>& buckets_one = buckets.at(index);
			for (auto it = buckets_one.begin(); it != buckets_one.end(); it++) {
				if (it->first == key) {
					buckets_one.erase(it);
					return true;
				}
			}
		}
		return false;
	}

};