# ft_irc

> 🧠 Servidor IRC minimalista desarrollado en C++ como parte del programa 42

## 📌 Descripción

`ft_irc` es un servidor IRC (Internet Relay Chat) implementado en C++98 que sigue el estándar definido por el [RFC 2812](https://tools.ietf.org/html/rfc2812). El objetivo del proyecto es comprender y aplicar conceptos de programación en red, manejo de múltiples conexiones, parsing de comandos y arquitectura cliente-servidor.

Este proyecto fue desarrollado sin frameworks externos, utilizando únicamente la biblioteca estándar y llamadas al sistema (syscalls) de bajo nivel.

## ⚙️ Características

- Soporte para múltiples clientes simultáneos
- Gestión de canales: creación, unión, salida, y expulsión de usuarios
- Implementación de comandos IRC como:
  - `NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, `KICK`, `QUIT`, `MODE`...
- Autenticación con contraseña del servidor
- Multiplexación de sockets con `poll()`
- Manejo de errores y desconexiones limpias

## 🧪 Requisitos

- Sistema UNIX (Linux o macOS)
- Compilador compatible con C++98 (g++, clang++)

## 🛠️ Compilación

```bash
git clone https://github.com/LycaonPictus/ft_irc.git
cd ft_irc
make
