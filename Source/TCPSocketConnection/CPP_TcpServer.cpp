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


bool UCPP_TcpServer::GetMessage(UCPP_SocketWrap* Connection, FString& Message)
{
	// If the passed in socket is not valid
	if (!IsValid(Connection))
	{
		return false;
	}

	// Set an FSocket pointer to the socket inside of the passed in USocket
	FSocket* MySocket = Connection->GetSocket();

	// Check if it is not a null pointer
	if (MySocket == nullptr)
	{
		return false;
	}


	// Credit to RAMA for this converter!
	//Binary Array!
	TArray<uint8> BinaryData;
	uint32 Size;

	//while (MySocket->HasPendingData(Size)==0);


	int32 Read = 0;





	if (!MySocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1))) {       
		return nullptr;
	}
	// if (MySocket->Recv(BinaryData.GetData(), BinaryData.Num(), Read, ESocketReceiveFlags::Type::None))
	// 	return nullptr;


	
	
	
	while (MySocket->HasPendingData(Size))
	{
		// Be sure that the array doesn't become absolutely insanely large
		BinaryData.Init(0, FMath::Min(Size, 65507u));

		// Set the counter for the ammount of bytes read to 0
		int32 Reado = 0;
		// Recieve the data from the socket and put it into the binary array
		MySocket->Recv(BinaryData.GetData(), BinaryData.Num(), Reado);
	}

	// Check if there was actually data read into the array
	if (BinaryData.Num() <= 0)
	{
		// No data was read!
		UE_LOG(LogTemp, Warning, TEXT("No data to read!"));
		return false;
	}
	else
	{
		// Be sure to \0 terminate the array
		BinaryData.Add(0);
		// Convert it to an fstring and set the passed in message parameter
		Message = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryData.GetData())));
		MySocket->Close();
		MySocket->Shutdown(ESocketShutdownMode::ReadWrite);

		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(MySocket);
		Connection->ConditionalBeginDestroy();

		return true;
	}
}
