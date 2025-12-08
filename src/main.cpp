#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <filesystem>

#include "network.hpp" 

#undef ERROR
#include "logger.hpp" 

#ifdef ERROR
    #undef ERROR
#endif

int main(int argc, char* argv[]) { 
    try {
        ZHTTP::Network net_manager; 
        std::ofstream log("report.log", std::ios::app);

        std::string filename = "index.html"; 
        if (argc > 1) {
            filename = argv[1]; 
        }

        // Cek file ada atau nggak (Optional tapi bagus)
        if (!std::filesystem::exists(filename)) {
            auto err_msg = ZHTTP::CreateLog(ZHTTP::Level::WARN, std::format("File '{}' not found! fallback to default(index.html).", filename));
            std::cerr << err_msg << "\n";
            log << err_msg << "\n";
        }

        // 3. Baca file yang dipilih
        std::ifstream html_file(filename);
        std::string body;

        if (html_file.is_open()) {
            std::stringstream buffer;
            buffer << html_file.rdbuf();
            body = buffer.str();
            
            auto msg = ZHTTP::CreateLog(ZHTTP::Level::INFO, std::format("Loaded file: {}", filename));
            std::cout << msg << "\n";
            log << msg << "\n";
        } else {
            body = "<h1>404 Not Found</h1><p>Yah, file <b>" + filename + "</b> gak ketemu, Ra.</p>";
        }

        ZHTTP::Socket server;
        ZHTTP::SockAddr bind_addr(ZHTTP::IPv4(0u), 8080);

        auto start_msg = ZHTTP::CreateLog(ZHTTP::Level::INFO, std::format("Server listening on {}", bind_addr.toString()));
        log << start_msg << "\n";
        std::cout << start_msg << "\n";

        server.bind(bind_addr);
        server.listen();

        while (true) {
            ZHTTP::SockAddr client_addr;
            ZHTTP::Socket client = server.accept(&client_addr);

            auto guest_msg = ZHTTP::CreateLog(ZHTTP::Level::INFO, std::format("Request from {}", client_addr.toString()));
            std::cout << guest_msg << "\n";
            log << guest_msg << "\n";

            // Response Header
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n" // Tambahin ini biar browser tau ini HTML
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "\r\n" + body;

            client.send(response);
        }

    } catch (const std::exception& e) {
        std::cerr << "[CRITICAL ERROR] " << e.what() << "\n";
    }

    return 0;
}