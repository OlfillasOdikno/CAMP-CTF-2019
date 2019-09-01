FROM php:7.2-apache

MAINTAINER localo & A2nkF
LABEL CHALLENGE="Camp CTF 2019 noname"

RUN apt update && apt install -y python3 python3-pip
RUN pip3 install pycrypto

RUN a2enmod rewrite

COPY www /srv/www

COPY server.conf /etc/apache2/sites-available/000-default.conf
RUN docker-php-ext-install mysqli

COPY init.sh /root/init.sh
RUN chmod +x /root/init.sh
WORKDIR /var/www/html
CMD /root/init.sh