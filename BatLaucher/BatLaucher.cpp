// BatLaucher.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#pragma warning(disable:4996)

int main()
{
    std::cout << "Launching CMD Script\n";
	freopen("Launch.lch", "r", stdin);
	char cmd[20000];
	while (gets_s(cmd))
	{
		system(cmd);
	}
	std::cout << "Done\n";
	return 0;
}

