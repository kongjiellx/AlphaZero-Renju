From tensorflow/build:2.9-python3.8

RUN git clone https://github.com/kongjiellx/AlphaZero-Renju.git
WORKDIR AlphaZero-Renju
RUN git checkout docker
RUN bazel build //cpp/src:main
