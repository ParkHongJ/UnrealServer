// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
struct PacketHeader
{
	uint16 size;
	uint16 id;
};

class SendBuffer : public TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return buffer.Num(); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);
private:
	TArray<BYTE> buffer;
	int32 _writeSize = 0;
};

#define USING_SHARED_PTR(name) using name##Ref = TSharedPtr<class name>;

USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);