// wing_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../win32/WingString.class.h"
#include "../../win32/WingString.class.c"


int main()
{
	char str[] = " 123   ";
	wing_str_trim(str);
	printf(">%s<", str);
    return 0;
}

