tshark -D | grep "Loopback"
tshark -i lo -f "tcp port 8081" >> server_traffic.pcap
