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
	//构造一个指定容量的空字符串，如需使用此函数需要维护好str_length
	//在当前类中只有重载+用过此函数，且已维护str_length

	//构造函数的初始化顺序，取决于变量声明顺序，而不是构造函数的顺序，这会导致什么呢？
	//这可能会导致new char[str_size + 1]的时候，str_size还没初始化，出现异常
	mystring(size_t str_size) 
		:str_length(0)
		,str_size(str_size)
		,_str(new char[str_size + 1])
	{
		this->_str[0] = '\0';
	}
	//通过c风格字符串构造
	mystring(const char* other) 
		:str_length(strlen(other))
		,str_size(str_length)
		,_str(new char[this->str_size +1])
	{
		strcpy_s(this->_str, this->str_size + 1, other);
	}
	//通过string构造
	mystring(const string& other) 
		:str_length(size(other))
		,str_size(str_length)
		,_str(new char[this->str_size + 1])
	{
			strcpy_s(this->_str, this->str_size + 1, other.c_str());
	}
	//拷贝构造函数
	mystring(const mystring& _str) 
		: str_length(_str.str_length)
		, str_size(_str.str_size)
		, _str(new char[this->str_size + 1])
	{
			strcpy_s(this->_str, this->str_size + 1, _str._str);
	}
	//移动构造函数
	//这里就不需要给_str重新分配长度了，因为这是纯粹的移动，将指针指向了一个已经存在的资源
	//如果重新new分配长度，就成了复制了
	//同样的下面的重载移动=也一样
	mystring(mystring&& other) noexcept: str_length(0), str_size(0), _str(nullptr){
		this->str_length = other.str_length;
		this->str_size = other.str_size;
		this->_str = other._str;
		//这里需要把other的内容设为空，否则析构函数可能会将已经转移走的资源清空
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


	//重载赋值=号
	//这里需要排除拷贝自身情况，因为此处需要释放原this资源
	mystring& operator=(const mystring& other) {
		if (this != &other) {
			delete[] this->_str; // 释放当前持有的资源
			this->str_length = other.str_length;
			this->str_size = other.str_size;
			this->_str = new char[str_size + 1]; // 分配新内存
			strcpy_s(this->_str, str_size + 1, other._str);
		}
		return *(this);
	}

	//重载移动=号，移动不能接收const函数
	//通常这样的函数是用在接收函数返回值的地方
	mystring& operator=(mystring&& other) noexcept {
		if (this != &other) {
			delete[] this->_str;
			this->str_length = other.str_length;
			this->str_size = other.str_size;
			this->_str = other._str;

			//这里需要把my_的内容设为空，否则析构函数可能会将已经转移走的资源清空
			other._str = nullptr;
			other.str_size = 0;
			other.str_length = 0;
		}
		return *(this);
	}

	//从C风格字符串赋值
	mystring& operator=(const char* str) {
		delete[] this->_str; // 释放当前持有的资源
		size_t str_length = strlen(str);
		this->str_length = str_length;
		this->str_size = str_length;
		this->_str = new char[str_length + 1]; // 分配新内存
		strcpy_s(this->_str, str_size + 1, str);
		return *(this);
	}

	//------------------------------------------------------------------------------------------------------------

	//获取有效长度
	size_t length() {
		return this->str_length;
	}
	//获取size
	size_t getsize() {
		return this->str_size;
	}
	//判断是否为空
	bool empty() {
		return this->str_length == 0;
	}
	//清空
	void clear() {
		this->_str = new char[1];
		this->_str[0] = '\0';
		this->str_length = 0;
		this->str_size = 0;
	}

	//交换两个字符串
	//此处使用了拷贝构造函数和赋值=号
	void swap(mystring& other) {
		mystring temp_str(*this);
		*this = other;
		other = temp_str;
	}

	//------------------------------------------------------------------------------------------------------------

	//重载+号
	mystring operator+(const mystring& other) {
		size_t temp_len = this->str_length + other.str_length;
		mystring temp_str(temp_len);
		
		strcat_s(temp_str._str, temp_len + 1, this->_str);
		strcat_s(temp_str._str, temp_len + 1, other._str);
		temp_str.str_length = temp_len;
		return temp_str;
	}

	//重载+=号
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

	//重载[]
	char &operator[](size_t index) {
		if (index > this->str_length - 1) {
			throw std::out_of_range("index out of range!");
		}
		return this->_str[index];
	}



	//从指定下标开始查找，并返回下标
	int find(const char* _str, size_t index = 0) {
		size_t _str_len = strlen(_str);
		if (_str_len == 0) {
			return 0;
		}
		if (_str_len > this->str_length) {
			return -1;
		}
		//这里可以使用strncmp
		//strncmp(a,b,c)，a：比较的第一个字符串，b：比较的第二个字符串，c：比较的最多位数
		//char*不仅仅是一个字符串，同时还是一个数组，因此可以通过移动首指针来处理字符串
		//当返回值等于0时，代表两个字符串相等
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


	//截取字符串
	mystring substring(int begin_index, int length = -1) {
		if (length == -1) {
			length = this->str_length - begin_index;
		}
		//处理越界异常
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

	//根据制定字符串分割字符串
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


	//重载输出符号，友元用来访问私有成员
	/*
	*此处必须用常量mystring作为参数，因为【引用必须绑定到一个有名字的变量，不能指向一个临时对象】
	*而常量可以，因为【常量引用可以绑定到临时对象】
	*否则的话，当某个函数返回值是mystring时，直接将它放入到cout中，会由于临时对象问题导致报错
	*/
	friend std::ostream& operator<<(std::ostream& cout, const mystring& my_str);

	
	//重载输入符号>>


	//------------------------------------------------------------------------------------------------------------

	//转换为c风格字符串
	//如果说此处给一个复制char*，就不可避免的通过new分配长度，调用者需要维护new出来的char，
	//如果说此处直接给char*，即返回this->_str，则可以通过这个指针直接修改mystring的内部结构，这不安全
	//经过思考，我决定返回const char*
	const char* to_charstring() {
		return this->_str;
	}

	//转换为string字符串
	std::string to_string() {
		return std::string(this->_str);
	}

	//------------------------------------------------------------------------------------------------------------
	
	//设置字符串改为大写

	//转换为16进制

	
	
};



//重载输出符号
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

