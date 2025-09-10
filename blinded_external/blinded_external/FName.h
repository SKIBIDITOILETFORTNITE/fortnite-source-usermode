#pragma once

class FName {
public:
	FName()
	{
		ComparisonIndex = 0;
	}

	FName(uintptr_t object)
	{
		if (object)
		{
			ComparisonIndex = memory.Read<int32_t>(object + 0xC);
		}
	}

	int32_t ComparisonIndex;

	std::string ToString() const
	{
		return ToString(ComparisonIndex);
	}

	static std::string ToString(int32_t index)
	{
		int32_t decryptedIndex = DecryptIndex(index);

	ResolveAddress:
		if (decryptedIndex <= 0) { return ""; }

		uint32_t chunkIndex = decryptedIndex >> 16;
		uint16_t nameEntryIndex = static_cast<uint16_t>(decryptedIndex);

		uint64_t namePoolChunk = memory.Read<uint64_t>(Base + (0x17A31B80 + 8 * chunkIndex + 16)) + 2 * nameEntryIndex;

		if (!(namePoolChunk)) { return ""; }

		uint16_t pool = memory.Read<uint16_t>(namePoolChunk);

		if (((pool ^ 0x15E) & 0x7FE) == 0)
		{
			decryptedIndex = DecryptIndex(memory.Read<int32_t>(namePoolChunk + 2));
			goto ResolveAddress;
		}

		const int32_t length = ((pool ^ 0x15Eu) >> 1) & 0x3FF;
		if (length <= 0 || length > 2048) { return ""; }

		std::vector<char> nameBuffer(length);
		driver::ReadPhysical(PVOID(namePoolChunk + 2), nameBuffer.data(), length);

		return DecryptFName(nameBuffer.data(), length);
	}

	static int32_t DecryptIndex(int32_t index)
	{
		if (index == 0) { return 0; }

		int32_t v4 = 0xF186FD88;
		int32_t v5 = -static_cast<int32_t>((index - 1) ^ 0xF186FD87);

		if (v5)
		{
			v4 = v5;
		}

		return v4;
	}

	static std::string DecryptFName(char* buffer, int32_t length)
	{
		if (length <= 0) { return ""; }
		std::string decrypted(length, '\0');
		char* decryptedPtr = decrypted.data();

		uint32_t v6 = 8392 * length + 22783311;

		if (length > 0)
		{
			char* v7 = decryptedPtr;
			char* a1 = buffer;
			int32_t v8 = length;

			do
			{
				*v7 = v6 - *a1 - 112;

				++v7;
				++a1;

				v6 = 8392 * v6 + 22783311;

				--v8;
			} while (v8);
		}

		return decrypted;
	}
};