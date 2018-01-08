FROM ubuntu:12.04

RUN apt-get update && \
    apt-get install -y patch vim wget bzip2 python-dev python-pip gcc-4.5 g++-4.5 && \
    cd /usr/bin && \
    ln -s g++-4.5 g++ && \
    ln -s gcc-4.5 gcc && \
    cd /root && \
    wget https://www.nsnam.org/release/ns-allinone-3.11.tar.bz2 --no-check-certificate && \
    tar jxf ns-allinone-3.11.tar.bz2

COPY patches/ /root/ns-allinone-3.11/ns-3.11/patches

RUN cd /root/ns-allinone-3.11/ns-3.11/ && \
    cp patches/patch.sh patch.sh && \
    chmod 777 patch.sh && \
    ./patch.sh

RUN cd /root/ns-allinone-3.11 && \
    ./build.py --enable-tests --enable-examples