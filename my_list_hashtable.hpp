#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<utility>
#include<Iterator>
#include<algorithm>


//链地址法哈希表
/*
链地址法，指的是维护一个vector，里面存储一个个链表，这些链表又叫桶，数据就存储在桶里。
当地址冲突时，只是代表它们会插入到同一个链表里的不同位置

链地址法实现简单，可处理大量冲突，因为底层是链表，所以插入删除比较快。
*/
template<typename Tkey,typename Tval>
class my_list_hashtable {
private:
	/*
		链地址法哈希表，在vector中存储链表，这些链表被称为桶，插入数据时，
		根据计算得出的下标找到对应的链表，然后插入到链表中。
		当出现完全一致的数据时，直接覆盖
	*/
	using Bucket = std::list< std::pair<Tkey, Tval>>;
	using v_Bucket = std::vector<Bucket>;
	v_Bucket buckets;


	//计算哈希值
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
			//其它类型
			//这里调用的是hash类重载()符号，将对象名当作函数名用法
			return std::hash<Tkey>()(key) % buckets.size();
		}
	}


public:
	//构造，给个13初值
	my_list_hashtable()
		:buckets(13){}

	//插入操作
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

	//查找
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

	//删除
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