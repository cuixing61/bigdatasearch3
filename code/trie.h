#include "myQueue.h"

#ifndef _TRIE_H
#define _TRIE_H

typedef struct _NODE {
	char c;
	unsigned int isWord;
	struct _NODE *child, *bro, *fail;//�ú����ֵܷ��洢����ʡ�ռ�
}node;

typedef struct _TREE {
	struct _NODE *root;				//���ڵ� 
	int size;						//��С 
}tree;

void init(struct _TREE *tree)
{
	tree->root = (struct _NODE *)malloc(sizeof(struct _NODE));	//������ڵ�ռ� 
	memset(tree->root, 0, sizeof(struct _NODE));				//���˶��ڴ���� 
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
	insert(tmp, data, index + 1);//β�ݹ�����Ż�
}

void updateFail(struct _NODE *root)//����fail 
{
	if (root == NULL)
		return;
	myQueue queue;			//��ʼ��һ������ 
	queueInit(&queue);
	if (root->c == '\0') {//rootΪ���ĸ��ڵ�
		struct _NODE *tmp = root->child;
		while (tmp != NULL) {
			tmp->fail = root;
			push(tmp, &queue);//��tmp�ŵ��������棬�Ա��ں����Ĳ�α���
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
			if (res != NULL)//�����whileѭ��������������������ж���ߵı��ʽ����������Ҫ���ж�while�Ƿ�����Ϊ��ߵı��ʽΪ���˳���
				tmp->fail = res;
			else//�������ffail->fail == NULL������ffailΪroot
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
		printf("�Ѵ�\"%s\"\n��ʼ��ȡģʽ��...\r", fileName);
	}
	char buffer[100];
	while (!feof(fp)) {
		fgets(buffer, 99, fp);
		if (buffer[0] == '\0')
			continue;
		buffer[strlen(buffer) - 1] = '\0';//��ȡһ���������ַ������������뵽����
		insert(tree->root, buffer, 0);
	}
	fclose(fp);
	printf("ģʽ����ȡ��ϣ�  \n");

	printf("��ʼ����failָ��...\r");
	updateFail(tree->root);
	printf("����failָ����ϣ� \n");
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
		printf("�Ѵ�\"%s\"\n��ʼƥ��...\r", fileName);
	}
	struct _NODE *cur = tree->root, *tmp;
	while (!feof(fp)) {
		char c = fgetc(fp);
		if (c == '\n') {
			cur = tree->root;
			continue;
		}
		tmp = findNext(cur, c);
		if (tmp == NULL && cur != tree->root) {//ƥ��ʧ��
			cur = cur->fail;
			tmp = findNext(cur, c);//��fail������ƥ��
		}
		if (tmp == NULL) {//�����Ǵ�root��ƥ��ʧ��
			cur = tree->root;
			continue;
		}
		cur = tmp;
		while (tmp != tree->root && tmp->isWord != 0) {
			++tmp->isWord;
			tmp = tmp->fail;//��ֹĳЩģʽ���Ǵ�ģʽ���ĺ�׺
		}
	}

	fclose(fp);
	printf("Ŀ���ı�ƥ����ϣ�\n");
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
	printf("���ڱ���ƥ����...\r");
	FILE *fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("error when opening file \"%s\"\n", fileName);
		return;
	}
	bfs(tree->root, fp);
	fclose(fp);
	printf("ƥ�����ѱ��浽\"%s\"��\n", fileName);
}

#endif // !_TRIE_H
