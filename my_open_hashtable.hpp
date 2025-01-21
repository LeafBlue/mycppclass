#pragma once
#include<vector>
#include<utility>
#include<functional>

//开放寻址法哈希表
/*
开放寻址法，指的是维护一个vector，并且根据特定的函数算出位置，插入数据。
当位置出现冲突时，用另一个特定的函数计算得出其他位置插入
*/
template<class Tkey,class Tval>
class my_open_hashtable {

protected:
	std::vector<std::pair<Tkey, Tval>> hash_t;
	std::vector<int> flag;//标志：0 空槽 1有效槽 2已删除槽
	//维护一个变量：哈希表容量，也是vector长度，省的每次都计算
	size_t capacity;
	//维护插入数据量，方便计算负载因子
	size_t data_num;

	//给定一个自带默认值的成员，它预设了一些质数用来计算扩容值
	const std::vector<size_t> resize_datas = { 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
		97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173 };

	//获取接近质数
	size_t get_resize_datas(size_t size_) {
		//二分查找法
		size_t len = resize_datas.size();
		size_t left = 0;
		size_t right = len - 1;
		if (size_ > resize_datas[right]) {
			return size_;
		}
		while (left < right) {
			size_t mid = (right + left) / 2;
			//质数不可能是2的倍数，传进来的数字已经*2，不用考虑相等情况
			if (resize_datas[mid] > size_) {
				right = mid;
			}
			else if (resize_datas[mid] < size_) {
				left = mid + 1;
			}
		}
		return resize_datas[left];
	}

	//检测负载因子是否超过0.7，判断是否要扩容
	void chech_len() {
		size_t data = static_cast<size_t>((data_num * 10) / capacity);
		if (data >= 7) {
			//扩容
			v_resize();
		}
	}

	//扩容函数，扩大两倍，然后找到距离两倍数最近的质数，作为扩容后的大小
	void v_resize() {
		size_t hash_size = hash_t.size();
		size_t new_size = get_resize_datas(hash_size * 2);
		std::vector<pair<Tkey, Tval>> new_hash_t(new_size);
		std::vector<int> new_flag(new_size);
		//因为我的添加的函数都是基于成员vector的，所以在这里先交换一下
		new_hash_t.swap(hash_t);
		new_flag.swap(flag);
		capacity = new_size;
		//计数器重置
		data_num = 0;
		for (size_t i = 0; i < hash_size; i++)
		{
			//当前位置 已占用，不是空，也不是已删除，理论上我只要处理这些就行了
			if (new_flag[i] == 1) {
				insertdata(new_hash_t[i].first, new_hash_t[i].second);
			}
		}
	}

	//计算哈希值
	//这个函数使用了标准库hash计算法，并且象征性地特别考虑了int和string
	size_t get_hash_index(const Tkey& key) {
		if constexpr (std::is_same_v<Tkey, int>) {
			return std::hash<int>()(key) % capacity;
		}
		else if constexpr (std::is_same_v<Tkey, string>) {
			size_t hashvalue = 0;
			for (size_t i = 0; i < key.size(); i++)
			{
				char c = key[i];
				hashvalue = hashvalue * 31 + c;
			}
			return hashvalue % capacity;
		}
		else {
			return std::hash<Tkey>()(key) % capacity;
		}
	}

	//第二个函数
	//此函数应满足以下条件：
	// 1，不为0，否则会无限循环在同一位置
	// 2，最好与哈希表大小互质
	size_t get_step(const Tkey& key) {
		//先得到主哈希值，这里我们直接调用标准库函数
		size_t main_hash = std::hash<Tkey>()(key);
		//设置一个固定的较小模数
		size_t mod_data = 7;
		//公式（万一算出来是0，再加一个1）
		return (mod_data - (main_hash % mod_data)) % mod_data + 1;
	}

public:
	//构造方法
	//这里初始大小可以设为13
	my_open_hashtable() {
		hash_t.resize(13);
		flag.resize(13, 0);
		capacity = 13;
		data_num = 0;
	}

	my_open_hashtable(size_t size) {
		hash_t.resize(size);
		flag.resize(size, 0);
		capacity = size;
		data_num = 0;
	}

	//析构函数，因为vector和pair都带有自己的析构函数，所以不需要额外处理
	~my_open_hashtable() = default;

	//插入数据
	void insertdata(const Tkey& key, const Tval& val) {
		size_t index = get_hash_index(key);
		std::pair<Tkey, Tval> p1(key, val);
		if (index >= capacity) {
			//扩容
			v_resize();
		}
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//可直接插入
			flag[index] = 1;
			hash_t[index] = p1;
			++data_num;
			chech_len();
		}
		else if (flag_num == 1) {
			//占用情况
			/*
				这里有几种解法。
				1，线性探测，当前位置被占用就去找下一个。这很容易造成堆积，甚至退化成线性查找
				2，二次探测，通过一个计算公式获取下个探查位置
				3，双重散列，设置两个计算哈希的函数，根据第二个函数返回一个步长，作为增量
					双重散列的公式： (函数1 + i * 函数2)) % capacity，其中i为探测次数，从0开始
			*/


			//占用情况下，key键和要插入的数据是否一致，如果一致，可更新val值
			if (hash_t[index].first == key) {
				hash_t[index].second = val;
				return;
			}
			//定义一个检测次数，每次循环减少一次
			size_t check_times = capacity;
			while (true) {

				//探测下一个坐标
				//这里采用取模运算，保证取值一直在表容量内
				index = (index + get_step(key)) % capacity;
				if (check_times == 0) {
					//check_times减完了没找到，扩容
					v_resize();
					//更新index初始值和查找次数
					index = get_hash_index(key);
					check_times = capacity;
					continue;
				}
				flag_num = flag.at(index);
				if (flag_num == 1) {//占用情况，检查当前占用值是不是key，是的话修改值后结束函数
					if (hash_t[index].first == key) {
						hash_t[index].second = val;
						return;
					}
				}
				else {//剩下空槽和删除情况
					//插入数据
					flag[index] = 1;
					hash_t[index] = p1;
					++data_num;
					chech_len();
					break;
				}
				--check_times;
				//继续循环
			}
		}
		else if (flag_num == 2) {
			//已删除
			//可直接插入
			flag[index] = 1;
			hash_t[index] = p1;
			++data_num;
			chech_len();
		}
	}

	//查找
	Tval* find_key(const Tkey& key) {
		//计算它有可能在的位置
		size_t index = get_hash_index(key);
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//空槽，说明数据根本没有进来过
			return nullptr;
		}
		else if (flag_num == 1) {
			//占用，检查当前是否一致
			if (hash_t[index].first == key) {
				return &hash_t[index].second;
			}
		}
		//走到这里的皆是需要查找其他位置
		//执行查找其他位置操作
		size_t check_times = capacity;
		while (true) {
			if (check_times == 0) {
				//check_times减完了没找到
				return nullptr;
			}
			//探测下一个坐标
			//这里采用取模运算，保证取值一直在表容量内
			index = (index + get_step(key)) % capacity;
			flag_num = flag.at(index);
			if (flag_num == 1) {//占用情况
				if (hash_t[index].first == key) {
					return &hash_t[index].second;
				}
			}
			--check_times;
			//继续循环
		}
	}


	//删除
	void delete_key(const Tkey& key) {
		//计算它有可能在的位置
		size_t index = get_hash_index(key);
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//空槽，说明没有这个数据
			return;
		}
		else if (flag_num == 1) {
			//占用，检查当前是否一致
			if (hash_t[index].first == key) {
				//此处如果使用erase，会使得后面的数据前移
				//等待此处数据被填充即可，修改标志位
				//hash_t[index] = p1;
				flag[index] = 2;//标志设为已删除
				--data_num;
				return;
			}
		}
		//走到这里的皆是需要查找其他位置
		//执行查找其他位置操作
		size_t check_times = capacity;
		while (true) {
			if (check_times == 0) {
				//check_times减完了没找到
				return;
			}
			//探测下一个坐标
			//这里采用取模运算，保证取值一直在表容量内
			index = (index + get_step(key)) % capacity;
			flag_num = flag.at(index);
			if (flag_num == 1) {//占用情况
				if (hash_t[index].first == key) {
					flag[index] = 2;//标志设为已删除
					--data_num;
					return;
				}
			}
			--check_times;
			//继续循环
		}
	}

	size_t count(const Tkey& key) {
		//计算它有可能在的位置
		size_t index = get_hash_index(key);
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//空槽，说明数据根本没有进来过
			return 0;
		}
		else if (flag_num == 1) {
			//占用，检查当前是否一致
			if (hash_t[index].first == key) {
				return 1;
			}
		}
		//走到这里的皆是需要查找其他位置
		//执行查找其他位置操作
		size_t check_times = capacity;
		while (true) {
			if (check_times == 0) {
				//check_times减完了没找到
				return 0;
			}
			//探测下一个坐标
			//这里采用取模运算，保证取值一直在表容量内
			index = (index + get_step(key)) % capacity;
			flag_num = flag.at(index);
			if (flag_num == 1) {//占用情况
				if (hash_t[index].first == key) {
					return 1;
				}
			}
			--check_times;
			//继续循环
		}
	}

	//为哈希表设计迭代器
	class iterator {
	private:
		//成员
		my_open_hashtable<Tkey,Tval>* hash_table;
		size_t index_;

		//用这个做返回值精准控制值，控制key不会被修改
		using reference = std::pair<const Tkey&, Tval&>;
		using pointer = std::pair<const Tkey*, Tval*>;

		bool get_next() {
			if (index_ < hash_table->capacity) {
				++index_;//不检查当前index
			}
			else {
				return false;
			}
			while (index_ < hash_table->capacity)
			{
				if (hash_table->flag[index_] == 1) {//1：有效槽
					return true;
				}
				++index_;
			}
			return false;
		}

		bool get_prev() {
			if (index_ > 0) {
				--index_;
			}
			else {
				return false;
			}
			while (index_ >= 0)
			{
				if (my_open_hashtable::flag[index_] == 1) {//1：有效槽
					return true;
				}
				--index_;
			}
			return false;
		}

		friend class my_open_hashtable<Tkey, Tval>;

	public:
		iterator() {}

		iterator(my_open_hashtable<Tkey, Tval>* new_hash_t,size_t newindex) {
			hash_table = new_hash_t;
			index_ = newindex;

			//确保第一个index是有效的
			if (index_ < hash_table->capacity&& hash_table->flag[index_] != 1) {
				get_next();
			}
		}

		// * 和 ->
		//我们这里返回的是一个std::pair<const Tkey&, Tval&>，因此我们需要构造一个临时的pair，它里面的两个键绑定了容器里的数据
		reference operator*() {
			return { hash_table->hash_t[index_].first, hash_table->hash_t[index_].second };
		}
		//我们这里返回的是一个std::pair<const Tkey*, Tval*>
		//在这两个例子中，我们返回的是一个临时对象，但是它内部包含的是指针和引用。
		//指针和引用的值来自容器内部，是一个长期存在的值。
		//因此这个临时对象会被销毁，但我们依然可以访问到里面的值。
		pointer operator->() {
			return { hash_table->hash_t[index_].first, hash_table->hash_t[index_].second };
		}

		iterator operator+(int n) {
			while (n > 0) {
				get_next();
				n--;
			}
			return this;
		}
		//这个函数的实现不支持负数
		iterator operator+(size_t n) {
			iterator temp = *this;
			while (n > 0) {
				if (!temp.get_next()) {
					break;
				}
				--n;
			}
			if (n > 0) {
				temp.index_ = hash_table->capacity;
			}
			return temp;
		}
		//这个函数的实现不支持负数
		iterator operator-(size_t n) {
			iterator temp = *this;
			while (n > 0) {
				if (!temp.get_prev()) {
					break;
				}
				--n;
			}
			if (n > 0) {
				temp.index_ = -1;
			}
			return temp;
		}

		
		iterator& operator++() {
			if (get_next()) {
				return *this;
			}
			index_ = hash_table->capacity;
			return *this;
		}

		iterator& operator--() {
			if (get_prev()) {
				return *this;
			}
			index_ = -1;
			return *this;
		}

		iterator operator++(int) {
			iterator temp = *this;
			if (get_next()) {
				return temp;
			}
			index_ = hash_table->capacity;
			return temp;
		}

		iterator operator--(int) {
			iterator temp = *this;
			if (get_prev()) {
				return *temp;
			}
			index_ = -1;
			return *temp;
		}

		bool operator==(const iterator& other)const noexcept {
			return index_ == other.index_;
		}

		bool operator!=(const iterator& other)const noexcept {
			return index_ != other.index_;
		}

	};

	iterator begin() {
		return iterator(this,0);
	}

	iterator end() {
		return iterator(this, capacity);
	}

	size_t getcount() {
		return data_num;
	}

	size_t getcapacity() {
		return capacity;
	}
};