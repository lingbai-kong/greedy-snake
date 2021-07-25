#include"Client.h"

static inline uint64_t getMsTimestamp() {
	timeb t;
	ftime(&t);
	return (uint64_t)t.time * 1000ULL + t.millitm;
}

static inline void quitWithError(LPCSTR message) {
	MessageBoxA(GetForegroundWindow(), message, "Application Error", MB_ICONEXCLAMATION | MB_OK);
	quick_exit(0xffffffff);
}

namespace NetworkHelper {

	static inline uint16_t readUint16(const char* buffer, const  size_t offset) {
		uint16_t ret = 0;
		for (size_t i = 0; i < 2; i++) {
			ret |= uint16_t((uint8_t)buffer[offset + i]) << (i * 8);
		}
		return ret;
	}

	static inline uint32_t readUint32(const char* buffer, const  size_t offset) {
		uint32_t ret = 0;
		for (size_t i = 0; i < 4; i++) {
			ret |= uint32_t((uint8_t)buffer[offset + i]) << (i * 8);
		}
		return ret;
	}

	static inline uint64_t readUint64(const char* buffer, const size_t offset) {
		uint64_t ret = 0;
		for (size_t i = 0; i < 8; i++) {
			ret |= uint64_t((uint8_t)buffer[offset + i]) << (i * 8);
		}
		return ret;
	}

	static inline void writeUint16(char* buffer, const size_t offset, const uint16_t value) {
		for (size_t i = 0; i < 2; i++) {
			buffer[offset + i] = (char)((value >> (8 * i)) & 0xff);
		}
	}

	static inline void writeUint32(char* buffer, const size_t offset, const uint32_t value) {
		for (size_t i = 0; i < 4; i++) {
			buffer[offset + i] = (char)((value >> (8 * i)) & 0xff);
		}
	}

	static inline void writeUint64(char* buffer, const size_t offset, const uint64_t value) {
		for (size_t i = 0; i < 8; i++) {
			buffer[offset + i] = (char)((value >> (8 * i)) & 0xff);
		}
	}

	msg_t::msg_t() : msgContent(nullptr) {}
	void msg_t::clean() { if (msgContent != nullptr)delete[] msgContent; msgContent = nullptr; msgLen = 0; }
	msg_t::~msg_t() { clean(); }

	msg_package_t::msg_package_t() : msgs(nullptr) {}
	void msg_package_t::clean() {
		if (msgs != nullptr) {
			for (uint32_t i = 0; i < msgNum; i++) {
				delete msgs[i];
			}
			delete[]msgs;
			msgs = nullptr;
			msgNum = 0;
		}
	}
	msg_package_t::~msg_package_t() { clean(); }

	msg_queue_t::msg_queue_t() :msgs(nullptr) {
		InitializeCriticalSection(&cs);
		queueCap = 64;
		msgs = new msg_t * [64];
	}
	void msg_queue_t::getControl() {
		EnterCriticalSection(&cs);
	}
	void msg_queue_t::freeControl() {
		LeaveCriticalSection(&cs);
	}
	void msg_queue_t::increase() {
		getControl();
		msg_t** tmp = new msg_t * [(size_t)queueCap * 2];
		memcpy(tmp, msgs, sizeof(msg_t*) * queueCap);
		queueCap *= 2;
		freeControl();
	}
	void msg_queue_t::push(msg_t* msg) {
		getControl();
		if (queueLen == queueCap) increase();
		msgs[queueLen++] = msg;
		freeControl();
	}
	void msg_queue_t::fetch(msg_package_t& msg_out) {
		getControl();
		msg_out.clean();
		msg_out.recvTime = getMsTimestamp();
		msg_out.msgNum = queueLen;
		msg_out.msgs = new msg_t * [queueLen];
		memcpy(msg_out.msgs, msgs, sizeof(msg_t*) * queueLen);
		queueLen = 0;
		freeControl();
	}
	bool msg_queue_t::empty() { return queueLen == 0; }
	msg_queue_t::~msg_queue_t() {
		if (msgs != nullptr) {
			for (uint32_t i = 0; i < queueLen; i++) {
				delete msgs[i];
			}
			delete[] msgs;
		}
		DeleteCriticalSection(&cs);
	}

	room_t::room_t() : everyOne(nullptr) {};
	void room_t::clean() { if (everyOne != nullptr)delete[] everyOne; existing = 0; }
	room_t::~room_t() { clean(); }

	room_list_t::room_list_t() : rooms(nullptr) {}
	void room_list_t::clean() { if (rooms != nullptr)delete[] rooms; rooms = nullptr; roomNum = 0; }
	room_list_t::~room_list_t() { clean(); }

	package_t::package_t() {
		InitializeCriticalSection(&cs);
	}
	void package_t::getControl() {
		EnterCriticalSection(&cs);
	}
	void package_t::freeControl() {
		LeaveCriticalSection(&cs);
	}
	void package_t::clear() {
		getControl();
		if (msgContent != nullptr)delete[] msgContent;
		msgContent = nullptr;
		msgRead = 0;
		msgCap = 0;
		freeControl();
	}
	void package_t::renew(uint32_t len) {
		getControl();
		if (msgContent != nullptr)delete[] msgContent;
		msgContent = new char[len];
		msgRead = 0;
		msgCap = len;
		freeControl();
	}
	void package_t::append(uint32_t len) {
		getControl();
		char* tmp = new char[size_t(msgCap) + len];
		if (msgContent != nullptr) {
			memcpy(tmp, msgContent, msgRead);
			delete[]msgContent;
		}
		msgContent = tmp;
		msgCap += len;
		freeControl();
	}
	void package_t::copyIn(const char* buffer, const uint32_t len) {
		if (len == 0) {
			msgRead = 0;
			return;
		}
		if (len > msgCap) {
			renew(len);
		}
		getControl();
		memcpy(msgContent, buffer, len);
		msgRead = len;
		freeControl();
	}
	void package_t::copyIn(package_t& pkg) {
		pkg.getControl();
		getControl();
		msg_type = pkg.msg_type;
		latency = pkg.latency;
		sendTime = pkg.sendTime;
		msgLen = pkg.msgLen;
		state = pkg.state;
		copyIn(pkg.msgContent, pkg.msgLen);
		freeControl();
		pkg.freeControl();
	}
	void package_t::copyAppend(const char* buffer, const uint32_t len) {
		if (len == 0)return;
		if (len + msgRead > msgCap) {
			append(len);
		}
		getControl();
		memcpy(msgContent + msgRead, buffer, len);
		msgRead += len;
		freeControl();
	}
	bool package_t::copyOut(char* buffer, const uint32_t bufferLen) {
		if (msgLen == 0)return true;
		if (bufferLen < msgLen) {
			return false;
		}
		getControl();
		memcpy(buffer, msgContent, msgLen);
		freeControl();
		return true;
	}
	void package_t::setRemain(const char* buffer, const uint32_t len) {
		if (len > 14)return;
		getControl();
		memcpy(lastRemain, buffer, len);
		remainLen = len;
		freeControl();
	}
	void package_t::appendRemain(const char* buffer, const uint32_t len) {
		if (remainLen + len > 14)return;
		getControl();
		memcpy(lastRemain + remainLen, buffer, len);
		remainLen += len;
		freeControl();
	}
	void package_t::clearRemain() {
		getControl();
		remainLen = 0;
		freeControl();
	}
	package_t::~package_t() {
		getControl();
		if (msgContent != nullptr)delete[] msgContent;
		msgContent = nullptr;
		freeControl();
		DeleteCriticalSection(&cs);
	}
	bool package_t::isEmpty() { return msgLen == 0; }
	bool package_t::hasRemain() { return remainLen != 0; }

	io_context_t::io_context_t() :
		socket(INVALID_SOCKET),
		operation(operation_t::NULL_POSTED) {

		wsa_buffer.buf = buffer;
		wsa_buffer.len = MAX_BUFFER_LEN;
	}

	io_context_t::~io_context_t() {
	}

	void io_context_t::cleanBuffer() {
		memset(buffer, 0, MAX_BUFFER_LEN);
	}

	socket_context_t::socket_context_t() : socket(INVALID_SOCKET),
		send(nullptr),
		recv(nullptr) {
		memset(&client_addr, 0, sizeof(client_addr));
		send = new io_context_t;
		recv = new io_context_t;
	}

	socket_context_t::~socket_context_t()
	{
		if (socket != INVALID_SOCKET) {
			closesocket(socket);
			socket = INVALID_SOCKET;
		}
		delete send;
		delete recv;
	}

	io_context_t* socket_context_t::getRecvIo() {
		return recv;
	}

	io_context_t* socket_context_t::getSendIo() {
		return send;
	}

	bool ClientHelper::checkSend() {
		if (except_type_first != msg_type_t::PING_MSG) {
			printf("something in request queue\n");
			return false;
		}
		return true;
	}

	void ClientHelper::clearSend() {
		except_type_first = msg_type_t::PING_MSG;
	}

	bool ClientHelper::ping() {
		if (checkSend() && _sendMessage(nullptr, msg_type_t::PING_MSG, msg_type_t::PONG_MSG, msg_type_t::PONG_MSG, 0)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("ping time out\n");
				return false;
			}
			int64_t tic = (int64_t)readUint64(except_pkg.msgContent, 0);
			int64_t diff = (int64_t)getMsTimestamp() - tic;
			printf("pong! diff: %lld\n", (int64_t)except_pkg.sendTime - tic - diff);
			return true;
		}
		return false;
	}

	bool ClientHelper::getRoomList(room_list_t& rlt) {
		if (checkSend() && _sendMessage(nullptr, msg_type_t::GET_ROOM_LIST, msg_type_t::ROOM_LIST, msg_type_t::ROOM_LIST, 0)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("get room list time out\n");
				return false;
			}
			rlt.clean();
			except_pkg.getControl();
			size_t offset = 0;
			rlt.roomNum = readUint32(except_pkg.msgContent, offset);
			offset += 4;
			if (rlt.roomNum) {
				rlt.rooms = new room_t[rlt.roomNum];
				for (uint32_t i = 0; i < rlt.roomNum; i++) {
					rlt.rooms[i].roomId = readUint32(except_pkg.msgContent, offset);
					offset += 4;
					rlt.rooms[i].existing = readUint32(except_pkg.msgContent, offset);
					offset += 4;
					memcpy(rlt.rooms[i].roomName, except_pkg.msgContent + offset, 40);
					offset += 40;
				}
			}
			except_pkg.freeControl();
			return true;
		}
		return false;
	}

	bool ClientHelper::getRoomInfo(uint32_t roomID, room_t& room) {
		char buffer[4];
		writeUint32(buffer, 0, roomID);
		if (checkSend() && _sendMessage(buffer, msg_type_t::CHECK_ROOM_STATUS, msg_type_t::ROOM_STATUS, msg_type_t::ROOM_STATUS, 4)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("get room info time out\n");
				return false;
			}
			room.clean();
			except_pkg.getControl();
			size_t offset = 0;
			room.roomId = readUint32(except_pkg.msgContent, offset);
			offset += 4;
			room.existing = readUint32(except_pkg.msgContent, offset);
			offset += 4;
			memcpy(room.roomName, except_pkg.msgContent + offset, 40);
			offset += 40;
			if (room.existing) {
				room.everyOne = new msg_t[room.existing];
				for (uint32_t i = 0; i < room.existing; i++) {
					room.everyOne[i].time = readUint64(except_pkg.msgContent, offset);
					offset += 8;
					memcpy(room.everyOne[i].ip, except_pkg.msgContent + offset, 20);
					offset += 20;
					memcpy(room.everyOne[i].name, except_pkg.msgContent + offset, 40);
					offset += 40;
				}
			}
			except_pkg.freeControl();
			return true;
		}
		return false;
	}

	bool ClientHelper::getRoomInfo(room_t& room) {
		return getRoomInfo(room.roomId, room);
	}

	bool ClientHelper::hostNewRoom(LPCSTR roomName, uint32_t& roomID) {
		if (checkSend() && _sendMessage(roomName, msg_type_t::HOST_NEW_ROOM, msg_type_t::HOST_ROOM_FAILED, msg_type_t::HOST_ROOM_SUCCESS, (uint32_t)strlen(roomName) + 1)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("host new room time out\n");
				return false;
			}

			if (except_pkg.msg_type == msg_type_t::HOST_ROOM_FAILED) {
				printf("host room failed\n");
				return false;
			}

			roomID = readUint32(except_pkg.msgContent, 0);

			bRoomStatus = TRUE;

			return true;
		}
		return false;
	}

	bool ClientHelper::connectToRoom(uint32_t roomID) {
		char buffer[4];
		writeUint32(buffer, 0, roomID);
		if (checkSend() && _sendMessage(buffer, msg_type_t::CONNECT_ROOM, msg_type_t::CONNECT_ROOM_FAILED, msg_type_t::CONNECT_ROOM_SUCCESS, 4)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("host new room time out\n");
				return false;
			}

			if (except_pkg.msg_type == msg_type_t::CONNECT_ROOM_FAILED) {
				printf("connect room failed\n");
				return false;
			}

			bRoomStatus = TRUE;

			return true;
		}
		return false;
	}
	bool ClientHelper::connectToRoom(room_t& room) {
		return connectToRoom(room.roomId);
	}
	bool ClientHelper::disconnectFromRoom() {
		if (checkSend() && _sendMessage(nullptr, msg_type_t::DISCONNECT_ROOM, msg_type_t::DISCONNECT_ROOM_SUCCESS, msg_type_t::DISCONNECT_ROOM_SUCCESS, 0)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("disconnect room time out\n");
				return false;
			}

			return true;

			bRoomStatus = FALSE;
		}
		return false;
	}
	bool ClientHelper::disconnectFromServer() {
		closesocket(clientContext->socket);
		bConnectStatus = FALSE;
		return true;
	}
	bool ClientHelper::sendMsg(LPCSTR msg) {
		if (checkSend() && _sendMessage(msg, msg_type_t::SEND_MSG, msg_type_t::SEND_MSG_FAILED, msg_type_t::SEND_MSG_SUCCESS, (uint32_t)strlen(msg) + 1)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("send message time out\n");
				return false;
			}

			if (except_pkg.msg_type == msg_type_t::SEND_MSG_FAILED) {
				printf("send message failed\n");
				return false;
			}

			return true;
		}
		return false;
	}
	bool ClientHelper::sendBinaryMsg(const char* buffer, uint32_t msgLen) {
		if (checkSend() && _sendMessage(buffer, msg_type_t::SEND_MSG, msg_type_t::SEND_MSG_FAILED, msg_type_t::SEND_MSG_SUCCESS, msgLen)) {
			if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
				printf("send message time out\n");
				return false;
			}

			if (except_pkg.msg_type == msg_type_t::SEND_MSG_FAILED) {
				printf("send message failed\n");
				return false;
			}

			return true;
		}
		return false;
	}
	bool ClientHelper::recvMsg(msg_package_t& m) {
		if (msg_queue.empty()) {
			m.clean();
			return false;
		}
		msg_queue.fetch(m);
		return true;
	}

	bool ClientHelper::checkRoomStatus() {
		return bRoomStatus == TRUE;
	}                                                                                                                                                         

	bool ClientHelper::_sendMessage(const char* msg, msg_type_t msgType, msg_type_t except, msg_type_t except_2, uint32_t msgLen) {
		io_context_t* io_ctx = clientContext->getSendIo();
		io_ctx->socket = clientContext->socket;

		send_pkg.getControl();
		send_pkg.sendTime = getMsTimestamp();
		send_pkg.msg_type = msgType;
		send_pkg.msgLen = msgLen;
		send_pkg.copyIn(msg, msgLen);

		if (!_buildPackage(io_ctx->buffer, MAX_BUFFER_LEN, io_ctx->wsa_buffer.len, send_pkg)) {
			send_pkg.freeControl();
			return false;
		}
		send_pkg.freeControl();

		except_type_first = except;
		except_type_second = except_2;
		return _send_pushToQueue(io_ctx);
	}

	bool ClientHelper::_parseMessage(package_t& pkg) {
		if (pkg.msg_type == except_type_first || pkg.msg_type == except_type_second) {
			except_pkg.copyIn(pkg);
			except_type_first = msg_type_t::PING_MSG;
			except_type_second = msg_type_t::PING_MSG;
			SetEvent(hGlobalEvent);
			return true;
		}
		else if (pkg.msg_type == msg_type_t::ROOM_MSG) {
			msg_t* msg = new msg_t;
			pkg.getControl();
			msg->time = pkg.sendTime;
			msg->latency = uint32_t(getMsTimestamp() - msg->time);
			msg->msgLen = readUint32(pkg.msgContent, 0);
			memcpy(msg->ip, pkg.msgContent + 4, 20);
			memcpy(msg->name, pkg.msgContent + 24, 40);
			if (msg->msgLen > 0) {
				msg->msgContent = new char[msg->msgLen];
				memcpy(msg->msgContent, pkg.msgContent + 64, msg->msgLen);
			}
			pkg.freeControl();
			msg_queue.push(msg);
			return true;
		}
		else if (pkg.msg_type == msg_type_t::ROOM_CLEAN_OUT) {
			printf("room closed\n");
			bRoomStatus = FALSE;
			return true;
		}
		return false;
	}

ClientHelper::ClientHelper() :
	hIOCP(INVALID_HANDLE_VALUE),
	hExitFlag(NULL),
	hGlobalEvent(NULL),
	threads(0),
	workers(NULL),
	bConnectStatus(FALSE),
	bRoomStatus(FALSE),
	clientContext(NULL),
	lpfnConnecter(NULL) {

	init();
}

bool ClientHelper::connectToServer(LPCSTR ip, int port, LPCSTR name) {
	io_context_t* io_ctx = clientContext->getRecvIo();
	io_ctx->socket = clientContext->socket;
	if (!_connect_pushToQueue(io_ctx, ip, port)) {
		return false;
	}

	if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
		quitWithError("connect to server time out!");
	}

	if (_sendMessage(name, msg_type_t::SET_NAME, msg_type_t::SET_NAME_SUCCESS, msg_type_t::SET_NAME_FAILED, (uint32_t)strlen(name) + 1)) {
		if (WaitForSingleObject(hGlobalEvent, timeout) != WAIT_OBJECT_0) {
			quitWithError("set name time out!");
		}

		if (except_pkg.msg_type == msg_type_t::SET_NAME_FAILED) {
			quitWithError("set name failed!");
		}
		return true;
	}

	return false;
}

DWORD __stdcall ClientHelper::workerFnCallerEx(void* lpParam) {
	if (lpParam == nullptr) {
		return 0;
	}

	ClientHelper* lpThis = reinterpret_cast<ClientHelper*>(lpParam);
	return lpThis->workerFn();
}

unsigned int ClientHelper::workerFn() {
	DWORD transferred_bytes = 0;
	socket_context_t* socket_ctx = NULL;
	OVERLAPPED* overlapped = NULL;
	DWORD error_;

	while (WaitForSingleObject(hExitFlag, 0) != WAIT_OBJECT_0) {
		BOOL IOCPStatus = GetQueuedCompletionStatus(hIOCP, &transferred_bytes, (PULONG_PTR)&socket_ctx, &overlapped, INFINITE);//10000U
		error_ = GetLastError();

		if (socket_ctx == 0) { break; }

		if (IOCPStatus) {
			io_context_t* io_ctx = CONTAINING_RECORD(overlapped, io_context_t, overlapped);
			switch (io_ctx->operation) {
			case operation_t::CONNECT_POSTED:
				_connect(socket_ctx, io_ctx);
				break;

			case operation_t::RECV_POSTED:
				_recv(socket_ctx, io_ctx, transferred_bytes);
				break;

			case operation_t::SEND_POSTED:
				//_send(socket_ctx, io_ctx);
				break;
			}
		}
		else {
			if (error_ == WAIT_TIMEOUT) {
				continue;
			}
			else if (error_ == ERROR_NETNAME_DELETED) {
				closesocket(socket_ctx->socket);
				socket_ctx->socket = INVALID_SOCKET;
				break;
			}
			else {
				closesocket(socket_ctx->socket);
				socket_ctx->socket = INVALID_SOCKET;
				break;
			}
		}
	}
	return 0;
}

void ClientHelper::init() {
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int result = WSAStartup(wVersionRequested, &wsaData);
	if (result != 0) { quitWithError("WSAStartup failed"); }

	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	hExitFlag = CreateEventW(NULL, FALSE, FALSE, NULL);
	hGlobalEvent = CreateEventW(NULL, FALSE, FALSE, NULL);

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	threads = si.dwNumberOfProcessors * 2;
	threads = 1;

	workers = new HANDLE[threads];
	for (DWORD i = 0; i < threads; i++) {
		DWORD dwThreadId = 0;
		workers[i] = CreateThread(NULL, 0, workerFnCallerEx, this, 0, &dwThreadId);
	}

	clientContext = new socket_context_t;
	clientContext->socket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	HANDLE h = CreateIoCompletionPort((HANDLE)clientContext->socket, hIOCP, (ULONG_PTR)clientContext, 0);
	if (h != hIOCP) { quitWithError("create Iocp failed"); }

	GUID GuidConnectEx = WSAID_CONNECTEX;
	DWORD bytes = 0;
	result = WSAIoctl(clientContext->socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidConnectEx, sizeof(GuidConnectEx),
		&lpfnConnecter, sizeof(lpfnConnecter),
		&bytes, NULL, NULL);
	if (result == SOCKET_ERROR) { quitWithError("WSAIoctl failed"); }
}

bool ClientHelper::_buildPackage(char* output, const uint32_t bufferLen, ULONG& writeLen, package_t& msg) {
	msg.getControl();
	if (bufferLen < 14UL + msg.msgLen) {
		msg.freeControl();
		return false;
	}

	writeUint16(output, 0, (uint16_t)msg.msg_type);
	writeUint32(output, 2, msg.msgLen);
	writeUint64(output, 6, msg.sendTime);

	msg.copyOut(output + 14, bufferLen - 14);

	writeLen = 14 + msg.msgLen;
	msg.freeControl();
	return true;
}

bool ClientHelper::_parsePackage(const char* input, const uint32_t packageLen, uint32_t& parseLen, package_t& msg_out) {
	msg_out.getControl();

	uint64_t current = getMsTimestamp();
	if (current - msg_out.lastProcess > 1500) {
		msg_out.state = state_t::NEW_PACKAGE;
	}
	msg_out.lastProcess = current;

	switch (msg_out.state) {
	case state_t::NEW_PACKAGE:
		// Header len check
		if (packageLen - parseLen < 14) {
			msg_out.setRemain(input, packageLen - parseLen);
			msg_out.state = state_t::MISS_HEADER;
			parseLen = packageLen;
		}
		else {
			msg_out.state = state_t::READING_HEADER;
		}
		break;
	case state_t::READING_HEADER:
		msg_out.msg_type = (msg_type_t)readUint16(input, 0);
		msg_out.msgLen = readUint32(input, 2);
		msg_out.sendTime = readUint64(input, 6);
		msg_out.msgRead = 0;
		parseLen += 14;
		// Body len check
		if (msg_out.msgLen > 65536) {
			msg_out.state = state_t::PACKAGE_ERROR;
		}
		else if (msg_out.msgLen == 0) {
			msg_out.state = state_t::PACKAGE_READY;
		}
		else if (packageLen - parseLen < msg_out.msgLen) {
			msg_out.state = state_t::MISS_BODY;
		}
		else {
			msg_out.state = state_t::READING_BODY;
		}
		break;
	case state_t::READING_BODY:
		msg_out.copyIn(input, msg_out.msgLen);
		parseLen += msg_out.msgLen;
		msg_out.state = state_t::PACKAGE_READY;
		break;
	case state_t::MISS_HEADER:
		// Header len check
		if (packageLen - parseLen < 14 - msg_out.remainLen) {
			msg_out.appendRemain(input, packageLen - parseLen);
			parseLen = packageLen;
		}
		else {
			parseLen += 14 - msg_out.remainLen;
			msg_out.appendRemain(input, 14 - msg_out.remainLen);

			msg_out.msg_type = (msg_type_t)readUint16(msg_out.lastRemain, 0);
			msg_out.msgLen = readUint32(msg_out.lastRemain, 2);
			msg_out.sendTime = readUint64(msg_out.lastRemain, 6);

			msg_out.clearRemain();

			// Body len check
			if (msg_out.msgLen > 65536) {
				msg_out.state = state_t::PACKAGE_ERROR;
			}
			else if (msg_out.msgLen == 0) {
				msg_out.state = state_t::PACKAGE_READY;
			}
			else if (packageLen - parseLen < msg_out.msgLen) {
				msg_out.state = state_t::MISS_BODY;
			}
			else {
				msg_out.state = state_t::READING_BODY;
			}
		}
		break;
	case state_t::MISS_BODY:
		// Body len check
		if (packageLen - parseLen < msg_out.msgLen - msg_out.msgRead) {
			msg_out.copyAppend(input, packageLen - parseLen);
			parseLen = packageLen;
		}
		else {
			parseLen += msg_out.msgLen - msg_out.msgRead;
			msg_out.copyAppend(input, msg_out.msgLen - msg_out.msgRead);
			msg_out.state = state_t::PACKAGE_READY;
		}
		break;
	case state_t::PACKAGE_ERROR:
		// Pass the error package
		parseLen = packageLen;
		break;
	}

	msg_out.freeControl();
	return true;
}

bool ClientHelper::_connect_pushToQueue(io_context_t* io_ctx, LPCSTR ip, int port) {
	if (io_ctx == NULL)
		return false;
	io_ctx->operation = operation_t::CONNECT_POSTED;
	io_ctx->cleanBuffer();

	// ConnectEx requires the socket to be initially bound.
	struct sockaddr_in addr0 = { 0 };
	addr0.sin_family = AF_INET;
	addr0.sin_addr.s_addr = INADDR_ANY;
	addr0.sin_port = 0;
	int result = bind(io_ctx->socket, (SOCKADDR*)&addr0, sizeof(addr0));
	if (result != 0) {
		quitWithError("bind ip/port failed!");
		return false;
	}

	struct sockaddr_in addr1 = { 0 };
	addr1.sin_family = AF_INET;
	inet_pton(AF_INET, ip, (void*)&addr1.sin_addr.s_addr);
	addr1.sin_port = htons(port);

	result = lpfnConnecter(io_ctx->socket,
		reinterpret_cast<const sockaddr*>(&addr1), sizeof(addr1),
		NULL, 0, NULL, &io_ctx->overlapped);
	int error_ = WSAGetLastError();
	if (result == SOCKET_ERROR && error_ != WSA_IO_PENDING) {
		return false;
	}

	return true;
}

bool ClientHelper::_recv_pushToQueue(io_context_t* io_ctx) {
	if (io_ctx == NULL)
		return false;

	io_ctx->operation = operation_t::RECV_POSTED;
	io_ctx->cleanBuffer();

	DWORD flags = 0;
	int result = WSARecv(io_ctx->socket, &io_ctx->wsa_buffer, 1, &io_ctx->recv_len, &flags, &io_ctx->overlapped, NULL);
	int error_ = WSAGetLastError();
	if (result == SOCKET_ERROR && error_ != WSA_IO_PENDING) {
		printf("_recv_push error: %d", error_);
		return false;
	}

	return true;
}

bool ClientHelper::_send_pushToQueue(io_context_t* io_ctx/*, LPCSTR msg, int msg_len*/) {
	if (io_ctx == NULL)
		return false;

	io_ctx->operation = operation_t::SEND_POSTED;
	// io_ctx->cleanBuffer();
	// memcpy(io_ctx->wsa_buffer.buf, msg, msg_len);
	// io_ctx->wsa_buffer.len = msg_len;

	int result = WSASend(io_ctx->socket, &io_ctx->wsa_buffer, 1, &io_ctx->sent_len, 0, &io_ctx->overlapped, NULL); // &io_ctx->overlapped
	int error_ = WSAGetLastError();
	if (result == SOCKET_ERROR && error_ != WSA_IO_PENDING) {
		printf("_send_push error: %d", error_);
		return false;
	}

	return true;
}

bool ClientHelper::_connect(socket_context_t* socket_ctx, io_context_t* io_ctx) {
	SetEvent(hGlobalEvent);

	bConnectStatus = TRUE;

	if (!_recv_pushToQueue(io_ctx)) {
		quitWithError("_connect_pushRecvQueue failed!");
		return false;
	}

	return true;
}

bool ClientHelper::_recv(socket_context_t* socket_ctx, io_context_t* io_ctx, uint32_t recv_len) {
	//printf("receive len %u\n", recv_len);
	recv_pkg.getControl();
	uint32_t parseLen = 0;
	while (recv_len - parseLen > 0) {
		_parsePackage(io_ctx->wsa_buffer.buf + parseLen, recv_len, parseLen, recv_pkg);
		if (recv_pkg.state == state_t::PACKAGE_READY) {
			//printf("receive type %u, len %u, time %llu, diff %llu\n", recv_pkg.msg_type, recv_pkg.msgLen, recv_pkg.sendTime, getMsTimestamp() - recv_pkg.sendTime);
			_parseMessage(recv_pkg);
			recv_pkg.state = state_t::NEW_PACKAGE;
		}
	}
	recv_pkg.freeControl();

	if (!_recv_pushToQueue(io_ctx)) {
		printf("_recv_pushRecvQueue failed!\n");
		return false;
	}

	return true;
}

bool ClientHelper::_send(socket_context_t* socket_ctx, io_context_t* io_ctx) {
	return true;
}

ClientHelper::~ClientHelper() {
	SetEvent(hExitFlag);
	SetEvent(hGlobalEvent);
	CloseHandle(hExitFlag);
	CloseHandle(hGlobalEvent);
	for (size_t i = 0; i < threads; i++) {
		PostQueuedCompletionStatus(hIOCP, 0, 0U, NULL);
	}
	closesocket(clientContext->socket);
	if (clientContext != NULL) {
		delete clientContext;
		clientContext = NULL;
	}
	if (WAIT_OBJECT_0 != WaitForMultipleObjects(threads, workers, TRUE, 5000))
		printf("WaitForMultipleObjects() failed: %d\n", GetLastError());
	else
		for (DWORD i = 0; i < threads; i++) {
			if (workers[i] != INVALID_HANDLE_VALUE) CloseHandle(workers[i]);
			workers[i] = INVALID_HANDLE_VALUE;
		}
	delete[] workers;
	WSACleanup();
	CloseHandle(hIOCP);
}

}
