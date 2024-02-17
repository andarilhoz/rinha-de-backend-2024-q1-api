FROM drogonframework/drogon

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.P
 RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends git gcc g++ cmake\
     libjsoncpp-dev uuid-dev zlib1g-dev libhiredis-dev libmariadb-dev\
     curl libyaml-cpp-dev && rm -rf /var/lib/apt/lists/*

# Definir a versão do Docker Compose que você deseja instalar
ARG DOCKER_COMPOSE_VERSION=1.29.2

# Baixar a versão específica do Docker Compose e torná-la executável
RUN curl -L "https://github.com/docker/compose/releases/download/${DOCKER_COMPOSE_VERSION}/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose \
    && chmod +x /usr/local/bin/docker-compose

# Comando opcional para verificar a instalação
RUN docker-compose --version