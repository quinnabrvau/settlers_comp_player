FROM ubuntu
RUN apt-get update &&       \
    apt-get intall -y gcc   \
    apt-get intall -y cmake 
