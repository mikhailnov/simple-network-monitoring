# Simple network monitoring

This program is intended to be run via cron, e.g. every minute, or as a "daemon" (via e.g. systemd) and logs when one of defined IPs or hosts is not reachable.

Example installation:

```
make
make run
sudo install -m0755 network-monitoring /usr/local/bin/network-monitoring
```

Example `crontab` (`crontab -u root -e`):

```
* * * * * /usr/local/bin/network-monitoring >/dev/null 2>&1
```

Checked to work on Linux, FreeBSD and DragonFlyBSD.
