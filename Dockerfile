# Use the takigama/platformio image from Docker Hub
FROM takigama/platformio:latest

# Set the working directory to the root of the PlatformIO project
WORKDIR /app

# Copy the entire PlatformIO project into the container
COPY . .

# Run the PlatformIO command to build the project
RUN platformio run

# Set the entry point to the PlatformIO command
CMD ["platformio"]
