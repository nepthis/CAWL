/*
 * cawl.c
 *
 *  Created on: May 7, 2014
 *      Author: Håkan Therén
 *
 *      Thanks to alexkr.com for examples of SCTP implementaion
 *
 *      Description: Class to use within CAWL-framework for sending and recieving data
 *      			 via SCTP in an easy to use manner.
 *
 */

#include "cawl.h"

Netapi::cawl::cawl(int socket) {
	long value = 1;
	SctpSocket = socket;
	if(setsockopt(SctpSocket, IPPROTO_SCTP, SCTP_NODELAY, &value,sizeof(value))<0){
		::exit(EXIT_FAILURE);
		//Logga fel
	}
	if(setsockopt(SctpSocket, IPPROTO_SCTP, SCTP_DISABLE_FRAGMENTS, &value,sizeof(value))<0){
		::exit(EXIT_FAILURE);
		//Logga fel
	}
	//Borde ändra RTO etc i sysctl
}

Netapi::cawl::~cawl() {
}

int Netapi::cawl::sctp_bindx(int sd, struct sockaddr* addrs, int addrcnt,
		int flags) {
	return ::sctp_bindx(sd, addrs, addrcnt, flags);
}

int Netapi::cawl::sctp_connectx(int sd, struct sockaddr* addrs, int addrcnt,
		sctp_assoc_t* id) {
	return ::sctp_connectx(sd, addrs, addrcnt, id);
}

int Netapi::cawl::sctp_sendmsg(int s, const void* msg, size_t len,
		struct sockaddr* to, socklen_t tolen, uint32_t ppid, uint32_t flags,
		uint16_t stream_no, uint32_t timetolive, uint32_t context) {
	return ::sctp_sendmsg(s, msg, len, to, tolen, ppid, flags, stream_no, timetolive, context);
}

int Netapi::cawl::sctp_send(int s, const void* msg, size_t len,
		const struct sctp_sndrcvinfo* sinfo, int flags) {
	return ::sctp_send(s, msg, len, sinfo, flags);
}

int Netapi::cawl::sctp_recvmsg(int s, void* msg, size_t len,
		struct sockaddr* from, socklen_t* fromlen,
		struct sctp_sndrcvinfo* sinfo, int* msg_flags) {
	return ::sctp_recvmsg(s, msg, len, from, fromlen, sinfo, msg_flags);
}

int Netapi::cawl::sctp_peeloff(int sd, sctp_assoc_t assoc_id) {
	return ::sctp_peeloff(sd, assoc_id);
}

int Netapi::cawl::sctp_opt_info(int sd, sctp_assoc_t id, int opt, void* arg,
		socklen_t* size) {
	return ::sctp_opt_info(sd, id, opt, arg, size);
}

int Netapi::cawl::sctp_getpaddrs(int sd, sctp_assoc_t id,
		struct sockaddr** addrs) {
	return ::sctp_getpaddrs(sd, id, addrs);
}

int Netapi::cawl::sctp_freepaddrs(struct sockaddr* addrs) {
	return ::sctp_freepaddrs( addrs);

}

int Netapi::cawl::sctp_getladdrs(int sd, sctp_assoc_t id,
		struct sockaddr** addrs) {
	return ::sctp_getladdrs(sd, id, addrs);
}

int Netapi::cawl::sctp_freeladdrs(struct sockaddr* addrs) {
	return ::sctp_freeladdrs(addrs);
}

int Netapi::cawl::sctp_getaddrlen(sa_family_t family) {
	return sctp_getaddrlen(family);
}
