#include <iostream>

int main()
{

    union {
        char bytes[16];
        uint32_t num[4];
    } data;

    data.num[0] = 0x11223345;
    data.num[0] = 0x55667788;
    data.num[0] = 0xaabbccdd;

    std::cout << std::hex;

    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << (uint16_t)data.bytes[i] << ' ';
    }

    std::cout << std::endl;

    return 0;
}
