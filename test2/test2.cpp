// test2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"

int main()
{
	HINSTANCE  h = LoadLibrary(L"D:/php/php7-sdk/phpdev/vc14/x86/php-7.0.12/ext/wing_process/Release/wing_process.dll");
	if (h == NULL) {
		printf("����ʧ��");
	}
	else {
		printf("���سɹ�");
	}
	return 0;
}

