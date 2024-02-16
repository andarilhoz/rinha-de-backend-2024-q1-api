FROM drogonframework/drogon:latest

RUN apt-get update -y \
  && apt-get install -y \
  ccache \
  curl \
  tar \
  jq \
  ninja-build \
  libsqlite3-dev \
  clang \
  libfmt-dev \
  && apt-get purge --auto-remove cmake \
  && apt-get upgrade -y