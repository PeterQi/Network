#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 100
#define MAXSTACK 2048
#define MAXSTR 100

char *mystr[MAXSTR] = {0};
char mystack[MAXSTACK] = {0};
int cnt = 0;
int c_str = 0;
int len = 0;

void insert_mystr(char *str)
{
	int i;
	int flag = 0;
	int length = strlen(str);
	for(i=0;i<c_str;i++)
	{
		if(!strcmp(mystr[i],str))
		{
			flag = 1;
			break;
		}
	}
	if(!flag)
	{
		mystr[c_str] = (char *)malloc(length+1);
		strcpy(mystr[c_str],str);
		mystr[c_str][length] = 0;
		c_str++;
	}
}

void init_stack()
{
	memset(mystack,0,MAXSTACK);
	cnt = len;
}

char push(char c)
{
	if(cnt == 0)
		return 0;
	cnt--;
	mystack[cnt] = c;
	return c;
}

char pop()
{
	char tmp;
	if(cnt == len)
		return 0;
	tmp = mystack[cnt];
	mystack[cnt] = 0;
	cnt++;
	return tmp;
}

void clear()
{
	memset(mystack, 0, MAXSTACK);
	cnt = 0;
}

void prt_stack()
{
	printf("%s\n",&mystack[cnt]);
}

void LCSLength(char *x, char *y, int m, int n, int c[][MAXLEN], int b[][MAXLEN])
{
	int i, j;

	for(i = 0; i <= m; i++)
		c[i][0] = 0;
	for(j = 1; j <= n; j++)
		c[0][j] = 0;
	for(i = 1; i<= m; i++)
	{
		for(j = 1; j <= n; j++)
		{
			if(x[i-1] == y[j-1])
			{
				c[i][j] = c[i-1][j-1] + 1;
				b[i][j] = 0;
			}
			else if(c[i-1][j] > c[i][j-1])
			{
				c[i][j] = c[i-1][j];
				b[i][j] = 1;
			}
			else if(c[i-1][j] < c[i][j-1])
			{
				c[i][j] = c[i][j-1];
				b[i][j] = -1;
			}
			else
			{
				c[i][j] = c[i][j-1];
				b[i][j] = 2;
			}
		}
	}
}

void PrintLCS(int b[][MAXLEN],int c[][MAXLEN], char *x,char *y, int i, int j,int dep,int str)
{
	if(i == 0 || j == 0)
	{
		if(cnt == 0)
			insert_mystr(mystack);
		return;
	}
	if(b[i][j] == 0)
	{
		push(x[i-1]);
		PrintLCS(b,c, x,y, i-1, j-1,dep+1,str);
		pop();
	}
	else if(b[i][j] == 2)
	{
		PrintLCS(b,c, x,y, i, j-1,dep,str);
		PrintLCS(b,c, x,y, i-1, j,dep,str+1);
	}
	else if(b[i][j] == 1)
		PrintLCS(b,c, x,y, i-1, j,dep,str);
	else
		PrintLCS(b,c, x,y, i, j-1,dep,str);
}




int main()
{
	char x[MAXLEN];
	char y[MAXLEN];
    printf("First sequence:");
    scanf("%s", x);
    printf("Second sequence:");
    scanf("%s", y);
	int b[MAXLEN][MAXLEN];
	int c[MAXLEN][MAXLEN];
	int m, n;
	int i;

	m = strlen(x);
	n = strlen(y);


	LCSLength(x, y, m, n, c, b);
	len = c[m][n];
	init_stack();

	PrintLCS(b,c,x,y,m,n,0,0);

	for(i=0 ;i<c_str;i++)
	{
		printf("%s\n",mystr[i]);
	}


	return 0;
}
