
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>

int main()
{
	std::ifstream input("humantanksBB.ald", std::ios::binary);

	std::vector<char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

	std::cout << buffer.size();

	return 0;
}
