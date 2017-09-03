// wing_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Locale.h>
#include "../../win32/WingString.class.h"
#include "../../win32/WingString.class.cpp"
#define WIN32 1

class Test {
private:
	char str[1024];
public:
	Test() {
		memset(this->str,0,1024);
		strcpy(this->str, "hello word");
	}

	void print() {
		printf("%s\r\n", this->str);
	}
};
int main()
{
	char str[] = " 123   ";
	wing_str_trim(str);
	printf(">%s<", str);

	Test test;
	test.print();

	WingString wstr1("1");
	WingString wstr2("2");

	if (wstr1 < L"2") {
		printf("bu deng\r\n");
	}
	else {
		printf("deng\r\n");
	}

	//WingString �Ĳ�������
	setlocale(LC_ALL, "chs");
	char trim_str1[] = " 123 ";
	wing_str_trim(trim_str1);
	printf("==>%s<==\r\n", trim_str1);

	char* wing_str1 =  wing_str_wchar_to_char(L"����й�");
	printf("1==>%s<==\r\n", wing_str1);
	if (wing_str1) free(wing_str1);
	
	wchar_t* wing_str2 = wing_str_char_to_wchar("����й�");
	wprintf(L"2==>%ls<==\r\n" ,wing_str2);
	if (wing_str2) free(wing_str2);

	char* wing_str3 = wing_str_char_to_utf8("����й�");
	printf("3==>%s<==\r\n", wing_str3);
	if (wing_str3) free(wing_str3);

	char* wing_str4 = wing_str_wchar_to_utf8(L"����й�");
	printf("4==>%s<==\r\n", wing_str4);
	if (wing_str4) free(wing_str4);
    return 0;
}

