// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkWorker.h"
#include "PacketSession.h"
#include "Sockets.h"

RecvWorker::RecvWorker(FSocket* Socket, TSharedPtr<PacketSession> Session)
	: Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

RecvWorker::~RecvWorker()
{
}

bool RecvWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
	
	return true;
}

uint32 RecvWorker::Run()
{
	while (bRunning)
	{
		TArray<uint8> Packet;

		if (ReceivePacket(OUT Packet))
		{
			//패킷조립을 완성했을때
			if (TSharedPtr<PacketSession> Session = SessionRef.Pin())
			{
				//Session에 Enqueue를 하는 중간에 Session이 날라갈 수 있을까?
				//없다. WeakPtr에서 SharedPtr로 변환하는 과정에서 RefCount가 1 증가하기 때문에.
				Session->RecvPacketQueue.Enqueue(Packet);
			}
		}
	}
	return 0;
}

void RecvWorker::Exit()
{
	FRunnable::Exit();
}

void RecvWorker::Destroy()
{
	bRunning = false;
}

bool RecvWorker::ReceivePacket(TArray<uint8>& OutPacket)
{
	const int32 HeaderSize = sizeof(FPacketHeader);

	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);//resize

	//패킷헤더의 조립
	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
		return false;

	//Header 추출
	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;

		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
	}

	OutPacket = HeaderBuffer;

	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	OutPacket.AddZeroed(PayloadSize);//resize

	//아무것도 보내지 않았다면 
	if(PayloadSize <= 0)
		return true;
	
	//패킷 페이로드(헤더를 제외한 나머지)의 조립
	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize))
		return true;
	
	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
	uint32 PendingDataSize;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;
	//요청한 Size만큼 Recv를 계속 실행한다.
	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}
	
	return true;
}

//SendWorker

SendWorker::SendWorker(FSocket* Socket, TSharedPtr<PacketSession> Session)
	: Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker::~SendWorker()
{
}

bool SendWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));
	return true;
}

uint32 SendWorker::Run()
{
	while (bRunning)
	{
		SendBufferRef SendBuffer;

		if (TSharedPtr<PacketSession> Session = SessionRef.Pin())
		{
			if (Session->SendPacketQueue.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}

		// Sleep?
	}

	return 0;
}

void SendWorker::Exit()
{
	FRunnable::Exit();
}

void SendWorker::Destroy()
{
	bRunning = false;
}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
	return false;

	return true;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (Socket->Send(Buffer, Size, BytesSent) == false)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}
