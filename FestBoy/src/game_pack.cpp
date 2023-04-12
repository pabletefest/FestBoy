#include "game_pack.h"
#include "no_mbc.h"

#include <fstream>

gb::GamePak::GamePak(const std::string& filename)
{
    std::memset(&header, 0x00, sizeof(CartridgeHeader));

    std::ifstream ifs;

    ifs.open(filename, std::ifstream::binary);

    if (ifs.is_open())
    {
        // TEMP: skipping bootrom
        ifs.seekg(256, std::ios_base::cur);

        // Read file header
        ifs.read((char*)&header, sizeof(CartridgeHeader));

        gameName = std::string((const char*)header.title);

        u64 romSize = romSizesTable[header.romSize];
        nROMBanks = static_cast<u8>(romSize / convertKBToBytes(16));

        ifs.seekg(0);
        vROMMemory.resize(nROMBanks * convertKBToBytes(16));
        ifs.read((char*)vROMMemory.data(), romSize);

        switch (header.cartridgeType)
        {
        case 0x00:
        case 0x01:
            mapper = std::make_unique<NoMBCMapper>(nROMBanks);
            break;
        default:
            break;
        }
    }
}

auto gb::GamePak::read(u16 addr, u8& data) -> bool
{
    u16 mappedAddress = 0x0000;

    if (mapper->mapRead(addr, mappedAddress))
    {
        data = vROMMemory[mappedAddress];
        return true;
    }

    return false;
}

auto gb::GamePak::write(u16 addr, u8 data) -> bool
{
    u16 mappedAddress = 0x0000;

    if (mapper->mapWrite(addr, mappedAddress, data))
    {
        // No need to modify ROM memory
        //vROMMemory[mappedAddress] = data;
        return true;
    }

    return false;
}