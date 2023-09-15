// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerStudy.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ServerStudy, "ServerStudy" );

SendBuffer::SendBuffer(int32 bufferSize)
{
	buffer.SetNum(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	memcpy(buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}
