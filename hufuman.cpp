#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 60
#define M 2*N-1
#define MAXSIZE 10000
#define MAXINT 32767
#define MAXChars 53

/*��̬��������ǰn���洢Ҷ�ӽ�㣬��n-1���洢�������ɵ��½��*/
typedef struct {
	int Weight;
	char *encode;		//��Ӧ�ı��� 
	char ch;			//��Ӧ���ַ� 
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

/*������������*/
void CrtHuffmanTree(HuffmanTree ht, CHars w, int n);

//ѡȡ˫��Ϊ0��Ȩֵ��С������� 
void select(HuffmanTree ht, int m, int *s1, int *s2);

//ͳ���ļ����ַ��Ĵ���������Ȩֵ 
CHars StatisticsFromFile(char *filemessage);

//����������
void CrtHuffmanCode(HuffmanTree ht, int n); 

//���ַ������� 
void encodingString(HuffmanTree ht, char *string);

//���ַ���д���ļ� 
void writeToFile(char *coding); 

//����
void DecodeHuffmanCode(HuffmanTree ht, char *code, int n);

//���ļ��ж������� 
char* readCodeFromFile();

//��ӡ
void PrintHuffmanTree(HuffmanTree ht, int n);

//���˵����� 
void menu(HuffmanTree ht, int flag);

//��ʼ�� 
CHars initChars();

//����
void encoding(HuffmanTree ht);

//���� 
void decode(HuffmanTree ht);

int main(){
	HuffmanTree ht;
	menu(ht, 0);
	return 0;
} 

//���������� 
void CrtHuffmanTree(HuffmanTree ht, CHars w, int n) {
	int m = 2*n-1; //�ܽ�����
	int i;
	int s1, s2;
	for(i = 1; i <= n; i++) {	//��ʼ��ǰn��ʹ���Ϊ���ڵ� 
		ht[i].Weight = w.chars[i - 1].Weight;
		ht[i].Parent = 0;
		ht[i].Rchild = 0;
		ht[i].Lchild = 0;
		ht[i].ch = w.chars[i-1].ch;
	} 
	
	for(i = n+1; i <= m; i++) {	//��ʼ����n-1��Ԫ�� 
		ht[i].Weight = 0;
		ht[i].Parent = 0;
		ht[i].Rchild = 0;
		ht[i].Lchild = 0;
	}
	
	for(i = n+1; i <= m; i++) {	//�����½�� 
		select(ht, i-1, &s1, &s2);
		ht[i].Weight = ht[s1].Weight +ht[s2].Weight;
		ht[i].Rchild = s2;
		ht[i].Lchild = s1;
		ht[s2].Parent = i;
		ht[s1].Parent = i;
	}
}

//ѡ��˫��Ϊ0����Ȩֵ��С��������� 
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

//���ݹ���������Ҷ�ӽ����б��� 
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

//��������Ϣ 
void PrintHuffmanTree(HuffmanTree ht, int n) {
	int i;
	printf("�ַ�\tȨֵ\t˫��\t����\t�Һ���\t����\n");
	for(i = 1; i <= n; i++) {
		printf("%c\t%d\t%d\t%d\t%d\t%s\n", ht[i].ch, ht[i].Weight, ht[i].Parent, ht[i].Lchild, ht[i].Rchild, ht[i].encode);
	}
	system("pause");
	system("cls");
	printf("�Ƿ񷵻���һ��: (0/1)\n");
	int choice;
	while((scanf("%d", &choice) )!=1 || choice < 0 || choice > 1) {
		printf("�밴��Ҫ������\n");
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

//���ַ������ݹ��������������� 
void DecodeHuffmanCode(HuffmanTree ht, char *code, int n) {
	int length;
	length = strlen(code);
	int i = 0, j = 1, k = 0;
	char coding[MAXSIZE];
//	printf("\n%s\n", code);
	while(length) {
		j = 2*n-1;					//�Ӹ���㿪ʼ�� 
		while(ht[j].Lchild != 0 && ht[j].Rchild != 0) {
			if(code[i] == '0') {			//����ÿһλ�����0��1ȷ�������������������� 
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
	printf("�ļ�����ɹ���\n");
	printf("����Ϊ:\n%s\n", coding);
	printf("������д���ļ�\n");
	writeToFile(coding);
}

//ͳ���ļ����ַ�����Ӧ��Ȩֵ 
CHars StatisticsFromFile(char *filem) {
	char filename[N];
	FILE *fp;
	printf("�������ļ���");
	scanf("%s", filename);
	fp = fopen(filename, "r");
	char filemessage[MAXSIZE]; 
	if(fp == NULL) {
		printf("�ļ���ʧ��");
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
	//ͳ���ַ�����Ȩֵ 
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
	printf("�ܹ���%d���ַ�\n", Chars.length);
	for(int i = 0; i < Chars.length; i++) {
		printf("%c������%d��\n", Chars.chars[i].ch, Chars.chars[i].Weight);
	} 
	return Chars;
}

//���˵� 
void menu(HuffmanTree ht, int flag) {
	system("color B0");
	system("cls");
	printf("\t\t\t*************************��ӭʹ�ù���������/������*************************\n");
	printf("\t\t\t*                                                                         *\n");
	printf("\t\t\t*           1.�����ļ����ݽ��б���                                        *\n");
	printf("\t\t\t*           2.���ݽ��õĶ�������������                                    *\n");
	printf("\t\t\t*           3.��ӡ�����Ϣ                                                *\n");
	printf("\t\t\t*           4.�˳�                                                        *\n");
	printf("\t\t\t*                                                                         *\n");
	printf("\t\t\t*                                                                         *\n");
	printf("\t\t\t***************************************************************************\n");
	printf("��ѡ��1-3��(�����������)\n");
	int choice, n = 3;
	while ((scanf("%d", &choice) != 1 || choice <= 0 || choice > 4) && n){
		printf("����������, Ҫ�úÿ���ʾ��\n");
		fflush(stdin);
		n--;
	}
	if(!n && (choice <= 0||choice > 4)) {
		printf("����������࣡��ʾ��һ�ۣ��Ͳ������>:<\n");
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
			if(flag == 1) {			//�Ѿ���������Ӧ�Ĺ������� 
				decode(ht);
			} else {
				printf("��δ������Ӧ�Ĺ������������Ƚ������룬������Ӧ�Ķ�����\n");
				system("pause");
				system("cls");
				menu(ht, 0);
			}
			break;
		case 3:
			system("pause");
			system("cls");
			if(flag == 1) {			//�Ѿ���������Ӧ�Ĺ������� 
				PrintHuffmanTree(ht, Chars.length);
			} else {
				printf("��δ������Ӧ�Ĺ������������Ƚ������룬������Ӧ�Ķ�����\n");
				system("pause");
				system("cls");
				menu(ht, 0);
			}
		case 4:
			exit(0);
	}
}

//���� 
void encoding(HuffmanTree ht) {
	char filemessage[MAXSIZE];
	system("pause");
	system("cls");
	Chars = StatisticsFromFile(filemessage);
	system("pause");
	system("cls");
	CrtHuffmanTree(ht, Chars, Chars.length); 		//���������� 
	CrtHuffmanCode(ht, Chars.length);				//���ݶ���������Ҷ�ӽ��ı���
	encodingString(ht, filemessage);				//���ݶ��������ַ����б���
	printf("����ɹ�!\n");
	printf("����Ϊ��\n%s\n", Chars.coding);
	printf("������д���ļ�\n");
	writeToFile(Chars.coding);						//������д���ļ� 
	system("pause");
	system("cls");
	printf("�Ƿ񷵻����˵�����0/1��\n");
	int choice;
	while((scanf("%d", &choice) )!=1 || choice < 0 || choice > 1) {
		printf("�밴��Ҫ������\n");
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

//���� 
void decode(HuffmanTree ht) {
	char *coding;
	coding = readCodeFromFile();
	DecodeHuffmanCode(ht, coding, Chars.length);
	system("pause");
	system("cls");
	printf("�Ƿ񷵻����˵�: (0/1)\n");
	int choice;
	while((scanf("%d", &choice) )!=1 || choice < 0 || choice > 1) {
		printf("�밴��Ҫ������\n");
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

//��ʼ���ַ���Ȩֵ 
CHars initChars() {
	CHars chars;
	chars.length = 0;
	for(int i = 0; i < MAXChars; i++) {
		chars.chars[i].Weight = 0;
		chars.chars[i].ch = NULL;
	}
	return chars;
}

//���ַ������б��� 
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

//���ַ�д���ļ� 
void writeToFile(char *coding) {
	FILE *fp;
	char filename[MAXSIZE];
//	printf("coding%s\n", coding);
	printf("�������ļ��� :");
	scanf("%s", filename);
	fp = fopen(filename, "w+");
	if(fp == NULL) {
		printf("�ļ���ʧ��");
		exit(1);
	} else {
		fprintf(fp, "%s", coding);
	}
	fclose(fp);
	printf("д���ļ��ɹ���");
}

//���ļ��ж������� 
char* readCodeFromFile() {
	FILE *fp;
	char *coding;
	coding = (char*)malloc(sizeof(char)*MAXSIZE);
	fp = fopen("code.txt", "r");
	if(fp == NULL) {
		printf("�ļ�����ʧ��");
		exit(1);
	} else {
		fgets(coding, MAXSIZE, fp);
	}
	fclose(fp);
	return coding;
}
