#include <iostream>
#include <vector>
#include "DrawATree.h"
using namespace std;
struct treeNode {
	int data;
	int height;
	treeNode* left, * right;
};
//�������
void inorderTra(treeNode* curr) {
	if (!curr) return;
	inorderTra(curr->left);
	cout << curr->data << " ";
	inorderTra(curr->right);
}
//��ȡ�߶�
int getHeight(treeNode* curr) {
	if (!curr) return 0;
	return curr->height;
}
//��ȡƽ������
int getFactor(treeNode* curr) {
	if (!curr) return 0;
	return getHeight(curr->left) - getHeight(curr->right);
}
//LL�ͣ���������
treeNode* LL(treeNode*& curr) {
	treeNode* leftNode = curr->left;
	curr->left = leftNode->right;
	leftNode->right = curr;
	curr = leftNode;
	//����������غ�����curr->height
	//curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	curr->right->height = max(getHeight(curr->right->left), getHeight(curr->right->right)) + 1;
	return curr;
}
//RR�ͣ���������
treeNode* RR(treeNode*& curr) {
	treeNode* rightNode = curr->right;
	curr->right = rightNode->left;
	rightNode->left = curr;
	curr = rightNode;
	//����������غ�����curr->height
	//curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	curr->left->height = max(getHeight(curr->left->left), getHeight(curr->left->right)) + 1;
	return curr;
}
//LR�ͣ��ȶ�����������������
treeNode* LR(treeNode*& curr) {
	RR(curr->left);
	return LL(curr);
}
//RL�ͣ��ȶ�����������������
treeNode* RL(treeNode*& curr) {
	LL(curr->right);
	return RR(curr);
}
//��ƽ��
void rebalance(treeNode*& curr) {
	int factor = getFactor(curr);
	if (factor > 1 && getFactor(curr->left) > 0) LL(curr);
	else if (factor > 1 && getFactor(curr->left) < 0) LR(curr);
	else if (factor < -1 && getFactor(curr->right) > 0) RL(curr);
	else if (factor < -1 && getFactor(curr->right) < 0) RR(curr);
	else return;
}
//����Ҷ�ӽ�㣬�ݹ�
//ע��ʹ��ָ�������
void insert(int num, treeNode*& curr) {
	//���Ϊ�գ����ҵ���Ҷ�ӽ���λ�ã�����ռ�
	if (!curr) {
		curr = new treeNode();
		curr->data = num;
		curr->left = nullptr;
		curr->right = nullptr;
	}
	//�����ͬ�Ͳ���Ҫ������
	else if (curr->data == num) return;
	//������ֱȵ�ǰ����ֵС�������뵱ǰ���������������ж�
	else if (num < curr->data) insert(num, curr->left);
	//������ֱȵ�ǰ�ڵ��ֵ�󣬼����뵱ǰ�ڵ�������������ж�
	else if (num > curr->data) insert(num, curr->right);
	//��ƽ��
	rebalance(curr);
	//���¸��¸߶�
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
}
void dele(int num, treeNode*& curr) {
	if (!curr) return;
	//С�ڣ�����������
	if (num < curr->data) dele(num, curr->left);
	//���ڣ�����������
	else if (num > curr->data) dele(num, curr->right);
	//���ڣ�ɾ��
	else if (num == curr->data) {
		//Ҷ�ӽڵ㣬ֱ��ɾ��
		if (!curr->left && !curr->right) {
			delete curr;
			curr = nullptr;
			return;
		}
		//ֻ���������������������
		else if (!curr->left && curr->right) {
			auto save = curr;
			curr = curr->right;
			delete save;
		}
		//ֻ���������������������
		else if (curr->left && !curr->right) {
			auto save = curr;
			curr = curr->left;
			delete save;
		}
		//�����������У���ǰ������ֵ�滻��ɾ��ǰ����㼴��
		else {
			//�ҵ�ǰ�����
			auto save = curr->left;
			while (save->right) save = save->right;
			//��¼ǰ������ֵ�������µݹ���ǰ����㣨һ����һ��Ҷ�ӽڵ㣩
			//������������������ɾ��ֱ��ɾ��ǰ����㣬��Ϊ����ʱҪ������ƽ��
			int value = save->data;
			dele(value, curr);
			curr->data = value;
		}
	}
	//��ƽ��
	rebalance(curr);
	//���¸��¸߶�
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
}
void createTree(vector<int> v, treeNode*&pRoot) {
	for (int i = 0; i < static_cast<int>(v.size()); i++) {
		insert(v[i], pRoot);
	}
	//���������ƽ�������������Ķ���������
	//inorderTra(pRoot);
}

int main() {
	vector<int> v = { 49,38,65,97,76,13,27,100 };
	treeNode* pRoot = nullptr;
	createTree(v,pRoot);
	DrawTree::Draw(pRoot);
	int num;
	string action;
	while (true) {
		cout << "ɾ��/���/�˳���d/a/z��: ";
		cin >> action;
		if (action == "d") {
			cin >> num;
			dele(num, pRoot);
			DrawTree::Draw(pRoot);
		}
		else if (action == "a") {
			cin >> num;
			insert(num, pRoot);
			DrawTree::Draw(pRoot);
		}
		else if (action == "z") break;
	}
	return 0;
}