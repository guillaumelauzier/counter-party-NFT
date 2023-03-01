#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Set up the Counterparty API endpoint
const std::string CP_API = "http://localhost:14000/api/";

// Helper function to send an HTTP POST request to the Counterparty API
std::string send_request(const std::string& method, const json& data) {
    std::string url = CP_API + method;
    std::string post_data = data.dump();

    CURL *curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char* ptr, size_t size, size_t nmemb, std::string* userdata){
            userdata->append(ptr, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "Error sending HTTP request: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return response;
}

int main() {
    // Create a new asset
    std::string asset_name = "MyNFT";
    std::string asset_description = "My first NFT on Counterparty";
    json asset_data = {
        {"artist", "John Smith"},
        {"year", 2023},
        {"medium", "Oil on canvas"}
    };
    json issuance_data = {
        {"source", "myaddress"},
        {"asset", asset_name},
        {"quantity", 1},
        {"description", asset_description},
        {"divisible", false},
        {"callable", false},
        {"transfer_destination", "myaddress"},
        {"data", asset_data}
    };

    std::string issuance_response = send_request("create_issuance", issuance_data);
    json issuance_result = json::parse(issuance_response);

    std::cout << "Asset created: " << issuance_result.dump() << std::endl;

    // Issue the new asset
    json tx_data = {
        {"tx_hex", issuance_result},
        {"allow_unconfirmed_inputs", true}
    };

    std::string tx_response = send_request("send_tx", tx_data);
    json tx_result = json::parse(tx_response);

    std::cout << "Asset issued: " << tx_result.dump() << std::endl;

    return 0;
}
