#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include "../utils.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void set_default_headers(http::response<http::string_body>& res) {
    // Configurar encabezados CORS
    res.set(http::field::access_control_allow_origin, "*");
    res.set(http::field::access_control_allow_methods, "POST, OPTIONS");
    res.set(http::field::access_control_allow_headers, "Content-Type");
}

void handle_request(http::request<http::string_body>&& req, http::response<http::string_body>& res) {
    // Configurar encabezados CORS por defecto
    
    set_default_headers(res);

    // Manejo de solicitudes OPTIONS (CORS preflight)
    if (req.method() == http::verb::options) {
        res.result(http::status::ok);
        res.set(http::field::content_type, "text/plain");
        res.body() = "";
        res.prepare_payload();
        return;
    }

    // Comprueba si la solicitud es para una ruta específica y es de tipo POST
    if (req.method() == http::verb::post && req.target() == "/upload") {
        try {
            // Suponiendo que el cuerpo de la solicitud es la ruta del archivo de audio
            std::string audioFilePath = req.body();

            // Construye el comando para ejecutar el script de Python
            std::string command = "python3 ../Processing/queryAudio.py " + audioFilePath;

            // Ejecuta el comando
            int result = std::system(command.c_str());

            if (result != 0) {
                // Manejo de errores en la ejecución del script
                throw std::runtime_error("Error al ejecutar el script de Python");
            }

            std::string nearestNeighbor = nameOfNearestNeighbor();

            // Establecer respuesta
            res.result(http::status::ok);
            res.body() = nearestNeighbor;
            res.prepare_payload();
        } catch (const std::exception& e) {
            // Manejo de errores
            res.result(http::status::internal_server_error);
            res.body() = e.what();
            res.prepare_payload();
        }
    } else {
        // Maneja otros tipos de solicitudes o rutas aquí
        res.result(http::status::not_found);
        res.body() = "Ruta no encontrada";
        res.prepare_payload();
    }
}


int main() {
    try {
        auto const address = net::ip::make_address("127.0.0.1");
        unsigned short port = 8080;

        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};

        std::cout << "Server listening on http://127.0.0.1:" << port << std::endl;

        while (true) {
            
            acceptor.accept(socket);

            beast::tcp_stream stream{std::move(socket)};
            beast::flat_buffer buffer;

            http::request<http::string_body> req;
            http::response<http::string_body> res;

            http::read(stream, buffer, req);
            handle_request(std::move(req), res);

            beast::error_code ec;
            http::write(stream, res, ec);

            stream.socket().shutdown(tcp::socket::shutdown_send, ec);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
