#!/bin/bash
echo "Running SSH daemon in bash script to test SSH protocols with localhost..."
/usr/bin/sshd -E sshd_out.log
echo "Using standard port 22. Ensure localhost connections are routed to this port."
read -p "Press <enter> to continue:"

