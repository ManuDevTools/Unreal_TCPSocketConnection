// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_SocketWrap.h"

bool UCPP_SocketWrap::SetSocket(FSocket* RecvSocket)
{
	this->Socket = RecvSocket;
	return false;
}

FSocket* UCPP_SocketWrap::GetSocket() const
{
	return Socket;
}
