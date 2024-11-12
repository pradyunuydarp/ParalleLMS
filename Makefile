#server:
#	gcc -o server src/server.c
#
#client:
#	gcc -o client src/client.c
# Specify the executables and their sources
SERVER_EXEC = server
CLIENT_EXEC = client
SERVER_SRC = src/server.c
CLIENT_SRC = src/client.c

# Define rules for building the executables
all: $(SERVER_EXEC) $(CLIENT_EXEC)

$(SERVER_EXEC): $(SERVER_SRC)
	rm -f $(SERVER_EXEC)
	gcc -o $(SERVER_EXEC) $(SERVER_SRC)

$(CLIENT_EXEC): $(CLIENT_SRC)
	rm -f $(CLIENT_EXEC)
	gcc -o $(CLIENT_EXEC) $(CLIENT_SRC)

# Ensure that 'make' always rebuilds the executables
.PHONY: all $(SERVER_EXEC) $(CLIENT_EXEC)

# Clean rule to remove the executables
clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)
