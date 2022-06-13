# Babel - Let the people speak

Babel is tchat and voice software that enable communication between different peoples over the network with the help of a central server (that only serves as a gateway).

The communications are encrypted using TLS between the clients <-> server.
The voice communications are encrypted between the clients with a random key.

## Usage

### Server

```bash
# Generate self-signed RSA-2048 TLS certificate (for testing purpose only)
openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem
```

```bash
./build/server/babel_server # Server starts on port 5000 by default (see --help for more info)
```

### Client

```bash
./build/client/babel_client --unsafe-cert "127.0.0.1" # Unsafe cert is ONLY for self-signed certificate
```

#### Multiple Client on the same machine

Each client must be launched from a different current directory!!!

```bash
mkdir client_1 client_2
```

```bash
cd client_1
../build/client/babel_client --unsafe-cert "127.0.0.1"
```

```bash
cd client_2
../build/client/babel_client --unsafe-cert "127.0.0.1"
```

## Prerequires

 * cmake + C++ compiler
 * QT 5.15 >=
 * Boost (with program_options and serizlization)
 * PortAudio
 * OpenSSL
 * Opus

On archlinux it should be something like this:

```bash
$ sudo pacman -S cmake gcc qt5-base boost boost-libs openssl portaudio
```

## Building and testting

```bash
cmake -B build
cmake --build build/
```
