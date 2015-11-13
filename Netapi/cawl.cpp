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
	value = 1;
	//metrics = GatherMetrics();
	//-----------------------------------------------------------------------------------------------------
	memset(&heartbeat,  0, sizeof(struct sctp_paddrparams));
	memset(&rtoinfo,    0, sizeof(struct sctp_rtoinfo));
	memset(&assoc,      0, sizeof(struct sctp_assocparams));
	memset(&srttinfo,   0, sizeof(struct sctp_paddrinfo));
	//-----------------------------------------------------------------------------------------------------
	heartbeat.spp_flags 		= SPP_HB_ENABLE;
	heartbeat.spp_hbinterval 	= 3000;				//default value: 15000
	heartbeat.spp_pathmaxrxt 	= 1;				//default value: 5
	rtoinfo.srto_max 			= 6000;				//default value: 60000
	rtoinfo.srto_min 			= 500;				//default value: 1000
	rtoinfo.srto_initial		= 1500;				//default value: 3000
	assoc.sasoc_asocmaxrxt		= 6;
	event.sctp_data_io_event    = 1;
	//-----------------------------------------------------------------------------------------------------
	SctpSocket = socket;
	/*Disable Nagles algorithm*/
	if(setsockopt(SctpSocket, IPPROTO_SCTP, SCTP_NODELAY, &value,sizeof(value))<0){
		::exit(EXIT_FAILURE);
		//Logga fel
		perror("setsockopt");
	}
	/*Disable fragmentation*/
	if(setsockopt(SctpSocket, IPPROTO_SCTP, SCTP_DISABLE_FRAGMENTS, &value,sizeof(value))<0){
		::exit(EXIT_FAILURE);
		//	Logga fel
	}
	/*Set Heartbeats*/
	if(setsockopt(socket, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS , &heartbeat, sizeof(heartbeat)) != 0){
		perror("setsockopt");
	}
	/*Set srttinfo struct*/
	if(setsockopt(socket, IPPROTO_SCTP, SCTP_GET_PEER_ADDR_INFO , &srttinfo, sizeof(srttinfo)) != 0){
		perror("setsockopt");
	}
	/*Set rto_max*/
	if(setsockopt(socket, IPPROTO_SCTP, SCTP_RTOINFO , &rtoinfo, sizeof(rtoinfo)) != 0){
		::exit(EXIT_FAILURE);
	}
	/*Set association_max_retransmit*/
	if(setsockopt(socket, IPPROTO_SCTP, SCTP_ASSOCINFO , &assoc, sizeof(assoc)) != 0){
		::exit(EXIT_FAILURE);
	}
	if(setsockopt(socket, IPPROTO_SCTP, SCTP_EVENTS, &event, sizeof(struct sctp_event_subscribe)) < 0){
		//printf("After setsockopt errno: %d\n", errno);
		::exit(EXIT_FAILURE);
	}

	/*
	 * PF_RETRANS, Not available through socket or sysctl.h
	 * need to recompile sctp with new source update kernel etc.
	 * to access variable
	 *
	 * is default 0, draft-nishida-tsvwg-sctp-failover-00, suggest 5
	 *
	 * could be done by piping command to bash >> sysctl
	default 0			same as def PMR	5?
    FILE* pipe = popen("sudo sysctl -w net.sctp.pf_retrans=5", "r");
    if (!pipe) return "ERROR";
    pclose(pipe);

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    must then be set back to default in destructor
	 */
}

Netapi::cawl::~cawl() {
	//redo kernel specific default values set by sysctl

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

	socklen_t opt_len = (socklen_t) sizeof(struct sctp_status);
	if(getsockopt(s, IPPROTO_SCTP, SCTP_STATUS, &status, &opt_len) < 0){
		//What to do?
		return 0;
	}
	int sstat_assoc_id 				= status.sstat_assoc_id;
	int sstat_state					= status.sstat_state;
	int sstat_rwnd					= status.sstat_rwnd;
	int sstat_unackdata				= status.sstat_unackdata;
	int sstat_penddata				= status.sstat_penddata;
	int sstat_instrms				= status.sstat_instrms;
	int sstat_outstrms				= status.sstat_outstrms;
	int sstat_fragmentation_point	= status.sstat_fragmentation_point;

	//std::string spinfo_address		= status.sstat_primary.spinfo_address;
	int spinfo_state				= status.sstat_primary.spinfo_state;
	int spinfo_cwnd					= status.sstat_primary.spinfo_cwnd;
	int spinfo_srtt					= status.sstat_primary.spinfo_srtt;
	int spinfo_rto					= status.sstat_primary.spinfo_rto;
	int spinfo_mtu					= status.sstat_primary.spinfo_mtu;


	printf("srtt: %d\n", spinfo_srtt);
	int ret = ::sctp_sendmsg(s, msg, len, to, tolen, ppid, flags, stream_no, timetolive, context);
	return ret;
}

int Netapi::cawl::sctp_send(int s, const void* msg, size_t len,
		const struct sctp_sndrcvinfo* sinfo, int flags) {

	int ret = ::sctp_send(s, msg, len, sinfo, flags);
	updateStatus(s, sinfo->sinfo_assoc_id ,&status);
	//metrics.setMeasure(STATUS,);

	return ret;
}

int Netapi::cawl::sctp_recvmsg(int s, void* msg, size_t len,
		struct sockaddr* from, socklen_t* fromlen,
		struct sctp_sndrcvinfo* sinfo, int* msg_flags) {

	return ::sctp_recvmsg(s, msg, len, from, fromlen, sinfo, msg_flags);
	updateStatus(s, sinfo->sinfo_assoc_id ,&status);
	//metrics.setMeasure(STATUS,);

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

int Netapi::cawl::updateStatus(int SctpSock, sctp_assoc_t id,
		sctp_status* stat){

	socklen_t opt_len = (socklen_t) sizeof(struct sctp_status);

	int ret = sctp_opt_info(SctpSock,status.sstat_assoc_id,
			SCTP_STATUS,&stat,&opt_len);
	return ret;
}
