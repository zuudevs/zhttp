#include <iostream>
#include <string>

#include "network.hpp" // Class baru

int main() {
    try {
        // 1. Init Network Otomatis (RAII)
        // Begitu variabel ini dibuat, WSAStartup jalan.
        // Begitu fungsi main selesai, WSACleanup jalan.
        ZHTTP::Network net_manager; 

        // 2. Buka Toko
        ZHTTP::Socket server;
        ZHTTP::SockAddr bind_addr(ZHTTP::IPv4(0u), 8080);

        std::cout << "[Zuu] Mengikat ke " << bind_addr.toString() << "...\n";
        server.bind(bind_addr);
        server.listen();

        while (true) {
            // 3. Terima Client
            ZHTTP::SockAddr client_addr;
            ZHTTP::Socket client = server.accept(&client_addr);
            std::cout << "[Zuu] Tamu masuk: " << client_addr.toString() << "\n";

            // 4. Kirim Data (Pakai method baru!)
            std::string body = "<h1>Halo Rara! Code kamu makin rapi!</h1>";
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "\r\n" + body;

            // Gak perlu cast-cast aneh lagi, langsung kirim string view
            client.send(response);
            
            // Client otomatis close pas loop berulang (destructor dipanggil)
        }

    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
    }

    return 0;
}