#pragma once
#include<iostream>
#include<vector>
#
/*
https://blog.csdn.net/zcg3063126760/article/details/144312287
https://blog.csdn.net/BuiderCodes/article/details/143034584
*/
class mystring {
private:
	
	size_t str_length;
	size_t str_size;
	char* _str;

public:

	//------------------------------------------------------------------------------------------------------------
	mystring() {
		this->_str = nullptr;
		this->str_length = 0;
		this->str_size = 0;
	}
	//����һ��ָ�������Ŀ��ַ���������ʹ�ô˺�����Ҫά����str_length
	//�ڵ�ǰ����ֻ������+�ù��˺���������ά��str_length

	//���캯���ĳ�ʼ��˳��ȡ���ڱ�������˳�򣬶����ǹ��캯����˳����ᵼ��ʲô�أ�
	//����ܻᵼ��new char[str_size + 1]��ʱ��str_size��û��ʼ���������쳣
	mystring(size_t str_size) 
		:str_length(0)
		,str_size(str_size)
		,_str(new char[str_size + 1])
	{
		this->_str[0] = '\0';
	}
	//ͨ��c����ַ�������
	mystring(const char* other) 
		:str_length(strlen(other))
		,str_size(str_length)
		,_str(new char[this->str_size +1])
	{
		strcpy_s(this->_str, this->str_size + 1, other);
	}
	//ͨ��string����
	mystring(const string& other) 
		:str_length(size(other))
		,str_size(str_length)
		,_str(new char[this->str_size + 1])
	{
			strcpy_s(this->_str, this->str_size + 1, other.c_str());
	}
	//�������캯��
	mystring(const mystring& _str) 
		: str_length(_str.str_length)
		, str_size(_str.str_size)
		, _str(new char[this->str_size + 1])
	{
			strcpy_s(this->_str, this->str_size + 1, _str._str);
	}
	//�ƶ����캯��
	//����Ͳ���Ҫ��_str���·��䳤���ˣ���Ϊ���Ǵ�����ƶ�����ָ��ָ����һ���Ѿ����ڵ���Դ
	//�������new���䳤�ȣ��ͳ��˸�����
	//ͬ��������������ƶ�=Ҳһ��
	mystring(mystring&& other) noexcept: str_length(0), str_size(0), _str(nullptr){
		this->str_length = other.str_length;
		this->str_size = other.str_size;
		this->_str = other._str;
		//������Ҫ��other��������Ϊ�գ����������������ܻὫ�Ѿ�ת���ߵ���Դ���
		other._str = nullptr;
		other.str_size = 0;
		other.str_length = 0;
	}

	~mystring() {
		delete[] this->_str;
		this->_str = nullptr;
		this->str_length = 0;
		this->str_size = 0;
	}
	//------------------------------------------------------------------------------------------------------------


	//���ظ�ֵ=��
	//������Ҫ�ų����������������Ϊ�˴���Ҫ�ͷ�ԭthis��Դ
	mystring& operator=(const mystring& other) {
		if (this != &other) {
			delete[] this->_str; // �ͷŵ�ǰ���е���Դ
			this->str_length = other.str_length;
			this->str_size = other.str_size;
			this->_str = new char[str_size + 1]; // �������ڴ�
			strcpy_s(this->_str, str_size + 1, other._str);
		}
		return *(this);
	}

	//�����ƶ�=�ţ��ƶ����ܽ���const����
	//ͨ�������ĺ��������ڽ��պ�������ֵ�ĵط�
	mystring& operator=(mystring&& other) noexcept {
		if (this != &other) {
			delete[] this->_str;
			this->str_length = other.str_length;
			this->str_size = other.str_size;
			this->_str = other._str;

			//������Ҫ��my_��������Ϊ�գ����������������ܻὫ�Ѿ�ת���ߵ���Դ���
			other._str = nullptr;
			other.str_size = 0;
			other.str_length = 0;
		}
		return *(this);
	}

	//��C����ַ�����ֵ
	mystring& operator=(const char* str) {
		delete[] this->_str; // �ͷŵ�ǰ���е���Դ
		size_t str_length = strlen(str);
		this->str_length = str_length;
		this->str_size = str_length;
		this->_str = new char[str_length + 1]; // �������ڴ�
		strcpy_s(this->_str, str_size + 1, str);
		return *(this);
	}

	//------------------------------------------------------------------------------------------------------------

	//��ȡ��Ч����
	size_t length() {
		return this->str_length;
	}
	//��ȡsize
	size_t getsize() {
		return this->str_size;
	}
	//�ж��Ƿ�Ϊ��
	bool empty() {
		return this->str_length == 0;
	}
	//���
	void clear() {
		this->_str = new char[1];
		this->_str[0] = '\0';
		this->str_length = 0;
		this->str_size = 0;
	}

	//���������ַ���
	//�˴�ʹ���˿������캯���͸�ֵ=��
	void swap(mystring& other) {
		mystring temp_str(*this);
		*this = other;
		other = temp_str;
	}

	//------------------------------------------------------------------------------------------------------------

	//����+��
	mystring operator+(const mystring& other) {
		size_t temp_len = this->str_length + other.str_length;
		mystring temp_str(temp_len);
		
		strcat_s(temp_str._str, temp_len + 1, this->_str);
		strcat_s(temp_str._str, temp_len + 1, other._str);
		temp_str.str_length = temp_len;
		return temp_str;
	}

	//����+=��
	mystring& operator+=(const mystring& other) {
		size_t temp_len = this->str_size + other.str_size;
		mystring temp_str(this->_str);
		this->_str = new char[temp_len + 1];
		this->_str[0] = '\0';
		strcat_s(this->_str, temp_len + 1, temp_str._str);
		strcat_s(this->_str, temp_len + 1, other._str);
		return *this;
	}

	mystring& append(const mystring& other) {
		size_t temp_len = this->str_size + other.str_size;
		mystring temp_str(this->_str);
		this->_str = new char[temp_len + 1];
		this->_str[0] = '\0';
		strcat_s(this->_str, temp_len + 1, temp_str._str);
		strcat_s(this->_str, temp_len + 1, other._str);
		return *this;
	}

	//------------------------------------------------------------------------------------------------------------

	//����[]
	char &operator[](size_t index) {
		if (index > this->str_length - 1) {
			throw std::out_of_range("index out of range!");
		}
		return this->_str[index];
	}



	//��ָ���±꿪ʼ���ң��������±�
	int find(const char* _str, size_t index = 0) {
		size_t _str_len = strlen(_str);
		if (_str_len == 0) {
			return 0;
		}
		if (_str_len > this->str_length) {
			return -1;
		}
		//�������ʹ��strncmp
		//strncmp(a,b,c)��a���Ƚϵĵ�һ���ַ�����b���Ƚϵĵڶ����ַ�����c���Ƚϵ����λ��
		//char*��������һ���ַ�����ͬʱ����һ�����飬��˿���ͨ���ƶ���ָ���������ַ���
		//������ֵ����0ʱ�����������ַ������
		if (this->str_length > _str_len) {
			size_t check_len = this->str_length - _str_len;
			for (int i = index; i < check_len + 1; i++)
			{
				if (strncmp(this->_str + i, _str, _str_len) == 0) {
					return i;
				}
			}
		}
		return -1;
	}


	//��ȡ�ַ���
	mystring substring(int begin_index, int length = -1) {
		if (length == -1) {
			length = this->str_length - begin_index;
		}
		//����Խ���쳣
		if (begin_index < 0 || begin_index >= this->str_length) {
			throw std::out_of_range("begin_index out of range!");
		}
		if (length < -1 || length > this->str_length - begin_index) {
			throw std::out_of_range("length out of range!");
		}
		mystring temp_str(length);
		int _str_index = 0;
		for (size_t i = 0; i < this->str_length; i++)
		{
			if (i >= begin_index && _str_index < length) {
				temp_str._str[_str_index] = this->_str[i];
				_str_index++;
			}
		}
		temp_str._str[length] = '\0';
		return temp_str;
	}

	//�����ƶ��ַ����ָ��ַ���
	std::vector<mystring> split(const char* keyword) {
		std::vector<mystring> vsplit;
		size_t str_len = strlen(keyword);
		if (str_len == 0) {
			return vsplit;
		}
		bool findflag = true;
		int startindex = 0;
		while (findflag) {
			int index = this->find(keyword, startindex);
			if (index == -1) {
				findflag = false;
				break;
			}
			mystring my_str = substring(startindex, index - startindex);
			vsplit.push_back(my_str);
			startindex = index + str_len;
		}
		return vsplit;
	}

	


	//------------------------------------------------------------------------------------------------------------


	//����������ţ���Ԫ��������˽�г�Ա
	/*
	*�˴������ó���mystring��Ϊ��������Ϊ�����ñ���󶨵�һ�������ֵı���������ָ��һ����ʱ����
	*���������ԣ���Ϊ���������ÿ��԰󶨵���ʱ����
	*����Ļ�����ĳ����������ֵ��mystringʱ��ֱ�ӽ������뵽cout�У���������ʱ�������⵼�±���
	*/
	friend std::ostream& operator<<(std::ostream& cout, const mystring& my_str);

	
	//�����������>>


	//------------------------------------------------------------------------------------------------------------

	//ת��Ϊc����ַ���
	//���˵�˴���һ������char*���Ͳ��ɱ����ͨ��new���䳤�ȣ���������Ҫά��new������char��
	//���˵�˴�ֱ�Ӹ�char*��������this->_str�������ͨ�����ָ��ֱ���޸�mystring���ڲ��ṹ���ⲻ��ȫ
	//����˼�����Ҿ�������const char*
	const char* to_charstring() {
		return this->_str;
	}

	//ת��Ϊstring�ַ���
	std::string to_string() {
		return std::string(this->_str);
	}

	//------------------------------------------------------------------------------------------------------------
	
	//�����ַ�����Ϊ��д

	//ת��Ϊ16����

	
	
};



//�����������
std::ostream& operator<<(std::ostream& cout, const mystring& my_str) {
	std::cout << my_str._str;
	return cout;
}

char* mystrcpy(char* str_a,const size_t size,const char* str_b) {
	size_t len = mystrlen(str_b);
	if (size < len) {
		return nullptr;
	}
	if (str_a == nullptr || str_b == nullptr || size == 0) {
		return nullptr;
	}

	for (size_t i = 0; i < len; i++)
	{
		str_a[i] = str_b[i];
	}
	str_a[len] = '\0';
	return str_a;
}

size_t mystrlen(const char* str) {
	if (str == NULL) {
		return 0;
	}
	size_t i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i;
}

