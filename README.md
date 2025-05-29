# 📡 ft_irc

## 📝 Introduction

**ft_irc** is a 42 school project that consists of building a basic IRC (Internet Relay Chat) server, partially compliant with the [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459) specification.

The goal of this project is to gain a deeper understanding of socket programming, network protocols, multiplexing with `poll`, and how a multi-client chat server operates behind the scenes.

Our server supports multiple simultaneous clients, user authentication, channel creation and management, private messages, an incredible bot and more features.

## ⚙️ Compilation

To compile the project, make sure you have a C++ compiler installed and simply run:

```bash
git clone https://github.com/your-username/ft_irc.git
cd ft_irc
make
```
## Usability

**1. To run the server and leave it waiting for client connections, run:**
```bash
./ircserv <port> <password>
```

```<port>``` is the port where the server listens for incoming connections.

```<password>``` is the password that each new client must provide to authenticate with the server.

**Now that the server is up and running, let's take a look at the main IRC commands supported by our server.**

These commands can be typed directly in clients like **Netcat** or used automatically by IRC clients like **HexChat**.

---

### 🔐 `PASS <password>`

This command is used to provide the server password upon connection.  
It **must be the first command** sent by the client before any other.

---

### 🧑 `NICK <nickname>`

Defines your nickname in the IRC network.


### 👤 `USER <username> <hostname> <servername> <realname>`

Sets the username and real name of the client.  
For our server, the middle two fields can be anything (they're ignored).

---

### 📥 `JOIN <#channel>`

Joins (or creates if it doesn't exist) a channel. Channels must start with `#`.

---

### 📩 `PRIVMSG <target> :<message>`

Sends a private message to a user or to a channel.

---

### 🚪 `KICK <channel> <nickname> [:reason]`

Removes a user from the channel.

---

### 📩 `INVITE <nickname> <channel>`

Invites a user to a channel that is marked as invite-only.

---

### ⚙️ `MODE`

Used to change the channel's settings.

#### Mode flags:

- `+o` / `-o` — Give/remove operator status
- `+t` / `-t` — Only operators can set the channel topic
- `+l` — Set a user limit
- `+i` — Set channel as invite-only
- `+k` — Set a password (key) to enter the channel

**3 - Now that we know how to use the commands, we can create a connection using either Netcat or HexChat.**

You can connect nc using:

- **Netcat (`nc`)** — a simple command-line tool used to manually test the server by sending IRC commands:

```bash
  nc localhost <port>
```

- **HexChat**:

