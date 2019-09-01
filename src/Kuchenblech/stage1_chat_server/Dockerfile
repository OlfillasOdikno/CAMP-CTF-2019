FROM python

MAINTAINER localo & A2nkF

LABEL CHALLENGE="Camp CTF 2019 noname"

RUN pip install websockets

RUN useradd -d /home/ctf/ -m -p ctf -s /bin/bash ctf
RUN echo "ctf:ctf" | chpasswd
WORKDIR /home/ctf
COPY msg_server.py .
RUN chown ctf:ctf ./msg_server.py

USER ctf

ENTRYPOINT python msg_server.py
