#include "lex.h"


int digit(char c)
{
	return '0' <= c && c <= '9';
}
int lowerCase(char c)
{
	return 'a' <= c && c <= 'z'; 
}
int upperCase(char c)
{
	return 'A' <= c && c <= 'Z';
}
int alpha(char c)
{
	return lowerCase(c) || upperCase(c);
}
int eos(Source * s)
{
	return s->cur >= s->len;
}
int cur(Source * s)
{
	return s->code[s->cur];
}

void deleteSource(Source ** s)
{
	if (s) {
		Source * _s = *s;
		if (_s && _s->len) {
			free(_s->code);
		}
		free(_s->stringPool.pool);
		free(_s);
		*s = NULL;
	}
}


Source * newSource(unsigned int size)
{
	Source * s = (Source*)new(sizeof(Source));
	if (!s) return NULL;
	s->len = 0;
	s->code = new(size);
	if (!s->code) {
		deleteSource(&s);
		return NULL;
	}
	s->len = size;
	s->cur = 0;

	const int strPoolSize = 4096;
	s->stringPool.size = strPoolSize;
	s->stringPool.use = 0;
	s->stringPool.pool = new(strPoolSize);

	s->look.tag = NoLook;
	return s;
}

Source * sourceFromFile(const char * filePath)
{
	if (!filePath) return NULL;

	FILE * f = fopen(filePath, "r");
	if (!f) {
		printf("file not exists\n");
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	unsigned int fsize = ftell(f);
	if (!fsize) return NULL;

	rewind(f);
	Source * source = newSource(fsize);
	if (!source) {
		fclose(f);
		return NULL;
	}
	if (!fread(source->code, fsize, 1, f)) {
		fclose(f);
		deleteSource(&source);
		return NULL;
	}

	return source;
}

Source * sourceFromStr(const char * code)
{
	if (!code) return NULL;
	
	int len = strlen(code);
	if (!len) return NULL;

	Source * source = newSource(len);
	if (!source) return NULL;

	strcpy(source->code, code);
	return source;
}

Token parseNum(Source * s)
{
	char * c = s->code;
	int cur = s->cur;
	//-?\w+.?\w+ 
	Token t;
	int use = t.attr = s->stringPool.use;
	t.tag = ConstInt; //默认int
	// -?
	if (c[cur] == '-') {
		s->stringPool.pool[s->stringPool.use++] = c[cur++];
	}
	//\w+
	while (!eos(s) && digit(c[cur])) {
		s->stringPool.pool[s->stringPool.use++] = c[cur++];		
	}
	//.?
	if (c[cur] == '.') {	
		t.tag = ConstDouble;
		s->stringPool.pool[s->stringPool.use++] = c[cur++];
	}else {
		s->cur = cur;
		s->stringPool.pool[s->stringPool.use++] = 0; 
		return t;
	}

	//\w+
	while (!eos(s) && digit(c[cur])) {
		s->stringPool.pool[s->stringPool.use++] = c[cur++];		
	}
	if (c[cur] == 'f' || c[cur] == 'F') {
		t.tag = ConstFloat;
		s->stringPool.pool[s->stringPool.use++] = c[cur++];
	}else if (c[cur] == 'l' || c[cur] == 'L') {
		t.tag = ConstLong;
		s->stringPool.pool[s->stringPool.use++] = c[cur++];
	}
	s->cur = cur;
	s->stringPool.pool[s->stringPool.use++] = 0; 
	return t;
}


int ** getTable()
{
	static int table[200][26] = {0};
	static int init = 0;
	if (init) return table;
	init = 1;
	
	int s = 1;
	for (int i = 0 ; i < sizeof(keyMap)/sizeof(MapEntry); i++) {
		//printf("%s : %d\n",keyMap[i].text, keyMap[i].tag);
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
				//printf("table[%d]['%c'] = %d\n", pre, c+'a', table[pre][c]);
				pre = s;
			}
		}
	}
	fflush(stdout);
	return table;
}
Token parseIdOrKeyword(Source * s)
{
	int (* table)[26] = getTable();

	int state = 1;
	Token t;
	t.tag = Id;
	t.attr = s->stringPool.use;
	while (!eos(s)) {

		char c = s->code[s->cur];
		if (!digit(c) && !alpha(c) && c!='_') {
			break;
		}
		s->stringPool.pool[s->stringPool.use++] = c;
		if (state != Id && lowerCase(c))  {
			if (state >= Void) {
				state = Id;
				//printf("%d -%c-> Id\n",state,c);
			}else {
				int next = table[state][c-'a'];
				if (next == 0) next = Id;
				//printf("%d -%c-> %d\n",state,c,next);
				state = next;
			}
		}else {
			state = Id;
		}
		s->cur ++;
	}
	if (state < Void)
		state = Id;
	t.tag = state;
	s->stringPool.pool[s->stringPool.use++] = 0; 
	return t;
}

Token next(Source * s)
{
	Token t = {END, 0};
	
	//检查有没有向前看了
	if (s->look.tag != NoLook) {
		t = s->look;
		s->look.tag = NoLook;
		return t;
	}
	
	if (eos(s)) return t;
	char * code = s->code;

	//跳过空格和空白行 
	// (' ' | '\t' | '\n' )
	while (!eos(s) && 
			((code[s->cur] == ' ') 
			 || (code[s->cur] == '\n')
			 || (code[s->cur] == '\t')
			 )
			) s->cur++;
	// ^#*'\n'
	if (code[s->cur] == '#' ) {
		while (!eos(s) && code[s->cur] != '\n') s->cur ++;
		s->cur ++;
		return next(s);
	}
	// /* */
	if (code[s->cur] == '/') {
		if (code[s->cur+1] == '*') {
			s->cur += 2;
			while ( !eos(s) && !(code[s->cur] == '*' && code[s->cur+1] == '/')) s->cur++;
			s->cur += 2;
			return next(s);
		}else if (code[s->cur+1] == '/') {
			s->cur += 2;
			while (!eos(s) && code[s->cur] != '\n') s->cur ++;
			s->cur ++;
		}
	}

	if (eos(s)) return t;
	t.tag = code[s->cur];
	if (code[s->cur] == '-' && digit(code[s->cur+1])) {
			return parseNum(s);
	}	
	//标识符 或者关键字 
	else if (alpha(code[s->cur]) || code[s->cur] == '_') {
		return parseIdOrKeyword(s);
	}

	//数字常量
	else if (digit(code[s->cur])) {
		return parseNum(s);
	}
	//字符串常量
	else if (code[s->cur] == '\"') {
		s->cur ++;
		t.tag = ConstStr;
		t.attr = s->stringPool.use;
		while (code[s->cur] != '\"') {
			char c = code[s->cur];
			if (code[s->cur] == '\\') {
				s->cur ++;
				switch(code[s->cur]) {
					case 't':
						c = '\t';
						break;
					case 'n':
						c = '\n';
						break;
					case 'b':
						c = '\b';
						break;
					case '\"':
						c = '\"';
						break;
					case '\\':
						c = '\\';
						break;
					default:
						printf("不支持的转义字符\\%c\n",c);
				}
			}
			s->stringPool.pool[s->stringPool.use++] = c;
			s->cur ++;
		}
		s->cur ++;
	}
	
	#define OP3(A,B,C,O) \
	else if (code[s->cur] == (A) && code[s->cur+1] == (B) && code[s->cur+2] == (C)) { \
		t.tag = O; \
		s->cur += 2; \
	}
	
	#define OP2(F,S,O) \
	else if (code[s->cur] == F && code[s->cur+1] == S) { \
		s->cur ++; \
		t.tag = O; \
	}
	#define OP(X) \
	else if (code[s->cur] == (X) ) {t.tag = (X);}
	

	OP3('<','<','=',LsEq)
	OP3('>','>','=',RsEq)
	//表示F后面如果跟着S,那么它是O,否则它是F
	OP2('-', '>', Pto)
	OP2('-', '-', SSub)
	OP2('+', '+', SSub)
	OP2('<', '<', Lsft)
	OP2('>', '>', Rsft)

	OP2('+', '=', Aeq)
	OP2('-', '=', Seq)
	OP2('*', '=', Teq)
	OP2('/', '=', Deq)
	OP2('%', '=', Meq)

	OP2('&', '=', AndEq)
	OP2('|', '=', OrEq)
	OP2('^', '=', XorEq)
	
	OP2('=', '=', Eq)
	OP2('!', '=', Neq)
	OP2('<', '=', Leq)
	OP2('>', '=', Geq)
	OP2('&', '&', And)
	OP2('|', '|', Or)	


	OP('[')
	OP(']')
	OP('(')
	OP(')')
	OP('{')
	OP('}')
	OP('.')
	OP(';')
	OP(',')
	OP('+')
	OP('-')
	OP('*')
	OP('%')
	OP('^')
	OP('?')
	OP(':')
	OP('>')
	OP('<')
	OP('^')
	OP('&')
	OP('|')
	OP('!')
	OP('=')

	#undef OP3
	#undef OP2
	#undef OP
	
	else if (code[s->cur] == EOF) {

	}else {
		printf("unexpected character %c\n",code[s->cur]);
		s->cur ++;
		return next(s);
	}
	s->cur ++;
	return t;
}


