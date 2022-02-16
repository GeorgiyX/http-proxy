FROM ubuntu

RUN apt -y update
RUN apt install -y build-essential
RUN apt install -y libboost-all-dev
RUN apt install -y cmake

WORKDIR /app
COPY . .
WORKDIR /app/build
RUN cmake .. && make .

CMD ./http_proxy

EXPOSE 80