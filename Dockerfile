FROM gcc:13-bookworm

WORKDIR /app

COPY . .

RUN g++ -O2 -std=c++17 -pthread src/main.cpp -o server

ENV PORT=8080

EXPOSE 8080

CMD ["./server"]