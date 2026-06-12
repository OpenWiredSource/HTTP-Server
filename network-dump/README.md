# Network Analysis

The bash script here dumps any network traffic happening within the local interface (lo).

> Note: Make sure to reconfigure tshark ownership non-root

```bash
tshark -D | grep "Loopback"
tshark -i lo -f "tcp port 8081" >> server_traffic.pcap
```
