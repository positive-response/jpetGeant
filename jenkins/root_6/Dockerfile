FROM ubuntu:xenial
LABEL MAINTAINER Aleksander Gajos <aleksander.gajos@uj.edu.pl>

RUN apt-get update && apt-get -y install git libboost-all-dev libtbb-dev cmake libfftw3-3 libfftw3-dev sshpass wget g++
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /

RUN wget https://root.cern/download/root_v6.12.06.Linux-ubuntu16-x86_64-gcc5.4.tar.gz
RUN tar xzf root_v6.12.06.Linux-ubuntu16-x86_64-gcc5.4.tar.gz
RUN rm /root_v6.12.06.Linux-ubuntu16-x86_64-gcc5.4.tar.gz

RUN mkdir -p geant4
RUN mkdir -p geant4-install
RUN wget https://github.com/Geant4/geant4/archive/v10.4.2.tar.gz -P geant4

WORKDIR /geant4
RUN tar xvxf v10.4.2.tar.gz
RUN mkdir -p geant4-10.4.2/build && cd geant4-10.4.2/build && cmake -DCMAKE_INSTALL_PREFIX=/geant4-install .. && make && make install

WORKDIR /J-PET-geant4
COPY docker-entrypoint.sh /usr/local/bin/
RUN chmod 777 /usr/local/bin/docker-entrypoint.sh && ln -s /usr/local/bin/docker-entrypoint.sh / && ln -s /usr/local/bin/docker-entrypoint.sh /J-PET-geant4
ENTRYPOINT [ "bash", "-c", "docker-entrypoint.sh" ]
