#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024



typedef struct node
{
	char character;
	char success;
	struct node *parent;
	struct node *child;
	struct node *next;
}*p_node;

p_node status[MAX_SIZE];
char ac_base[MAX_SIZE];
char ac_next[MAX_SIZE];
char ac_check[MAX_SIZE];
int status_count;

p_node node_init()
{
	p_node root = (p_node)malloc(sizeof(*root));
	memset(root, 0, sizeof(*root));
	return root;
}

p_node node_get_max_dep(p_node root, char *str, int *dep)
{
	p_node p = root;
	p_node c = NULL;
	int flag = 0;
	int len = strlen(str);
	int index = 0;
	while (p)
	{
		if (index == len)
			break;
		c = p->child;
		while (c)
		{
			if (c->character == str[index])
			{
				p = c;
				index++;
				flag = 1;
				break;
			}
			c = c->next;
		}
		if (!flag)
			break;
		flag = 0;
	}
	*dep = index;
	return p;

}

int node_make(p_node p, char*str)
{
	int len= strlen(str);
	int i=0;
	p_node cur, pre;
	if (len ==0)
	{
		p->success=1;
		return 1;
	}
	p_node first= node_init();
	first->character= str[i];
	first->parent= p;

	if(!p->child)
	{
		p->child = first;
	}
	else
	{
		pre = p->child;
		while (pre->next)
		{
			pre = pre->next;
		}
		pre->next = first;
	}

	cur = first;
	for (i = 1; i <len; i++)
	{
		pre = cur;
		cur = node_init();
		cur->character = str[i];
		cur->parent = pre;
		pre->child = cur;
	}
	cur->success=1;
	return 1;
}

int insert_str(p_node root, char *str)
{
	int str_len = strlen(str);
	int max_dep = 0;
	p_node parent = node_get_max_dep(root, str, &max_dep);
	node_make(parent, &str[max_dep]);
	return 1;
}

int node_get_child_count(p_node node)
{
	int i = 0;
	if (!node)
		return 0;
	node = node->child;
	while (node)
	{
		i++;
		node = node->next;
	}
	return i;
}

int node_get_count(p_node root, int *cnt)
{
	int c_cnt = node_get_child_count(root);
	if (c_cnt == 0)
	{
		return 0;
	}
	*cnt += c_cnt;
	node_get_count(root->child, cnt);
	node_get_count(root->next, cnt);
	return 0;
}

int node_init_status(p_node root, p_node *stat, int *cnt)
{
	int base = 0;
	int cur = 1;
	int tmp = 0;
	int max = 0;
	node_get_count(root, &max);
	*cnt = max;
	int i;
	p_node p;
	stat[0]= root;

	while(1)
	{
		tmp= cur;
		if (tmp> max)
			break;
		for (i= base; i< tmp; i++)
		{
			p = stat[i]->child;
			while (p)
			{
				stat[cur] = p;
				cur++;
				p=p->next;
			}
		}
		base= tmp;
	}

}

char node_get_min_child_c(p_node root)
{
	p_node p= root->child;
	char out= 127;
	while (p)
	{
		if (p->character< out)
			out=p->character;
		p=p->next;
	}
	return out;
}

int ac_get_status_index(p_node node)
{
	int i=0;
	for (i=0; i< MAX_SIZE; i++)
	{
		if (status[i] == node)
			return i;
	}
	return 0;
}

int find_loc(p_node node, int base)
{
	int j;
	p_node p = node->child;

	for (j = 1; j < MAX_SIZE; j++)
	{
BAD:
		if (ac_next[j] == 0)
		{
			p = node->child;
			while (p)
			{
				if (ac_next[p->character - base + j] != 0)
				{
					j++;
					goto BAD;
				}
				p = p->next;
			}
			return j;
		}
	}
	return 0;
}

int ac_create()
{
	int base = 0;
	int cur = 1;
	int tmp = 0;
	char c_tmp = 0;
	int i,j;

	p_node p;
	p_node pc;

	while (1)
	{
		if (tmp == cur)
			break;
		tmp = cur;
		if (tmp > status_count)
			break;

		for (i = base; i < tmp; i++)
		{
			p = status[i];
			c_tmp = node_get_min_child_c(p);
			if (c_tmp == 0 || c_tmp == 127)
				continue;

			j = find_loc(p, c_tmp);

			ac_base[i] = j - c_tmp;
			pc = p->child;
			while (pc)
			{
				cur++;
				ac_next[(char)(ac_base[i] + pc->character)] = ac_get_status_index(pc);
				ac_check[ac_get_status_index(pc)] = i;
				pc = pc->next;
			}
		}
		base = tmp;
	}
}


int print_str(p_node c, int index)
{
	char stack[MAX_SIZE] = { 0 };
	int i = 0;
	int loc = 0;
	while (c->parent)
	{
		stack[i] = c->character;
		c = c->parent;
		i++;
	}
	loc = index - i;
	printf("%d: ", loc);
	while (i)
	{
		printf("%c", stack[i-1]);
		i--;
	}
	printf("\n");
	return loc;
}

int run(char *str)
{
	char stat = 0;
	int next_stat = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i < strlen(str); i++)
	{
		for (j = i;; j++)
		{
			next_stat = ac_next[ac_base[stat] + str[j]];
			if (next_stat && ac_check[next_stat] == stat)
			{
				stat = next_stat;
				if (status[stat]->success)
				{
					print_str(status[stat], j + 1);
				}
			}
			else
			{
				stat = 0;
				break;
			}
		}

	}
	return 0;
}


int main(int argc, char *argv[])
{
	p_node root = node_init();
	int i = 0;

	printf("Please input pattern number:");
	int num = 0;
	scanf("%d",&num);
	char tmp_str[MAX_SIZE];
	printf("Please input patterns:\n");
	while(num)
    {
        scanf("%s", tmp_str);
        insert_str(root, tmp_str);
        num --;
    }


	node_get_count(root, &i);
	node_init_status(root, status, &status_count);
	ac_create();
	printf("base:");
	for (i = 0; i < status_count; i++)
	{
        printf("%d ", ac_base[i]);
	}
	printf("\nnext:");
	for (i = 0; i < status_count; i++)
	{
        printf("%d ", ac_next[i]);
	}
	printf("\ncheck:");
	for (i = 0; i < status_count; i++)
	{
        printf("%d ", ac_check[i]);
	}
	printf("\nPlease input txt:");
	scanf("%s", tmp_str);
	run(tmp_str);
	return 0;
}
