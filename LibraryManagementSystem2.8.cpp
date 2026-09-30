#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<cstring>
using namespace std;

//1.全局变量 

int Role=0;//0未登录 1用户 2管理员
int Page=1;//1登录 2图书系统 3图书管理 4账号管理
int Booksortsign=0;//0未排序 1已排序

//2.书籍管理 

//bookmanager

//2.1书籍信息

struct bookInfo
{
	char name[20];//书名
	float price;//价格
	int num; //数量
	char author[20];//作者
	char publishinghouse[20];//出版社
	char ISBN[20];//ISBN
	int readnum; // 借阅次数
	int publishtime; // 录入顺序编号 - 新增
};

//2.2书籍链表 

//链表
struct Nodebook
{
	struct bookInfo data;
	struct Nodebook* next;
};
struct Nodebook* booklist=NULL;
//创建表头
struct Nodebook* createbookHead()
{
	struct Nodebook* headNode=new Nodebook;
	headNode->next=NULL;
	return headNode;
}
//创建节点
struct Nodebook* createbookNode(struct bookInfo data)
{
	struct Nodebook* newNode=new Nodebook;
	newNode->data=data;
	newNode->next=NULL;
	return newNode;
}
//插入节点：头插法
void insertbookNodeByHead(struct Nodebook* headNode,struct bookInfo data)
{
	struct Nodebook* newNode=createbookNode(data);
	newNode->next=headNode->next;
	headNode->next=newNode;
	Booksortsign=0; 
}
//指定元素删除
void deletebookNodeByName(struct Nodebook* headNode,char *bookName)
{
	struct Nodebook* posLeftNode=headNode;
	struct Nodebook* posNode=headNode->next;

	while(posNode!=NULL&&strcmp(posNode->data.name,bookName))
	{
		posLeftNode=posNode;
		posNode=posLeftNode->next;
	}
	if(posNode==NULL)return;
	else
	{
		printf("删除成功\n");
		posLeftNode->next=posNode->next;
		delete posNode;
		posNode=NULL;
	}
	Booksortsign=0; 
}

//打印链表
void printbookList(struct Nodebook* headNode)
{
	struct Nodebook* pMove = headNode->next;
	// 使用固定宽度格式化，-表示左对齐，数字表示最小宽度
	printf("%-20s\t%-8s\t%-6s\t%-15s\t%-20s\t%-13s\t%-10s\n",
	       "书名", "价格", "数量", "作者", "出版社", "ISBN/ISSN", "借阅次数");
	while (pMove != NULL)
	{
		printf("%-20s\t%-8.1f\t%-6d\t%-15s\t%-20s\t%-13s\t%-10d\n",
		       pMove->data.name,
		       pMove->data.price,
		       pMove->data.num,
		       pMove->data.author,
		       pMove->data.publishinghouse,
		       pMove->data.ISBN,
		       pMove->data.readnum);  // 输出借阅次数
		pMove = pMove->next;
	}
}
//释放链表
void freebookList(struct Nodebook* headNode)
{
	struct Nodebook* pMove = headNode->next;
	while (pMove != NULL)
	{
		struct Nodebook* temp = pMove;
		pMove = pMove->next;
		delete temp;
	}
    delete headNode;  // ← 释放头节点
}

//2.3书籍查找 

//名字查找 
struct Nodebook* searchbookByName(struct Nodebook* headNode,char* bookName)
{
	struct Nodebook* posNode=headNode->next;
	while(posNode!=NULL&&strcmp(posNode->data.name,bookName))
	{
		posNode=posNode->next;
	}
	return posNode;
}
//ISBN查找 
struct Nodebook* searchbookByISBN(struct Nodebook* headNode, char* bookISBN)
{
	struct Nodebook* posNode = headNode->next;
	while (posNode != NULL &&strcmp(posNode->data.ISBN,bookISBN))
	{
		posNode = posNode->next;
	}
	return posNode;
}
//作者查找 
void searchAndPrintBooksByAuthor(struct Nodebook* headNode, char* authorName)
{
	struct Nodebook* posNode = headNode->next;
	int count = 0;
	printf("作者 '%s' 的书籍列表\n\n", authorName);
	while(posNode != NULL)
	{
		if(strcmp(posNode->data.author, authorName) == 0)
		{
			count++;
			printf("%d. ", count);
			printf("书名:%s  ", posNode->data.name);
			printf("ISBN:%s  ", posNode->data.ISBN);
			printf("价格:%.2f  ", posNode->data.price);
			printf("数量:%d  ", posNode->data.num);
			printf("出版社:%s  ", posNode->data.publishinghouse);
			printf("借阅次数:%d\n\n", posNode->data.readnum);  // 输出借阅次数
		}
		posNode = posNode->next;
	}

	if(count == 0)
	{
		printf("未找到作者 '%s' 的任何书籍。\n", authorName);
	}
	else
	{
		printf("总计:%d本书籍\n", count);
	}
}

//2.4书籍排序 

//书籍归并排序根据字典序排序
// 合并两个有序链表（按书名字典序升序）
struct Nodebook* mergeSortedbookLists(struct Nodebook* left, struct Nodebook* right)
{
	struct Nodebook dummy;  // 临时头节点
	struct Nodebook* tail = &dummy;
	dummy.next=NULL;
	while (left!=NULL&&right!=NULL)
	{
		//比较书名，按字典序升序排列
		if (strcmp(left->data.name,right->data.name) <= 0)
		{
			tail->next = left;
			left=left->next;
		}
		else
		{
			tail->next=right;
			right=right->next;
		}
		tail=tail->next;
	}

	// 连接剩余部分
	if (left!=NULL) tail->next = left;
	if (right!=NULL) tail->next = right;

	return dummy.next;
}

// 使用快慢指针找到链表中间节点（保持不变）
struct Nodebook* getMiddlebookNode(struct Nodebook* head)
{
	if (head == NULL || head->next == NULL) return head;

	struct Nodebook* slow = head;
	struct Nodebook* fast = head->next;

	while (fast != NULL && fast->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

// 归并排序主函数（递归）（保持不变）
struct Nodebook* mergeSortbookListInternal(struct Nodebook* head)
{
	if (head == NULL || head->next == NULL) return head;

	// 找到中间节点并分割链表
	struct Nodebook* middle = getMiddlebookNode(head);
	struct Nodebook* nextToMiddle = middle->next;
	middle->next = NULL;  // 切断链表

	// 递归排序两部分
	struct Nodebook* left = mergeSortbookListInternal(head);
	struct Nodebook* right = mergeSortbookListInternal(nextToMiddle);

	// 合并已排序的两部分
	return mergeSortedbookLists(left, right);
}

// 原函数名保持不变，内部改为归并排序（按书名排序）
void mergeSortbookList(struct Nodebook* headNode)
{
	if (headNode == NULL || headNode->next == NULL || headNode->next->next == NULL)
		return;  // 空链表或只有一个节点，无需排序

	// 获取实际的数据节点（跳过头节点）
	struct Nodebook* dataHead = headNode->next;

	// 进行归并排序
	dataHead = mergeSortbookListInternal(dataHead);

	// 将排序后的链表重新连接到头节点
	headNode->next = dataHead;
}


//2.5排行榜功能 

//2.5.1借阅次数排行榜
// 比较函数，用于qsort（借阅次数降序）
int compareByReadNum(const void* a, const void* b)
{
	struct Nodebook* bookA = *(struct Nodebook**)a;
	struct Nodebook* bookB = *(struct Nodebook**)b;

	// 按借阅次数降序排列（次数多的在前）
	return bookB->data.readnum - bookA->data.readnum;
}
//借阅次数排行榜
void displayTop10ByReadNum(struct Nodebook* headNode)
{
	printf("=== 借阅次数排行榜 TOP 10 ===\n");
	// 计算书籍总数
	int count = 0;
	struct Nodebook* temp = headNode->next;
	while (temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	if (count == 0)
	{
		printf("暂无书籍数据！\n");
		return;
	}
	// 创建数组存储书籍指针
	Nodebook** books = new Nodebook*[count];

	temp = headNode->next;
	for (int i = 0; i < count; i++)
	{
		books[i] = temp;
		temp = temp->next;
	}
	// 使用qsort快速排序（O(n log n)）
	qsort(books, count, sizeof(struct Nodebook*), compareByReadNum);
	// 显示前10本（或更少）
	int displayCount = count < 10 ? count : 10;
	printf("%-4s %-20s\t%-15s\t%-8s\t%-10s\n",
	       "排名", "书名", "作者", "价格", "借阅次数");
	printf("-----------------------------------------------------------\n");
	for (int i = 0; i < displayCount; i++)
	{
		printf("%-4d %-20s\t%-15s\t%-8.1f\t%-10d\n",
		       i + 1,
		       books[i]->data.name,
		       books[i]->data.author,
		       books[i]->data.price,
		       books[i]->data.readnum);
	}
	delete[]books;
	printf("=========================================\n");
}

// 2. 最新出版排行榜（按publishtime排序，数值越大表示越新）
// 比较函数，用于qsort（出版时间降序）
int compareByPublishTime(const void* a, const void* b)
{
	struct Nodebook* bookA = *(struct Nodebook**)a;
	struct Nodebook* bookB = *(struct Nodebook**)b;

	// 按出版时间降序排列（新的在前）
	return bookB->data.publishtime - bookA->data.publishtime;
}
// 获取当前最大的publishtime
int getMaxPublishtime(struct Nodebook* headNode)
{
	int maxTime = 0;
	struct Nodebook* temp = headNode->next;
	while(temp != NULL)
	{
		if(temp->data.publishtime > maxTime)
		{
			maxTime = temp->data.publishtime;
		}
		temp = temp->next;
	}
	return maxTime;
}
//最新出版排行榜 
void displayTop10ByPublishTime(struct Nodebook* headNode)
{
	printf("=== 最新出版排行榜 TOP 10 ===\n");

	// 计算书籍总数
	int count = 0;
	struct Nodebook* temp = headNode->next;
	while (temp != NULL)
	{
		count++;
		temp = temp->next;
	}

	if (count == 0)
	{
		printf("暂无书籍数据！\n");
		return;
	}

	// 创建数组存储书籍指针
	Nodebook** books = new Nodebook*[count];

	temp = headNode->next;
	for (int i = 0; i < count; i++)
	{
		books[i] = temp;
		temp = temp->next;
	}

	// 使用qsort快速排序（O(n log n)）
	qsort(books, count, sizeof(struct Nodebook*), compareByPublishTime);

	// 显示前10本（或更少）
	int displayCount = count < 10 ? count : 10;
	printf("%-4s %-20s\t%-15s\t%-8s\t%-10s\n",
	       "排名", "书名", "作者", "价格", "借阅次数");
	printf("-----------------------------------------------------------\n");

	for (int i = 0; i < displayCount; i++)
	{
		printf("%-4d %-20s\t%-15s\t%-8.1f\t%-10d\n",
		       i + 1,
		       books[i]->data.name,
		       books[i]->data.author,
		       books[i]->data.price,
		       books[i]->data.readnum);
	}

	delete[] books;
	printf("============================================================\n");
}


















//3.登录模块 
 
//register

//3.1账号信息

struct userInfo
{
	char Account[20];
	char Password[20];
	int Role;
};

//3.2账号链表

struct Nodeuser
{
	struct userInfo data;
	struct Nodeuser* next;
};
struct Nodeuser* userlist=NULL;
//创建表头
struct Nodeuser* createuserHead()
{
	struct Nodeuser* headNode=new Nodeuser;
	headNode->next=NULL;
	return headNode;
}
//创建节点
struct Nodeuser* createuserNode(struct userInfo data)
{
	struct Nodeuser* newNode=new Nodeuser;
	newNode->data=data;
	newNode->next=NULL;
	return newNode;
}
//插入节点：头插法
void insertuserNodeByHead(struct Nodeuser* headNode,struct userInfo data)
{
	struct Nodeuser* newNode=createuserNode(data);
	newNode->next=headNode->next;
	headNode->next=newNode;
}
//指定元素删除
void deleteuserNodeByName(struct Nodeuser* headNode,char *userName)
{
	struct Nodeuser* posLeftNode=headNode;
	struct Nodeuser* posNode=headNode->next;

	while(posNode!=NULL&&strcmp(posNode->data.Account,userName))
	{
		posLeftNode=posNode;
		posNode=posLeftNode->next;
	}
	if(posNode==NULL)return;
	else
	{
		printf("删除成功\n");
		posLeftNode->next=posNode->next;
		delete posNode;
		posNode=NULL;
	}
}

//打印链表
void printuserList(struct Nodeuser* headNode)
{
	struct Nodeuser* pMove = headNode->next;
	// 使用固定宽度格式化，-表示左对齐，数字表示最小宽度
	printf("%-20s\t%-8s\t%-6s\n",
	       "账号", "密码", "身份");
	while (pMove != NULL)
	{
		printf("%-20s\t%-8s\t%-6d\n",
		       pMove->data.Account,
		       pMove->data.Password,
		       pMove->data.Role);
		pMove = pMove->next;
	}
}
//释放链表
void freeuserList(struct Nodeuser* headNode)
{
	struct Nodeuser* pMove = headNode->next;
	while (pMove != NULL)
	{
		struct Nodeuser* temp = pMove;
		pMove = pMove->next;
		delete temp;
	}
	delete headNode;  // ← 释放头节点
}
//查找账号
struct Nodeuser* searchuserByName(struct Nodeuser* headNode,char* userName)
{
	struct Nodeuser* posNode=headNode->next;
	while(posNode!=NULL&&strcmp(posNode->data.Account,userName))
	{
		posNode=posNode->next;
	}
	return posNode;
}

//3.3账号排序 

//账号归并排序按字典序
// 合并两个有序链表（按账号名升序）
struct Nodeuser* mergeSorteduserLists(struct Nodeuser* left, struct Nodeuser* right)
{
	struct Nodeuser dummy;  // 临时头节点
	struct Nodeuser* tail = &dummy;
	dummy.next = NULL;

	while (left != NULL && right != NULL)
	{
		if (strcmp(left->data.Account, right->data.Account) <= 0)
		{
			tail->next = left;
			left = left->next;
		}
		else
		{
			tail->next = right;
			right = right->next;
		}
		tail = tail->next;
	}

	// 连接剩余部分
	if (left != NULL) tail->next = left;
	if (right != NULL) tail->next = right;

	return dummy.next;
}
// 使用快慢指针找到链表中间节点
struct Nodeuser* getMiddleuserNode(struct Nodeuser* head)
{
	if (head == NULL || head->next == NULL) return head;

	struct Nodeuser* slow = head;
	struct Nodeuser* fast = head->next;

	while (fast != NULL && fast->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}
// 归并排序主函数（递归）
struct Nodeuser* mergeSortuserListInternal(struct Nodeuser* head)
{
	if (head == NULL || head->next == NULL) return head;

	// 找到中间节点并分割链表
	struct Nodeuser* middle = getMiddleuserNode(head);
	struct Nodeuser* nextToMiddle = middle->next;
	middle->next = NULL;  // 切断链表

	// 递归排序两部分
	struct Nodeuser* left = mergeSortuserListInternal(head);
	struct Nodeuser* right = mergeSortuserListInternal(nextToMiddle);

	// 合并已排序的两部分
	return mergeSorteduserLists(left, right);
}
// 原函数名保持不变，内部改为归并排序
void mergeSortuserList(struct Nodeuser* headNode)
{
	if (headNode == NULL || headNode->next == NULL || headNode->next->next == NULL)
		return;  // 空链表或只有一个节点，无需排序

	// 获取实际的数据节点（跳过头节点）
	struct Nodeuser* dataHead = headNode->next;

	// 进行归并排序
	dataHead = mergeSortuserListInternal(dataHead);

	// 将排序后的链表重新连接到头节点
	headNode->next = dataHead;
}


















//4.菜单 

//Page1→Role1→Page2→Page5/6
//       Role2→Page2→Page3/4/5/6

//Page1
void makeuserMenu()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】\n");
	printf("\t0.退出系统\n");
	printf("\t1.登录\n");
	printf("\t2.注册\n");
	printf("------------------------------\n");
	printf("请输入(0~2):");
}
//Page2
void makebookMenu()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】\n");
	if(Role==2)printf("管理员模式\n");
	else if(Role==1)printf("用户模式\n");
	printf("\t0.退出系统\n");
	printf("\t1.退出登录\n");
	printf("\t2.查找书籍\n");
	printf("\t3.浏览书籍\n");
	printf("\t4.借阅书籍\n");
	printf("\t5.归还书籍\n");
	printf("\t6.排行榜\n");
	if(Role==2)
	{
		printf("\t7.图书管理\n");
		printf("\t8.账号管理\n");
		printf("------------------------------\n");
		printf("请输入(0~8):");
	}
	else
	{
		printf("------------------------------\n");
		printf("请输入(0~6):");
	}
}

//Page3
void makebookMenuPage3()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】->【图书管理】\n");
	printf("管理员模式\n");
	printf("\t0.返回\n");
	printf("\t1.登记书籍\n");
	printf("\t2.删除书籍\n");
	printf("------------------------------\n");
	printf("请输入(0~2):");
}

//Page4
void makebookMenuPage4()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】->【账号管理】\n");
	printf("管理员模式\n");
	printf("\t0.返回\n");
	printf("\t1.账号信息\n");
	printf("\t2.注册账号\n");
	printf("\t3.删除账号\n");
	printf("\t4.重置密码\n");
	printf("\t5.查找账号\n");
	printf("------------------------------\n");
	printf("请输入(0~5):");
}

//Page5
void makebookMenuPage5()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】->【查找书籍】\n");
	if(Role==1)printf("用户模式\n");
	else if(Role==2)printf("管理员模式\n");
	printf("\t0.返回\n");
	printf("\t1.查找书籍名称\n");
	printf("\t2.查找书籍ISBN\n");
	printf("\t3.查找书籍作者\n");
	printf("------------------------------\n");
	printf("请输入(0~3):");
}

//Page6
void makebookMenuPage6()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】->【排行榜】\n");
	if(Role==1)printf("用户模式\n");
	else if(Role==2)printf("管理员模式\n");
	printf("\t0.返回\n");
	printf("\t1.借阅次数排行榜\n");
	printf("\t2.最新出版排行榜\n");
	printf("------------------------------\n");
	printf("请输入(0~2):");
}


















//5.文件操作

//5.1 图书模块文件操作 

//存操作
void savebookInfoToFile(const char *fileName,struct Nodebook* headNode)
{
	FILE* fp=fopen(fileName,"w");
	struct Nodebook* pMove=headNode->next;
	while(pMove!=NULL)
	{
		fprintf(fp,"%s\t%.1f\t%d\t%s\t%s\t%s\t%d\t%d\n",  // 格式修改，添加publishtime
		        pMove->data.name,
		        pMove->data.price,
		        pMove->data.num,
		        pMove->data.author,
		        pMove->data.publishinghouse,
		        pMove->data.ISBN,
		        pMove->data.readnum,
		        pMove->data.publishtime);  // 新增：保存publishtime
		pMove=pMove->next;
	}
	fclose(fp);
}
//读操作
void readbookInfoFromFile(const char *fileName,struct Nodebook* headNode)
{
	FILE* fp=fopen(fileName,"r");
	if(fp==NULL)
	{
		fp=fopen(fileName,"w+");
		fclose(fp);
		fp=fopen(fileName,"r");
	}
	struct bookInfo tempData;

	// 检查文件是否为空
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(fileSize > 0)
	{
		while(fscanf(fp,"%19s\t%f\t%d\t%s\t%s\t%s\t%d\t%d\n",
		             tempData.name,
		             &tempData.price,
		             &tempData.num,
		             tempData.author,
		             tempData.publishinghouse,
		             tempData.ISBN,
		             &tempData.readnum,
		             &tempData.publishtime)!=EOF)
		{
			insertbookNodeByHead(booklist,tempData);
		}
	}
	fclose(fp);
	Booksortsign=0; 
}
 
//5.2登录模块文件操作

//存操作
void saveuserInfoToFile(const char *fileName,struct Nodeuser* headNode)
{
	FILE* fp=fopen(fileName,"w");
	struct Nodeuser* pMove=headNode->next;
	while(pMove!=NULL)
	{
		fprintf(fp,"%s\t%s\t%d\n",pMove->data.Account,pMove->data.Password,pMove->data.Role);
		pMove=pMove->next;
	}
	fclose(fp);
}
//读操作
void readuserInfoFromFile(const char *fileName,struct Nodeuser* headNode)
{
	FILE* fp=fopen(fileName,"r");
	if(fp==NULL)
	{
		fp=fopen(fileName,"w+");
	}
	struct userInfo tempData;
	while(fscanf(fp,"%s\t%s\t%d\n",tempData.Account,tempData.Password,&tempData.Role)!=EOF)
	{
		insertuserNodeByHead(userlist,tempData);
	}
	fclose(fp);
}


















//6.交互模块
 
//6.1 Page1 

void userkeyDown()
{
	int userKey=0;
	struct userInfo tempUser;//产生临时变量存储账号信息
	struct Nodeuser* userresult=NULL;
	scanf("%d",&userKey);
	switch(userKey)
	{
		case 0:
			printf("【退出】");
			printf("退出成功\n");
			system("pause");
			exit(0);
			break;
		case 1:
			printf("【登录】\n");
			printf("请输入账号密码，用空格/回车分隔:\n");
			printf("键入【rt】返回\n");
			cin>>tempUser.Account;
			if(strcmp(tempUser.Account,"rt")==0)break;
			cin>>tempUser.Password;
			if(strcmp(tempUser.Password,"rt")==0)break;
			userresult = searchuserByName(userlist, tempUser.Account);
			if (userresult == NULL)
			{
				printf("用户不存在\n");
			}
			else if(strcmp(userresult->data.Password, tempUser.Password) != 0)
			{
				printf("密码错误\n");
			}
			else
			{
				Role=userresult->data.Role;
				printf("-----------------------------------------------------------------------------------------\n");
				if(Role==1)printf("用户");
				else if(Role==2)printf("管理员");
				printf("%-20s\n",userresult->data.Account);
				printf("登录成功\n");
				Page=2;
			}
			break;
		case 2:
			printf("【注册】");
			printf("输入用户的信息(Account,Password,Role)(Role=1用户;Role=2管理员):");
			printf("键入【rt】返回\n");
			cin>>tempUser.Account;
			if(strcmp(tempUser.Account,"rt")==0)break;
			cin>>tempUser.Password;
			if(strcmp(tempUser.Password,"rt")==0)break;
			char tempRole[10];
			cin>>tempRole;
			if(strcmp(tempRole,"1")==0)
			{
				Role=1;
				tempUser.Role=1;
			}
			else if(strcmp(tempRole,"2")==0)
			{
				Role=2;
				tempUser.Role=2;
			}
			else if(strcmp(tempRole,"rt")==0)break;
			else
			{
				cout<<"Role输入错误，请输入1(用户)或2(管理员)\n";
				break;
			}
			userresult = searchuserByName(userlist, tempUser.Account);
			if (userresult == NULL)
			{
				insertuserNodeByHead(userlist,tempUser);
				saveuserInfoToFile("userinfo.txt",userlist);
				printf("-----------------------------------------------------------------------------------------\n");
				printf("注册成功\n");
				Page=2;
			}
			else
			{
				printf("注册失败，账号已存在!\n");
				Role=0;
			}
			break;
		default:
			break;
	}
}

//6.2 Page2Role1

void bookkeyDownRole1()//用户模式
{
	int userKey=0;
	struct bookInfo tempBook;//产生临时变量存储书籍信息
	struct userInfo tempUser;//产生临时变量存储账号信息
	struct Nodebook* bookresult=NULL;
	scanf("%d",&userKey);
	switch(userKey)
	{
		case 0:
			printf("【退出系统】");
			printf("退出成功\n");
			system("pause");
			exit(0);
			break;
		case 1:
			printf("【退出登录】");
			printf("退出成功\n");
			Role=0;
			break;
		case 2:
			printf("【查找书籍】");
			Page=5;
			break;
		case 3:
			printf("【浏览书籍】\n");
			if(Booksortsign==0)
			{
				mergeSortbookList(booklist);
				savebookInfoToFile("bookinfo.txt",booklist);
				Booksortsign=1;
			}
			printbookList(booklist);
			break;
		case 4:
			printf("【借阅】\n");
			printf("请输入借阅的书名：");
			printf("键入【rt】返回\n");
			scanf("%s",tempBook.name);
			if(strcmp(tempBook.name,"rt")==0)break;
			bookresult=searchbookByName(booklist,tempBook.name);
			if(bookresult==NULL)
			{
				printf("没有相关书籍无法借阅！\n");
			}
			else
			{
				if(bookresult->data.num>0)
				{
					bookresult->data.num--;
					bookresult->data.readnum++; // 借阅次数+1
					savebookInfoToFile("bookinfo.txt",booklist);
					printf("借阅成功！\n");
					printf("%-6s 剩余数量 %-6d 借阅次数 %-6d\n",
					       bookresult->data.name,
					       bookresult->data.num,
					       bookresult->data.readnum); // 显示借阅次数
				}
				else
				{
					printf("当前书籍无库存，借阅失败！\n");
				}
			}
			break;
		case 5:
			printf("【归还】\n");
			printf("请输入归还的书名：");
			printf("键入【rt】返回\n");
			scanf("%s",tempBook.name);
			if(strcmp(tempBook.name,"rt")==0)break;
			bookresult=searchbookByName(booklist,tempBook.name);
			if(bookresult==NULL)
			{
				printf("该书不属于图书馆！\n");
			}
			else
			{
				bookresult->data.num++;
				savebookInfoToFile("bookinfo.txt",booklist);
				printf("书籍归还成功！\n");
				printf("%-6s 剩余数量 %-6d 借阅次数 %-6d\n",
				       bookresult->data.name,
				       bookresult->data.num,
				       bookresult->data.readnum); // 显示借阅次数
			}
			break;
		case 6:
			printf("【排行榜】");
			Page=6;
			break;
		default:
			printf("【返回】");
			Page=2;
			break;
	}
}

//6.3 Page2Role2

void bookkeyDownRole2()//管理员模式
{
	int userKey=0;
	struct bookInfo tempBook;//产生临时变量存储书籍信息
	struct userInfo tempUser;//产生临时变量存储账号信息
	struct Nodebook* bookresult=NULL;
	struct Nodeuser* userresult=NULL;
	scanf("%d",&userKey);
	switch(userKey)
	{
		case 0:
			printf("【退出系统】");
			printf("退出成功\n");
			system("pause");
			exit(0);
			break;
		case 1:
			printf("【退出登录】");
			printf("退出成功\n");
			Role=0;
			break;
		case 2:
			printf("【查找书籍】");
			Page=5;
			break;
		case 3:
			printf("【浏览书籍】\n");
			if(Booksortsign==0)
			{
				mergeSortbookList(booklist);
				savebookInfoToFile("bookinfo.txt",booklist);
				Booksortsign=1;
			}
			printbookList(booklist);
			break;
		case 4:
			printf("【借阅书籍】\n");
			printf("请输入借阅的书名：\n");
			printf("键入【rt】返回\n");
			scanf("%s",tempBook.name);
			if(strcmp(tempBook.name,"rt")==0)break;
			bookresult=searchbookByName(booklist,tempBook.name);
			if(bookresult==NULL)
			{
				printf("没有相关书籍无法借阅！\n");
			}
			else
			{
				if(bookresult->data.num>0)
				{
					bookresult->data.num--;
					bookresult->data.readnum++; // 借阅次数+1
					savebookInfoToFile("bookinfo.txt",booklist);
					printf("借阅成功！\n");
					printf("%-6s 剩余数量 %-6d 借阅次数 %-6d\n",
					       bookresult->data.name,
					       bookresult->data.num,
					       bookresult->data.readnum); // 显示借阅次数
				}
				else
				{
					printf("当前书籍无库存，借阅失败！\n");
				}
			}
			break;
		case 5:
			printf("【归还书籍】\n");
			printf("请输入归还的书名：\n");
			printf("键入【rt】返回\n");
			scanf("%s",tempBook.name);
			if(strcmp(tempBook.name,"rt")==0)break;
			bookresult=searchbookByName(booklist,tempBook.name);
			if(bookresult==NULL)
			{
				printf("该书不属于图书馆！\n");
			}
			else
			{
				bookresult->data.num++;
				savebookInfoToFile("bookinfo.txt",booklist);
				printf("书籍归还成功！\n");
				printf("%-6s 剩余数量 %-6d 借阅次数 %-6d\n",
				       bookresult->data.name,
				       bookresult->data.num,
				       bookresult->data.readnum); // 显示借阅次数
			}
			break;
		case 6:
			printf("【排行榜】");
			Page=6;
			break;
		case 7:
			printf("【书籍管理】");
			Page=3;
			break;
		case 8:
			printf("【账号管理】");
			Page=4;
			break;
		default:
			printf("【返回】");
			Page=2;
			break;
	}
}



//6.4  Page3Role2 

void bookkeyDownRole2book()
{
	int userKey=0;
	struct bookInfo tempBook;//产生临时变量存储书籍信息
	struct userInfo tempUser;//产生临时变量存储账号信息
	struct Nodebook* bookresult=NULL;
	struct Nodeuser* userresult=NULL;
	scanf("%d",&userKey);
	switch(userKey)
	{
		case 0:
			printf("【返回】");
			Page=2;
			break;
		case 1:
			printf("【登记书籍】");
			printf("输入书籍的信息(name,price,num,author,publishinghouse,ISBN/ISSN):\n");
			printf("键入【rt】返回\n");
			cin>>tempBook.name;
			if(strcmp(tempBook.name,"rt")==0)break;
			scanf("%f%d%s%s%s",
			      &tempBook.price,
			      &tempBook.num,
			      tempBook.author,
			      tempBook.publishinghouse,
			      tempBook.ISBN);
			tempBook.readnum = 0; // 初始借阅次数为0
			// 设置publishtime为当前最大值+1
			tempBook.publishtime = getMaxPublishtime(booklist) + 1;
			insertbookNodeByHead(booklist,tempBook);
			savebookInfoToFile("bookinfo.txt",booklist);
			printf("登记成功\n");
			break;
		case 2:
			printf("【删除书籍】\n");
			printf("请输入删除的书名:\n");
			printf("键入【rt】返回\n");
			scanf("%s",tempBook.name);
			if(strcmp(tempBook.name,"rt")==0)break;
			deletebookNodeByName(booklist,tempBook.name);
			savebookInfoToFile("bookinfo.txt",booklist);
			break;
		default:
			printf("【返回】");
			Page=2;
			break;
	}
}

//6.5 Page4Role2 

void bookkeyDownRole2user()
{
	int userKey=0;
	struct bookInfo tempBook;//产生临时变量存储书籍信息
	struct userInfo tempUser;//产生临时变量存储账号信息
	struct Nodebook* bookresult=NULL;
	struct Nodeuser* userresult=NULL;
	scanf("%d",&userKey);
	switch(userKey)
	{
		case 0:
			printf("【返回】");
			Page=2;
			break;
		case 1:
			printf("【浏览账号】\n");
			mergeSortuserList(userlist);
			printuserList(userlist);
			break;
		case 2:
			printf("【注册账号】");
			printf("输入用户的信息(Account,Password,Role):");
			printf("键入【rt】返回\n");
			cin>>tempUser.Account;
			if(strcmp(tempUser.Account,"rt")==0)break;
			cin>>tempUser.Password;
			if(strcmp(tempUser.Password,"rt")==0)break;
			char tempRole[10];
			cin>>tempRole;
			if(strcmp(tempRole,"1")==0)tempUser.Role=1;
			else if(strcmp(tempRole,"2")==0)tempUser.Role=2;
			else if(strcmp(tempRole,"rt")==0)break;
			else
			{
				cout<<"Role输入错误，请输入1(用户)或2(管理员)\n";
				break;
			}
			userresult = searchuserByName(userlist, tempUser.Account);
			if (userresult == NULL)
			{
				insertuserNodeByHead(userlist,tempUser);
				saveuserInfoToFile("userinfo.txt",userlist);
				printf("注册成功\n");
			}
			else
			{
				printf("注册失败，账号已存在!\n");
			}
			break;
		case 3:
			printf("【删除账号】\n");
			printf("请输入删除的账号名:\n");
			printf("键入【rt】返回\n");
			scanf("%s",tempUser.Account);
			if(strcmp(tempUser.Account,"rt")==0)break;
			deleteuserNodeByName(userlist,tempUser.Account);
			saveuserInfoToFile("userinfo.txt",userlist);
			break;
		case 4:
			printf("【重置密码】\n");
			printf("请输入需要重置密码的账号\n");
			printf("键入【rt】返回\n");
			scanf("%s",tempUser.Account);
			if(strcmp(tempUser.Account,"rt")==0)break;
			userresult = searchuserByName(userlist, tempUser.Account);
			if (userresult == NULL)
			{
				printf("用户不存在\n");
			}
			else
			{
				printf("当前账号：%s\n",userresult->data.Account);
				printf("当前密码：%s\n",userresult->data.Password);
				printf("确定要重置密码为123456吗？是请输入1，否请输入0\n");
				char tempA[20];
				cin>>tempA;
				if(strcmp(tempA,"1")==0)
				{
					printf("重置默认密码：123456\n");
					strcpy(userresult->data.Password, "123456");
					saveuserInfoToFile("userinfo.txt",userlist);
				}
				else printf("【返回】");
			}
			break;
		case 5:
			printf("【查找账号】");
			printf("输入查找的账号:");
			printf("键入【rt】返回\n");
			cin>>tempUser.Account;
			if(strcmp(tempUser.Account,"rt")==0)break;
			userresult = searchuserByName(userlist, tempUser.Account);
			if (userresult == NULL)
			{
				printf("该账号不存在\n");
			}
			else
			{
				printf("%-20s\t%-8s\t%-6s\n",
				       "账号", "密码", "权限");
				printf("%-20s\t%-8s\t%-6d\n",
				       userresult->data.Account,
				       userresult->data.Password,
				       userresult->data.Role);
			}
			break;
		default:
			printf("【返回】");
			Page=2;
			break;
	}
}

//6.6 Page5

void bookkeyDownPage5()
{
	int userKey=0;
	struct bookInfo tempBook;//产生临时变量存储书籍信息
	struct Nodebook* bookresult=NULL;
	scanf("%d",&userKey);
	switch(userKey)
	{
		case 0:
			printf("【返回】");
			Page=2;
			break;
		case 1:
			printf("【按书名查找】\n");
			printf("请输入查找的书名:\n");
			printf("键入【rt】返回\n");
			scanf("%s", tempBook.name);
			if(strcmp(tempBook.name,"rt")==0)break;
			bookresult = searchbookByName(booklist, tempBook.name);
			if (bookresult == NULL)
			{
				printf("未找到相关信息\n");
			}
			else
			{
				printf("%-20s\t%-8s\t%-6s\t%-15s\t%-20s\t%-13s\t%-10s\n",
				       "书名", "价格", "数量", "作者", "出版社", "ISBN", "借阅次数");
				printf("-------------------------------------------------------------------------------------------------------------------\n");
				printf("%-20s\t%-8.1f\t%-6d\t%-15s\t%-20s\t%-13s\t%-10d\n",
				       bookresult->data.name,
				       bookresult->data.price,
				       bookresult->data.num,
				       bookresult->data.author,
				       bookresult->data.publishinghouse,
				       bookresult->data.ISBN,
				       bookresult->data.readnum); // 输出借阅次数
			}
			break;
		case 2:
			printf("【按ISBN查找】\n");
			printf("请输入查找的ISBN:\n");
			printf("键入【rt】返回\n");
			scanf("%s", tempBook.ISBN);
			if(strcmp(tempBook.ISBN,"rt")==0)break;
			bookresult = searchbookByISBN(booklist, tempBook.ISBN);
			if (bookresult == NULL)
			{
				printf("未找到相关信息\n");
			}
			else
			{
				printf("%-20s\t%-8s\t%-6s\t%-15s\t%-20s\t%-13s\t%-10s\n",
				       "书名", "价格", "数量", "作者", "出版社", "ISBN", "借阅次数");
				printf("-------------------------------------------------------------------------------------------------------------------\n");
				printf("%-20s\t%-8.1f\t%-6d\t%-15s\t%-20s\t%-13s\t%-10d\n",
				       bookresult->data.name,
				       bookresult->data.price,
				       bookresult->data.num,
				       bookresult->data.author,
				       bookresult->data.publishinghouse,
				       bookresult->data.ISBN,
				       bookresult->data.readnum); // 输出借阅次数
			}
			break;
		case 3:
			printf("【按作者查找】\n");
			printf("请输入作者姓名:\n");
			printf("键入【rt】返回\n");
			char authorName[20];
			scanf("%s", authorName);
			if(strcmp(authorName,"rt")==0)break;
			searchAndPrintBooksByAuthor(booklist, authorName);
			break;
		default:
			printf("【返回】");
			Page=2;
			break;
	}
}

//6.7 Page6

void bookkeyDownPage6()
{
	int userKey=0;
	//struct bookInfo tempBook;//产生临时变量存储书籍信息
	//struct Nodebook* bookresult=NULL;
	scanf("%d",&userKey);
	switch(userKey)
	{
		case 0:
			printf("【返回】");
			Page=2;
			break;
		case 1:
			printf("【借阅次数排行榜】\n");
			displayTop10ByReadNum(booklist);
			break;
		case 2:
			printf("【最新出版排行榜】\n");
			displayTop10ByPublishTime(booklist);
			break;
		default:
			printf("【返回】");
			Page=2;
			break;
	}
}


















//7.main函数 

int main()
{
	userlist=createuserHead();
	booklist=createbookHead();
	readbookInfoFromFile("bookinfo.txt",booklist);
	readuserInfoFromFile("userinfo.txt",userlist);

	while(1)
	{
		makeuserMenu();
		userkeyDown();
		system("pause");
		system("cls");
		while(Role!=0)
		{
			makebookMenu();
			if(Page==2&&Role==1)bookkeyDownRole1();
			else if(Page==2&&Role==2)bookkeyDownRole2();
			else if(Page==3)
			{
				system("cls");
				makebookMenuPage3();
				bookkeyDownRole2book();
			}
			else if(Page==4)
			{
				system("cls");
				makebookMenuPage4();
				bookkeyDownRole2user();
			}
			else if(Page==5)
			{
				system("cls");
				makebookMenuPage5();
				bookkeyDownPage5();
			}
			else if(Page==6)
			{
				system("cls");
				makebookMenuPage6();
				bookkeyDownPage6();
			}
			system("pause");
			system("cls");
		}
	}
	system("pause");
	freeuserList(userlist);
	freebookList(booklist);
	return 0;
}






//bookmanager
//1.3 25.12.28改进了输出格式对齐的问题。
//1.4 25.12.28改进了用户界面，借阅和归还时会显示书籍剩余数量
//1.5 25.12.29改进了命名冲突问题，为与register代码合并做准备。
//register
//1.0 25.12.29 将图书管理系统的图书管理功能改为一个登录系统的雏形。目前只有基本的注册功能，还未实现登录功能
//1.1 25.12.29 实现了登录功能
//1.2 25.12.29 改进了命名冲突问题，为与bookmanager代码合并做准备。
//Library Management System
//1.0 25.12.29 初步将bookmanager部分和register部分整合到一起。
//1.1 25.12.29 实现了登录后才能访问图书馆的功能
//1.2 25.12.30 实现了退出登录功能
//1.3 25.12.30 初步处理了用户与管理员权限的问题
//1.4 25.12.30 实现了用户与管理员权限的分离
//1.5 25.12.30 处理了部分非法输入问题
//1.6 25.12.30 增加了管理员按字典序排序账号的功能;规范了部分命名;
//1.7 25.12.30 将排序功能由管理员操作改为自动操作，不作为单独功能，查看账号时将自动排序;将变量ISBN改为longlong类型
//1.8 25.12.30 增加了管理员注册账号的功能
//1.9 25.12.30 增加了管理员查找账号的功能
//2.0 25.12.30 实现了基本完整的分级菜单显示和跳转功能
//2.1 25.12.31 增加了按ISBN查找图书和按作者查找图书的功能，还未实际安装
//2.2 26.01.01 完成了查找图书和它的分级菜单
//2.3 26.01.04 添加了readnum借阅次数和publishtime出版时间
//2.4 26.01.05 添加了借阅次数排行榜和最近出版排行榜
//2.5 26.01.05 将排序书籍和账号中的冒泡排序改为归并排序，将图书按字典序排序合并到阅览书籍中，不作为单独功能；对多处进行了优化
//2.6 26.01.05 将排行榜冒泡排序改为快速排序,优化了系统
//2.7 26.01.05 优化了系统，添加Booksortsign作为书籍排序的条件 
//2.8 26.03.17 将部分c写法替换为c++写法，并将代码块整理增加可读性 
