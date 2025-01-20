
#include "strings2.h"

#include <fstream>

int main() {

	/*
	const int muhint = 69420123;
	std::ofstream out("boi.dat", std::ios::binary);
	out.write(reinterpret_cast<const char*>(&muhint), sizeof(muhint));
	*/
	
	std::ifstream in("boi.dat", std::ios::binary);

	int data = -1;

	in.read(reinterpret_cast<char*>(&data), sizeof(data));

	printf("%d \n",data);
	


	/*
	std::fstream in("derp.txt");

	if (!in) {
		str::print("\nbig pantsu hacker boi");
		return -1;
	}

	in.seekg(0, std::ios_base::end);
	const int len = in.tellg(); 

	str::print("\nFile Size: ");
	printf("%d\n ", len);

	in.seekg(0);

	for (char c = in.get(); in.good(); c = in.get()) {
		_putch(c);
	}

	if (in.bad()) {
		str::print("\nbigus dickus\n");
	}
	else if (in.fail()) {
		str::print("\ni always fail. failing is good\n");
	}

	std::ofstream out("out.txt");

	for (char c = _getch(); c != 13; c = _getch()) {
		out.put(c);
		_putch(c);
	}
	*/


	while (!_kbhit());
	return 0;
}