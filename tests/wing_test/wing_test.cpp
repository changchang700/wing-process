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

	WingString str1;
	WingString str2("123�й�");
	WingString str3(L"456�й�");

	printf("str1 size:%zu,length:%d,data:%s\r\n", str1.size(), str1.length(), str1.data());
	printf("str2 size:%zu,length:%d,data:%s\r\n", str2.size(), str2.length(), str2.data());
	wprintf(L"str3 size:%zu,length:%d,data:%ls\r\n", str3.size(), str3.length(), str3.data());

	WingString str4("123�й�", sizeof("123�й�"));
	WingString str5(L"456�й�", sizeof(L"123�й�"));

	//printf("str1 size:%zu,length:%d\r\n", str1.size(), str1.length());
	printf("str4 size:%zu,length:%d,data:%s\r\n", str4.size(), str4.length(), str4.data());
	wprintf(L"str5 size:%zu,length:%d,data:%ls\r\n", str5.size(), str5.length(), str5.data());

	//copy
	void* cstr1 = str1.copy();
	void* cstr2 = str2.copy();
	void* cstr3 = str3.copy();
	void* cstr4 = str4.copy();
	void* cstr5 = str5.copy();
	printf("str1:%s\r\n", cstr1);
	printf("str2:%s\r\n", cstr2);
	wprintf(L"str3:%ls\r\n", cstr3);
	printf("str4:%s\r\n", cstr4);
	wprintf(L"str5:%s\r\n", cstr5);

	if (cstr1) free(cstr1);
	if (cstr2) free(cstr2);
	if (cstr3) free(cstr3);
	if (cstr4) free(cstr4);
	if (cstr5) free(cstr5);


	 cstr1 = str1.c_str();
	 cstr2 = str2.c_str();
	 cstr3 = str3.c_str();
	 cstr4 = str4.c_str();
	 cstr5 = str5.c_str();
	
	printf("c_str str1:%s\r\n", cstr1);
	printf("c_str str2:%s\r\n", cstr2);
	printf("c_str str3:%s\r\n", cstr3);
	printf("c_str str4:%s\r\n", cstr4);
	printf("c_str str5:%s\r\n", cstr5);

	if (cstr1) free(cstr1);
	if (cstr2) free(cstr2);
	if (cstr3) free(cstr3);
	if (cstr4) free(cstr4);
	if (cstr5) free(cstr5);


	cstr1 = str1.w_str();
	cstr2 = str2.w_str();
	cstr3 = str3.w_str();
	cstr4 = str4.w_str();
	cstr5 = str5.w_str();

	wprintf(L"w_str str1:%ls\r\n", cstr1);
	wprintf(L"w_str str2:%ls\r\n", cstr2);
	wprintf(L"w_str str3:%ls\r\n", cstr3);
	wprintf(L"w_str str4:%ls\r\n", cstr4);
	wprintf(L"w_str str5:%ls\r\n", cstr5);

	if (cstr1) free(cstr1);
	if (cstr2) free(cstr2);
	if (cstr3) free(cstr3);
	if (cstr4) free(cstr4);
	if (cstr5) free(cstr5);

    return 0;
}

