// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerStudy.h"

/**
 * 
 */
class FSocket;
class SERVERSTUDY_API PacketSession : public TSharedFromThis<PacketSession>
{
public:
	PacketSession(FSocket* Socket);
	~PacketSession();

	void Run();
	
	void Disconnect();

	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);
public:
	FSocket* Socket;

	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;
	
	//표준 std::queue를 사용하며 안될까?
	//안된다. TQueue는 LockFree방식으로 작동되서 사용해도 되지만 std는 그렇지않다
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;
};
