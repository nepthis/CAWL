/*
 * Server.h
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-mobile
 */

#ifndef CAWL_H_
#define CAWL_H_

#include <netinet/sctp.h>
#include <cstdlib>
#include <sys/sysctl.h>
#include <iostream>
#include <stdio.h>
//#include "../Netapi/GatherMetrics.h"

namespace Netapi{

class cawl {
private:
	int SctpSocket;

    int sysctlopts[] = {CTL_KERN, KERN_PRINTK};
    int newval[2] = {};
public:
	cawl(int socket);
	~cawl();

	int sctp_bindx(int sd, struct sockaddr *addrs, int addrcnt, int flags);

	int sctp_connectx(int sd, struct sockaddr *addrs, int addrcnt,
			sctp_assoc_t *id);


	/* This library function assists the user with the advanced features
	 * of SCTP.  This is a new SCTP API described in the section 8.7 of the
	 * Sockets API Extensions for SCTP. This is implemented using the
	 * sendmsg() interface.
	 */

	int sctp_sendmsg(int s, const void *msg, size_t len, struct sockaddr *to,
			socklen_t tolen, uint32_t ppid, uint32_t flags,
			uint16_t stream_no, uint32_t timetolive, uint32_t context);

	/* This library function assist the user with sending a message without
	 * dealing directly with the CMSG header.
	 */

	int sctp_send(int s, const void *msg, size_t len,
			const struct sctp_sndrcvinfo *sinfo, int flags);

	/* This library function assists the user with the advanced features
	 * of SCTP.  This is a new SCTP API described in the section 8.8 of the
	 * Sockets API Extensions for SCTP. This is implemented using the
	 * recvmsg() interface.
	 */

	int sctp_recvmsg(int s, void *msg, size_t len, struct sockaddr *from,
			socklen_t *fromlen, struct sctp_sndrcvinfo *sinfo,
			int *msg_flags);

	int sctp_peeloff(int sd, sctp_assoc_t assoc_id);

	/* Prototype for the library function sctp_opt_info defined in
	 * API 7. Socket Options.
	 */
	int sctp_opt_info(int sd, sctp_assoc_t id, int opt, void *arg, socklen_t *size);

	/* Get all peer address on a socket.  This is a new SCTP API
	 * described in the section 8.3 of the Sockets API Extensions for SCTP.
	 * This is implemented using the getsockopt() interface.
	 */
	int sctp_getpaddrs(int sd, sctp_assoc_t id, struct sockaddr **addrs);

	/* Frees all resources allocated by sctp_getpaddrs().  This is a new SCTP API
	 * described in the section 8.4 of the Sockets API Extensions for SCTP.
	 */
	int sctp_freepaddrs(struct sockaddr *addrs);

	/* Get all locally bound address on a socket.  This is a new SCTP API
	 * described in the section 8.5 of the Sockets API Extensions for SCTP.
	 * This is implemented using the getsockopt() interface.
	 */
	int sctp_getladdrs(int sd, sctp_assoc_t id, struct sockaddr **addrs);

	/* Frees all resources allocated by sctp_getladdrs().  This is a new SCTP API
	 * described in the section 8.6 of the Sockets API Extensions for SCTP.
	 */
	int sctp_freeladdrs(struct sockaddr *addrs);

	/* Return the address length for an address family. */
	int sctp_getaddrlen(sa_family_t family);
};
}
#endif /* CAWL_H_ */
