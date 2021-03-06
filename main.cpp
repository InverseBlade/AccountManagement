// main.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "menu.h"
#include "global.h"
#include "tool.h"

int main()
{
	int user_input = 0;

	while (true)
	{
		outputMenu();                            //输出菜单

		fflush(stdin);
		scanf("%d", &user_input);                //监听请求
		clean_stdin();

		if (_EXIT_ == controller(user_input))    //处理请求
			break;
	}

    return 0;
}