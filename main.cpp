#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <vector>

#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

/*
目录 输入p+序号搜索
1.全局变量
2.函数
3.书籍管理
4.登录模块
5.菜单
6.文件操作
7.交互模块
8.main函数
*/

//------------------------------p1.全局变量 ------------------------------

int Role = 0;		  // 0未登录 1用户 2管理员
int Page = 1;		  // 1登录 2图书系统 3图书管理 4账号管理
int Booksortsign = 0; // 0未排序 1已排序

//------------------------------p2.函数 ---------------------------------

// 2.1 输入函数readValue

// 本程序交互输入的统一入口，底层使用 cin >> value。

// 从cin读取一个 T 类型的值，成功返回 true；失败时clear、ignore、提示，并返回 false。
// 用法示例：if (!readValue(变量名)) return或break;

template <class T>
bool readValue(T &value)
{
	if (cin >> value)
		return true;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "输入不合法，操作已取消\n";
	return false;
}

//------------------------------p3.书籍管理 ------------------------------

// bookmanager

// 3.1书籍信息

struct bookInfo
{
	std::string name;			 // 书名
	float price;				 // 价格
	int num;					 // 数量
	std::string author;			 // 作者
	std::string publishinghouse; // 出版社
	std::string ISBN;			 // ISBN
	int readnum;				 // 借阅次数
	int publishtime;			 // 录入顺序编号 - 新增
};

// 3.2书籍链表

// 链表
struct Nodebook
{
	struct bookInfo data;
	struct Nodebook *next;
};
struct Nodebook *booklist = NULL;
// 创建表头
struct Nodebook *createbookHead()
{
	struct Nodebook *headNode = new Nodebook;
	headNode->next = NULL;
	return headNode;
}
// 创建节点
struct Nodebook *createbookNode(struct bookInfo data)
{
	struct Nodebook *newNode = new Nodebook;
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
// 插入节点：头插法
void insertbookNodeByHead(struct Nodebook *headNode, struct bookInfo data)
{
	struct Nodebook *newNode = createbookNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
	Booksortsign = 0;
}
// 指定元素删除
void deletebookNodeByName(struct Nodebook *headNode, std::string bookName)
{
	struct Nodebook *posLeftNode = headNode;
	struct Nodebook *posNode = headNode->next;

	while (posNode != NULL && posNode->data.name != bookName)
	{
		posLeftNode = posNode;
		posNode = posLeftNode->next;
	}
	if (posNode == NULL)
		return;
	else
	{
		printf("删除成功\n");
		posLeftNode->next = posNode->next;
		delete posNode;
		posNode = NULL;
	}
	Booksortsign = 0;
}

// 打印链表
void printbookList(struct Nodebook *headNode)
{
	struct Nodebook *pMove = headNode->next;

	//[索引][表格]
	while (pMove != NULL)
	{
		std::cout << "书名: " << pMove->data.name
				  << " | 价格: " << pMove->data.price
				  << " | 数量: " << pMove->data.num
				  << " | 作者: " << pMove->data.author
				  << " | 出版社: " << pMove->data.publishinghouse
				  << " | ISBN: " << pMove->data.ISBN
				  << " | 借阅次数: " << pMove->data.readnum
				  << "\n";
		pMove = pMove->next;
	}
}
// 释放链表
void freebookList(struct Nodebook *headNode)
{
	struct Nodebook *pMove = headNode->next;
	while (pMove != NULL)
	{
		struct Nodebook *temp = pMove;
		pMove = pMove->next;
		delete temp;
	}
	delete headNode; // ← 释放头节点
}

// 3.3书籍查找

// 名字查找
struct Nodebook *searchbookByName(struct Nodebook *headNode, std::string bookName)
{
	struct Nodebook *posNode = headNode->next;
	while (posNode != NULL && posNode->data.name != bookName)
	{
		posNode = posNode->next;
	}
	return posNode;
}
// ISBN查找
struct Nodebook *searchbookByISBN(struct Nodebook *headNode, std::string bookISBN)
{
	struct Nodebook *posNode = headNode->next;
	while (posNode != NULL && posNode->data.ISBN != bookISBN)
	{
		posNode = posNode->next;
	}
	return posNode;
}
// 作者查找
void searchAndPrintBooksByAuthor(struct Nodebook *headNode, const std::string &authorName)
{
	struct Nodebook *posNode = headNode->next;
	int count = 0;

	printf("作者 '%s' 的书籍列表\n\n", authorName.c_str());

	//[索引][表格]
	while (posNode != NULL)
	{
		if (posNode->data.author == authorName)
		{
			count++;
			std::cout << "书名: " << posNode->data.name
					  << " | ISBN: " << posNode->data.ISBN
					  << " | 价格: " << posNode->data.price
					  << " | 数量: " << posNode->data.num
					  << " | 出版社: " << posNode->data.publishinghouse
					  << " | 借阅次数: " << posNode->data.readnum
					  << "\n";
		}
		posNode = posNode->next;
	}

	if (count == 0)
	{
		printf("未找到作者 '%s' 的任何书籍。\n", authorName.c_str());
	}
	else
	{
		printf("总计:%d本书籍\n", count);
	}
}

// 3.4书籍排序

// 书籍归并排序根据字典序排序
//  合并两个有序链表（按书名字典序升序）
struct Nodebook *mergeSortedbookLists(struct Nodebook *left,
									  struct Nodebook *right)
{
	struct Nodebook dummy; // 临时头节点
	struct Nodebook *tail = &dummy;
	dummy.next = NULL;
	while (left != NULL && right != NULL)
	{
		// 比较书名，按字典序升序排列
		if (left->data.name <= right->data.name)
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
	if (left != NULL)
		tail->next = left;
	if (right != NULL)
		tail->next = right;

	return dummy.next;
}

// 使用快慢指针找到链表中间节点（保持不变）
struct Nodebook *getMiddlebookNode(struct Nodebook *head)
{
	if (head == NULL || head->next == NULL)
		return head;

	struct Nodebook *slow = head;
	struct Nodebook *fast = head->next;

	while (fast != NULL && fast->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

// 归并排序主函数（递归）（保持不变）
struct Nodebook *mergeSortbookListInternal(struct Nodebook *head)
{
	if (head == NULL || head->next == NULL)
		return head;

	// 找到中间节点并分割链表
	struct Nodebook *middle = getMiddlebookNode(head);
	struct Nodebook *nextToMiddle = middle->next;
	middle->next = NULL; // 切断链表

	// 递归排序两部分
	struct Nodebook *left = mergeSortbookListInternal(head);
	struct Nodebook *right = mergeSortbookListInternal(nextToMiddle);

	// 合并已排序的两部分
	return mergeSortedbookLists(left, right);
}

void mergeSortbookList(struct Nodebook *headNode)
{
	if (headNode == NULL || headNode->next == NULL ||
		headNode->next->next == NULL)
		return; // 空链表或只有一个节点，无需排序

	// 获取实际的数据节点（跳过头节点）
	struct Nodebook *dataHead = headNode->next;

	// 进行归并排序
	dataHead = mergeSortbookListInternal(dataHead);

	// 将排序后的链表重新连接到头节点
	headNode->next = dataHead;
}

// 3.5排行榜功能

// 3.5.1借阅次数排行榜
//  比较函数，用于qsort（借阅次数降序）
int compareByReadNum(const void *a, const void *b)
{
	struct Nodebook *bookA = *(struct Nodebook **)a;
	struct Nodebook *bookB = *(struct Nodebook **)b;

	// 按借阅次数降序排列（次数多的在前）
	return bookB->data.readnum - bookA->data.readnum;
}
// 借阅次数排行榜
void displayTop10ByReadNum(struct Nodebook *headNode)
{
	printf("=== 借阅次数排行榜 TOP 10 ===\n");

	int count = 0;
	struct Nodebook *temp = headNode->next;
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

	Nodebook **books = new Nodebook *[count];

	temp = headNode->next;
	for (int i = 0; i < count; i++)
	{
		books[i] = temp;
		temp = temp->next;
	}

	qsort(books, count, sizeof(struct Nodebook *), compareByReadNum);

	int displayCount = count < 10 ? count : 10;

	//[索引][表格]
	for (int i = 0; i < displayCount; i++)
	{
		std::cout << "排名: " << (i + 1)
				  << " | 书名: " << books[i]->data.name
				  << " | 作者: " << books[i]->data.author
				  << " | 价格: " << books[i]->data.price
				  << " | 借阅次数: " << books[i]->data.readnum
				  << "\n";
	}

	delete[] books;
}

// 3.5.2 最新出版排行榜（按publishtime排序，数值越大表示越新）
// 比较函数，用于qsort（出版时间降序）
int compareByPublishTime(const void *a, const void *b)
{
	struct Nodebook *bookA = *(struct Nodebook **)a;
	struct Nodebook *bookB = *(struct Nodebook **)b;

	// 按出版时间降序排列（新的在前）
	return bookB->data.publishtime - bookA->data.publishtime;
}
// 获取当前最大的publishtime
int getMaxPublishtime(struct Nodebook *headNode)
{
	int maxTime = 0;
	struct Nodebook *temp = headNode->next;
	while (temp != NULL)
	{
		if (temp->data.publishtime > maxTime)
		{
			maxTime = temp->data.publishtime;
		}
		temp = temp->next;
	}
	return maxTime;
}
// 最新出版排行榜
void displayTop10ByPublishTime(struct Nodebook *headNode)
{
	printf("=== 最新出版排行榜 TOP 10 ===\n");

	int count = 0;
	struct Nodebook *temp = headNode->next;
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

	Nodebook **books = new Nodebook *[count];

	temp = headNode->next;
	for (int i = 0; i < count; i++)
	{
		books[i] = temp;
		temp = temp->next;
	}

	qsort(books, count, sizeof(struct Nodebook *), compareByPublishTime);

	int displayCount = count < 10 ? count : 10;

	//[索引][表格]
	for (int i = 0; i < displayCount; i++)
	{
		std::cout << "排名: " << i + 1
				  << " | 书名: " << books[i]->data.name
				  << " | 作者: " << books[i]->data.author
				  << " | 价格: " << books[i]->data.price
				  << " | 借阅次数: " << books[i]->data.readnum
				  << "\n";
	}

	delete[] books;
}

//------------------------------p4.登录模块 ------------------------------

// register

// 4.1账号信息

struct userInfo
{
	std::string Account;
	std::string Password;
	int Role;
};

// 4.2账号链表

struct Nodeuser
{
	struct userInfo data;
	struct Nodeuser *next;
};
struct Nodeuser *userlist = NULL;
// 创建表头
struct Nodeuser *createuserHead()
{
	struct Nodeuser *headNode = new Nodeuser;
	headNode->next = NULL;
	return headNode;
}
// 创建节点
struct Nodeuser *createuserNode(struct userInfo data)
{
	struct Nodeuser *newNode = new Nodeuser;
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
// 插入节点：头插法
void insertuserNodeByHead(struct Nodeuser *headNode, struct userInfo data)
{
	struct Nodeuser *newNode = createuserNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
}
// 指定元素删除
void deleteuserNodeByName(struct Nodeuser *headNode, const std::string &userName)
{
	struct Nodeuser *posLeftNode = headNode;
	struct Nodeuser *posNode = headNode->next;

	while (posNode != NULL && posNode->data.Account != userName)
	{
		posLeftNode = posNode;
		posNode = posLeftNode->next;
	}
	if (posNode == NULL)
		return;
	else
	{
		printf("删除成功\n");
		posLeftNode->next = posNode->next;
		delete posNode;
		posNode = NULL;
	}
}

// 打印链表
void printuserList(struct Nodeuser *headNode)
{
	struct Nodeuser *pMove = headNode->next;

	//[索引][表格]
	while (pMove != NULL)
	{
		std::cout << "账号: " << pMove->data.Account
				  << " | 密码: " << pMove->data.Password
				  << " | 权限: " << pMove->data.Role
				  << "\n";
		pMove = pMove->next;
	}
}
// 释放链表
void freeuserList(struct Nodeuser *headNode)
{
	struct Nodeuser *pMove = headNode->next;
	while (pMove != NULL)
	{
		struct Nodeuser *temp = pMove;
		pMove = pMove->next;
		delete temp;
	}
	delete headNode; // ← 释放头节点
}
// 查找账号
struct Nodeuser *searchuserByName(struct Nodeuser *headNode, const std::string &userName)
{
	struct Nodeuser *posNode = headNode->next;
	while (posNode != NULL && posNode->data.Account != userName)
	{
		posNode = posNode->next;
	}
	return posNode;
}

// 4.3账号排序

// 账号归并排序按字典序
//  合并两个有序链表（按账号名升序）
struct Nodeuser *mergeSorteduserLists(struct Nodeuser *left,
									  struct Nodeuser *right)
{
	struct Nodeuser dummy; // 临时头节点
	struct Nodeuser *tail = &dummy;
	dummy.next = NULL;

	while (left != NULL && right != NULL)
	{
		if (left->data.Account <= right->data.Account)
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
	if (left != NULL)
		tail->next = left;
	if (right != NULL)
		tail->next = right;

	return dummy.next;
}
// 使用快慢指针找到链表中间节点
struct Nodeuser *getMiddleuserNode(struct Nodeuser *head)
{
	if (head == NULL || head->next == NULL)
		return head;

	struct Nodeuser *slow = head;
	struct Nodeuser *fast = head->next;

	while (fast != NULL && fast->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}
// 归并排序主函数（递归）
struct Nodeuser *mergeSortuserListInternal(struct Nodeuser *head)
{
	if (head == NULL || head->next == NULL)
		return head;

	// 找到中间节点并分割链表
	struct Nodeuser *middle = getMiddleuserNode(head);
	struct Nodeuser *nextToMiddle = middle->next;
	middle->next = NULL; // 切断链表

	// 递归排序两部分
	struct Nodeuser *left = mergeSortuserListInternal(head);
	struct Nodeuser *right = mergeSortuserListInternal(nextToMiddle);

	// 合并已排序的两部分
	return mergeSorteduserLists(left, right);
}
// 原函数名保持不变，内部改为归并排序
void mergeSortuserList(struct Nodeuser *headNode)
{
	if (headNode == NULL || headNode->next == NULL ||
		headNode->next->next == NULL)
		return; // 空链表或只有一个节点，无需排序

	// 获取实际的数据节点（跳过头节点）
	struct Nodeuser *dataHead = headNode->next;

	// 进行归并排序
	dataHead = mergeSortuserListInternal(dataHead);

	// 将排序后的链表重新连接到头节点
	headNode->next = dataHead;
}

//------------------------------p5.菜单 ------------------------------

// Page1→Role1→Page2→Page5/6
//      →Role2→Page2→Page3/4/5/6

// Page1
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
// Page2
void makebookMenu()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】\n");
	if (Role == 2)
		printf("管理员模式\n");
	else if (Role == 1)
		printf("用户模式\n");
	printf("\t0.退出系统\n");
	printf("\t1.退出登录\n");
	printf("\t2.查找书籍\n");
	printf("\t3.浏览书籍\n");
	printf("\t4.借阅书籍\n");
	printf("\t5.归还书籍\n");
	printf("\t6.排行榜\n");
	if (Role == 2)
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

// Page3
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

// Page4
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

// Page5
void makebookMenuPage5()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】->【查找书籍】\n");
	if (Role == 1)
		printf("用户模式\n");
	else if (Role == 2)
		printf("管理员模式\n");
	printf("\t0.返回\n");
	printf("\t1.查找书籍名称\n");
	printf("\t2.查找书籍ISBN\n");
	printf("\t3.查找书籍作者\n");
	printf("------------------------------\n");
	printf("请输入(0~3):");
}

// Page6
void makebookMenuPage6()
{
	printf("------------------------------\n");
	printf("图书管理系统\n");
	printf("【登录】->【图书系统】->【排行榜】\n");
	if (Role == 1)
		printf("用户模式\n");
	else if (Role == 2)
		printf("管理员模式\n");
	printf("\t0.返回\n");
	printf("\t1.借阅次数排行榜\n");
	printf("\t2.最新出版排行榜\n");
	printf("------------------------------\n");
	printf("请输入(0~2):");
}

//------------------------------p6.文件操作------------------------------

// 5.1 图书模块文件操作

// 存操作
void savebookInfoToFile(const char *fileName, struct Nodebook *headNode)
{
	std::ofstream ofs(fileName);
	if (!ofs)
		return; // 打开失败直接返回

	struct Nodebook *pMove = headNode->next;
	while (pMove != NULL)
	{
		ofs << pMove->data.name << '\t'
			<< pMove->data.price << '\t'
			<< pMove->data.num << '\t'
			<< pMove->data.author << '\t'
			<< pMove->data.publishinghouse << '\t'
			<< pMove->data.ISBN << '\t'
			<< pMove->data.readnum << '\t'
			<< pMove->data.publishtime << '\n';
		pMove = pMove->next;
	}
	ofs.close();
}

// 读操作
void readbookInfoFromFile(const char *fileName)
{
	std::ifstream ifs(fileName);
	if (!ifs)
	{
		// 文件不存在，创建空文件后返回
		std::ofstream ofs(fileName);
		ofs.close();
		Booksortsign = 0;
		return;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue; // 跳过空行

		// 直接在本循环内按 '\t' 拆分
		std::vector<std::string> f;
		std::stringstream ss(line);
		std::string item;
		while (std::getline(ss, item, '\t'))
		{
			f.push_back(item);
		}

		if (f.size() < 8)
			continue; // 字段不够，跳过这行

		struct bookInfo tempData;
		tempData.name = f[0];
		tempData.price = std::stof(f[1]);
		tempData.num = std::stoi(f[2]);
		tempData.author = f[3];
		tempData.publishinghouse = f[4];
		tempData.ISBN = f[5];
		tempData.readnum = std::stoi(f[6]);
		tempData.publishtime = std::stoi(f[7]);

		insertbookNodeByHead(booklist, tempData);
	}
	ifs.close();
	Booksortsign = 0;
}

// 5.2登录模块文件操作

// 存操作
void saveuserInfoToFile(const char *fileName, struct Nodeuser *headNode)
{
	std::ofstream ofs(fileName);
	if (!ofs)
		return;

	struct Nodeuser *pMove = headNode->next;
	while (pMove != NULL)
	{
		ofs << pMove->data.Account << '\t'
			<< pMove->data.Password << '\t'
			<< pMove->data.Role << '\n';
		pMove = pMove->next;
	}
	ofs.close();
}

// 读操作
void readuserInfoFromFile(const char *fileName)
{
	std::ifstream ifs(fileName);
	if (!ifs)
	{
		std::ofstream ofs(fileName); // 创建空文件
		ofs.close();
		return;
	}

	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;

		// 直接在本循环内按 '\t' 拆分
		std::vector<std::string> f;
		std::stringstream ss(line);
		std::string item;
		while (std::getline(ss, item, '\t'))
		{
			f.push_back(item);
		}

		if (f.size() < 3)
			continue;

		struct userInfo tempData;
		tempData.Account = f[0];
		tempData.Password = f[1];
		tempData.Role = std::stoi(f[2]);

		insertuserNodeByHead(userlist, tempData);
	}
	ifs.close();
}

//------------------------------p7.交互模块------------------------------

// 7.1 Page1

void userkeyDown()
{
	int userKey = -1;
	struct userInfo tempUser; // 产生临时变量存储账号信息
	struct Nodeuser *userresult = NULL;

	if (!readValue(userKey))
		return;

	switch (userKey)
	{
	case 0:
	{
		cout << "【退出】";
		cout << "退出成功\n";
		system("pause");
		exit(0);
		break;
	}
	case 1:
	{
		cout << "【登录】\n";
		cout << "请输入账号密码，用空格/回车分隔:\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempUser.Account))
			break;
		if (tempUser.Account == "rt")
			break;

		if (!readValue(tempUser.Password))
			break;
		if (tempUser.Password == "rt")
			break;

		userresult = searchuserByName(userlist, tempUser.Account);
		if (userresult == NULL)
		{
			cout << "用户不存在\n";
		}
		else if (userresult->data.Password != tempUser.Password)
		{
			cout << "密码错误\n";
		}
		else
		{
			Role = userresult->data.Role;
			cout << "----------------------------------------------------------------"
					"-------------------------\n";
			if (Role == 1)
				cout << "用户";
			else if (Role == 2)
				cout << "管理员";
			cout << left << setw(20) << userresult->data.Account << "\n";
			cout << "登录成功\n";
			Page = 2;
		}
		break;
	}
	case 2:
	{
		cout << "【注册】";
		cout << "输入用户的信息(Account,Password,Role)(Role=1用户;Role=2管理员):";
		cout << "键入【rt】返回\n";
		if (!readValue(tempUser.Account))
			break;
		if (tempUser.Account == "rt")
			break;

		if (!readValue(tempUser.Password))
			break;
		if (tempUser.Password == "rt")
			break;

		std::string tempRole;
		if (!readValue(tempRole))
			break;
		if (tempRole == "1")
		{
			Role = 1;
			tempUser.Role = 1;
		}
		else if (tempRole == "2")
		{
			Role = 2;
			tempUser.Role = 2;
		}
		else if (tempRole == "rt")
			break;
		else
		{
			cout << "Role输入错误，请输入1(用户)或2(管理员)\n";
			break;
		}
		userresult = searchuserByName(userlist, tempUser.Account);
		if (userresult == NULL)
		{
			insertuserNodeByHead(userlist, tempUser);
			saveuserInfoToFile("userinfo.txt", userlist);
			cout << "----------------------------------------------------------------"
					"-------------------------\n";
			cout << "注册成功\n";
			Page = 2;
		}
		else
		{
			cout << "注册失败，账号已存在!\n";
			Role = 0;
		}
		break;
	}
	default:
	{
		cout << "请输入有效的选项。按任意键返回上一界面\n";
		break;
	}
	}
}

// 7.2 Page2Role1

void bookkeyDownRole1() // 用户模式
{
	int userKey = 0;
	struct bookInfo tempBook; // 产生临时变量存储书籍信息
	struct Nodebook *bookresult = NULL;
	if (!readValue(userKey))
		return;
	switch (userKey)
	{
	case 0:
		cout << "【退出系统】";
		cout << "退出成功\n";
		system("pause");
		exit(0);
		break;
	case 1:
		cout << "【退出登录】";
		cout << "退出成功\n";
		Role = 0;
		break;
	case 2:
		cout << "【查找书籍】";
		Page = 5;
		break;
	case 3:
		cout << "【浏览书籍】\n";
		if (Booksortsign == 0)
		{
			mergeSortbookList(booklist);
			savebookInfoToFile("bookinfo.txt", booklist);
			Booksortsign = 1;
		}
		printbookList(booklist);
		break;
	case 4:
		cout << "【借阅】\n";
		cout << "请输入借阅的书名：";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.name))
			break;
		if (tempBook.name == "rt")
			break;
		bookresult = searchbookByName(booklist, tempBook.name);
		if (bookresult == NULL)
		{
			cout << "没有相关书籍无法借阅！\n";
		}
		else
		{
			if (bookresult->data.num > 0)
			{
				bookresult->data.num--;
				bookresult->data.readnum++; // 借阅次数+1
				savebookInfoToFile("bookinfo.txt", booklist);
				printf("借阅成功！\n");

				//[索引][表格]
				std::cout << "书名: " << bookresult->data.name
						  << " | 库存: " << bookresult->data.num
						  << " | 借阅次数: " << bookresult->data.readnum
						  << "\n";
			}
			else
			{
				cout << "当前书籍无库存，借阅失败！\n";
			}
		}
		break;
	case 5:
		cout << "【归还】\n";
		cout << "请输入归还的书名：";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.name))
			break;
		if (tempBook.name == "rt")
			break;
		bookresult = searchbookByName(booklist, tempBook.name);
		if (bookresult == NULL)
		{
			cout << "该书不属于图书馆！\n";
		}
		else
		{
			bookresult->data.num++;
			savebookInfoToFile("bookinfo.txt", booklist);
			printf("书籍归还成功！\n");

			//[索引][表格]
			std::cout << "书名: " << bookresult->data.name
					  << " | 库存: " << bookresult->data.num
					  << " | 借阅次数: " << bookresult->data.readnum
					  << "\n";
		}
		break;
	case 6:
		cout << "【排行榜】";
		Page = 6;
		break;
	default:
		cout << "【返回】";
		Page = 2;
		break;
	}
}

// 7.3 Page2Role2

void bookkeyDownRole2() // 管理员模式
{
	int userKey = 0;
	struct bookInfo tempBook; // 产生临时变量存储书籍信息
	struct Nodebook *bookresult = NULL;
	if (!readValue(userKey))
		return;
	switch (userKey)
	{
	case 0:
		cout << "【退出系统】";
		cout << "退出成功\n";
		system("pause");
		exit(0);
		break;
	case 1:
		cout << "【退出登录】";
		cout << "退出成功\n";
		Role = 0;
		break;
	case 2:
		cout << "【查找书籍】";
		Page = 5;
		break;
	case 3:
		cout << "【浏览书籍】\n";
		if (Booksortsign == 0)
		{
			mergeSortbookList(booklist);
			savebookInfoToFile("bookinfo.txt", booklist);
			Booksortsign = 1;
		}
		printbookList(booklist);
		break;
	case 4:
		cout << "【借阅书籍】\n";
		cout << "请输入借阅的书名：\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.name))
			break;
		if (tempBook.name == "rt")
			break;
		bookresult = searchbookByName(booklist, tempBook.name);
		if (bookresult == NULL)
		{
			cout << "没有相关书籍无法借阅！\n";
		}
		else
		{
			if (bookresult->data.num > 0)
			{
				bookresult->data.num--;
				bookresult->data.readnum++; // 借阅次数+1
				savebookInfoToFile("bookinfo.txt", booklist);
				printf("借阅成功！\n");

				//[索引][表格]
				std::cout << "书名: " << bookresult->data.name
						  << " | 库存: " << bookresult->data.num
						  << " | 借阅次数: " << bookresult->data.readnum
						  << "\n";
			}
			else
			{
				cout << "当前书籍无库存，借阅失败！\n";
			}
		}
		break;
	case 5:
		cout << "【归还书籍】\n";
		cout << "请输入归还的书名：\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.name))
			break;
		if (tempBook.name == "rt")
			break;
		bookresult = searchbookByName(booklist, tempBook.name);
		if (bookresult == NULL)
		{
			cout << "该书不属于图书馆！\n";
		}
		else
		{
			bookresult->data.num++;
			savebookInfoToFile("bookinfo.txt", booklist);
			printf("书籍归还成功！\n");

			//[索引][表格]
			std::cout << "书名: " << bookresult->data.name
					  << " | 库存: " << bookresult->data.num
					  << " | 借阅次数: " << bookresult->data.readnum
					  << "\n";
		}
		break;
	case 6:
		cout << "【排行榜】";
		Page = 6;
		break;
	case 7:
		cout << "【书籍管理】";
		Page = 3;
		break;
	case 8:
		cout << "【账号管理】";
		Page = 4;
		break;
	default:
		cout << "【返回】";
		Page = 2;
		break;
	}
}

// 7.4  Page3Role2

void bookkeyDownRole2book()
{
	int userKey = 0;
	struct bookInfo tempBook; // 产生临时变量存储书籍信息
	if (!readValue(userKey))
		return;
	switch (userKey)
	{
	case 0:
		cout << "【返回】";
		Page = 2;
		break;
	case 1:
		cout << "【登记书籍】";
		cout
			<< "输入书籍的信息(name,price,num,author,publishinghouse,ISBN/ISSN):\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.name))
		{
			cout << "登记已取消\n";
			break;
		}
		if (tempBook.name == "rt")
			break;

		if (!readValue(tempBook.price))
		{
			cout << "登记已取消\n";
			break;
		}
		if (!readValue(tempBook.num))
		{
			cout << "登记已取消\n";
			break;
		}
		if (!readValue(tempBook.author))
		{
			cout << "登记已取消\n";
			break;
		}
		if (!readValue(tempBook.publishinghouse))
		{
			cout << "登记已取消\n";
			break;
		}
		if (!readValue(tempBook.ISBN))
		{
			cout << "登记已取消\n";
			break;
		}

		//查重，同 ISBN 或同名都拒绝登记
		if (searchbookByISBN(booklist, tempBook.ISBN) != NULL)
		{
			cout << "该 ISBN 已存在，登记取消\n";
			break;
		}
		if (searchbookByName(booklist, tempBook.name) != NULL)
		{
			cout << "已存在同名书籍，登记取消\n";
			break;
		}

		tempBook.readnum = 0;									// 初始借阅次数为0
		tempBook.publishtime = getMaxPublishtime(booklist) + 1; // 设置publishtime为当前最大值+1
		insertbookNodeByHead(booklist, tempBook);
		savebookInfoToFile("bookinfo.txt", booklist);
		cout << "登记成功\n";
		break;
	case 2:
		cout << "【删除书籍】\n";
		cout << "请输入删除的书名:\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.name))
			break;
		if (tempBook.name == "rt")
			break;
		deletebookNodeByName(booklist, tempBook.name);
		savebookInfoToFile("bookinfo.txt", booklist);
		break;
	default:
		cout << "【返回】";
		Page = 2;
		break;
	}
}

// 7.5 Page4Role2

void bookkeyDownRole2user()
{
	int userKey = 0;
	struct userInfo tempUser; // 产生临时变量存储账号信息
	struct Nodeuser *userresult = NULL;
	if (!readValue(userKey))
		return;
	switch (userKey)
	{
	case 0:
	{
		cout << "【返回】";
		Page = 2;
		break;
	}
	case 1:
	{
		cout << "【浏览账号】\n";
		mergeSortuserList(userlist);
		printuserList(userlist);
		break;
	}
	case 2:
	{
		cout << "【注册账号】";
		cout << "输入用户的信息(Account,Password,Role):";
		cout << "键入【rt】返回\n";
		if (!readValue(tempUser.Account))
			break;
		if (tempUser.Account == "rt")
			break;
		if (!readValue(tempUser.Password))
			break;
		if (tempUser.Password == "rt")
			break;
		string tempRole;
		if (!readValue(tempRole))
			break;
		if (tempRole == "1")
			tempUser.Role = 1;
		else if (tempRole == "2")
			tempUser.Role = 2;
		else if (tempRole == "rt")
			break;
		else
		{
			cout << "Role输入错误，请输入1(用户)或2(管理员)\n 按任意键返回";
			break;
		}
		userresult = searchuserByName(userlist, tempUser.Account);
		if (userresult == NULL)
		{
			insertuserNodeByHead(userlist, tempUser);
			saveuserInfoToFile("userinfo.txt", userlist);
			cout << "注册成功\n";
		}
		else
		{
			cout << "注册失败，账号已存在!\n";
		}
		break;
	}
	case 3:
	{
		cout << "【删除账号】\n";
		cout << "请输入删除的账号名:\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempUser.Account))
			break;
		if (tempUser.Account == "rt")
			break;
		deleteuserNodeByName(userlist, tempUser.Account);
		saveuserInfoToFile("userinfo.txt", userlist);
		break;
	}
	case 4:
	{
		cout << "【重置密码】\n";
		cout << "请输入需要重置密码的账号\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempUser.Account))
			break;
		if (tempUser.Account == "rt")
			break;
		userresult = searchuserByName(userlist, tempUser.Account);
		if (userresult == NULL)
		{
			cout << "用户不存在\n";
		}
		else
		{
			cout << "当前账号：" << userresult->data.Account << "\n";
			cout << "当前密码：" << userresult->data.Password << "\n";
			cout << "确定要重置密码为123456吗？是请输入1，否请输入0\n";

			string tempA;
			if (!readValue(tempA))
				break;

			if (tempA == "1")
			{
				cout << "重置默认密码：123456\n";
				userresult->data.Password = "123456";
				saveuserInfoToFile("userinfo.txt", userlist);
			}
			else if (tempA == "0")
			{
				cout << "【返回】";
				break;
			}
			else
			{
				cout << "无效的输入，请输入0或1\n【返回】";
			}
		}
		break;
	}
	case 5:
	{
		cout << "【查找账号】";
		cout << "输入查找的账号:";
		cout << "键入【rt】返回\n";
		if (!readValue(tempUser.Account))
			break;
		if (tempUser.Account == "rt")
			break;
		userresult = searchuserByName(userlist, tempUser.Account);
		if (userresult == NULL)
		{
			cout << "该账号不存在\n";
		}
		else
		{
			//[索引][表格]
			std::cout << "账号: " << userresult->data.Account
					  << " | 密码: " << userresult->data.Password
					  << " | 权限: " << userresult->data.Role
					  << "\n";
		}
		break;
	}
	default:
	{
		cout << "【返回】";
		Page = 2;
		break;
	}
	}
}

// 7.6 Page5

void bookkeyDownPage5()
{
	int userKey = 0;
	struct bookInfo tempBook; // 产生临时变量存储书籍信息
	struct Nodebook *bookresult = NULL;
	if (!readValue(userKey))
		return;
	switch (userKey)
	{
	case 0:
	{
		cout << "【返回】";
		Page = 2;
		break;
	}
	case 1:
	{
		cout << "【按书名查找】\n";
		cout << "请输入查找的书名:\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.name))
			break;
		if (tempBook.name == "rt")
			break;
		bookresult = searchbookByName(booklist, tempBook.name);
		if (bookresult == NULL)
		{
			cout << "未找到相关信息\n";
		}
		else
		{
			//[索引][表格]
			std::cout << "书名: " << bookresult->data.name
					  << " | 价格: " << bookresult->data.price
					  << " | 数量: " << bookresult->data.num
					  << " | 作者: " << bookresult->data.author
					  << " | 出版社: " << bookresult->data.publishinghouse
					  << " | ISBN: " << bookresult->data.ISBN
					  << " | 借阅次数: " << bookresult->data.readnum
					  << "\n";
		}
		break;
	}
	case 2:
	{
		cout << "【按ISBN查找】\n";
		cout << "请输入查找的ISBN:\n";
		cout << "键入【rt】返回\n";
		if (!readValue(tempBook.ISBN))
			break;
		if (tempBook.ISBN == "rt")
			break;
		bookresult = searchbookByISBN(booklist, tempBook.ISBN);
		if (bookresult == NULL)
		{
			cout << "未找到相关信息\n";
		}
		else
		{
			//[索引][表格]
			std::cout << "书名: " << bookresult->data.name
					  << " | 价格: " << bookresult->data.price
					  << " | 数量: " << bookresult->data.num
					  << " | 作者: " << bookresult->data.author
					  << " | 出版社: " << bookresult->data.publishinghouse
					  << " | ISBN: " << bookresult->data.ISBN
					  << " | 借阅次数: " << bookresult->data.readnum
					  << "\n";
		}
		break;
	}
	case 3:
	{
		cout << "【按作者查找】\n";
		cout << "请输入作者姓名:\n";
		cout << "键入【rt】返回\n";
		std::string authorName;
		if (!readValue(authorName))
			break;
		if (authorName == "rt")
			break;
		searchAndPrintBooksByAuthor(booklist, authorName);
		break;
	}
	default:
	{
		cout << "【返回】";
		Page = 2;
		break;
	}
	}
}

// 7.7 Page6

void bookkeyDownPage6()
{
	int userKey = 0;
	// struct bookInfo tempBook;//产生临时变量存储书籍信息
	// struct Nodebook* bookresult=NULL;
	if (!readValue(userKey))
		return;

	switch (userKey)
	{
	case 0:
		cout << "【返回】";
		Page = 2;
		break;
	case 1:
		cout << "【借阅次数排行榜】\n";
		displayTop10ByReadNum(booklist);
		break;
	case 2:
		cout << "【最新出版排行榜】\n";
		displayTop10ByPublishTime(booklist);
		break;
	default:
		cout << "【返回】";
		Page = 2;
		break;
	}
}

//------------------------------p8.main函数 ------------------------------

int main()
{
	std::cout << std::fixed << std::setprecision(2);
	userlist = createuserHead();
	booklist = createbookHead();
	readbookInfoFromFile("bookinfo.txt");
	readuserInfoFromFile("userinfo.txt");

	while (1)
	{
		makeuserMenu();
		userkeyDown();
		system("pause");
		system("cls");
		while (Role != 0)
		{
			makebookMenu();
			if (Page == 2 && Role == 1)
				bookkeyDownRole1();
			else if (Page == 2 && Role == 2)
				bookkeyDownRole2();
			else if (Page == 3)
			{
				system("cls");
				makebookMenuPage3();
				bookkeyDownRole2book();
			}
			else if (Page == 4)
			{
				system("cls");
				makebookMenuPage4();
				bookkeyDownRole2user();
			}
			else if (Page == 5)
			{
				system("cls");
				makebookMenuPage5();
				bookkeyDownPage5();
			}
			else if (Page == 6)
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
