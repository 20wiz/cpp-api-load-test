# Use an official GCC image as a parent image
FROM gcc:latest

# Set the working directory in the container
WORKDIR /usr/src/app

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Compile the server.cpp source file
RUN g++ -o server server-multi.cpp

# Make port 8080 available to the world outside this container
EXPOSE 8080

# Run the server executable
CMD ["./server"]