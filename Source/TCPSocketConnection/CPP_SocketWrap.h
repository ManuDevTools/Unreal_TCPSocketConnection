// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPP_SocketWrap.generated.h"

class FSocket;
/**
 * 
 */
UCLASS()
class TCPSOCKETCONNECTION_API UCPP_SocketWrap : public UObject
{
	GENERATED_BODY()
	
public:
	bool SetSocket(FSocket* RecvSocket);
	FSocket* GetSocket() const;

private:
	FSocket* Socket;
};
