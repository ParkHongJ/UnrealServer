// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerStudy.h"

/**
 * 
 */
struct SERVERSTUDY_API FPacketHeader
{
	FPacketHeader() : PacketSize(0), PacketID(0)
	{
		
	}

	FPacketHeader(uint16 PacketSize, uint16 PacketID) : PacketSize(PacketSize), PacketID(PacketID)
	{
		
	}

	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.PacketID;
		return Ar;
	}

	//헤더사이즈 포함한 사이즈
	uint16 PacketSize;
	uint16 PacketID;
};

class SERVERSTUDY_API RecvWorker : public FRunnable
{
public:	
	RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~RecvWorker();
	
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);

	//원하는 크기의 패킷이 올때까지 기다린다.
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);
	
protected:
	FRunnableThread* Thread = nullptr;
	bool bRunning = true;
	FSocket* Socket;
	TWeakPtr<PacketSession> SessionRef;
};

class SERVERSTUDY_API SendWorker : public FRunnable
{
public:	
	SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~SendWorker();
	
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool SendPacket(SendBufferRef SendBuffer);

	bool SendDesiredBytes(const uint8* Buffer, int32 Size);
	
protected:
	FRunnableThread* Thread = nullptr;
	bool bRunning = true;
	FSocket* Socket;
	TWeakPtr<PacketSession> SessionRef;
};
