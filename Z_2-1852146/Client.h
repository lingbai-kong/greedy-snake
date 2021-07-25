#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#include <sys/timeb.h>
#include <stdint.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <winsock2.h>
#include <process.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER_LEN        8192  

static inline uint64_t getMsTimestamp();

static inline void quitWithError(LPCSTR message);

namespace NetworkHelper {

	static inline uint16_t readUint16(const char* buffer, const  size_t offset);
	static inline uint32_t readUint32(const char* buffer, const  size_t offset);
	static inline uint64_t readUint64(const char* buffer, const size_t offset);
	static inline void writeUint16(char* buffer, const size_t offset, const uint16_t value);
	static inline void writeUint32(char* buffer, const size_t offset, const uint32_t value);
	static inline void writeUint64(char* buffer, const size_t offset, const uint64_t value);
	struct msg_t {
		char ip[20] = { 0 };  // sender ip addr
		char name[40] = { 0 };// sender name
		uint32_t latency = 0; // latency to server
		uint32_t msgLen = 0;  // message length
		uint64_t time = 0;    // send from server time
		char* msgContent;
		msg_t();
		void clean();
		~msg_t();
	};

	struct msg_package_t {
		uint32_t msgNum = 0;
		uint64_t recvTime = 0;
		msg_t** msgs;
		msg_package_t();
		void clean();
		~msg_package_t();
	};

	struct msg_queue_t {
		msg_t** msgs;
		uint32_t queueCap = 0;
		uint32_t queueLen = 0;
		CRITICAL_SECTION cs;
		msg_queue_t();
		void getControl();
		void freeControl();
		void increase();
		void push(msg_t* msg);
		void fetch(msg_package_t& msg_out);
		bool empty();
		~msg_queue_t();
	};

	struct room_t {
		uint32_t roomId = 0;
		uint32_t existing = 0;
		char roomName[40] = { 0 };
		msg_t* everyOne;
		room_t();
		void clean();
		~room_t();
	};

	struct room_list_t {
		uint32_t roomNum = 0;
		room_t* rooms;
		room_list_t();
		void clean();
		~room_list_t();
	};

	enum class state_t {
		NEW_PACKAGE,
		PACKAGE_READY,
		READING_HEADER,
		READING_BODY,
		MISS_HEADER,
		MISS_BODY,
		PACKAGE_ERROR
	};

	enum class msg_type_t : uint16_t {
		PING_MSG,
		PONG_MSG,
		SET_NAME,
		SET_NAME_SUCCESS,
		SET_NAME_FAILED,
		GET_ROOM_LIST,
		ROOM_LIST,
		HOST_NEW_ROOM,
		HOST_ROOM_SUCCESS,
		HOST_ROOM_FAILED,
		CONNECT_ROOM,
		CONNECT_ROOM_SUCCESS,
		CONNECT_ROOM_FAILED,
		CHECK_ROOM_STATUS,
		ROOM_STATUS,
		DISCONNECT_ROOM,
		DISCONNECT_ROOM_SUCCESS,
		ROOM_MSG,
		SEND_MSG,
		SEND_MSG_SUCCESS,
		SEND_MSG_FAILED,
		ROOM_CLEAN_OUT
	};

	struct package_t {
		msg_type_t msg_type = msg_type_t::PING_MSG;
		uint16_t latency = 0;
		uint32_t msgRead = 0;
		uint32_t msgLen = 0;
		uint32_t msgCap = 0;
		uint32_t remainLen = 0;
		uint64_t sendTime = 0;
		uint64_t lastProcess = 0;
		state_t state = state_t::NEW_PACKAGE;
		char* msgContent = nullptr;
		char lastRemain[20] = { 0 };
		CRITICAL_SECTION cs;
		package_t();
		void getControl();
		void freeControl();
		void clear();
		void renew(uint32_t len);
		void append(uint32_t len);
		void copyIn(const char* buffer, const uint32_t len);
		void copyIn(package_t& pkg);
		void copyAppend(const char* buffer, const uint32_t len);
		bool copyOut(char* buffer, const uint32_t bufferLen);
		void setRemain(const char* buffer, const uint32_t len);
		void appendRemain(const char* buffer, const uint32_t len);
		void clearRemain();
		~package_t();
		bool isEmpty();
		bool hasRemain();
	};

	enum class operation_t {
		ACCEPT_POSTED,
		CONNECT_POSTED,
		SEND_POSTED,
		RECV_POSTED,
		NULL_POSTED
	};

	struct io_context_t {
		OVERLAPPED     overlapped = { 0 };
		SOCKET         socket;
		DWORD          recv_len = 0;
		DWORD          sent_len = 0;
		WSABUF         wsa_buffer = { 0 };
		CHAR           buffer[MAX_BUFFER_LEN] = { 0 };
		operation_t    operation;

		io_context_t();

		~io_context_t();

		void cleanBuffer();

	};

	struct socket_context_t {
		SOCKET      socket;
		SOCKADDR_IN client_addr;
		io_context_t* send;
		io_context_t* recv;

		socket_context_t();

		~socket_context_t();

		io_context_t* getRecvIo();

		io_context_t* getSendIo();
	};

	class ClientHelper {
	public:
		ClientHelper();

		bool connectToServer(LPCSTR ip, int port, LPCSTR name);

		bool checkSend();

		void clearSend();

		bool ping();

		bool getRoomList(room_list_t& rlt);

		bool getRoomInfo(uint32_t roomID, room_t& room);

		bool getRoomInfo(room_t& room);

		bool hostNewRoom(LPCSTR roomName, uint32_t& roomID);

		bool connectToRoom(uint32_t roomID);
		bool connectToRoom(room_t& room);
		bool disconnectFromRoom();
		bool disconnectFromServer();
		bool sendMsg(LPCSTR msg);
		bool sendBinaryMsg(const char* buffer, uint32_t msgLen);
		bool recvMsg(msg_package_t& m);
		bool checkRoomStatus();
		~ClientHelper();
	private:
		HANDLE hIOCP;
		HANDLE hExitFlag;
		HANDLE hGlobalEvent;

		DWORD threads;
		HANDLE* workers;

		BOOL bConnectStatus;
		BOOL bRoomStatus;

		DWORD timeout = 5000;

		msg_type_t except_type_first;
		msg_type_t except_type_second;
		package_t except_pkg;
		package_t send_pkg;
		package_t recv_pkg;

		msg_queue_t msg_queue;

		socket_context_t* clientContext;

		LPFN_CONNECTEX lpfnConnecter;

		static DWORD __stdcall workerFnCallerEx(void*);
		unsigned int workerFn();
		void init();

		bool _sendMessage(const char* msg, msg_type_t msgType, msg_type_t except, msg_type_t except_2, uint32_t msgLen);

		bool _connect_pushToQueue(io_context_t*, LPCSTR, int);
		bool _recv_pushToQueue(io_context_t*);
		bool _send_pushToQueue(io_context_t*);

		bool _buildPackage(char*, const uint32_t, ULONG&, package_t&);
		bool _parsePackage(const char*, const uint32_t, uint32_t&, package_t&);

		bool _parseMessage(package_t& pkg);

		bool _connect(socket_context_t*, io_context_t*);
		bool _recv(socket_context_t*, io_context_t*, uint32_t);
		bool _send(socket_context_t*, io_context_t*);
	};
}
#endif  // CLIENT_H
