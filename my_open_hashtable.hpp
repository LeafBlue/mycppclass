#pragma once
#include<vector>
#include<utility>
#include<functional>

//����Ѱַ����ϣ��
/*
����Ѱַ����ָ����ά��һ��vector�����Ҹ����ض��ĺ������λ�ã��������ݡ�
��λ�ó��ֳ�ͻʱ������һ���ض��ĺ�������ó�����λ�ò���
*/
template<class Tkey,class Tval>
class my_open_hashtable {

protected:
	std::vector<std::pair<Tkey, Tval>> hash_t;
	std::vector<int> flag;//��־��0 �ղ� 1��Ч�� 2��ɾ����
	//ά��һ����������ϣ��������Ҳ��vector���ȣ�ʡ��ÿ�ζ�����
	size_t capacity;
	//ά��������������������㸺������
	size_t data_num;

	//����һ���Դ�Ĭ��ֵ�ĳ�Ա����Ԥ����һЩ����������������ֵ
	const std::vector<size_t> resize_datas = { 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
		97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173 };

	//��ȡ�ӽ�����
	size_t get_resize_datas(size_t size_) {
		//���ֲ��ҷ�
		size_t len = resize_datas.size();
		size_t left = 0;
		size_t right = len - 1;
		if (size_ > resize_datas[right]) {
			return size_;
		}
		while (left < right) {
			size_t mid = (right + left) / 2;
			//������������2�ı������������������Ѿ�*2�����ÿ���������
			if (resize_datas[mid] > size_) {
				right = mid;
			}
			else if (resize_datas[mid] < size_) {
				left = mid + 1;
			}
		}
		return resize_datas[left];
	}

	//��⸺�������Ƿ񳬹�0.7���ж��Ƿ�Ҫ����
	void chech_len() {
		size_t data = static_cast<size_t>((data_num * 10) / capacity);
		if (data >= 7) {
			//����
			v_resize();
		}
	}

	//���ݺ���������������Ȼ���ҵ������������������������Ϊ���ݺ�Ĵ�С
	void v_resize() {
		size_t hash_size = hash_t.size();
		size_t new_size = get_resize_datas(hash_size * 2);
		std::vector<pair<Tkey, Tval>> new_hash_t(new_size);
		std::vector<int> new_flag(new_size);
		//��Ϊ�ҵ���ӵĺ������ǻ��ڳ�Աvector�ģ������������Ƚ���һ��
		new_hash_t.swap(hash_t);
		new_flag.swap(flag);
		capacity = new_size;
		//����������
		data_num = 0;
		for (size_t i = 0; i < hash_size; i++)
		{
			//��ǰλ�� ��ռ�ã����ǿգ�Ҳ������ɾ������������ֻҪ������Щ������
			if (new_flag[i] == 1) {
				insertdata(new_hash_t[i].first, new_hash_t[i].second);
			}
		}
	}

	//�����ϣֵ
	//�������ʹ���˱�׼��hash���㷨�����������Ե��ر�����int��string
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

	//�ڶ�������
	//�˺���Ӧ��������������
	// 1����Ϊ0�����������ѭ����ͬһλ��
	// 2��������ϣ���С����
	size_t get_step(const Tkey& key) {
		//�ȵõ�����ϣֵ����������ֱ�ӵ��ñ�׼�⺯��
		size_t main_hash = std::hash<Tkey>()(key);
		//����һ���̶��Ľ�Сģ��
		size_t mod_data = 7;
		//��ʽ����һ�������0���ټ�һ��1��
		return (mod_data - (main_hash % mod_data)) % mod_data + 1;
	}

public:
	//���췽��
	//�����ʼ��С������Ϊ13
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

	//������������Ϊvector��pair�������Լ����������������Բ���Ҫ���⴦��
	~my_open_hashtable() = default;

	//��������
	void insertdata(const Tkey& key, const Tval& val) {
		size_t index = get_hash_index(key);
		std::pair<Tkey, Tval> p1(key, val);
		if (index >= capacity) {
			//����
			v_resize();
		}
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//��ֱ�Ӳ���
			flag[index] = 1;
			hash_t[index] = p1;
			++data_num;
			chech_len();
		}
		else if (flag_num == 1) {
			//ռ�����
			/*
				�����м��ֽⷨ��
				1������̽�⣬��ǰλ�ñ�ռ�þ�ȥ����һ�������������ɶѻ��������˻������Բ���
				2������̽�⣬ͨ��һ�����㹫ʽ��ȡ�¸�̽��λ��
				3��˫��ɢ�У��������������ϣ�ĺ��������ݵڶ�����������һ����������Ϊ����
					˫��ɢ�еĹ�ʽ�� (����1 + i * ����2)) % capacity������iΪ̽���������0��ʼ
			*/


			//ռ������£�key����Ҫ����������Ƿ�һ�£����һ�£��ɸ���valֵ
			if (hash_t[index].first == key) {
				hash_t[index].second = val;
				return;
			}
			//����һ����������ÿ��ѭ������һ��
			size_t check_times = capacity;
			while (true) {

				//̽����һ������
				//�������ȡģ���㣬��֤ȡֵһֱ�ڱ�������
				index = (index + get_step(key)) % capacity;
				if (check_times == 0) {
					//check_times������û�ҵ�������
					v_resize();
					//����index��ʼֵ�Ͳ��Ҵ���
					index = get_hash_index(key);
					check_times = capacity;
					continue;
				}
				flag_num = flag.at(index);
				if (flag_num == 1) {//ռ���������鵱ǰռ��ֵ�ǲ���key���ǵĻ��޸�ֵ���������
					if (hash_t[index].first == key) {
						hash_t[index].second = val;
						return;
					}
				}
				else {//ʣ�¿ղۺ�ɾ�����
					//��������
					flag[index] = 1;
					hash_t[index] = p1;
					++data_num;
					chech_len();
					break;
				}
				--check_times;
				//����ѭ��
			}
		}
		else if (flag_num == 2) {
			//��ɾ��
			//��ֱ�Ӳ���
			flag[index] = 1;
			hash_t[index] = p1;
			++data_num;
			chech_len();
		}
	}

	//����
	Tval* find_key(const Tkey& key) {
		//�������п����ڵ�λ��
		size_t index = get_hash_index(key);
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//�ղۣ�˵�����ݸ���û�н�����
			return nullptr;
		}
		else if (flag_num == 1) {
			//ռ�ã���鵱ǰ�Ƿ�һ��
			if (hash_t[index].first == key) {
				return &hash_t[index].second;
			}
		}
		//�ߵ�����Ľ�����Ҫ��������λ��
		//ִ�в�������λ�ò���
		size_t check_times = capacity;
		while (true) {
			if (check_times == 0) {
				//check_times������û�ҵ�
				return nullptr;
			}
			//̽����һ������
			//�������ȡģ���㣬��֤ȡֵһֱ�ڱ�������
			index = (index + get_step(key)) % capacity;
			flag_num = flag.at(index);
			if (flag_num == 1) {//ռ�����
				if (hash_t[index].first == key) {
					return &hash_t[index].second;
				}
			}
			--check_times;
			//����ѭ��
		}
	}


	//ɾ��
	void delete_key(const Tkey& key) {
		//�������п����ڵ�λ��
		size_t index = get_hash_index(key);
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//�ղۣ�˵��û���������
			return;
		}
		else if (flag_num == 1) {
			//ռ�ã���鵱ǰ�Ƿ�һ��
			if (hash_t[index].first == key) {
				//�˴����ʹ��erase����ʹ�ú��������ǰ��
				//�ȴ��˴����ݱ���伴�ɣ��޸ı�־λ
				//hash_t[index] = p1;
				flag[index] = 2;//��־��Ϊ��ɾ��
				--data_num;
				return;
			}
		}
		//�ߵ�����Ľ�����Ҫ��������λ��
		//ִ�в�������λ�ò���
		size_t check_times = capacity;
		while (true) {
			if (check_times == 0) {
				//check_times������û�ҵ�
				return;
			}
			//̽����һ������
			//�������ȡģ���㣬��֤ȡֵһֱ�ڱ�������
			index = (index + get_step(key)) % capacity;
			flag_num = flag.at(index);
			if (flag_num == 1) {//ռ�����
				if (hash_t[index].first == key) {
					flag[index] = 2;//��־��Ϊ��ɾ��
					--data_num;
					return;
				}
			}
			--check_times;
			//����ѭ��
		}
	}

	size_t count(const Tkey& key) {
		//�������п����ڵ�λ��
		size_t index = get_hash_index(key);
		int flag_num = flag.at(index);
		if (flag_num == 0) {
			//�ղۣ�˵�����ݸ���û�н�����
			return 0;
		}
		else if (flag_num == 1) {
			//ռ�ã���鵱ǰ�Ƿ�һ��
			if (hash_t[index].first == key) {
				return 1;
			}
		}
		//�ߵ�����Ľ�����Ҫ��������λ��
		//ִ�в�������λ�ò���
		size_t check_times = capacity;
		while (true) {
			if (check_times == 0) {
				//check_times������û�ҵ�
				return 0;
			}
			//̽����һ������
			//�������ȡģ���㣬��֤ȡֵһֱ�ڱ�������
			index = (index + get_step(key)) % capacity;
			flag_num = flag.at(index);
			if (flag_num == 1) {//ռ�����
				if (hash_t[index].first == key) {
					return 1;
				}
			}
			--check_times;
			//����ѭ��
		}
	}

	//Ϊ��ϣ����Ƶ�����
	class iterator {
	private:
		//��Ա
		my_open_hashtable<Tkey,Tval>* hash_table;
		size_t index_;

		//�����������ֵ��׼����ֵ������key���ᱻ�޸�
		using reference = std::pair<const Tkey&, Tval&>;
		using pointer = std::pair<const Tkey*, Tval*>;

		bool get_next() {
			if (index_ < hash_table->capacity) {
				++index_;//����鵱ǰindex
			}
			else {
				return false;
			}
			while (index_ < hash_table->capacity)
			{
				if (hash_table->flag[index_] == 1) {//1����Ч��
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
				if (my_open_hashtable::flag[index_] == 1) {//1����Ч��
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

			//ȷ����һ��index����Ч��
			if (index_ < hash_table->capacity&& hash_table->flag[index_] != 1) {
				get_next();
			}
		}

		// * �� ->
		//�������ﷵ�ص���һ��std::pair<const Tkey&, Tval&>�����������Ҫ����һ����ʱ��pair������������������������������
		reference operator*() {
			return { hash_table->hash_t[index_].first, hash_table->hash_t[index_].second };
		}
		//�������ﷵ�ص���һ��std::pair<const Tkey*, Tval*>
		//�������������У����Ƿ��ص���һ����ʱ���󣬵������ڲ���������ָ������á�
		//ָ������õ�ֵ���������ڲ�����һ�����ڴ��ڵ�ֵ��
		//��������ʱ����ᱻ���٣���������Ȼ���Է��ʵ������ֵ��
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
		//���������ʵ�ֲ�֧�ָ���
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
		//���������ʵ�ֲ�֧�ָ���
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