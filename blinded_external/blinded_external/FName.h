#pragma once

class FName
{
public:
	int32_t ComparisonIndex;

	std::string ToString()
	{
		return ToString(ComparisonIndex);
	}

	static std::string ToString(int32_t index)
	{
		int32_t DecryptedIndex = DecryptIndex(index);
		uint64_t NamePoolChunk = read<uint64_t>(Addresses::base + (0x17404940 + 8 * (DecryptedIndex >> 16) + 16)) + 2 * (uint16_t)DecryptedIndex;
		uint16_t Pool = read<uint16_t>(NamePoolChunk);

		if (((Pool ^ 0x1E4) & 0x7FE) <= 0)
		{
			DecryptedIndex = DecryptIndex(read<int32_t>(NamePoolChunk + 2));
			NamePoolChunk = read<uint64_t>(Addresses::base + (0x17404940 + 8 * (DecryptedIndex >> 16) + 16)) + 2 * (uint16_t)DecryptedIndex;
			Pool = read<uint16_t>(NamePoolChunk);
		}

		int32_t Length = ((Pool ^ 0x1E4u) >> 1) & 0x3FF * ((Pool & 1) != 0 ? 2 : 1);

		char* NameBuffer = new char[Length + 1];
		Driver::ReadPhysicalMemory(PVOID(NamePoolChunk + 2), NameBuffer, Length);
		DecryptFName(NameBuffer, Length);
		return std::string(NameBuffer);
	}

	static int32_t DecryptIndex(int32_t index)
	{
		if (index)
		{
			int32_t DecryptedIndex = ((uint32_t((index - 1) ^ 0xB1D979CD) >> 12) | (uint32_t((index - 1) ^ 0xB1D979CD) << 20)) + 1;
			return DecryptedIndex ? DecryptedIndex : 1663361641;
		}

		return 0;
	}

	static void DecryptFName(char* buffer, int length)
	{
		if (length)
		{
			unsigned int v4 = 0x127D236 + 8853 * length;
			for (int i = 0; i < length; ++i)
			{
				buffer[i] = (uint8_t(buffer[i] ^ v4) >> 1) | (uint8_t(buffer[i] ^ v4) << 7);
				v4 = 0x127D236 + 8853 * v4;
			}
		}

		buffer[length] = '\0';
	}
};