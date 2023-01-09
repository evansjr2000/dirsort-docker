FROM debian as build-stage
RUN /bin/sh -c set -ex; apt-get update -y; apt-get upgrade -y;
RUN apt-get install -y build-essential
RUN apt-get install -y --no-install-recommends make;
RUN apt-get install -y --no-install-recommends autoconf automake bzip2	dpkg-dev file
RUN apt-get install -y --no-install-recommends  g++ 
RUN apt-get install -y --no-install-recommends gcc
RUN apt-get install -y --no-install-recommends cmake
RUN apt-get install -y --no-install-recommends libbz2-dev libc6-dev libcurl4-openssl-dev
RUN apt-get install -y libdb-dev libevent-dev libffi-dev libgdbm-dev libglib2.0-dev 		libgmp-dev 		libjpeg-dev 		libkrb5-dev 		liblzma-dev 		libmagickcore-dev 		libmagickwand-dev 		libmaxminddb-dev 	libncurses5-dev 		libncursesw5-dev 		libpng-dev 		libpq-dev 		libreadline-dev 	libsqlite3-dev 		libssl-dev 		libtool 		libwebp-dev 		libxml2-dev 		libxslt-dev 		libyaml-dev patch
RUN apt-get install -y --no-install-recommends unzip xz-utils zlib1g-dev \
$(if apt-cache show 'default-libmysqlclient-dev' 2>/dev/null | grep -q '^Version:'; then 			echo 'default-libmysqlclient-dev'; 			else 				echo 'libmysqlclient-dev'; 		fi 		) 	; 	rm -rf /var/lib/apt/lists/*
COPY . .
RUN type make
RUN cd /complete-apue/lib.svr4; make clean; make

FROM debian:buster
COPY --from=build-stage ./dirsort/dirsort ./dirsort
