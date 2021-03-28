#include "myQueue.h"

#ifndef _TRIE_H
#define _TRIE_H

typedef struct _NODE {
	char c;
	unsigned int isWord;
	struct _NODE *child, *bro, *fail;//用孩子兄弟法存储，节省空间
}node;

typedef struct _TREE {
	struct _NODE *root;				//根节点 
	int size;						//大小 
}tree;

void init(struct _TREE *tree)
{
	tree->root = (struct _NODE *)malloc(sizeof(struct _NODE));	//分配根节点空间 
	memset(tree->root, 0, sizeof(struct _NODE));				//将此段内存清空 
	tree->size = 0;
}

struct _NODE *findNext(struct _NODE *cur, char c)
{
	if (cur == NULL || cur->child == NULL)
		return NULL;
	struct _NODE *tmp = cur->child;
	while (tmp != NULL) {
		if (tmp->c == c)
			return tmp;
		tmp = tmp->bro;
	}
	return NULL;
}

struct _NODE *putNext(struct _NODE *cur, char c)
{
	if (cur == NULL)
		return NULL;
	struct _NODE *tmp = cur->child;
	if (tmp != NULL) {
		while (tmp->bro != NULL) tmp = tmp->bro;
		tmp = tmp->bro = (struct _NODE *)malloc(sizeof(struct _NODE));
	}
	else
		tmp = cur->child = (struct _NODE *)malloc(sizeof(struct _NODE));
	memset(tmp, 0, sizeof(struct _NODE));
	tmp->c = c;
	return tmp;
}

void insert(struct _NODE *cur, const char *data, int index)
{
	if (data[index] == '\0') {
		cur->isWord = 1;
		return;
	}
	struct _NODE *tmp = findNext(cur, data[index]);
	if (tmp == NULL)
		tmp = putNext(cur, data[index]);
	insert(tmp, data, index + 1);//尾递归便于优化
}

void updateFail(struct _NODE *root)//构建fail 
{
	if (root == NULL)
		return;
	myQueue queue;			//初始化一个队列 
	queueInit(&queue);
	if (root->c == '\0') {//root为树的根节点
		struct _NODE *tmp = root->child;
		while (tmp != NULL) {
			tmp->fail = root;
			push(tmp, &queue);//把tmp放到队列里面，以便于后续的层次遍历
			tmp = tmp->bro;
		}
	}
	while (!empty(&queue)) {
		root = (node *)pop(&queue);
		if (root->child == NULL)
			continue;
		struct _NODE *ffail, *tmp = root->child, *res;
		while (tmp != NULL) {
			ffail = root->fail;
			while ((res = findNext(ffail, tmp->c)) == NULL && ffail->fail != NULL)
				ffail = ffail->fail;
			if (res != NULL)//上面的while循环条件的与运算符会先判断左边的表达式，所以这里要先判断while是否是因为左边的表达式为假退出的
				tmp->fail = res;
			else//此情况下ffail->fail == NULL，表明ffail为root
				tmp->fail = ffail;
			push(tmp, &queue);
			tmp = tmp->bro;
		}
	}
	queueClear(&queue);
}



int insertFromFile(struct _TREE *tree, const char *fileName)
{
//	clear(tree);
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("error when opening file \"%s\"\n", fileName);
		return 1;
	}
	else {
		printf("已打开\"%s\"\n开始读取模式串...\r", fileName);
	}
	char buffer[100];
	while (!feof(fp)) {
		fgets(buffer, 99, fp);
		if (buffer[0] == '\0')
			continue;
		buffer[strlen(buffer) - 1] = '\0';//读取一个完整的字符串，把他插入到树里
		insert(tree->root, buffer, 0);
	}
	fclose(fp);
	printf("模式串读取完毕！  \n");

	printf("开始构建fail指针...\r");
	updateFail(tree->root);
	printf("构建fail指针完毕！ \n");
}

void queryInFile(struct _TREE *tree, const char *fileName)
{
	int i = 0;
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("error when opening file \"%s\"\n", fileName);
		return;
	}
	else {
		printf("已打开\"%s\"\n开始匹配...\r", fileName);
	}
	struct _NODE *cur = tree->root, *tmp;
	while (!feof(fp)) {
		char c = fgetc(fp);
		if (c == '\n') {
			cur = tree->root;
			continue;
		}
		tmp = findNext(cur, c);
		if (tmp == NULL && cur != tree->root) {//匹配失败
			cur = cur->fail;
			tmp = findNext(cur, c);//从fail处继续匹配
		}
		if (tmp == NULL) {//表明是从root处匹配失败
			cur = tree->root;
			continue;
		}
		cur = tmp;
		while (tmp != tree->root && tmp->isWord != 0) {
			++tmp->isWord;
			tmp = tmp->fail;//防止某些模式串是此模式串的后缀
		}
	}

	fclose(fp);
	printf("目标文本匹配完毕！\n");
}

void bfs(struct _NODE *cur, FILE *fp)
{
	static char stack[100];
	static int top = 0;
	if (cur->c != '\0') {
		stack[top++] = cur->c;
		if (cur->isWord > 0) {
			stack[top] = '\0';
			fprintf(fp, "%s %d\n", stack, cur->isWord);
		}
	}
	struct _NODE *tmp = cur->child;
	while (tmp != NULL) {
		bfs(tmp, fp);
		tmp = tmp->bro;
	}
	if (cur->c != '\0')
		--top;
}

void printToFile(struct _TREE *tree, const char *fileName)
{
	if (tree->root->child == NULL)
		return;
	printf("正在保存匹配结果...\r");
	FILE *fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("error when opening file \"%s\"\n", fileName);
		return;
	}
	bfs(tree->root, fp);
	fclose(fp);
	printf("匹配结果已保存到\"%s\"！\n", fileName);
}

#endif // !_TRIE_H
