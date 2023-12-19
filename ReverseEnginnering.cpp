#include <iostream>
#include <fstream>
#include <iomanip>

void hexDump(const char *filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::cout << "Hex Dump of File: " << filename << "\n\n";

    int byteCount = 0;
    char buffer[16];

    while (file.read(buffer, sizeof(buffer)))
    {
        std::cout << std::setw(8) << std::setfill('0') << std::hex << byteCount << ": ";
        byteCount += 16;

        for (int i = 0; i < sizeof(buffer); ++i)
        {
            std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(static_cast<unsigned char>(buffer[i])) << ' ';

            if ((i + 1) % 8 == 0)
                std::cout << ' ';
        }

        std::cout << ' ';

        for (int i = 0; i < sizeof(buffer); ++i)
        {
            char c = buffer[i];
            if (isprint(c))
                std::cout << c;
            else
                std::cout << '.';
        }

        std::cout << '\n';
    }

    file.close();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    hexDump(argv[1]);

    return 0;
}