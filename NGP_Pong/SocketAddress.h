#pragma once

using SocketAddressPtr = shared_ptr<class SocketAddress>;

class SocketAddress
{
	friend class TCPSocket;

public:
	SocketAddress()
	{
		ZeroMemory(&mSockAddr, sizeof(mSockAddr));

		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_port = 0;
		GetAsSockAddrIn()->sin_addr.s_addr = INADDR_ANY;
	}

	SocketAddress(const string& address, uint16_t port)
	{
		ZeroMemory(&mSockAddr, sizeof(mSockAddr));

		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_port = htons(port);
		GetAsSockAddrIn()->sin_addr.s_addr = inet_addr(address.c_str());
	}

	uint32_t GetSize() const { return sizeof(mSockAddr); }

	string ToString() const;

	bool operator==(const SocketAddress& other)
	{
		return !memcmp(&mSockAddr, &other.mSockAddr, GetSize());
	}

	bool operator!=(const SocketAddress& other)
	{
		return !(*this == other);
	}

private:
	const sockaddr_in* GetAsSockAddrIn() const { return reinterpret_cast<const sockaddr_in*>(&mSockAddr); }
	sockaddr_in* GetAsSockAddrIn() { return reinterpret_cast<sockaddr_in*>(&mSockAddr); }

private:
	sockaddr mSockAddr;
};

