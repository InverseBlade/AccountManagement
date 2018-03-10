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
		outputMenu();

		scanf("%d", &user_input);
		clean_stdin();

		if (_EXIT_ == controller(user_input))
			break;
	}

    return 0;
}