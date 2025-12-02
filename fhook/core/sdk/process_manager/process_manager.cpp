#include "process_manager.hpp"
#include <string>

inline const DWORD BLOCKMAXSIZE = 409600;

int GetSignatureArray(const std::string& Signature, std::vector<WORD>& SignatureArray) {
	std::string Sig = Signature;
	Sig.erase(std::remove(Sig.begin(), Sig.end(), ' '), Sig.end());

	std::size_t Size = Sig.size();

	if (Size % 2 != 0)
		return 0;

	for (size_t i = 0; i < Size; i += 2) {
		std::string ByteString = Sig.substr(i, 2);
		WORD Byte = (ByteString == "??") ? 256 : static_cast<WORD>(std::stoi(ByteString, nullptr, 16));
		SignatureArray.push_back(Byte);
	}
	return static_cast<int>(SignatureArray.size());
}

void GetNextArray(std::vector<short>& NextArray, const std::vector<WORD>& SignatureArray) {
	std::size_t Size = SignatureArray.size();
	for (size_t i = 0; i < Size; i++)
		NextArray[SignatureArray[i]] = static_cast<short>(i);
}

void SearchMemoryBlock(byte* MemoryBuffer, const std::vector<short>& NextArray, const std::vector<WORD>& SignatureArray, DWORD64 StartAddress, DWORD Size, std::vector<DWORD64>& ResultArray) {
	if (!_proc_manager.read_memory(StartAddress, *MemoryBuffer, Size))
		return;

	size_t SignatureLength = SignatureArray.size();

	for (size_t i = 0, j, k; i < Size;) {
		j = i; k = 0;

		for (; k < SignatureLength && j < Size && (SignatureArray[k] == MemoryBuffer[j] || SignatureArray[k] == 256); k++, j++);

		if (k == SignatureLength)
			ResultArray.push_back(StartAddress + i);

		if ((i + SignatureLength) >= Size)
			return;

		int Num = NextArray[MemoryBuffer[i + SignatureLength]];
		if (Num == -1)
			i += (SignatureLength - NextArray[256]);
		else
			i += (SignatureLength - Num);
	}
}

std::vector<DWORD64> ProcessManager::search_memory(const std::string& Signature, DWORD64 StartAddress, DWORD64 EndAddress, int SearchNum) {
	std::vector<DWORD64> ResultArray;
	std::vector<WORD> SignatureArray;
	std::vector<short> NextArray(260, -1);

	byte* MemoryBuffer = new byte[BLOCKMAXSIZE];

	if (GetSignatureArray(Signature, SignatureArray) <= 0)
		return ResultArray;

	GetNextArray(NextArray, SignatureArray);

	MEMORY_BASIC_INFORMATION mbi;
	int Count;
	while (VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(StartAddress), &mbi, sizeof(mbi)) != 0) {
		Count = 0;
		SIZE_T BlockSize = mbi.RegionSize;

		while (BlockSize >= BLOCKMAXSIZE) {
			if (static_cast<int>(ResultArray.size()) >= SearchNum)
				goto END;

			SearchMemoryBlock(MemoryBuffer, NextArray, SignatureArray, StartAddress + (BLOCKMAXSIZE * Count), BLOCKMAXSIZE, ResultArray);

			BlockSize -= BLOCKMAXSIZE;
			Count++;
		}

		SearchMemoryBlock(MemoryBuffer, NextArray, SignatureArray, StartAddress + (BLOCKMAXSIZE * Count), static_cast<DWORD>(BlockSize), ResultArray);

		StartAddress += mbi.RegionSize;

		if (static_cast<int>(ResultArray.size()) >= SearchNum || EndAddress != 0 && StartAddress > EndAddress)
			break;
	}

END:

	delete[] MemoryBuffer;
	return ResultArray;
}