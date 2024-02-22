FROM drogonframework/drogon

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.P
 RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends git gcc g++ cmake\
     libjsoncpp-dev uuid-dev zlib1g-dev libhiredis-dev \
     curl libyaml-cpp-dev gdb && rm -rf /var/lib/apt/lists/*

ENV workDir /home/drogon/app

WORKDIR ${workDir}

COPY . /home/drogon/app

RUN mkdir build &&\
    cd build &&\
    cmake .. &&\
    make

EXPOSE 8080

CMD ["./build/rinha-de-backend-2024-q1-api"]