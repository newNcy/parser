#include <stdio.h>
#include <string.h>
struct MapEntry
{
	char text[32];
	long tag;
};

typedef struct MapEntry MapEntry;

enum {
	Void = 256, Char, Short, Int, Float, Long, Double, Struct, Enum,
	If, Else, While, For, Break, Continue, Case, Default,Return,

	SPlus,SSub
};
MapEntry keyMap [] =
{
	{"void",	Void},
	{"char",	Char},
	{"short",	Short},
	{"int",		Int},
	{"float",	Float},
	{"long",	Long},
	{"double",	Double},
	{"struct",	Struct},
	{"enum",	Enum}
};



int main ()
{
	//小写字母
	short table[100][26] = {0};
	int s = 1;
	for (int i = 0 ; i < sizeof(keyMap)/sizeof(MapEntry); i++) {
		printf("%s : %d\n",keyMap[i].text, keyMap[i].tag);
		char * text = keyMap[i].text;
		int len = strlen(text);
		int pre = 1;
		for (int j = 0 ; j < len ; j++) {
			char c = text[j] - 'a';
			if (table[pre][c] != 0) {
				pre = table[pre][c];
				continue;
			}else {
				if (j == len-1) {
					table[pre][c] = keyMap[i].tag;
				}else {
					table[pre][c] = ++s;
				}
				printf("table[%d]['%c'] = %d\n", pre, c+'a', table[pre][c]);
				pre = s;
			}
		}
	}
}
