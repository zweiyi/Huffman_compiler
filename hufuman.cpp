#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 60
#define M 2*N-1
#define MAXSIZE 10000
#define MAXINT 32767
#define MAXChars 53

/*静态三叉链表，前n个存储叶子结点，后n-1个存储不断生成的新结点*/
typedef struct {
	int Weight;
	char *encode;		//对应的编码 
	char ch;			//对应的字符 
	int Parent, Lchild, Rchild;
}HTNode, HuffmanTree[M+1];

typedef struct {
	char ch;
	int Weight;
}CharAndWeight;

typedef struct {
	CharAndWeight chars[MAXChars];
	int length;
	char coding[MAXSIZE];
}CHars;

CHars Chars;

/*建立哈夫曼树*/
void CrtHuffmanTree(HuffmanTree ht, CHars w, int n);

//选取双亲为0且权值最小的两结点 
void select(HuffmanTree ht, int m, int *s1, int *s2);

//统计文件中字符的次数，计算权值 
CHars StatisticsFromFile(char *filemessage);

//哈夫曼编码
void CrtHuffmanCode(HuffmanTree ht, int n); 

//将字符串编码 
void encodingString(HuffmanTree ht, char *string);

//将字符串写入文件 
void writeToFile(char *coding); 

//译码
void DecodeHuffmanCode(HuffmanTree ht, char *code, int n);

//从文件中读出编码 
char* readCodeFromFile();

//打印
void PrintHuffmanTree(HuffmanTree ht, int n);

//主菜单函数 
void menu(HuffmanTree ht, int flag);

//初始化 
CHars initChars();

//编码
void encoding(HuffmanTree ht);

//译码 
void decode(HuffmanTree ht);

int main(){
	HuffmanTree ht;
	menu(ht, 0);
	return 0;
} 

//建立二叉树 
void CrtHuffmanTree(HuffmanTree ht, CHars w, int n) {
	int m = 2*n-1; //总结点个数
	int i;
	int s1, s2;
	for(i = 1; i <= n; i++) {	//初始化前n个使其成为根节点 
		ht[i].Weight = w.chars[i - 1].Weight;
		ht[i].Parent = 0;
		ht[i].Rchild = 0;
		ht[i].Lchild = 0;
		ht[i].ch = w.chars[i-1].ch;
	} 
	
	for(i = n+1; i <= m; i++) {	//初始化后n-1个元素 
		ht[i].Weight = 0;
		ht[i].Parent = 0;
		ht[i].Rchild = 0;
		ht[i].Lchild = 0;
	}
	
	for(i = n+1; i <= m; i++) {	//构造新结点 
		select(ht, i-1, &s1, &s2);
		ht[i].Weight = ht[s1].Weight +ht[s2].Weight;
		ht[i].Rchild = s2;
		ht[i].Lchild = s1;
		ht[s2].Parent = i;
		ht[s1].Parent = i;
	}
}

//选择双亲为0，且权值最小的两个结点 
void select(HuffmanTree ht, int m, int *s1, int *s2) {
	int i, m1, m2;
	m1 = m2 = MAXINT;
	for(i = 1; i <= m; i++) {
		if(ht[i].Parent == 0) {
			if(ht[i].Weight < m1){
				m2 = m1;
				m1 = ht[i].Weight;
				*s2 = *s1;
				*s1 = i;
			} else if(ht[i].Weight < m2) {
				m2 = ht[i].Weight;
				*s2 = i;
			} else {
				;
			}
		}
	}
}

//根据哈夫曼树将叶子结点进行编码 
void CrtHuffmanCode(HuffmanTree ht, int n) {
	char *cd;		 
	int i, start, c, p;
	int m;
	m = N;
	cd = (char *)malloc(sizeof(char) * N);
	cd[m-1] = '\0';
	for(i = 1; i <= n; i++) {
		start = m-1;
		c = i;
		p = ht[i].Parent;
		while(p!=0) {
			--start;
			if(ht[p].Lchild == c) {
				cd[start] = '0';
			} else {
				cd[start] = '1';
			}
			c = p;
			p = ht[p].Parent;
		}
		ht[i].encode = (char *)malloc(sizeof(char) * (m -start));
		strcpy(ht[i].encode, &cd[start]);
	}
	free(cd);
}

//输出相关信息 
void PrintHuffmanTree(HuffmanTree ht, int n) {
	int i;
	printf("字符\t权值\t双亲\t左孩子\t右孩子\t编码\n");
	for(i = 1; i <= n; i++) {
		printf("%c\t%d\t%d\t%d\t%d\t%s\n", ht[i].ch, ht[i].Weight, ht[i].Parent, ht[i].Lchild, ht[i].Rchild, ht[i].encode);
	}
	system("pause");
	system("cls");
	printf("是否返回上一层: (0/1)\n");
	int choice;
	while((scanf("%d", &choice) )!=1 || choice < 0 || choice > 1) {
		printf("请按照要求输入\n");
		fflush(stdin);
	}
	switch(choice) {
		case 0:
			exit(0);
			break;
		case 1:
			menu(ht, 1);
			break;
	}
}

//将字符串根据哈夫曼树进行译码 
void DecodeHuffmanCode(HuffmanTree ht, char *code, int n) {
	int length;
	length = strlen(code);
	int i = 0, j = 1, k = 0;
	char coding[MAXSIZE];
//	printf("\n%s\n", code);
	while(length) {
		j = 2*n-1;					//从根结点开始， 
		while(ht[j].Lchild != 0 && ht[j].Rchild != 0) {
			if(code[i] == '0') {			//根据每一位编码的0或1确定进入左子树或右子树 
				j = ht[j].Lchild;
				length--;
			} else {
				j = ht[j].Rchild;
				length--;
			}
			i++;
		}
		coding[k] = ht[j].ch;
//		printf("%c", coding[k]);
		k++;
	}
	coding[k]='\0';
	printf("文件译码成功！\n");
	printf("译码为:\n%s\n", coding);
	printf("将译码写入文件\n");
	writeToFile(coding);
}

//统计文件中字符及相应的权值 
CHars StatisticsFromFile(char *filem) {
	char filename[N];
	FILE *fp;
	printf("请输入文件名");
	scanf("%s", filename);
	fp = fopen(filename, "r");
	char filemessage[MAXSIZE]; 
	if(fp == NULL) {
		printf("文件打开失败");
		exit(1); 
	} else {
		while(!feof(fp)) {
			char ch[MAXSIZE];
			fgets(ch, MAXSIZE, fp);
		    strcat(filemessage, ch);
		}
	}
	fclose(fp);
	strcpy(filem, filemessage);
	int length = strlen(filemessage);
	CHars Chars;
	Chars = initChars();
	//统计字符计算权值 
	int flag = 0, j;
	for(int i = 0; i < length; i++) {
		flag = 0;
		for( j = 0; j < Chars.length; j++) {
			if(Chars.chars[j].ch == filemessage[i]) {
				flag = 1;
				Chars.chars[j].Weight++;
			}
		}
		if(flag == 0) {
			Chars.chars[j].ch = filemessage[i];
			Chars.chars[j].Weight++;
			Chars.length++;
		}
	}
	printf("总共有%d个字符\n", Chars.length);
	for(int i = 0; i < Chars.length; i++) {
		printf("%c出现了%d次\n", Chars.chars[i].ch, Chars.chars[i].Weight);
	} 
	return Chars;
}

//主菜单 
void menu(HuffmanTree ht, int flag) {
	system("color B0");
	system("cls");
	printf("\t\t\t*************************欢迎使用哈夫曼编码/译码器*************************\n");
	printf("\t\t\t*                                                                         *\n");
	printf("\t\t\t*           1.根据文件内容进行编码                                        *\n");
	printf("\t\t\t*           2.根据建好的二叉树进行译码                                    *\n");
	printf("\t\t\t*           3.打印相关信息                                                *\n");
	printf("\t\t\t*           4.退出                                                        *\n");
	printf("\t\t\t*                                                                         *\n");
	printf("\t\t\t*                                                                         *\n");
	printf("\t\t\t***************************************************************************\n");
	printf("请选择（1-3）(最多输入三次)\n");
	int choice, n = 3;
	while ((scanf("%d", &choice) != 1 || choice <= 0 || choice > 4) && n){
		printf("请重新输入, 要好好看提示的\n");
		fflush(stdin);
		n--;
	}
	if(!n && (choice <= 0||choice > 4)) {
		printf("输入次数过多！提示看一眼，就不会错了>:<\n");
		fflush(stdin);
		system("pause");
		system("cls");
		menu(ht, 0);
	}
	switch(choice) {
		case 1:
			encoding(ht);
			flag = 1;
			break;
		case 2:
			system("pause");
			system("cls");
			if(flag == 1) {			//已经建立过相应的哈夫曼树 
				decode(ht);
			} else {
				printf("还未建立相应的哈夫曼树，请先进行译码，建立相应的二叉树\n");
				system("pause");
				system("cls");
				menu(ht, 0);
			}
			break;
		case 3:
			system("pause");
			system("cls");
			if(flag == 1) {			//已经建立过相应的哈夫曼树 
				PrintHuffmanTree(ht, Chars.length);
			} else {
				printf("还未建立相应的哈夫曼树，请先进行译码，建立相应的二叉树\n");
				system("pause");
				system("cls");
				menu(ht, 0);
			}
		case 4:
			exit(0);
	}
}

//编码 
void encoding(HuffmanTree ht) {
	char filemessage[MAXSIZE];
	system("pause");
	system("cls");
	Chars = StatisticsFromFile(filemessage);
	system("pause");
	system("cls");
	CrtHuffmanTree(ht, Chars, Chars.length); 		//建立二叉树 
	CrtHuffmanCode(ht, Chars.length);				//根据二叉树进行叶子结点的编码
	encodingString(ht, filemessage);				//根据二叉树将字符进行编码
	printf("编码成功!\n");
	printf("编码为：\n%s\n", Chars.coding);
	printf("将编码写入文件\n");
	writeToFile(Chars.coding);						//将编码写入文件 
	system("pause");
	system("cls");
	printf("是否返回主菜单：（0/1）\n");
	int choice;
	while((scanf("%d", &choice) )!=1 || choice < 0 || choice > 1) {
		printf("请按照要求输入\n");
		fflush(stdin);
	}
	switch(choice) {
		case 0:
			exit(0);
			break;
		case 1:
			menu(ht, 1);
			break;
	}
}

//译码 
void decode(HuffmanTree ht) {
	char *coding;
	coding = readCodeFromFile();
	DecodeHuffmanCode(ht, coding, Chars.length);
	system("pause");
	system("cls");
	printf("是否返回主菜单: (0/1)\n");
	int choice;
	while((scanf("%d", &choice) )!=1 || choice < 0 || choice > 1) {
		printf("请按照要求输入\n");
		fflush(stdin);
	}
	switch(choice) {
		case 0:
			exit(0);
			break;
		case 1:
			menu(ht, 1);
			break;
	}
}

//初始化字符及权值 
CHars initChars() {
	CHars chars;
	chars.length = 0;
	for(int i = 0; i < MAXChars; i++) {
		chars.chars[i].Weight = 0;
		chars.chars[i].ch = NULL;
	}
	return chars;
}

//将字符串进行编码 
void encodingString(HuffmanTree ht, char *string) {
	int length = strlen(string);
	for(int i = 0; i < length; i++) {
		for(int j = 1; j <= Chars.length; j++) {
			if(string[i] == ht[j].ch) {
				char ch[MAXSIZE];
				strcpy(ch, ht[j].encode);
				strcat(Chars.coding,ch);
				break;
			}
		}
	}
}

//将字符写入文件 
void writeToFile(char *coding) {
	FILE *fp;
	char filename[MAXSIZE];
//	printf("coding%s\n", coding);
	printf("请输入文件名 :");
	scanf("%s", filename);
	fp = fopen(filename, "w+");
	if(fp == NULL) {
		printf("文件打开失败");
		exit(1);
	} else {
		fprintf(fp, "%s", coding);
	}
	fclose(fp);
	printf("写入文件成功！");
}

//从文件中读出编码 
char* readCodeFromFile() {
	FILE *fp;
	char *coding;
	coding = (char*)malloc(sizeof(char)*MAXSIZE);
	fp = fopen("code.txt", "r");
	if(fp == NULL) {
		printf("文件读入失败");
		exit(1);
	} else {
		fgets(coding, MAXSIZE, fp);
	}
	fclose(fp);
	return coding;
}
