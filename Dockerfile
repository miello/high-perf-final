FROM alpine:3.17.0 AS build

RUN apk update && \
    apk add --no-cache \
    libstdc++ \
    g++ \
    make

# Install openmp
RUN apk add --no-cache \
    libomp-dev

WORKDIR /app

COPY . .

RUN make build

FROM alpine:3.17.0 AS runtime

RUN apk update && \
    apk add --no-cache \
    libstdc++ \
    g++

COPY --from=build /app/power_grid /app/power_grid

ENTRYPOINT [ "/app/power_grid" ]