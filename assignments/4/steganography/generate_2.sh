#!/bin/bash

cp originals/original_2.jpg ./

steghide embed -cf original_2.jpg -ef data/secret.txt -p 123

sha1sum originals/original_2.jpg > originals/original_2.sha1
sha1sum original_2.jpg > original_2.sha1

