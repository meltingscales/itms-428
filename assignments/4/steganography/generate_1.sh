#!/bin/bash

cp originals/original_1.jpg ./

steghide embed -cf original_1.jpg -ef data/acheron.zip -p 123

sha1sum originals/original_1.jpg > originals/original_1.sha1
sha1sum original_1.jpg > original_1.sha1

