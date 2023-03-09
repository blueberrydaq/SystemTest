# Download base image
FROM ubuntu:22.04

# Output on terminal
#RUN echo "Simulator docker started."

# Label about custom image
#LABEL maintainer=""
#LABEL version="0.1"
#LABEL description="Docker image to run a simulated blueberry device for systemtest."

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive

# create directory to copy simulator to
RUN mkdir /usr/share/Simulator

# copy simulator to docker image
COPY simulator /usr/share/Simulator

# copy .deb package with bb binaries to docker image
COPY Blueberry-0.4.0-deb64.deb /usr/share/Simulator

# define a wokring directory so no need for further prefixing the path
WORKDIR /usr/share/Simulator

# install .deb package with bb binaries
RUN dpkg -i Blueberry-0.4.0-deb64.deb

# run copied simulator
CMD ./simulator