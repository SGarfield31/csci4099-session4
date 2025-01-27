#include <stdio.h>
#include <string.h>

#define PACKET_SIZE 1024
#define PORT 8080
#define PID 1234

//test
// Function prototypes
void sendData(const char *input);
void receiveData(const char *output_file);
void ApplicationLayer(const char *file);
void SessionLayer(const char *file);
void NetworkLayer(const char *file);
void DatalinkLayer(const char *file);
void PhysicalLayer(const char *file);
void PhysicalLayerReceive(const char *file);
void DataLinkLayerReceive(const char *file);
void NetworkLayerReceive(const char *file);
void SessionLayerReceive(const char *file);
void ApplicationLayerReceive(const char *file);

// Send function
void sendData(const char *input) {
    ApplicationLayer(input);
}

// Application Layer
void ApplicationLayer(const char *file) {
    FILE *input = fopen(file, "r");
    if (!input) {
        printf("Error opening file\n");
        return;
    }

    char buffer[PACKET_SIZE];
    size_t bytes = fread(buffer, 1, sizeof(buffer) - 1, input);
    buffer[bytes] = '\0';
    fclose(input);

    char header[9];
    sprintf(header, "%04d%04d", PORT, PID);

    char trailer[5];
    sprintf(trailer, "%04d", 1234);

    FILE *output = fopen("app.tmp", "w");
    fprintf(output, "%s%s%s", header, buffer, trailer);
    fclose(output);

    printf("Application Layer - Header: %s, Trailer: %s\n", header, trailer);
    SessionLayer("app.tmp");
}

// Session Layer
void SessionLayer(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    char header[9];
    sprintf(header, "%04d%04d", 1, 1);

    FILE *output = fopen("session.tmp", "w");
    fprintf(output, "%s%s", header, buffer);
    fclose(output);

    printf("Session Layer - Header: %s\n", header);
    NetworkLayer("session.tmp");
}

// Network Layer
void NetworkLayer(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    char header[9];
    sprintf(header, "1921%04d", PORT);

    FILE *output = fopen("network.tmp", "w");
    fprintf(output, "%s%s", header, buffer);
    fclose(output);

    printf("Network Layer - Header: %s\n", header);
    DatalinkLayer("network.tmp");
}

// Datalink Layer
void DatalinkLayer(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    char header[5];
    sprintf(header, "%04d", 1234);

    FILE *output = fopen("datalink.tmp", "w");
    fprintf(output, "%s%s", header, buffer);
    fclose(output);

    printf("Datalink Layer - Header: %s\n", header);
    PhysicalLayer("datalink.tmp");
}

// Physical Layer
void PhysicalLayer(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%s", buffer);
    fclose(output);

    printf("Physical Layer - Data written to output.txt\n");
}

// Receive function
void receiveData(const char *output_file) {
    PhysicalLayerReceive(output_file);
}

// Physical Layer Receive
void PhysicalLayerReceive(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%s", buffer);
    fclose(output);

    printf("Physical Layer - Data read\n");
    DataLinkLayerReceive("output.txt");
}

// Datalink Layer Receive
void DataLinkLayerReceive(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    char stripped[PACKET_SIZE * 2];
    strcpy(stripped, buffer + 4);
    
    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%s", stripped);
    fclose(output);

    printf("Datalink Layer - Stripped MAC Header\n");
    NetworkLayerReceive("output.txt");
}

// Network Layer Receive
void NetworkLayerReceive(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    char stripped[PACKET_SIZE * 2];
    strcpy(stripped, buffer + 8);

    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%s", stripped);
    fclose(output);

    printf("Network Layer - Stripped IP and Port\n");
    SessionLayerReceive("output.txt");
}

// Session Layer Receive
void SessionLayerReceive(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    char stripped[PACKET_SIZE * 2];
    strcpy(stripped, buffer + 8);

    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%s", stripped);
    fclose(output);

    printf("Session Layer - Stripped Packet Info\n");
    ApplicationLayerReceive("output.txt");
}

// Application Layer Receive
void ApplicationLayerReceive(const char *file) {
    FILE *input = fopen(file, "r");
    char buffer[PACKET_SIZE * 2];
    fgets(buffer, sizeof(buffer), input);
    fclose(input);

    char stripped[PACKET_SIZE * 2];
    strcpy(stripped, buffer + 8);
    stripped[strlen(stripped) - 4] = '\0';

    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%s", stripped);
    fclose(output);

    printf("Application Layer - Stripped Header and Trailer\n");
    printf("\nFinal Data: %s\n", stripped);
}

int main() {
    FILE *input = fopen("input.txt", "w");
    fprintf(input, "Hello, World!");
    fclose(input);

    printf("Transmitting File\n");
    sendData("input.txt");

    printf("\nReceiving File\n");
    receiveData("output.txt");

    remove("app.tmp");
    remove("session.tmp");
    remove("network.tmp");
    remove("datalink.tmp");

    return 0;
}
