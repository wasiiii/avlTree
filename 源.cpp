#include <iostream>
#include <vector>
#include "DrawATree.h"
using namespace std;
struct treeNode {
	int data;
	int height;
	treeNode* left, * right;
};
//中序遍历
void inorderTra(treeNode* curr) {
	if (!curr) return;
	inorderTra(curr->left);
	cout << curr->data << " ";
	inorderTra(curr->right);
}
//获取高度
int getHeight(treeNode* curr) {
	if (!curr) return 0;
	return curr->height;
}
//获取平衡因子
int getFactor(treeNode* curr) {
	if (!curr) return 0;
	return getHeight(curr->left) - getHeight(curr->right);
}
//LL型，右旋单旋
treeNode* LL(treeNode*& curr) {
	treeNode* leftNode = curr->left;
	curr->left = leftNode->right;
	leftNode->right = curr;
	curr = leftNode;
	//这个函数返回后会更改curr->height
	//curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	curr->right->height = max(getHeight(curr->right->left), getHeight(curr->right->right)) + 1;
	return curr;
}
//RR型，左旋单旋
treeNode* RR(treeNode*& curr) {
	treeNode* rightNode = curr->right;
	curr->right = rightNode->left;
	rightNode->left = curr;
	curr = rightNode;
	//这个函数返回后会更改curr->height
	//curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	curr->left->height = max(getHeight(curr->left->left), getHeight(curr->left->right)) + 1;
	return curr;
}
//LR型，先对左子树右旋再左旋
treeNode* LR(treeNode*& curr) {
	RR(curr->left);
	return LL(curr);
}
//RL型，先对右子树左旋再右旋
treeNode* RL(treeNode*& curr) {
	LL(curr->right);
	return RR(curr);
}
//重平衡
void rebalance(treeNode*& curr) {
	int factor = getFactor(curr);
	if (factor > 1 && getFactor(curr->left) > 0) LL(curr);
	else if (factor > 1 && getFactor(curr->left) < 0) LR(curr);
	else if (factor < -1 && getFactor(curr->right) > 0) RL(curr);
	else if (factor < -1 && getFactor(curr->right) < 0) RR(curr);
	else return;
}
//插入叶子结点，递归
//注意使用指针的引用
void insert(int num, treeNode*& curr) {
	//如果为空，即找到了叶子结点的位置，分配空间
	if (!curr) {
		curr = new treeNode();
		curr->data = num;
		curr->left = nullptr;
		curr->right = nullptr;
	}
	//如果相同就不需要插入了
	else if (curr->data == num) return;
	//如果数字比当前结点的值小，即进入当前结点的左子树继续判断
	else if (num < curr->data) insert(num, curr->left);
	//如果数字比当前节点的值大，即进入当前节点的右子树继续判断
	else if (num > curr->data) insert(num, curr->right);
	//重平衡
	rebalance(curr);
	//重新更新高度
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
}
void dele(int num, treeNode*& curr) {
	if (!curr) return;
	//小于，进入左子树
	if (num < curr->data) dele(num, curr->left);
	//大于，进入右子树
	else if (num > curr->data) dele(num, curr->right);
	//等于，删除
	else if (num == curr->data) {
		//叶子节点，直接删除
		if (!curr->left && !curr->right) {
			delete curr;
			curr = nullptr;
			return;
		}
		//只有右子树，用右子树替代
		else if (!curr->left && curr->right) {
			auto save = curr;
			curr = curr->right;
			delete save;
		}
		//只有左子树，用左子树替代
		else if (curr->left && !curr->right) {
			auto save = curr;
			curr = curr->left;
			delete save;
		}
		//左右子树都有，用前驱结点的值替换，删除前驱结点即可
		else {
			//找到前驱结点
			auto save = curr->left;
			while (save->right) save = save->right;
			//记录前驱结点的值，再往下递归找前驱结点（一定是一个叶子节点）
			//必须这样做，不可以删除直接删除前驱结点，因为回溯时要进行重平衡
			int value = save->data;
			dele(value, curr);
			curr->data = value;
		}
	}
	//重平衡
	rebalance(curr);
	//重新更新高度
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
}
void createTree(vector<int> v, treeNode*&pRoot) {
	for (int i = 0; i < static_cast<int>(v.size()); i++) {
		insert(v[i], pRoot);
	}
	//中序遍历，平衡二叉树是特殊的二叉排序树
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
		cout << "删除/添加/退出（d/a/z）: ";
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