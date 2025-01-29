#pragma once
#include<functional>
#include<utility>



/*
	红黑树：红黑树首先是一种二叉树，每个节点都有一个左节点和一个右节点。
		左节点比当前节点小，右节点比当前节点大
		除此之外，红黑树有一个特殊的属性，颜色。

		1，根节点是黑色
		2，所有叶子节点（即末端的，空节点）也是黑色
		3，如果一个节点是红色的，那么它的两个子节点都是黑色
		4，从任意节点到其每个叶子节点的黑色节点数量都一致。

		1，新插入的节点通常是红色，除了第一个，第一个是黑色。
*/
template<class Tkey,class Tval,class Compare = std::less<Tkey>>
class my_redblack_tree {
private:
	
	//这是一个什么？这不是成员，这是嵌套类型，它不占用这个类的空间
	//它的作用域在当前类
	//如果外部要访问它，需要通过类名::
	struct node {
		enum color {RED,BLACK};
		color node_color;
		std::pair<Tkey, Tval> data;
		node* left;
		node* right;
		node* father;

		//构造
		node(std::pair<Tkey, Tval> data_in) {
			node_color = node::color::RED;
			data = data_in;
			left = nullptr;
			right = nullptr;
			father = nullptr;
		}
	};
	node* rootnode;
	size_t size_;

	//这是一个防函数，也即是用对象调用的函数。
	//在这里定义这个对象，在模版参数定义它的类型，调用它的防函数比较大小
	Compare comp;

	bool is_equal(const Tkey& a,const Tkey& b) {
		return !comp(a, b) && !comp(b, a);
	}

	void check_color(node* cur_node,const int& flag1) {
		//flag1：1左2右
		int flag2 = 0;
		node* fa_node = cur_node->father;
		if (fa_node != nullptr) {
			if (fa_node->node_color == node::color::RED) {//当是黑色时直接插入即可，不用做任何处理
				//检查叔节点
				//1,获取curnode的叔节点，也就是thisnode的兄弟节点
				node* gra_node = fa_node->father;
				if (gra_node == nullptr) {
					return;
				}
				//获取叔节点
				node* uncle_node = nullptr;
				
				//父节点和祖父节点的关系
				if (comp(fa_node->data.first, gra_node->data.first)) {
					uncle_node = gra_node->right;
					flag2 = 1;
				}
				else {
					uncle_node = gra_node->left;
					flag2 = 2;
				}

				if (uncle_node != nullptr && uncle_node->node_color == node::color::RED) {
					//当叔节点不为空 并且叔节点是红色时
					fa_node->node_color = node::color::BLACK;
					uncle_node->node_color = node::color::BLACK;
					//将祖父节点作为当前节点 继续处理
					//理论上旋转只要局部旋转， 处理祖父节点不需要考虑旋转，给参数0
					check_color(gra_node,0);
				}
				else {
					if(flag1 == 0){
						return;
					}
					//叔节点为空或者是黑色情况
					//左左情况：当前节点是父节点的左孩子，父节点是祖父节点的左孩子。
					//此处父节点是不可能有右孩子的，即使有，也一定会在其他的情况中被处理！因此不需要对这种情况进行处理。
					//此种情况下，应当对祖父节点进行右旋
					if (flag1 == 1 && flag2 == 1) {
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {//如果存在曾祖父
							//两个步骤，更新曾祖父和父节点关系，曾祖父指向父亲，父亲也指向曾祖父
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = fa_node;
							}
							else {
								gra_fa_node->right = fa_node;
							}
							fa_node->father = gra_fa_node;
							//两个步骤，更新父节点和祖父关系，祖父成为父节点的儿，父节点变成祖父的爹
							fa_node->right = gra_node;
							gra_node->father = fa_node;
							//祖父变成了爹的儿子，切断它与左节点的关系，保留右节点关系
							gra_node->left = nullptr;
							//修改颜色
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {//不存在曾祖父，说明什么？说明祖父就是根节点
							//将父节点转换为根节点
							rootnode = fa_node;
							fa_node->father = nullptr;
							//更新父节点和祖父节点关系
							fa_node->right = gra_node;
							gra_node->father = fa_node;
							//修改颜色
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;//新插入的也是node::color::RED，此时它是当前根的左孩子，就不改了
						}
					}
					//左右情况：当前节点是父节点的右孩子，父节点是祖父节点的左孩子。
					if (flag1 == 2 && flag2 == 1) {
						//---------------执行父节点左旋---------------
						fa_node->father = cur_node;
						cur_node->left = fa_node;
						//切断父节点和孩子的联系
						fa_node->right = nullptr;
						//cur_node->father = gra_node;
						//---------------祖父节点右旋---------------
						//需要根据曾祖父节点是否存在，来判断祖父是不是根节点
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {
							//先将祖父右旋，挂在cur_node右节点
							cur_node->right = gra_node;
							gra_node->father = cur_node;
							//切断祖父和左节点的关系，保留右节点
							gra_node->left = nullptr;
							//接下来配对cur和曾祖父的关系
							//根据祖父和曾祖父的关系，判断该把cur挂在曾祖父的哪个位置
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = cur_node;
							}
							else {
								gra_fa_node->right = cur_node;
							}
							cur_node->father = gra_fa_node;
							//把当前节点改为黑色，（原）父亲已经是红色了，不需要修改
							cur_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {
							//祖父就是根节点
							//右旋祖父结点
							cur_node->right = gra_node;
							gra_node->father = nullptr;
							//切断祖父和左节点关系
							gra_node->left = nullptr;
							//当前设为根节点
							cur_node->father = nullptr;
							rootnode = cur_node;
							//重新设置颜色
							cur_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
					}
					//右右情况：当前节点是父节点的右孩子，父节点是祖父节点的右孩子。
					//此种情况下应当对祖父结点进行 左旋。这个刚写完比较好写，先写这个
					if (flag1 == 2 && flag2 == 2) {
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {//如果存在曾祖父
							//两个步骤，更新曾祖父和父节点关系，曾祖父指向父亲，父亲也指向曾祖父
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = fa_node;
							}
							else {
								gra_fa_node->right = fa_node;
							}
							fa_node->father = gra_fa_node;
							//两个步骤，更新父节点和祖父关系，祖父成为父节点的儿，父节点变成祖父的爹
							fa_node->left = gra_node;
							gra_node->father = fa_node;
							//祖父变成了爹的儿子，切断它与右节点的关系
							gra_node->right = nullptr;
							//修改颜色
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {//不存在曾祖父，说明什么？说明祖父就是根节点
							//将父节点转换为根节点
							rootnode = fa_node;
							fa_node->father = nullptr;
							//更新父节点和祖父节点关系
							fa_node->left = gra_node;
							gra_node->father = fa_node;
							//修改颜色
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
					}
					//右左情况：当前节点是父节点的左孩子，父节点是祖父节点的右孩子。
					if (flag1 == 1&& flag2 == 2) {
						//---------------执行父节点右旋---------------
						fa_node->father = cur_node;
						cur_node->right = fa_node;
						//切断父节点和孩子的联系
						fa_node->left = nullptr;
						//---------------祖父节点左旋---------------
						//需要根据曾祖父节点是否存在，来判断祖父是不是根节点
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {
							//先将祖父左旋，挂在cur_node左节点
							cur_node->left = gra_node;
							gra_node->father = cur_node;
							//切断祖父和右节点的关系，保留左节点
							gra_node->right = nullptr;
							//接下来配对cur和曾祖父的关系
							//根据祖父和曾祖父的关系，判断该把cur挂在曾祖父的哪个位置
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = cur_node;
							}
							else {
								gra_fa_node->right = cur_node;
							}
							cur_node->father = gra_fa_node;
							//把当前节点改为黑色，（原）父亲已经是红色了，不需要修改
							cur_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {
							//祖父就是根节点
							//左旋祖父结点
							cur_node->left = gra_node;
							gra_node->father = nullptr;
							//切断祖父和右节点关系
							gra_node->right = nullptr;
							//当前设为根节点
							cur_node->father = nullptr;
							rootnode = cur_node;
							//重新设置颜色
							cur_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
					}
				}
			}
		}
	}

	
public:
	my_redblack_tree():rootnode(nullptr),size_(0){}

	//先搁置
	~my_redblack_tree() {}

	//插入操作
	void insert(const Tkey& key,const Tval& val) {
		if (size_ == 0) {
			node *node_ = new node(std::pair<Tkey, Tval>(key,val));
			node_->node_color = node::color::BLACK;
			rootnode = node_;
			++size_;
		}
		else {
			node* fa_node = rootnode;
			while (fa_node != nullptr) {
				Tkey fa_key = fa_node->data.first;
				if (is_equal(key, fa_key)) {
					fa_node->data.second = val;
				}
				else if (comp(key, fa_key)) {
					//向左找插入点
					if (fa_node->left == nullptr) {
						//创建新的节点并初始化
						node *cur_node = new node(std::pair<Tkey, Tval>(key, val));
						cur_node->father = fa_node;
						fa_node->left = cur_node;
						//处理颜色和旋转
						check_color(cur_node,1);
						size_++;
						return;
					}
					fa_node = fa_node->left;
					continue;
				}
				else if (comp(fa_key,key)) {
					//向右找插入点
					if (fa_node->right == nullptr) {
						//创建新的节点并初始化
						node* cur_node = new node(std::pair<Tkey, Tval>(key, val));
						cur_node->father = fa_node;
						fa_node->right = cur_node;
						//处理颜色和旋转
						check_color(cur_node, 2);
						size_++;
						return;
					}
					fa_node = fa_node->right;
					continue;
				}
			}

		}
	}

	//查找操作
	Tval& find(const Tkey& key) {
		node* thisnode = rootnode;
		while (thisnode != nullptr) {
			if (is_equal(thisnode->data.first, key)) {
				return thisnode->data.second;
			}
			else if (comp(key,thisnode->data.first)) {
				thisnode = thisnode->left;
			}
			else if (comp(thisnode->data.first, key)) {
				thisnode = thisnode->right;
			}
		}
	}

};