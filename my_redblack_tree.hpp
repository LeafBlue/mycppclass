#pragma once
#include<functional>
#include<utility>



/*
	������������������һ�ֶ�������ÿ���ڵ㶼��һ����ڵ��һ���ҽڵ㡣
		��ڵ�ȵ�ǰ�ڵ�С���ҽڵ�ȵ�ǰ�ڵ��
		����֮�⣬�������һ����������ԣ���ɫ��

		1�����ڵ��Ǻ�ɫ
		2������Ҷ�ӽڵ㣨��ĩ�˵ģ��սڵ㣩Ҳ�Ǻ�ɫ
		3�����һ���ڵ��Ǻ�ɫ�ģ���ô���������ӽڵ㶼�Ǻ�ɫ
		4��������ڵ㵽��ÿ��Ҷ�ӽڵ�ĺ�ɫ�ڵ�������һ�¡�

		1���²���Ľڵ�ͨ���Ǻ�ɫ�����˵�һ������һ���Ǻ�ɫ��
*/
template<class Tkey,class Tval,class Compare = std::less<Tkey>>
class my_redblack_tree {
private:
	
	//����һ��ʲô���ⲻ�ǳ�Ա������Ƕ�����ͣ�����ռ�������Ŀռ�
	//�����������ڵ�ǰ��
	//����ⲿҪ����������Ҫͨ������::
	struct node {
		enum color {RED,BLACK};
		color node_color;
		std::pair<Tkey, Tval> data;
		node* left;
		node* right;
		node* father;

		//����
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

	//����һ����������Ҳ�����ö�����õĺ�����
	//�����ﶨ�����������ģ����������������ͣ��������ķ������Ƚϴ�С
	Compare comp;

	bool is_equal(const Tkey& a,const Tkey& b) {
		return !comp(a, b) && !comp(b, a);
	}

	void check_color(node* cur_node,const int& flag1) {
		//flag1��1��2��
		int flag2 = 0;
		node* fa_node = cur_node->father;
		if (fa_node != nullptr) {
			if (fa_node->node_color == node::color::RED) {//���Ǻ�ɫʱֱ�Ӳ��뼴�ɣ��������κδ���
				//�����ڵ�
				//1,��ȡcurnode����ڵ㣬Ҳ����thisnode���ֵܽڵ�
				node* gra_node = fa_node->father;
				if (gra_node == nullptr) {
					return;
				}
				//��ȡ��ڵ�
				node* uncle_node = nullptr;
				
				//���ڵ���游�ڵ�Ĺ�ϵ
				if (comp(fa_node->data.first, gra_node->data.first)) {
					uncle_node = gra_node->right;
					flag2 = 1;
				}
				else {
					uncle_node = gra_node->left;
					flag2 = 2;
				}

				if (uncle_node != nullptr && uncle_node->node_color == node::color::RED) {
					//����ڵ㲻Ϊ�� ������ڵ��Ǻ�ɫʱ
					fa_node->node_color = node::color::BLACK;
					uncle_node->node_color = node::color::BLACK;
					//���游�ڵ���Ϊ��ǰ�ڵ� ��������
					//��������תֻҪ�ֲ���ת�� �����游�ڵ㲻��Ҫ������ת��������0
					check_color(gra_node,0);
				}
				else {
					if(flag1 == 0){
						return;
					}
					//��ڵ�Ϊ�ջ����Ǻ�ɫ���
					//�����������ǰ�ڵ��Ǹ��ڵ�����ӣ����ڵ����游�ڵ�����ӡ�
					//�˴����ڵ��ǲ��������Һ��ӵģ���ʹ�У�Ҳһ����������������б�������˲���Ҫ������������д���
					//��������£�Ӧ�����游�ڵ��������
					if (flag1 == 1 && flag2 == 1) {
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {//����������游
							//�������裬�������游�͸��ڵ��ϵ�����游ָ���ף�����Ҳָ�����游
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = fa_node;
							}
							else {
								gra_fa_node->right = fa_node;
							}
							fa_node->father = gra_fa_node;
							//�������裬���¸��ڵ���游��ϵ���游��Ϊ���ڵ�Ķ������ڵ����游�ĵ�
							fa_node->right = gra_node;
							gra_node->father = fa_node;
							//�游����˵��Ķ��ӣ��ж�������ڵ�Ĺ�ϵ�������ҽڵ��ϵ
							gra_node->left = nullptr;
							//�޸���ɫ
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {//���������游��˵��ʲô��˵���游���Ǹ��ڵ�
							//�����ڵ�ת��Ϊ���ڵ�
							rootnode = fa_node;
							fa_node->father = nullptr;
							//���¸��ڵ���游�ڵ��ϵ
							fa_node->right = gra_node;
							gra_node->father = fa_node;
							//�޸���ɫ
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;//�²����Ҳ��node::color::RED����ʱ���ǵ�ǰ�������ӣ��Ͳ�����
						}
					}
					//�����������ǰ�ڵ��Ǹ��ڵ���Һ��ӣ����ڵ����游�ڵ�����ӡ�
					if (flag1 == 2 && flag2 == 1) {
						//---------------ִ�и��ڵ�����---------------
						fa_node->father = cur_node;
						cur_node->left = fa_node;
						//�жϸ��ڵ�ͺ��ӵ���ϵ
						fa_node->right = nullptr;
						//cur_node->father = gra_node;
						//---------------�游�ڵ�����---------------
						//��Ҫ�������游�ڵ��Ƿ���ڣ����ж��游�ǲ��Ǹ��ڵ�
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {
							//�Ƚ��游����������cur_node�ҽڵ�
							cur_node->right = gra_node;
							gra_node->father = cur_node;
							//�ж��游����ڵ�Ĺ�ϵ�������ҽڵ�
							gra_node->left = nullptr;
							//���������cur�����游�Ĺ�ϵ
							//�����游�����游�Ĺ�ϵ���жϸð�cur�������游���ĸ�λ��
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = cur_node;
							}
							else {
								gra_fa_node->right = cur_node;
							}
							cur_node->father = gra_fa_node;
							//�ѵ�ǰ�ڵ��Ϊ��ɫ����ԭ�������Ѿ��Ǻ�ɫ�ˣ�����Ҫ�޸�
							cur_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {
							//�游���Ǹ��ڵ�
							//�����游���
							cur_node->right = gra_node;
							gra_node->father = nullptr;
							//�ж��游����ڵ��ϵ
							gra_node->left = nullptr;
							//��ǰ��Ϊ���ڵ�
							cur_node->father = nullptr;
							rootnode = cur_node;
							//����������ɫ
							cur_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
					}
					//�����������ǰ�ڵ��Ǹ��ڵ���Һ��ӣ����ڵ����游�ڵ���Һ��ӡ�
					//���������Ӧ�����游������ �����������д��ȽϺ�д����д���
					if (flag1 == 2 && flag2 == 2) {
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {//����������游
							//�������裬�������游�͸��ڵ��ϵ�����游ָ���ף�����Ҳָ�����游
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = fa_node;
							}
							else {
								gra_fa_node->right = fa_node;
							}
							fa_node->father = gra_fa_node;
							//�������裬���¸��ڵ���游��ϵ���游��Ϊ���ڵ�Ķ������ڵ����游�ĵ�
							fa_node->left = gra_node;
							gra_node->father = fa_node;
							//�游����˵��Ķ��ӣ��ж������ҽڵ�Ĺ�ϵ
							gra_node->right = nullptr;
							//�޸���ɫ
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {//���������游��˵��ʲô��˵���游���Ǹ��ڵ�
							//�����ڵ�ת��Ϊ���ڵ�
							rootnode = fa_node;
							fa_node->father = nullptr;
							//���¸��ڵ���游�ڵ��ϵ
							fa_node->left = gra_node;
							gra_node->father = fa_node;
							//�޸���ɫ
							fa_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
					}
					//�����������ǰ�ڵ��Ǹ��ڵ�����ӣ����ڵ����游�ڵ���Һ��ӡ�
					if (flag1 == 1&& flag2 == 2) {
						//---------------ִ�и��ڵ�����---------------
						fa_node->father = cur_node;
						cur_node->right = fa_node;
						//�жϸ��ڵ�ͺ��ӵ���ϵ
						fa_node->left = nullptr;
						//---------------�游�ڵ�����---------------
						//��Ҫ�������游�ڵ��Ƿ���ڣ����ж��游�ǲ��Ǹ��ڵ�
						node* gra_fa_node = gra_node->father;
						if (gra_fa_node != nullptr) {
							//�Ƚ��游����������cur_node��ڵ�
							cur_node->left = gra_node;
							gra_node->father = cur_node;
							//�ж��游���ҽڵ�Ĺ�ϵ��������ڵ�
							gra_node->right = nullptr;
							//���������cur�����游�Ĺ�ϵ
							//�����游�����游�Ĺ�ϵ���жϸð�cur�������游���ĸ�λ��
							if (comp(gra_node->data.first, gra_fa_node->data.first)) {
								gra_fa_node->left = cur_node;
							}
							else {
								gra_fa_node->right = cur_node;
							}
							cur_node->father = gra_fa_node;
							//�ѵ�ǰ�ڵ��Ϊ��ɫ����ԭ�������Ѿ��Ǻ�ɫ�ˣ�����Ҫ�޸�
							cur_node->node_color = node::color::BLACK;
							gra_node->node_color = node::color::RED;
						}
						else {
							//�游���Ǹ��ڵ�
							//�����游���
							cur_node->left = gra_node;
							gra_node->father = nullptr;
							//�ж��游���ҽڵ��ϵ
							gra_node->right = nullptr;
							//��ǰ��Ϊ���ڵ�
							cur_node->father = nullptr;
							rootnode = cur_node;
							//����������ɫ
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

	//�ȸ���
	~my_redblack_tree() {}

	//�������
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
					//�����Ҳ����
					if (fa_node->left == nullptr) {
						//�����µĽڵ㲢��ʼ��
						node *cur_node = new node(std::pair<Tkey, Tval>(key, val));
						cur_node->father = fa_node;
						fa_node->left = cur_node;
						//������ɫ����ת
						check_color(cur_node,1);
						size_++;
						return;
					}
					fa_node = fa_node->left;
					continue;
				}
				else if (comp(fa_key,key)) {
					//�����Ҳ����
					if (fa_node->right == nullptr) {
						//�����µĽڵ㲢��ʼ��
						node* cur_node = new node(std::pair<Tkey, Tval>(key, val));
						cur_node->father = fa_node;
						fa_node->right = cur_node;
						//������ɫ����ת
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

	//���Ҳ���
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