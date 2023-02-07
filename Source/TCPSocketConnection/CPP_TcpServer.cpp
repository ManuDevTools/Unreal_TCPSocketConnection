// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TcpServer.h"
#include "CPP_SocketWrap.h"
#include "BehaviorTree/BehaviorTreeTypes.h"


UCPP_SocketWrap* UCPP_TcpServer::Connect(FString Ip, int32 Port)
{
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP Client Socket"), false);
	UCPP_SocketWrap* ReturnedSocket = NewObject<UCPP_SocketWrap>();
	
	FAddressInfoResult Result = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetAddressInfo(*Ip, nullptr, EAddressInfoFlags::Default, NAME_None);

	const TSharedRef<FInternetAddr> SocketAddress = Result.Results[0].Address;
	SocketAddress->SetPort(Port);
	
	
	if(!Socket->Connect(*SocketAddress))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not connect to TCP server."));
		return nullptr;
	}

	ReturnedSocket->SetSocket(Socket);
	return ReturnedSocket;
}


bool UCPP_TcpServer::ReceiveMessage(UCPP_SocketWrap* Connection, FString& Message)
{
	if (!IsValid(Connection))
		return false;
	
	FSocket* Socket = Connection->GetSocket();
	
	if (Socket == nullptr)
		return false;
	
	TArray<uint8> BinaryData;
	uint32 Size;

	if (!Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1)))   
		return nullptr;
	
	while (Socket->HasPendingData(Size))
	{
		BinaryData.Init(0, FMath::Min(Size, 65507u));
	
		int32 Read = 0;
		Socket->Recv(BinaryData.GetData(), BinaryData.Num(), Read);
	}

	if (BinaryData.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No data to read!"));
		return false;
	}

	BinaryData.Add(0);
	Message = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryData.GetData())));
	
	Socket->Close();
	Socket->Shutdown(ESocketShutdownMode::ReadWrite);

	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	Connection->ConditionalBeginDestroy();

	return true;
}
