FROM guusk/docker-systemc:2.3.0
MAINTAINER Pedro Cuadra <pjcuadra@gmail.com>

RUN apt-get update
RUN apt-get install -y git vim openssh-server supervisor unzip
RUN apt-get autoremove

RUN cp -r /usr/local/systemc-2.3.0/* /usr/local/
RUN update-rc.d ssh defaults

RUN echo 'root:password' | chpasswd
RUN sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

RUN mkdir -p /var/run/sshd

EXPOSE 22
CMD ["/usr/bin/supervisord"]
