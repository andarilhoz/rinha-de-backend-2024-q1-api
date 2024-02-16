FROM mcr.microsoft.com/devcontainers/cpp:0-debian-11

# Atualiza a lista de pacotes e instala ferramentas de desenvolvimento básicas
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive

# Instala Git
RUN apt-get -y install --no-install-recommends git

# Instala GCC
RUN apt-get -y install --no-install-recommends gcc

# Instala G++
RUN apt-get -y install --no-install-recommends g++

# Instala CMake
RUN apt-get -y install --no-install-recommends cmake

# Instala libjsoncpp-dev
RUN apt-get -y install --no-install-recommends libjsoncpp-dev

# Instala uuid-dev
RUN apt-get -y install --no-install-recommends uuid-dev

# Instala zlib1g-dev
RUN apt-get -y install --no-install-recommends zlib1g-dev

# Instala libhiredis-dev
RUN apt-get -y install --no-install-recommends libhiredis-dev

# Instala libmariadbclient-dev
RUN apt-get -y install --no-install-recommends libmariadbclient-dev

# Limpa o cache do APT para reduzir o tamanho da imagem
RUN apt-get clean && rm -rf /var/lib/apt/lists/*