#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <windows.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string extractTitle(std::string buffer) {
    size_t start = buffer.find("subdomain");
    size_t end = buffer.find("domain");
        if (start != std::string::npos && end != std::string::npos) {
            return buffer.substr(start, end);
        }
    return "Страница не найдена";
}

void teststr(int size, char stroka[])
{
    while (size > 0)
    {
        if (stroka[size] == '{')
        {
            stroka[size - 1]++;
            stroka[size] = 'a';
        }
        size--;
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    int size_username;
    std::cout << "Enter size: ";
    std::cin >> size_username;
    char *str = new char[size_username + 1];
    std::cout << "Enter username: ";
    std::cin >> str;
    int count = 0;
    std::ofstream titlelist;
    titlelist.open("words.txt", std::ios::app);
    if (!titlelist.is_open())
    {
        std::cerr << "Not open file" << std::endl;
        return 1;
    }
    if (curl) {
        while (str[0] != '{')
        {
            std::string url = "https://fragment.com/username/";
            url = url += str;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                std::string title = extractTitle(readBuffer);
                std::cout << url << std::endl;
                if (title == "Страница не найдена")
                {
                    titlelist << str << std::endl;
                }
            }
            else {
                std::cout << url << std::endl;
                std::cerr << "Error curl: " << curl_easy_strerror(res) << std::endl;
            }
            if (count == 300)
            {
                count = 0;
                std::cout << "Waiting...";
                Sleep(20000);
            }
            readBuffer.clear();
            system("cls");
            count++;
            str[size_username - 1]++;
            teststr(size_username - 1, str);
        }
        curl_easy_cleanup(curl);
        delete[] str;
    }
    curl_global_cleanup();
    titlelist.close();

    return 0;
}
