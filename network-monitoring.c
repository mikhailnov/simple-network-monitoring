/* This program monitors and logs network reachability
 * of multiple hosts. The idea is to run this monitoring
 * from multiple hosts in different networks and collect
 * logs to understand:
 * - are there any problems with network reachability
 * - if yes, how often do they happen
 * - ...and do they happen in all location at the same time
 * Must be able to run on Linux, FreeBSD, DragonFlyBSD
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

#ifdef __linux__
// for GNU ping
// Requires root!
#define PING_TIME "0,02"
#else
// for BSD ping
#define PING_TIME "0.02"
#endif

// "81.177.174.52",
char* hosts[]= {"81.176.226.156", "81.177.142.42", "81.177.142.83", "81.177.48.8", NULL};

int _log(char* host){
	char* logdir;
	char* logfile = "network-monitoring.csv";
	FILE *fp;
	int rc = -1;
	// getenv() returns a pointer to address where value is stored,
	// so LOGDIR > 8 bytes will work
	if (getenv("LOGDIR")) {
		logdir = getenv("LOGDIR");
	} else {
		logdir = "/var/log";
	}
	// +1 for "/", +1 for EOL
	char *path = malloc(strlen(logdir) + 1 + strlen(logfile) + 1);
	sprintf(path, "%s/%s", logdir, logfile);
	fprintf(stderr, "LOGDIR: %s\n", logdir);
	fprintf(stderr, "LOGFILE: %s\n", path);
	DIR* dir = opendir(logdir);
	if (dir) {
		goto write_file;
	} else {
		goto create_dir;
	}
create_dir:
	if (!(mkdir(logdir, 0755))) {
		fprintf(stderr, "Error creating directory %s\n", logdir);
		rc = -1;
		goto end;
	} else {
		dir = opendir(logdir);
		if (!(dir)) {
			fprintf(stderr, "Error openning created directory %s\n", logdir);
			return -1;
		}
	}
write_file:
	fp = fopen(path, "a");
	if (fp == NULL) {
		fprintf(stderr, "Error openning file %s for appending\n", path);
		rc = -1;
		goto end;
	}
	fprintf(fp, "%ld;ERROR;%s\n", time(NULL), host);
	if (fclose(fp) != 0) {
		fprintf(stderr, "Error closing file %s\n", path);
		rc = -1;
		goto end;
	}
	rc = 0;
end:
	closedir(dir);
	return rc;
}

int _ping(){
	int rc = -1;
	char cmd[100];
	for (int i = 0; hosts[i] != NULL; i++) {
		sprintf(cmd, "ping -c 3 -s 200 -i %s %s 2>/dev/null 1>&2", PING_TIME, hosts[i]);
		if (!(getenv("NETWORK_MONITORING_NO_VERBOSE")))
			fprintf(stderr, "running %s ...\n", cmd);
		rc = system(cmd);
		if (rc != 0) {
			fprintf(stderr, "Error pinging %s\n", hosts[i]);
			// TODO: add fallback to syslog()
			// TODO: it is not optimal to open/close file in _log() on every iteration
			_log(hosts[1]);
		}
	}
	return rc;
}

int main(){
	int rc = -1;
	fprintf(stderr, "%s\n", "Welcome to network monitoring");
	if (getenv("NETWORK_MONITORING_NO_LOOP")) {
		rc = _ping();
	} else {
		for (;;) {
			// ctrl+c must stop it
			rc = _ping();
		}
	}
	return rc;
}
