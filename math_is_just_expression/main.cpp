
#include "strings2.h"

#include <fstream>

/*
load- load a file for reading
save- make files out of data
add- entry with a name and a number
quit- quit
print- prints shit on the screen

*/
int main() {

	bool runs = true;
	constexpr int select_options = 5;


	std::vector<std::string> muh_database;


	while (runs) {

		str::print("\nDa BIG POOPOO QUEST:\n\n[1] load file\n[2] save current\n[3] add entry\n[4] quit\n[5] print\n\nInput >> ");

		const int select = str::ISTR::InputRange(select_options);

		switch (select) {
		case 1:
		{
			printf("\nenter filename: ");
			std::string filename = str::ISTR::InputStr();
			filename += ".txt";
			std::ifstream in(filename, std::ios::in);
			printf("\n");
			if (in.good()) {
				for (char c = in.get(); in.good(); c = in.get()) {
					_putch(c);
				}

				const char poop = _kbhit();
			}
			else {
				printf("\nfile with given name not found");
			}
		}
		break;
		case 2:
		{
			printf("\nenter filename: ");
			std::string filename = str::ISTR::InputStr();
			filename += ".txt";
			std::ofstream out(filename, std::ios::app);

			if (out.good()) {
				for (auto& each : muh_database) {
					out << each;
					out << "\n";
				}
			}

		}
		break;
		case 3:
		{
			printf("\nenter name: ");
			std::string entry = str::ISTR::InputStr();
			printf("\nenter size but be reasonable: ");
			const int sizerino = str::ISTR::InputNum();

			entry += "| ";
			for (int i = 0; i < sizerino; i++) {
				entry += "=";
			}

			muh_database.push_back(entry);

		}
		break;
		case 4:
		{
			printf("\n\nBuh bye!");
			const char poop = _kbhit();
			runs = false;
		}
			break;
		case 5:
		{
			printf("\n\nWOW such data!\n\n");
			for (auto& each : muh_database) {
				printf("%s\n",each.c_str());
			}
		
			const char poop = _kbhit();
		}
		break;
		default:
			printf("%s %d", "Something is fucked up with nums when", select);
			break;
		}

	}



	return 0;
}