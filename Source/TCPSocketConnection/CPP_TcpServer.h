// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "CPP_TcpServer.generated.h"

class UCPP_SocketWrap;
/**
 * 
 */
UCLASS()
class TCPSOCKETCONNECTION_API UCPP_TcpServer : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Connect server", Keywords = "tcp socket server"),Category = "Networking|Socket")
	static UCPP_SocketWrap* Connect(FString Ip, int32 Port);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get buffer (converted to FString) from server", Keywords = "Socketer send message tcpsend tcp tcpdisconnect socketersend"), Category = "Networking|Socket")
	static bool GetMessage(UCPP_SocketWrap* Connection, FString &Message);
	
};
