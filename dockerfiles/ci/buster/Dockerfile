FROM debian:buster AS base

ENV LANG=C.UTF-8
ENV DEBIAN_FRONTEND=noninteractive

ENV RUNTIME_DEPS \
    apache2 \
    apache2-dev \
    ca-certificates \
    clang-format \
    curl \
    debian-goodies \
    gdb \
    git \
    less \
    libcurl4-openssl-dev \
    libedit-dev \
    libmcrypt-dev \
    libonig-dev \
    libpq-dev \
    libsodium-dev \
    libsqlite3-dev \
    libssl-dev \
    libxml2-dev \
    libzip-dev \
    netbase \
    nginx \
    sudo \
    unzip \
    valgrind \
    vim \
    xz-utils \
    zip \
    zlib1g-dev

ENV PHPIZE_DEPS \
    autoconf \
    bison \
    dpkg-dev \
    file \
    g++ \
    gcc \
    libc-dev \
    make \
    pkg-config \
    re2c

RUN set -eux; \
# Set timezone to UTC by default
    ln -sf /usr/share/zoneinfo/Etc/UTC /etc/localtime; \
    \
# Use unicode
    locale-gen C.UTF-8 || true; \
    \
# Core Dumps
    ulimit -c unlimited; \
    \
# Ensure debug symbols are available
    echo "deb http://deb.debian.org/debian-debug/ buster-debug main" | \
        tee -a /etc/apt/sources.list; \
    \
# prevent Debian's PHP packages from being installed
# https://github.com/docker-library/php/pull/542
    { \
        echo 'Package: php*'; \
        echo 'Pin: release *'; \
        echo 'Pin-Priority: -1'; \
    } > /etc/apt/preferences.d/no-debian-php; \
    \
# persistent / runtime deps
    apt-get update; \
    apt-get install -y --no-install-recommends \
        $PHPIZE_DEPS \
        $RUNTIME_DEPS; \
    \
# circleci user + sudo
    groupadd --gid 3434 circleci; \
        useradd --uid 3434 --gid circleci --shell /bin/bash --create-home circleci; \
        echo 'circleci ALL=NOPASSWD: ALL' >> /etc/sudoers.d/50-circleci; \
        echo 'Defaults    env_keep += "DEBIAN_FRONTEND"' >> /etc/sudoers.d/env_keep; \
    \
# Allow nginx to be run as non-root for tests
    chown -R circleci:circleci /var/log/nginx/ /var/lib/nginx/; \
# JSON parser
    export JQ_URL="https://circle-downloads.s3.amazonaws.com/circleci-images/cache/linux-amd64/jq-latest"; \
    curl --silent --show-error --location --fail --retry 3 --output /usr/bin/jq $JQ_URL; \
    chmod +x /usr/bin/jq; \
    jq --version; \
    \
# Docker
    export COMPOSE_URL="https://circle-downloads.s3.amazonaws.com/circleci-images/cache/linux-amd64/docker-compose-latest"; \
    curl --silent --show-error --location --fail --retry 3 --output /usr/bin/docker-compose $COMPOSE_URL; \
    chmod +x /usr/bin/docker-compose; \
    docker-compose version; \
    \
    export DOCKERIZE_URL="https://circle-downloads.s3.amazonaws.com/circleci-images/cache/linux-amd64/dockerize-latest.tar.gz"; \
    curl --silent --show-error --location --fail --retry 3 --output /tmp/dockerize-linux-amd64.tar.gz $DOCKERIZE_URL; \
    tar -C /usr/local/bin -xzvf /tmp/dockerize-linux-amd64.tar.gz; \
    rm -rf /tmp/dockerize-linux-amd64.tar.gz; \
    dockerize --version;

# Apache config
ENV APACHE_CONFDIR /etc/apache2
ENV APACHE_ENVVARS $APACHE_CONFDIR/envvars

RUN set -eux; \
# generically convert lines like
#   export APACHE_RUN_USER=www-data
# into
#   : ${APACHE_RUN_USER:=www-data}
#   export APACHE_RUN_USER
# so that they can be overridden at runtime ("-e APACHE_RUN_USER=...")
    sed -ri 's/^export ([^=]+)=(.*)$/: ${\1:=\2}\nexport \1/' "$APACHE_ENVVARS"; \
    \
# setup directories and permissions
    . "$APACHE_ENVVARS"; \
    for dir in \
        "$APACHE_LOCK_DIR" \
        "$APACHE_RUN_DIR" \
        "$APACHE_LOG_DIR" \
    ; do \
        rm -rvf "$dir"; \
        mkdir -p "$dir"; \
        chown "$APACHE_RUN_USER:$APACHE_RUN_GROUP" "$dir"; \
# allow running as an arbitrary user (https://github.com/docker-library/php/issues/743)
        chmod 777 "$dir"; \
    done; \
    \
# delete the "index.html" that installing Apache drops in here
    rm -rvf /var/www/html/*; \
    \
# logs should go to stdout / stderr
    ln -sfT /dev/stderr "$APACHE_LOG_DIR/error.log"; \
    ln -sfT /dev/stdout "$APACHE_LOG_DIR/access.log"; \
    ln -sfT /dev/stdout "$APACHE_LOG_DIR/other_vhosts_access.log"; \
    chown -R --no-dereference "$APACHE_RUN_USER:$APACHE_RUN_GROUP" "$APACHE_LOG_DIR"; \
    \
# Apache + PHP requires preforking Apache for best results
    a2dismod mpm_event && a2enmod mpm_prefork ;\
# PHP files should be handled by PHP, and should be preferred over any other file type
    { \
        echo '<FilesMatch \.php$>'; \
        echo '\tSetHandler application/x-httpd-php'; \
        echo '</FilesMatch>'; \
        echo; \
        echo 'DirectoryIndex disabled'; \
        echo 'DirectoryIndex index.php index.html'; \
        echo; \
        echo '<Directory /var/www/>'; \
        echo '\tOptions -Indexes'; \
        echo '\tAllowOverride All'; \
        echo '</Directory>'; \
    } | tee "$APACHE_CONFDIR/conf-available/docker-php.conf" \
    && a2enconf docker-php;

# Install all the PHP versions
ENV PHP_SRC_DIR=/usr/local/src/php
ENV PHP_INSTALL_DIR=/opt/php

RUN set -eux; \
# Setup php source directory
    mkdir -p $PHP_SRC_DIR; \
    chown -R circleci:circleci /usr/local/src; \
# Setup php install directory
    mkdir -p $PHP_INSTALL_DIR; \
    chown -R circleci:circleci /opt;

# TODO Install PHP 5 deps into /opt/
# PHP 5
#FROM base AS base5
#COPY install-php-5 /usr/local/bin/
#FROM base5 AS php-54
#RUN install-php-5 5.4 https://www.php.net/get/php-5.4.45.tar.gz/from/this/mirror
#RUN install-php-5 5.5 https://www.php.net/get/php-5.5.38.tar.gz/from/this/mirror
#RUN install-php-5 5.6 https://www.php.net/get/php-5.6.40.tar.gz/from/this/mirror

# PHP 7
# TODO Install PHP 7 deps into /opt/
#FROM base AS base7
#COPY install-php-7 /usr/local/bin/
#FROM base7 AS php-70
#RUN install-php-7 7.0 https://www.php.net/get/php-7.0.33.tar.gz/from/this/mirror
#RUN install-php-7 7.1 https://www.php.net/get/php-7.1.33.tar.gz/from/this/mirror
#RUN install-php-7 7.2 https://www.php.net/get/php-7.2.33.tar.gz/from/this/mirror
#RUN install-php-7 7.3 https://www.php.net/get/php-7.3.21.tar.gz/from/this/mirror
#FROM base7 AS php-74
#RUN install-php-7 7.4 https://www.php.net/get/php-7.4.9.tar.gz/from/this/mirror
#RUN install-php-7-exts 7.4

# PHP 8
FROM base AS base8
COPY install-php-8 /usr/local/bin/
FROM base8 AS php-80
RUN install-php-8 8.0 https://github.com/php/php-src/archive/php-8.0.0beta4.tar.gz

# Custom builds of php-src
#FROM base as custom-80
#RUN install-php-8 8.0-custom https://github.com/SammyK/php-src/archive/php-8-instrumentation-hooks.tar.gz

# TODO Install memcached
# TODO Install mcrypt
# TODO Install Xdebug (two versions for testing on PHP 7.1 - 7.4)
# TODO Install ast
# TODO Install sodium
# TODO Install redis
# TODO Install mongodb

# Finalize the image
FROM base AS final

# TODO Copy PHP installs
#COPY --chown=circleci:circleci --from=php-70 $PHP_SRC_DIR $PHP_SRC_DIR
#COPY --chown=circleci:circleci --from=php-70 $PHP_INSTALL_DIR $PHP_INSTALL_DIR

#COPY --chown=circleci:circleci --from=php-74 $PHP_SRC_DIR $PHP_SRC_DIR
#COPY --chown=circleci:circleci --from=php-74 $PHP_INSTALL_DIR $PHP_INSTALL_DIR

COPY --chown=circleci:circleci --from=php-80 $PHP_SRC_DIR $PHP_SRC_DIR
COPY --chown=circleci:circleci --from=php-80 $PHP_INSTALL_DIR $PHP_INSTALL_DIR

# Install Composer
RUN set -eux; \
    curl -q https://raw.githubusercontent.com/composer/getcomposer.org/76a7060ccb93902cd7576b67264ad91c8a2700e2/web/installer | \
    ${PHP_INSTALL_DIR}/8.0-debug/bin/php -- --filename=composer --install-dir=/usr/local/bin;

COPY welcome /etc/motd
RUN set -eux; \
# Share welcome message with the world
    echo '[ ! -z "$TERM" -a -r /etc/motd ] && cat /etc/motd' \
        >> /etc/bash.bashrc;

# Run everything else as circleci user
USER circleci

# Set up PHP master branch that can be built by running `install-php-master`
COPY install-php-master /usr/local/bin/
RUN set -eux; \
    cd $PHP_SRC_DIR; \
    git clone --depth 1 --branch master https://github.com/php/php-src.git master;

COPY switch-php /usr/local/bin/
RUN set -eux; \
# Set the default PHP version
    switch-php 8.0-debug; \
# Pretty prompt
    echo "PS1='\[\033[01;32m\]\u\[\033[00m\]\[\033[00;35m\](buster)\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '" | \
        tee -a /home/circleci/.bashrc; \
# Handy aliases
    echo "alias ll='ls -al'" | \
        tee -a /home/circleci/.bash_aliases; \
# Please remember gdb history
    echo 'set history save on' >> /home/circleci/.gdbinit; \
        chmod 600 /home/circleci/.gdbinit;

WORKDIR /home/circleci

# Override stop signal to stop process gracefully
# https://github.com/php/php-src/blob/17baa87faddc2550def3ae7314236826bc1b1398/sapi/fpm/php-fpm.8.in#L163
STOPSIGNAL SIGQUIT

EXPOSE 9000
EXPOSE 80

CMD ["php-fpm", "-F"]
