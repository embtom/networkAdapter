FROM ubuntu:18.04

RUN apt-get update && apt-get install -y --no-install-recommends apt-utils \
  sudo git libcap2-bin\
  build-essential cmake \
  pbuilder ubuntu-dev-tools debootstrap devscripts quilt dh-make fakeroot\
  reprepro nginx \
  && rm -rf /var/lib/apt/lists/*
# Setup default user, when enter docker container

ARG USER=tom
ARG PASS=tom
ARG UID=1000
ARG GID=1000

#Create build user and add needed files
RUN useradd -m ${USER} --uid=${UID} -s /bin/bash
RUN echo "${USER}:${PASS}" | chpasswd
RUN usermod -aG sudo ${USER}
RUN echo "${USER} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Configure reprepro
RUN mkdir -p /var/lib/reprepro/conf
ADD configs/reprepro-distributions /var/lib/reprepro/conf/distributions
RUN reprepro -b /var/lib/reprepro export bionic

# Configure nginx
RUN echo "daemon off;" >> /etc/nginx/nginx.conf
RUN rm -f /etc/nginx/sites-enabled/default
ADD configs/nginx-default.conf /etc/nginx/sites-enabled/default

## add permissions for nginx user
RUN     chown -R ${UID}:${GID} /var/lib/nginx && \
        chown -R ${UID}:${GID} /var/log/nginx && \
        chown -R ${UID}:${GID} /etc/nginx/conf.d
RUN touch /var/run/nginx.pid && \
        chown -R ${UID}:${GID} /var/run/nginx.pid
RUN setcap 'cap_net_bind_service=+ep' /usr/sbin/nginx

RUN chown -R ${UID}:${GID} /var/lib/reprepro

EXPOSE 80

USER ${UID}:${GID}
WORKDIR /home/${USER}
RUN git config --global http.sslverify false
ADD build_dependencies.sh /home/${USER}/build_dependencies.sh
RUN sudo chmod +x /home/${USER}/build_dependencies.sh
ADD import_dependencies.sh /home/${USER}/import_dependencies.sh
RUN sudo chmod +x /home/${USER}/import_dependencies.sh
ADD build_netadapter.sh /home/${USER}/build_netadapter.sh
RUN sudo chmod +x /home/${USER}/build_netadapter.sh

