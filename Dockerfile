FROM ubuntu

RUN apt -y update
RUN apt install -y build-essential
RUN echo "tzdata "debconf/frontend" select 8" | debconf-set-selections && \
    apt install -y tzdata && apt install -y libboost-all-dev
RUN apt install -y cmake

WORKDIR /app
COPY . .
WORKDIR /app/build
RUN cmake .. && make

CMD ./http_proxy

EXPOSE 80